#include <world.h>

World::World(cl_resource* resource, Memory* memory){
    this->memory = memory;
    this->resource = resource;
    this->sphere_count = 0;
    this->light_count = 0;
    return;
}

void World::add_sphere(std::vector<float> sphere){
    this->sphere_count += 1;
    
    if(sphere[7] != 0) this->light_count += 1;

    for(int i = 0; i < sphere.size(); ++i){
        this->spheres.push_back(sphere[i]);
    }
    return;
}

void World::to_gpu(){
    cl_int int_ret;
    memory->spheres = clCreateBuffer(resource->context, CL_MEM_READ_WRITE, this->sphere_count * 8 * sizeof(float), NULL, &int_ret);
    fmt::print("to_gpu create buffer for spheres return code: {0}\n", int_ret);

    int_ret = clEnqueueWriteBuffer(resource->queue, memory->spheres, true, 0, this->sphere_count * 8 * sizeof(float), this->spheres.data(), 0, NULL, NULL);
    fmt::print("to_gpu buffer write return code: {0}\n", int_ret);
    return;
}

void World::bounce(){
    cl_int int_ret;
    cl_kernel kernel = clCreateKernel(resource->program, Kernels::render, &int_ret);
    fmt::print("intersector kernel creation return code: {0}\n", int_ret);

    clSetKernelArg(kernel, 0, sizeof(cl_mem), &memory->intersect);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &memory->origin);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &memory->direction);
    clSetKernelArg(kernel, 3, sizeof(cl_mem), &memory->color);
    clSetKernelArg(kernel, 4, sizeof(cl_mem), &memory->spheres);
    clSetKernelArg(kernel, 5, sizeof(cl_int), &sphere_count);
    clSetKernelArg(kernel, 6, sizeof(cl_int), &light_count);

    cl_event ev = schedule_work(resource, &kernel);
    clWaitForEvents(1, &ev);
    fmt::print("\n");
    clReleaseKernel(kernel);
    return;
}