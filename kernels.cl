#define ASPECT_RATIO 2
#define VRES 80
#define APERTURE 100

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
    int pixelx = offset / (VRES*ASPECT_RATIO);
    int pixely = offset % (VRES*ASPECT_RATIO);

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


__kernel void intersector(__global float* intersect, __global float* origin, __global float* direction, __global float* color, __global float* spheres, int sphere_count){
    
    int offset = get_offset();
    float3 x = vload3(0, spheres);

    vstore3(x, offset, color);

    return;
}