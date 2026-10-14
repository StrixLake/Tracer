#include "headers.cl"

inline float lngth(float3 a){
    return a.x * a.x + a.y * a.y + a.z * a.z;
}

float D(float3 origin, float3 direction, float3 center, float radius){
    float distance;

    // delta = [direction . (origin - center)] ** 2 - [||(origin - center)|| ** 2 - radius ** 2]
    float t1 = dot(direction, (origin - center));
    float delta = t1*t1 - (lngth(origin - center) - (radius * radius));

    if (delta < 0) delta = INFINITY;

    // d = - [direction . (origin - center)] +- sqrt(delta)

    float d1 = -dot(direction, (origin-center)) - sqrt(delta);
    float d2 = -dot(direction, (origin-center)) + sqrt(delta);

    if (d1 < 0) d1 = INFINITY;
    if (d2 < 0) d2 = INFINITY;

    distance = min(d1, d2);

    return distance;
}

void nearest_sphere(float3 origin, float3 direction, __global float* spheres, int sphere_count, int* off_min, float* d_min){
    int offset = 0;
    float distance = INFINITY;

    for (int i = 0; i < sphere_count; ++i){
        float4 ball = vload4(i, spheres);
        float3 center = ball.s123;
        float new_d = D(origin, direction, center, ball.s0);

        if(new_d < distance)
        {
            distance = new_d;
            offset = i;
        }
    }

    *d_min = distance;
    *off_min = offset;

    return;
}


void reflection(float3* rayOrigin, float3* rayDirection, float3 pointOnSphere, float4 ball, float d_min, float* reflection_coeff){
    float3 normal = normalize(pointOnSphere - ball.s123);
    *rayOrigin = *rayOrigin + (d_min-(0.00001f*ball.s0 + 0.1f))*(*rayDirection);
    float3 incident_ray = *rayDirection;
    *rayDirection = *rayDirection - 2*dot(normal, *rayDirection)*normal;
    // calculate the schlicks approximation
    //*reflection_coeff = *reflection_coeff + (1 - *reflection_coeff)*pown((1 + dot(incident_ray, *rayDirection)), 5);
}
