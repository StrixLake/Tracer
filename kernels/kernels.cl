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
    // early exit if the sphere hit is light sphere
    if(ball.s7 != 0){
        vstore3(ball.s456, offset, color);
        return;
    }

    // store the pointOnSphere too
    float3 pointOnSphere = rayOrigin + d_min*rayDirection;
    
    float3 shade = softShadow(pointOnSphere, ball, spheres, sphere_count, light_count);

    // reflect the ray
    reflection(&rayOrigin, &rayDirection, pointOnSphere, ball, d_min);
    // get the new nearest sphere
    nearest_sphere(rayOrigin, rayDirection, spheres, sphere_count, &off_min, &d_min);
    // load the next ball
    ball = vload8(off_min, spheres);
    // get the new pointOnSphere
    pointOnSphere = rayOrigin + d_min*rayDirection;

    // early exit if the sphere hit in reflection is a light sphere
    if(ball.s7 != 0){
        vstore3((float)0.5*ball.s456 + (float)0.5*shade, offset, color);
        return;
    }
    
    shade = (float)0.5*softShadow(pointOnSphere, ball, spheres, sphere_count, light_count) + (float)0.5*shade;
    
    vstore3(shade, offset, color);
    
    return;
}


