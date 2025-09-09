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

    return 0;
}