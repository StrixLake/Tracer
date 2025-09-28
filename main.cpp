#include <config.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <ocl.h>
#include <viewport.h>
#include <schedular.h>
#include <world.h>
#include <fmt/ranges.h>
#include <fmt/chrono.h>
#include <chrono>

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

    // green sphere
    world.add_sphere({250, 250, 75, 2000}, {0.5, 1, 0.5, 1}, {0, 0, 0, 0});
    // white sphere behind the camera
    world.add_sphere({250, -1000, -1000, 100}, {1, 1, 1, 0}, {0.3, 0, 0, 0});
    // blue sphere besides the green sphere
    world.add_sphere({250, -250, 75, 2000}, {0.5, 0.5, 1, 0}, {0.5, 0, 0, 0});
    // small purple sphere on the left
    world.add_sphere({50, -500, 280, 1400}, {0.4, 0.1, 0.8, 0}, {0.3, 0, 0, 0});
    // small blue on the right
    world.add_sphere({100, 250, 230, 1200}, {0.3, 0.7, 1, 0}, {0.2, 0, 0, 0});
    // world sphere
    world.add_sphere({10000, 0, 0, 0}, {1, 0.8, 0.5, 0}, {0, 0, 0, 0});
    // floor sphere
    world.add_sphere({3500000, 0, 3500000+325, 6000}, {1, 1, 1, 0}, {0.9, 0, 0, 0});
    
    world.to_gpu();
    
    const std::chrono::time_point<std::chrono::high_resolution_clock> t1 = std::chrono::high_resolution_clock::now();
    
    world.bounce();
    
    const std::chrono::time_point<std::chrono::high_resolution_clock> t2 = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double> render_time = t2 - t1;

    fmt::print("render time: {0}\n", render_time);

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