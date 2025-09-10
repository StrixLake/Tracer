#include <iostream>
#include <config.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <ocl.h>
#include <viewport.h>
#include <schedular.h>


int main(){
    cl_resource resource;
    initialize_resources(&resource);

    cl_int int_ret;
    size_t size_ret;

    Memory memory(&resource);
    
    create_viewport(&resource, &memory);

    float* d = new float[20];
    clEnqueueReadBuffer(resource.queue, memory.direction, true, 0, 4*20, d, 0, NULL, NULL);
    for (int i = 0; i < 20; ++i){
        fmt::print("{0}, ", d[i]);
    }

    return 0;
}