// helper functions for creating
// opencl contexts and managing & creating
// kernels on device

// provides a struct to hold
// all the necessary cl objects

#ifndef OCL
#define OCL
#include <cl/cl.h>
#include <vector>
#endif

#define PLATFORM 0
#define DEVICE 0

struct cl_resource{
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    std::vector<cl_kernel> kernels;
};

void initialize_resources(cl_resource* resource);