#ifndef HEADER
#define HEADER

#define ASPECT_RATIO 2
#define VRES 640
#define APERTURE 100
#define LIGHT {-500,-500,1}

// takes the point of intersection of the ray with the the sphere
// returns the color of the pixel
float3 lambert(float3 pointOnSphere, float8 sphere, float intersect);

// returns 0 or 1 of the pointOnSphere intersects with a sphere before the light point
float shadow(float3 pointOnSphere, __global float* spheres, int sphere_count);

// takes the ray direction & origin and the array of spheres
// and returns the d_min and offset of the nearest sphere
void nearest_sphere(float3 origin, float3 direction, __global float* spheres, int sphere_count, int* off_minO, float* d_minO);

float D(float3 origin, float3 direction, float3 center, float radius);

float xorshift(int state);

int get_offset(){
    int block = get_global_id(0);
    int y = get_global_id(1);
    int x = get_global_id(2);
    

    // determine the pixel
    size_t blocky = block / (VRES*ASPECT_RATIO/8);
    size_t blockx = block % (VRES*ASPECT_RATIO/8);
    int offset = (blocky * 8 * VRES*ASPECT_RATIO) + (VRES*ASPECT_RATIO*y) + (blockx * 8) + (x);
    return offset;
}

#endif
