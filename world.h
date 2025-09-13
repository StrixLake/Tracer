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
    

    // since each sphere has 8 floats that define its properties,
    // the sphere vector is of size 8*sizeof(float)*spherecount bytes
    // and this is how many bytes that need to be written in the gpu buffer
    // [0] = radius, [1] = centerX, [2] = centerY, [3] = centerZ
    // [4] = colorR, [5] = colorG, [6] = colorB, [7] = reflectivity
    void add_sphere(std::vector<float> sphere);
    
    void to_gpu();

    void bounce();

};