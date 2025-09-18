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
    int off_min;
    float d_min;

    // get the nearest sphere in the light vector direction
    nearest_sphere(pointOnSphere, light_vector, spheres, sphere_count, &off_min, &d_min);

    // get the point at that sphere
    float3 shadowPoint = pointOnSphere + d_min*light_vector;

    // get the vector from light point to shadow point
    shadowPoint = shadowPoint - light;

    // get the dot product of shadowPoint and light vector
    bool in_between = dot(shadowPoint, light_vector) < 0;
    
    return select(1, 0, in_between);
}


// takes the pointOnSphere (the sphere that is visible to viewport), the sphere itself,
// and a list of
// all spheres, and calculate the soft shadow on that point
// it is assumed that the light spheres are the first on the list
float3 softShadow(float3 pointOnSphere, float8 ball, __global float* spheres, int sphere_count, int lights_count){
    float3 shade = 0;

    // get normal from center to pointOnSphere
    float3 normal = normalize(pointOnSphere - ball.s123);

    float lights[3*NSAMPLE];

    // initialize the random points between [0,1]
    randomLight(lights);
    

    for(int i = 0; i < lights_count; ++i){
        float8 light_sphere = vload8(i, spheres);
        // for every light sphere, find the nearest sphere
        // from the pointOnSphere in the direction of that random point
        for (int j = 0; j < NSAMPLE; ++j){
            float3 randomPoint = vload3(j, lights);
            // scale the point
            randomPoint = randomPoint*light_sphere.s0;
            randomPoint += light_sphere.s123;

            // get the light vector
            randomPoint = normalize(randomPoint - pointOnSphere);
            // push pointOnSphere slightly towards randomPoint
            pointOnSphere += randomPoint*(float)0.1;
            // we already know that the pointOnSphere will
            // intersect any sphere in the direction of random point
            // so we will take the lamber color ahead of time anyway

            float brightness = dot(randomPoint, normal);
            brightness = select(brightness, (float)0, brightness < 0);

            
            int off_min;
            float d_min;
            nearest_sphere(pointOnSphere, randomPoint, spheres, sphere_count, &off_min, &d_min);

            float8 light_ball = vload8(off_min, spheres);

            shade += ball.s456*brightness*light_ball.s7;

            

        }
    }
    return shade/(NSAMPLE*lights_count);
}
