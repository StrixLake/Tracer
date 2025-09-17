#include <fstream>
#include <ios>
#include <iterator>
#include <ocl.h>
#include <vector>


void compile_kernels(cl_resource* resource){
    // load the source code of all kernel files
    std::ifstream kernel("C:\\Users\\yasha\\Documents\\DSA\\Tracer\\kernels\\kernels.cl");
    std::noskipws(kernel);
    std::ifstream viewport("C:\\Users\\yasha\\Documents\\DSA\\Tracer\\kernels\\viewport.cl");
    std::noskipws(viewport);
    std::ifstream shader("C:\\Users\\yasha\\Documents\\DSA\\Tracer\\kernels\\shader.cl");
    std::noskipws(shader);
    std::ifstream RNG("C:\\Users\\yasha\\Documents\\DSA\\Tracer\\kernels\\RNG.cl");
    std::noskipws(RNG);
    std::ifstream intersection("C:\\Users\\yasha\\Documents\\DSA\\Tracer\\kernels\\intersection.cl");
    std::noskipws(intersection);

    std::string kernelSource(std::istream_iterator<char>(kernel), {});
    std::string viewportSource(std::istream_iterator<char>(viewport), {});
    std::string shaderSource(std::istream_iterator<char>(shader), {});
    std::string RNGSource(std::istream_iterator<char>(RNG), {});
    std::string intersectionSource(std::istream_iterator<char>(intersection), {});

    cl_int int_ret;
    cl_uint uint_ret;
    size_t size_ret;
    std::vector<const char*> source = {kernelSource.data(), viewportSource.data(), shaderSource.data(), 
                                        RNGSource.data(), intersectionSource.data()};
    std::vector<size_t> lengths = {kernelSource.size(),  viewportSource.size(), shaderSource.size(), 
                                    RNGSource.size(), intersectionSource.size()};
    
    cl_program program = clCreateProgramWithSource(resource->context, 5, source.data(), lengths.data(), &int_ret);
    HANDLE_ERROR(int_ret, "Create program return code");

    int_ret = clBuildProgram(program, 1, (const cl_device_id*)&resource->device, Kernels::compile_options, NULL, NULL);

    if (int_ret != 0){
        // get build log
        clGetProgramBuildInfo(program, resource->device, CL_PROGRAM_BUILD_LOG, 0, NULL, &size_ret);
        std::string log;
        log.resize(size_ret);
        clGetProgramBuildInfo(program, resource->device, CL_PROGRAM_BUILD_LOG, size_ret, log.data(), &size_ret);
        fmt::print("Build Log:\n{0}\n", log);
    }

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