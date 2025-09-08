#include <iostream>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <ocl.h>


#define ASPECT_RATIO 2
#define VRES 800

int main(){
    cl_resource resource;
    initialize_resources(&resource);

    cl_int int_ret;

    Memory memory;

    memory.intersect = clCreateBuffer(resource.context, CL_MEM_READ_WRITE, VRES*VRES*ASPECT_RATIO*sizeof(float), NULL, &int_ret);
    fmt::print("Intersect matrix allocation return code: {0}\n", int_ret);
    memory.origin = clCreateBuffer(resource.context, CL_MEM_READ_WRITE, VRES*VRES*ASPECT_RATIO*3*sizeof(float), NULL, &int_ret);
    fmt::print("Origin matrix allocation return code: {0}\n", int_ret);
    memory.direction = clCreateBuffer(resource.context, CL_MEM_READ_WRITE, VRES*VRES*ASPECT_RATIO*3*sizeof(float), NULL, &int_ret);
    fmt::print("Direction matrix allocation return code: {0}\n", int_ret);
    memory.color = clCreateBuffer(resource.context, CL_MEM_READ_WRITE, VRES*VRES*ASPECT_RATIO*3*sizeof(float), NULL, &int_ret);
    fmt::print("Color matrix allocation return code: {0}\n", int_ret);


    std::cin >> int_ret;
    return 0;
}