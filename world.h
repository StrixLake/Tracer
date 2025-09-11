#include <config.h>
#include <ocl.h>
#include <vector>
#include <schedular.h>

class World{

    std::vector<float> spheres;
    int sphere_count;
    Memory* memory;
    cl_resource* resource;

public:
    World(cl_resource* resoure, Memory* memory);

    // [0] = radius, [1] = centerX, [2] = centerY, [3] = centerZ
    // [4] = colorR, [5] = colorG, [6] = colorB, [7] = reflectivity
    void add_sphere(std::vector<float> sphere);

    void to_gpu();

    void bounce();

};