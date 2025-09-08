#include <CL/cl.h>
#include <fstream>
#include <ios>
#include <iterator>
#include <ocl.h>
#include <fmt/base.h>
#include <string>


void compile_kernels(cl_resource* resource){
    std::ifstream f("C:\\Users\\yasha\\Documents\\DSA\\Tracer\\kernels.cl");
    std::noskipws(f);
    std::string source(std::istream_iterator<char>(f), {});
    //fmt::print("{0}\n", source);

    cl_int int_ret;
    cl_uint uint_ret;
    const char* s = source.data();
    size_t t = source.size();
    cl_program program = clCreateProgramWithSource(resource->context, 1, &s, &t, &int_ret);
    fmt::print("Create program return code: {0}\n", int_ret);

    int_ret = clBuildProgram(program, 1, (const cl_device_id*)&resource->device, NULL, NULL, NULL);
    fmt::print("Build program return code: {0}\n", int_ret);
    resource->program = program;

    return;
}

void initialize_resources(cl_resource* resource){
    cl_uint uint_ret;
    cl_int int_ret;
    size_t size_ret;
    clGetPlatformIDs(0, NULL, &uint_ret);
    fmt::print("Number of platforms available: {0}\n", uint_ret);
    
    cl_platform_id* platforms = new cl_platform_id[uint_ret];
    clGetPlatformIDs(uint_ret, platforms, NULL);
    resource->platform = platforms[PLATFORM];
    delete[] platforms;

    clGetPlatformInfo(resource->platform, CL_PLATFORM_NAME, 100000, NULL, &size_ret);
    std::string name;
    name.resize(size_ret);
    clGetPlatformInfo(resource->platform, CL_PLATFORM_NAME, size_ret, name.data(), &size_ret);
    fmt::print("Name of the platform being used: {0}\n", name);


    clGetDeviceIDs(resource->platform, CL_DEVICE_TYPE_ALL, 1, &resource->device, &uint_ret);
    fmt::print("Number of devices available: {0}\n", uint_ret);
    clGetDeviceInfo(resource->device, CL_DEVICE_NAME, 100000, NULL, &size_ret);
    name.resize(size_ret);
    clGetDeviceInfo(resource->device, CL_DEVICE_NAME, size_ret, name.data(), &size_ret);
    fmt::print("Name of the device: {0}\n", name);

    resource->context = clCreateContext(NULL, 1, &resource->device, NULL, NULL, &int_ret);
    fmt::print("Context creation return code: {0}\n", int_ret);

    compile_kernels(resource);
    
    return;

}