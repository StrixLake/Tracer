#include "headers.cl"


__kernel void render(__global float* intersect, __global float* originArray, __global float* directionArray,
                          __global float* color, __global float* spheres, int sphere_count, int light_count){
    
    int offset = get_offset();
    
    // load the direction and origin of the ray
    // for the current pixel
    float3 rayDirection = vload3(offset, directionArray);
    float3 rayOrigin = vload3(offset, originArray);

    int off_min;
    float d_min;

    // get the nearest sphere ray intersects with
    nearest_sphere(rayOrigin, rayDirection, spheres, sphere_count, &off_min, &d_min);

    float8 ball = vload8(off_min, spheres);

    // store the pointOnSphere too
    float3 pointOnSphere = rayOrigin + d_min*rayDirection;

    float3 shade = lambert(pointOnSphere, ball, d_min) * shadow(pointOnSphere, spheres, sphere_count);
    
    vstore3(shade, offset, color);
    
    return;
}


