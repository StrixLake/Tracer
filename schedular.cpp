#include <schedular.h>
#include <fmt/base.h>
#include <config.h>

// work complete will read data as 
// unsigned long long
// to print the amount of work that has been done
void CL_CALLBACK work_complete(cl_event event, cl_int status, void* data){
    double percentage = *(double*)(&data);
    if (fmod(percentage, 10) < 0.04){
        fmt::print("{0}% ", percentage);
    }
    return;
}

// schedular will take the kernel object
// and the resource object
// it will first get the amount of work group processors
// the device in resource has and will dispatch
// that many work groups at once, each work group
// will have (8,8) work items
// the kernel must have its arguments already set
cl_event schedule_work(cl_resource* resource, cl_kernel* kernel){
    cl_uint uint_ret;
    cl_int int_ret;

    // get the number of work group processors
    int_ret = clGetDeviceInfo(resource->device, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(uint_ret), &uint_ret, NULL);
    cl_uint wgp = uint_ret;

    // calculate the total number of work groups required
    int groups = VRES*VRES*ASPECT_RATIO/64;

    size_t global_work_offset[3] = {0,0,0};
    const size_t global_work_size[3] = {wgp, 8,8};
    const size_t local_work_size[3] = {1,8,8};

    // each kernel enqueued waits on the kernel enqueued before it
    // the first kernel waits on the user created event
    // that is set to complete after all the kernels have been enqueued
    cl_event event_received;
    cl_event event_to_wait = clCreateUserEvent(resource->context, &int_ret);
    HANDLE_ERROR(int_ret, "Handle creation code");

    cl_event user_event = event_to_wait;

    for(size_t i = 0; i < groups; i += wgp){
        
        // this is done because number of groups may not be
        // divisible by number WGP so it will not launch
        // the last few work groups if they are less than WGPs
        if(i + wgp > groups) break;
        
        int_ret = clEnqueueNDRangeKernel(resource->queue, *kernel, 3, (const size_t*)global_work_offset, global_work_size, local_work_size, 1, &event_to_wait, &event_received);
        event_to_wait = event_received;

        HANDLE_ERROR(int_ret, "Enqueue NDRange kernel");

        double percentage = (double)i*100/groups;
        long long x = *(long long*)(&percentage);
        clSetEventCallback(event_received, CL_COMPLETE, work_complete, (void*)x);
        global_work_offset[0] += wgp;
    }

    int_ret = clSetUserEventStatus(user_event, CL_COMPLETE);
    HANDLE_ERROR(int_ret, "Set user event status");
    int_ret = clReleaseEvent(user_event);
    HANDLE_ERROR(int_ret, "Release user event");

    return event_received;
}
