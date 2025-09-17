#include <config.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <ocl.h>
#include <viewport.h>
#include <schedular.h>
#include <world.h>
#include <fmt/ranges.h>

float sRGB(float x){
    float color;
    if (x <= 0.003){
        color = 12.93*x;
    }
    else{
        color = 1.055*pow(x, (float)(1/2.4)) - 0.055;
    }
    return color;
}

int main(){
    cl_resource resource;
    initialize_resources(&resource);

    cl_int int_ret;
    size_t size_ret;

    Memory memory(&resource);
    create_viewport(&resource, &memory);

    World world(&resource, &memory);

    world.add_sphere({250, -500, -500, -1000, 0.5, 1, 0.5, 1});
    world.add_sphere({250, 250, 0, 1000, 0.5, 1, 0.5, 0});
    world.add_sphere({250, -250, 0, 1000, 0.5, 0.5, 1, 0});
    world.add_sphere({25000, 0, 26000, 1400, 1, 0.5, 1, 0});
    world.add_sphere({10000, 0, 0, 0, 1, 0.8, 0.5, 0});

    world.to_gpu();
    world.bounce();

    float* d = new float[VRES*VRES*ASPECT_RATIO*3];
    clEnqueueReadBuffer(resource.queue, memory.color, true, 0, VRES*VRES*ASPECT_RATIO*3*sizeof(float), d, 0, NULL, NULL);
    uint8_t* c = new uint8_t[VRES*VRES*ASPECT_RATIO*3];
    for (int i = 0; i < VRES*VRES*ASPECT_RATIO*3; ++i){
        c[i] = (uint8_t)(sRGB(d[i])*255);
        //fmt::print("{0}, ", d[i]);
    }


    stbi_write_bmp("rendered.bmp", VRES*ASPECT_RATIO, VRES, 3, c);


    return 0;
}