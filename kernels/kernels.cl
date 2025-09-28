#include "headers.cl"


__kernel void render(__global float* raysOrigin, __global float* raysDirection,
                     __global float* color, __global float* rSpheres, __global float* cSphere, __global float* pSphere,
                     int sphere_count, int light_count){
    
    int offset = get_offset();
    
    // load the direction and origin of the ray
    // for the current pixel
    float3 rayDirection = vload3(offset, raysDirection);
    float3 rayOrigin = vload3(offset, raysOrigin);

    int off_min;
    float d_min;

    // get the nearest sphere ray intersects with
    nearest_sphere(rayOrigin, rayDirection, rSpheres, sphere_count, &off_min, &d_min);

    float4 ball = vload4(off_min, rSpheres);
    float4 ballColor = vload4(off_min, cSphere);
    // early exit if the sphere hit is light sphere
    if(ballColor.s3 != 0){
        vstore3(ballColor.s012, offset, color);
        return;
    }

    // store the pointOnSphere too
    float3 pointOnSphere = rayOrigin + d_min*rayDirection;
    
    float3 shade = softShadow(pointOnSphere, ball, ballColor, rSpheres, cSphere, sphere_count, light_count);

    float4 ballProp = vload4(off_min, pSphere);
    float reflection_coff = ballProp.s0;

    // early exit if reflectivity is 0
    if (reflection_coff == 0) {
        vstore3(shade, offset, color);
        return;
    }

    // reflect the ray
    reflection(&rayOrigin, &rayDirection, pointOnSphere, ball, d_min, &reflection_coff);
    // get the new nearest sphere
    nearest_sphere(rayOrigin, rayDirection, rSpheres, sphere_count, &off_min, &d_min);
    // load the next ball
    ball = vload4(off_min, rSpheres);
    ballColor = vload4(off_min, cSphere);
    // get the new pointOnSphere
    pointOnSphere = rayOrigin + d_min*rayDirection;

    // early exit if the sphere hit in reflection is a light sphere
    if(ballColor.s3 != 0){
        vstore3(reflection_coff*ballColor.s012 + (1-reflection_coff)*shade, offset, color);
        return;
    }
    
    shade = reflection_coff*softShadow(pointOnSphere, ball, ballColor, rSpheres, cSphere, sphere_count, light_count) + (1-reflection_coff)*shade;
    
    vstore3(shade, offset, color);
    
    return;
}


