#include "headers.cl"

float3 lambert(float3 pointOnSphere, float8 sphere, float intersect){
    
    // calculate the normal of the sphere on that point
    float3 center = {sphere.s1, sphere.s2, sphere.s3};
    float3 normal = normalize(pointOnSphere - center);

    float3 light_point = LIGHT;
    float3 direction = normalize(light_point - pointOnSphere);
    
    
    // calculate lambert intensity
    float brightness = dot(direction, normal);
    brightness = select(brightness, (float)0, brightness < 0);
    
    float3 color = {sphere.s4, sphere.s5, sphere.s6};
    color = color*brightness;
    
    return color;
}

float shadow(float3 pointOnSphere, __global float* spheres, int sphere_count){

    // get the vector from point on sphere to the light point
    float3 light = LIGHT;
    float3 light_vector = normalize(light - pointOnSphere);
    pointOnSphere += light_vector*(float)0.01;

    // find the closest sphere between point on sphere and light point
    int off_min = 0;
    float d_min = INFINITY;

    for (int i = 0; i < sphere_count; ++i){
        float8 ball = vload8(i, spheres);
        float3 center = {ball.s1, ball.s2, ball.s3};
        float new_d = D(pointOnSphere, light_vector, center, ball.s0);

        bool is_new_d_smaller = new_d < d_min;
        d_min = select(d_min, new_d, is_new_d_smaller);
        off_min = select(off_min, i, is_new_d_smaller);
    }

    // get the point at that sphere
    float3 shadowPoint = pointOnSphere + d_min*light_vector;

    // get the vector from light point to shadow point
    shadowPoint = shadowPoint - light;

    // get the dot product of shadowPoint and light vector
    bool in_between = dot(shadowPoint, light_vector) < 0;
    
    return select(1, 0, in_between);
}
