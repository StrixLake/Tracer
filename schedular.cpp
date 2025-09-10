#include <schedular.h>
#include <fmt/base.h>
#include <config.h>

// work complete will read data as 
// unsigned long long
// to print the amount of work that has been done
void CL_CALLBACK work_complete(cl_event event, cl_int status, void* data){
    unsigned long long percentage = (unsigned long long)data;
    fmt::print("{0}% ", percentage);
    return;
}

// schedular will take the kernel object
// and the resource object
// it will first get the amount of work group processors
// the device in resource has and will dispatch
// that many work groups at once, each work group
// will have (8,8) work items
// the kernel must have its arguments already set
// and the event object is the event that the first command will wait on
cl_event schedule_work(cl_resource* resource, cl_kernel* kernel){
    cl_uint uint_ret;
    cl_int int_ret;

    // get the number of work group processors
    int_ret = clGetDeviceInfo(resource->device, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(uint_ret), &uint_ret, NULL);
    fmt::print("device has {0} compute units\n", uint_ret);
    cl_uint wgp = uint_ret;

    // calculate the total number of work groups required
    int groups = VRES*VRES*ASPECT_RATIO/64;

    size_t global_work_offset[3] = {0,0,0};
    const size_t global_work_size[3] = {wgp, 8,8};
    const size_t local_work_size[3] = {1,8,8};

    cl_event ev;
    for(size_t i = 0; i < groups; i += wgp){
        
        if(i + wgp > groups) break;
        
        int_ret = clEnqueueNDRangeKernel(resource->queue, *kernel, 3, (const size_t*)global_work_offset, global_work_size, local_work_size, 0, NULL, &ev);
        clSetEventCallback(ev, CL_COMPLETE, work_complete, (void*)((i+1)*100/groups));
        global_work_offset[0] += wgp;
    }
    
    return ev;
}
