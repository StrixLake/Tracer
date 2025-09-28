#ifndef HEADER
#define HEADER

#define ASPECT_RATIO 2
#define VRES 1280
#define FOV 0.5
#define NSAMPLE 10000
#define LIGHT {-500,-500,1}

// takes the point of intersection of the ray with the the sphere
// returns the color of the pixel
float3 lambert(float3 pointOnSphere, float8 sphere, float intersect);

// returns 0 or 1 of the pointOnSphere intersects with a sphere before the light point
float shadow(float3 pointOnSphere, __global float* spheres, int sphere_count);

// takes the ray direction & origin and the array of spheres
// and returns the d_min and offset of the nearest sphere
void nearest_sphere(float3 origin, float3 direction, __global float* spheres, int sphere_count, int* off_min, float* d_min);

// gives the distance a ray and a sphere of radius
float D(float3 origin, float3 direction, float3 center, float radius);

// gives a random floating point number between 0 and 1;
float xorshift(uint* state);

// populates the ligth array of size 3*SAMPLE with
// random light points around a unit sphere
void randomLight(float* light);

// takes the pointOnSphere (the sphere that is visible to viewport), the center of that sphere,
// and a list of
// all spheres, and calculate the soft shadow on that point
// it is assumed that the light spheres are the first on the list
float3 softShadow(float3 pointOnSphere, float4 ball, float4 ballColor, __global float* rSpheres, __global float* cSpheres, int sphere_count, int lights_count);

// takes the rayOrigin and rayDirection of the ray that is to be reflected
// and ball and d_min from the ball the ray is to be reflected
void reflection(float3* rayOrigin, float3* rayDirection, float3 pointOnSphere, float4 ball, float d_min, float* reflection_coeff);

// takes all the same params as reflection and changes the direction and origin of the ray
// after it refracts and exits from the transparent sphere
void refraction(float3* rayOrigin, float3* rayDirection, float3 pointOnSphere, float4 ball, float d_min, float refractive_index);

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
