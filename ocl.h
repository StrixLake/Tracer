// helper functions for creating
// opencl contexts and managing & creating
// kernels on device

// provides a struct to hold
// all the necessary cl objects

#ifndef OCL
#define OCL
#include <cstddef>
#define CL_TARGET_OPENCL_VERSION 120
#include <cl/cl.h>
#include <vector>
#endif

#define PLATFORM 0
#define DEVICE 0

struct cl_resource{
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_program program;
};

struct Kernels{
    static constexpr char adderN[] = "adder";
    static const size_t adderS = sizeof(adderN);
};

void initialize_resources(cl_resource* resource);