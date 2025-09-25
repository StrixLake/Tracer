#include <config.h>
#include <ocl.h>
#include <vector>
#include <schedular.h>

class World{

    std::vector<float> rSpheres;
    std::vector<float> cSpheres;
    std::vector<float> pSpheres;
    int sphere_count;
    int light_count;
    Memory* memory;
    cl_resource* resource;
    
    public:
    World(cl_resource* resoure, Memory* memory);
    

    // each sphere has 12 floats that define
    // its position, color and properties
    // 4 floats for each array
    // position :: [0] = radius, [1] = centerX, [2] = centerY, [3] = centerZ;
    // color :: [0] = Red, [1] = Green, [2] = Blue, [3] = Luminance;
    // properties :: [0] = Reflectivity, [1] = Transparency, [2] = Refractive Index, [3] = {Undecided};
    void add_sphere(std::vector<float> position, std::vector<float> color, std::vector<float> properties);
    
    void to_gpu();

    void bounce();

};