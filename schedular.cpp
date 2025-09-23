#include <schedular.h>
#include <fmt/base.h>
#include <config.h>

// work complete will read data as 
// unsigned long long
// to print the amount of work that has been done
void CL_CALLBACK work_complete(cl_event event, cl_int status, void* data){
    double percentage = *(double*)(&data);
    fmt::print("{0}% ", percentage);
    return;
}

// schedular will take the kernel object
// and the resource object
// each tile is of (8,8) pixel large
// and each work group is 1 tile
// the kernel must have its arguments already set
cl_event schedule_work(cl_resource* resource, cl_kernel* kernel){
    cl_uint uint_ret;
    cl_int int_ret;
    
    // calculate the total number of tiles in the image
    int tiles = VRES*VRES*ASPECT_RATIO/64;
    // a superTile is the group of tiles rendered
    // at once
    cl_uint superTile = tiles/10;

    size_t global_work_offset[3] = {0,0,0};
    const size_t global_work_size[3] = {superTile, 8,8};
    const size_t local_work_size[3] = {1,8,8};

    // each kernel enqueued waits on the kernel enqueued before it
    // the first kernel waits on the user created event
    // that is set to complete after all the kernels have been enqueued
    cl_event event_received;
    cl_event event_to_wait = clCreateUserEvent(resource->context, &int_ret);
    HANDLE_ERROR(int_ret, "Handle creation code");

    cl_event user_event = event_to_wait;
    
    for(size_t i = 0; i < tiles; i += superTile){
        
        int_ret = clEnqueueNDRangeKernel(resource->queue, *kernel, 3, (const size_t*)global_work_offset, global_work_size, local_work_size, 1, &event_to_wait, &event_received);
        event_to_wait = event_received;

        HANDLE_ERROR(int_ret, "Enqueue NDRange kernel");

        double percentage = (double)i*100/tiles;
        long long x = *(long long*)(&percentage);
        clSetEventCallback(event_received, CL_COMPLETE, work_complete, (void*)x);
        global_work_offset[0] += superTile;
    }

    int_ret = clSetUserEventStatus(user_event, CL_COMPLETE);
    HANDLE_ERROR(int_ret, "Set user event status");
    int_ret = clReleaseEvent(user_event);
    HANDLE_ERROR(int_ret, "Release user event");

    return event_received;
}
