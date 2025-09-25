#include <world.h>

World::World(cl_resource* resource, Memory* memory){
    this->memory = memory;
    this->resource = resource;
    this->sphere_count = 0;
    this->light_count = 0;
    return;
}

void World::add_sphere(std::vector<float> position, std::vector<float> color, std::vector<float> properties){
    this->sphere_count += 1;
    
    if(color[3] != 0) this->light_count += 1;

    for(int i = 0; i < 4; ++i){
        this->rSpheres.push_back(position[i]);
        this->cSpheres.push_back(color[i]);
        this->pSpheres.push_back(properties[i]);
    }

    return;
}

void World::to_gpu(){
    cl_int int_ret;
    memory->rSpheres = clCreateBuffer(resource->context, CL_MEM_READ_ONLY, this->sphere_count * 4 * sizeof(float), NULL, &int_ret);
    HANDLE_ERROR(int_ret, "to_gpu create position buffer for spheres return code");
    memory->cSpheres = clCreateBuffer(resource->context, CL_MEM_READ_ONLY, this->sphere_count * 4 * sizeof(float), NULL, &int_ret);
    HANDLE_ERROR(int_ret, "to_gpu create color buffer for spheres return code");
    memory->pSpheres = clCreateBuffer(resource->context, CL_MEM_READ_ONLY, this->sphere_count * 4 * sizeof(float), NULL, &int_ret);
    HANDLE_ERROR(int_ret, "to_gpu create properties buffer for spheres return code");

    int_ret = clEnqueueWriteBuffer(resource->queue, memory->rSpheres, true, 0, this->sphere_count * 4 * sizeof(float), this->rSpheres.data(), 0, NULL, NULL);
    HANDLE_ERROR(int_ret, "to_gpu buffer position write return code");
    int_ret = clEnqueueWriteBuffer(resource->queue, memory->cSpheres, true, 0, this->sphere_count * 4 * sizeof(float), this->cSpheres.data(), 0, NULL, NULL);
    HANDLE_ERROR(int_ret, "to_gpu buffer color write return code");
    int_ret = clEnqueueWriteBuffer(resource->queue, memory->pSpheres, true, 0, this->sphere_count * 4 * sizeof(float), this->pSpheres.data(), 0, NULL, NULL);
    HANDLE_ERROR(int_ret, "to_gpu buffer properties write return code");
    return;
}

void World::bounce(){
    cl_int int_ret;
    cl_kernel kernel = clCreateKernel(resource->program, Kernels::render, &int_ret);
    HANDLE_ERROR(int_ret, "Render kernel creation return code");

    clSetKernelArg(kernel, 0, sizeof(cl_mem), &memory->origin);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &memory->direction);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &memory->color);
    clSetKernelArg(kernel, 3, sizeof(cl_mem), &memory->rSpheres);
    clSetKernelArg(kernel, 4, sizeof(cl_mem), &memory->cSpheres);
    clSetKernelArg(kernel, 5, sizeof(cl_mem), &memory->pSpheres);
    clSetKernelArg(kernel, 6, sizeof(cl_int), &sphere_count);
    clSetKernelArg(kernel, 7, sizeof(cl_int), &light_count);

    cl_event ev = schedule_work(resource, &kernel);
    clWaitForEvents(1, &ev);
    fmt::print("\n");
    clReleaseKernel(kernel);
    return;
}