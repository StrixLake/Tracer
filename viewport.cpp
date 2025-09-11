
#include <viewport.h>
#include <schedular.h>

void create_viewport(cl_resource* resource, Memory* memory){
    cl_int int_ret;
    cl_kernel kernel = clCreateKernel(resource->program, Kernels::viewport, &int_ret);
    fmt::print("Error code when creating the viewport kernel: {0}\n", int_ret);

    cl_float focal_length = FOCAL_LENGTH;

    clSetKernelArg(kernel, 0, sizeof(cl_mem), &memory->intersect);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &memory->origin);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &memory->direction);
    clSetKernelArg(kernel, 3, sizeof(cl_mem), &memory->color);
    clSetKernelArg(kernel, 4, sizeof(cl_float), &focal_length);

    cl_event event = schedule_work(resource, &kernel);
    int_ret = clReleaseKernel(kernel);
    fmt::print("Error code from release kernel inside viewport: {0}\n", int_ret);
    clWaitForEvents(1, &event);
    fmt::print("\n");
    return;
}