#include <config.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <ocl.h>
#include <viewport.h>
#include <schedular.h>
#include <world.h>


int main(){
    cl_resource resource;
    initialize_resources(&resource);

    cl_int int_ret;
    size_t size_ret;

    Memory memory(&resource);
    create_viewport(&resource, &memory);

    World world(&resource, &memory);

    world.add_sphere({30, 200, 200, 200, 1, 0.6, 0.5, 0});
    world.add_sphere({500, 0, 0, 0, 0.5, 0.3, 0.5, 0});

    world.to_gpu();
    world.bounce();

    float* d = new float[20];
    clEnqueueReadBuffer(resource.queue, memory.color, true, 0, 4*20, d, 0, NULL, NULL);
    for (int i = 0; i < 20; ++i){
        fmt::print("{0}, ", d[i]);
    }
    fmt::print("\n");

    return 0;
}