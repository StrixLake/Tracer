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
#include <string>


void HANDLE_ERROR(int error, std::string name);

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
    static constexpr char intersector[] = "intersector";
    static constexpr char render[] = "render";

    static constexpr char compile_options[] = "-I C:\\Users\\yasha\\Documents\\DSA\\Tracer\\kernels\\ -cl-fast-relaxed-math";

};

struct Memory{

    Memory(cl_resource* resource){
        cl_int int_ret;
        origin = clCreateBuffer(resource->context, CL_MEM_READ_WRITE, VRES*VRES*ASPECT_RATIO*3*sizeof(float), NULL, &int_ret);
        HANDLE_ERROR(int_ret, "Origin matrix allocation return code");
        direction = clCreateBuffer(resource->context, CL_MEM_READ_WRITE, VRES*VRES*ASPECT_RATIO*3*sizeof(float), NULL, &int_ret);
        HANDLE_ERROR(int_ret, "Direction matrix allocation return code");
        color = clCreateBuffer(resource->context, CL_MEM_READ_WRITE, VRES*VRES*ASPECT_RATIO*3*sizeof(float), NULL, &int_ret);
        HANDLE_ERROR(int_ret, "Color matrix allocation return code");
        return;
    }

    cl_mem origin;
    cl_mem direction;
    cl_mem color;
    // sphere position, color and properties array
    cl_mem rSpheres;
    cl_mem cSpheres;
    cl_mem pSpheres;
    

    ~Memory(){
        fmt::print("Dealocating memory from device\n");
        clReleaseMemObject(origin);
        clReleaseMemObject(direction);
        clReleaseMemObject(color);
        clReleaseMemObject(rSpheres);
        clReleaseMemObject(cSpheres);
        clReleaseMemObject(pSpheres);
        return;
    }
};

void initialize_resources(cl_resource* resource);
