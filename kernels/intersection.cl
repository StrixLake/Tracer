#include "headers.cl"

float D(float3 origin, float3 direction, float3 center, float radius){
    float distance;

    // delta = [direction . (origin - center)] ** 2 - [||(origin - center)|| ** 2 - radius ** 2]
    float delta = pow(dot(direction, (origin - center)), 2) - (pow(length(origin - center), 2) - pow(radius, 2));

    bool is_less_than_0 = delta < 0;
    delta = select(delta, INFINITY, is_less_than_0);

    // d = - [direction . (origin - center)] +- sqrt(delta)

    float d1 = -dot(direction, (origin-center)) - sqrt(delta);
    float d2 = -dot(direction, (origin-center)) + sqrt(delta);

    bool is_d1_less_than_0 = d1 < 0;
    bool is_d2_less_than_0 = d2 < 0;
    
    d1 = select(d1, INFINITY, is_d1_less_than_0);
    d2 = select(d2, INFINITY, is_d2_less_than_0);

    distance = min(d1, d2);

    return distance;
}

void nearest_sphere(float3 origin, float3 direction, __global float* spheres, int sphere_count, int* off_min, float* d_min){
    int offset = 0;
    float distance = INFINITY;

    for (int i = 0; i < sphere_count; ++i){
        float8 ball = vload8(i, spheres);
        float3 center = {ball.s1, ball.s2, ball.s3};
        float new_d = D(origin, direction, center, ball.s0);

        bool is_new_d_smaller = new_d < distance;
        distance = select(distance, new_d, is_new_d_smaller);
        offset = select(offset, i, is_new_d_smaller);
    }

    *d_min = distance;
    *off_min = offset;

    return;
}