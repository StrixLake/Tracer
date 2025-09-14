#include <fstream>
#include <ios>
#include <iterator>
#include <ocl.h>


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
    HANDLE_ERROR(int_ret, "Create program return code");

    int_ret = clBuildProgram(program, 1, (const cl_device_id*)&resource->device, NULL, NULL, NULL);
    HANDLE_ERROR(int_ret, "Build program return code");
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

    int_ret = clGetDeviceInfo(resource->device, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(uint_ret), &uint_ret, NULL);
    fmt::print("device has {0} compute units\n", uint_ret);

    resource->context = clCreateContext(NULL, 1, &resource->device, NULL, NULL, &int_ret);
    HANDLE_ERROR(int_ret, "Context creation return code");

    compile_kernels(resource);

    resource->queue = clCreateCommandQueueWithProperties(resource->context, resource->device, NULL, &int_ret);
    HANDLE_ERROR(int_ret, "Command queue creation return code");
    
    return;

}

void HANDLE_ERROR(int error, std::string name){
    if (error < 0){
        fmt::print("{0}: {1}", name, error);
        exit(1);
    }
    return;
}