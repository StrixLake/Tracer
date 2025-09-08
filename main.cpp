#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <fmt/base.h>
#include <ocl.h>


int main(){
    cl_resource resource;
    initialize_resources(&resource);
    return 0;
}