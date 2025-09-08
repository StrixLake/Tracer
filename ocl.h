// helper functions for creating
// opencl contexts and managing & creating
// kernels on device

// provides a struct to hold
// all the necessary cl objects

#ifndef OCL
#define OCL
#define CL_TARGET_OPENCL_VERSION 300
#include <cl/cl.h>
#include <fmt/base.h>
#endif

#define PLATFORM 0
#define DEVICE 0

struct cl_resource{
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_program program;
    cl_command_queue queue;

    ~cl_resource(){
        fmt::print("Dealocating all the resources\n");
        clReleaseCommandQueue(queue);
        clReleaseContext(context);
        clReleaseDevice(device);
        clReleaseProgram(program);
        return;
    }
};

struct Kernels{
    static constexpr char adderN[] = "adder";
    static const size_t adderS = sizeof(adderN);
};

struct Memory{
    cl_mem origin;
    cl_mem direction;
    cl_mem intersect;
    cl_mem color;
    cl_mem spheres;

    ~Memory(){
        fmt::print("Dealocating memory from device\n");
        clReleaseMemObject(origin);
        clReleaseMemObject(direction);
        clReleaseMemObject(intersect);
        clReleaseMemObject(color);
        return;
    }
};

void initialize_resources(cl_resource* resource);