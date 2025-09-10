// helper functions for creating
// opencl contexts and managing & creating
// kernels on device

// provides a struct to hold
// all the necessary cl objects

#pragma once
#define CL_TARGET_OPENCL_VERSION 300
#include <cl/cl.h>
#include <fmt/base.h>
#include <config.h>



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
    static constexpr char adder[] = "adder";
    static constexpr char viewport[] = "viewport";

};

struct Memory{

    Memory(cl_resource* resource){
        cl_int int_ret;
        intersect = clCreateBuffer(resource->context, CL_MEM_READ_WRITE, VRES*VRES*ASPECT_RATIO*sizeof(float), NULL, &int_ret);
        fmt::print("Intersect matrix allocation return code: {0}\n", int_ret);
        origin = clCreateBuffer(resource->context, CL_MEM_READ_WRITE, VRES*VRES*ASPECT_RATIO*3*sizeof(float), NULL, &int_ret);
        fmt::print("Origin matrix allocation return code: {0}\n", int_ret);
        direction = clCreateBuffer(resource->context, CL_MEM_READ_WRITE, VRES*VRES*ASPECT_RATIO*3*sizeof(float), NULL, &int_ret);
        fmt::print("Direction matrix allocation return code: {0}\n", int_ret);
        color = clCreateBuffer(resource->context, CL_MEM_READ_WRITE, VRES*VRES*ASPECT_RATIO*3*sizeof(float), NULL, &int_ret);
        fmt::print("Color matrix allocation return code: {0}\n", int_ret);
        return;
    }

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