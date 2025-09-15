#define ASPECT_RATIO 2
#define VRES 640
#define APERTURE 100
#define LIGHT {-500,-500,1}

// takes the point of intersection of the ray with the the sphere
// returns the color of the pixel
float3 lambert(float3 pointOnSphere, float8 sphere, float intersect);

float shadow(float3 pointOnSphere, __global float* spheres, int sphere_count);

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

__kernel void viewport(__global float* intersect, __global float* origin, __global float* direction, __global float* color, float focal_length){
   
    int offset = get_offset();
    int rbgOffset = 3*offset;

    // focal length is in the z direction

    color[rbgOffset] = 0;
    color[rbgOffset+1] = 0;
    color[rbgOffset+2] = 0;

    intersect[offset] = 0;

    // get the (x,y) form of the pixel from offset
    int pixelx = offset % (VRES*ASPECT_RATIO);
    int pixely = offset / (VRES*ASPECT_RATIO);

    // adjust the position with middle of the viewport in the center
    pixelx = pixelx - (VRES*ASPECT_RATIO/2);
    pixely = pixely - (VRES/2);
    
    float3 pixel = {pixelx, pixely, focal_length};

    float3 normal = normalize(pixel);

    // vstoren computes the 3* offset internally
    // so no need to give it rbgoffset
    vstore3(pixel, offset, origin);

    vstore3(normal, offset, direction);

    return;
}   


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



__kernel void intersector(__global float* intersect, __global float* origin, __global float* direction, __global float* color, __global float* spheres, int sphere_count){
    
    int offset = get_offset();
    
    float3 directioN = vload3(offset, direction);
    float3 origiN = vload3(offset, origin);

    int off_min = 0;
    float d_min = INFINITY;

    for (int i = 0; i < sphere_count; ++i){
        float8 ball = vload8(i, spheres);
        float3 center = {ball.s1, ball.s2, ball.s3};
        float new_d = D(origiN, directioN, center, ball.s0);

        bool is_new_d_smaller = new_d < d_min;
        d_min = select(d_min, new_d, is_new_d_smaller);
        off_min = select(off_min, i, is_new_d_smaller);
    }

    float8 ball = vload8(off_min, spheres);

    // store the pointOnSphere too
    float3 pointOnSphere = origiN + d_min*directioN;

    float3 shade = lambert(pointOnSphere, ball, d_min) * shadow(pointOnSphere, spheres, sphere_count);
    
    vstore3(shade, offset, color);
    
    return;
}


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
