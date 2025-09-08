#include <CL/cl.h>
#include <cstddef>
#include <ocl.h>
#include <fmt/base.h>
#include <string>

void compile_kernels(cl_resource* resource);

void initialize_resources(cl_resource* resource){
    cl_uint return_code;
    clGetPlatformIDs(0, NULL, &return_code);
    fmt::print("Number of platforms available: {0}\n", return_code);
    
    cl_platform_id* platforms = new cl_platform_id[return_code];
    clGetPlatformIDs(return_code, platforms, NULL);
    resource->platform = platforms[PLATFORM];
    delete[] platforms;

    size_t ret_code;
    clGetPlatformInfo(resource->platform, CL_PLATFORM_NAME, 100000, NULL, &ret_code);
    std::string name;
    name.resize(ret_code);
    clGetPlatformInfo(resource->platform, CL_PLATFORM_NAME, ret_code, name.data(), &ret_code);
    fmt::print("Name of the platform being used: {0}\n", name);


    clGetDeviceIDs(resource->platform, CL_DEVICE_TYPE_ALL, 1, &resource->device, &return_code);
    fmt::print("Number of devices available: {0}\n", return_code);
    clGetDeviceInfo(resource->device, CL_DEVICE_NAME, 100000, NULL, &ret_code);
    name.resize(ret_code);
    clGetDeviceInfo(resource->device, CL_DEVICE_NAME, ret_code, name.data(), &ret_code);
    fmt::print("Name of the device: {0}\n", name);

    cl_int rt_code;
    resource->context = clCreateContext(NULL, 1, &resource->device, NULL, NULL, &rt_code);
    fmt::print("Context creation return code: {0}\n", rt_code);

    return;

}