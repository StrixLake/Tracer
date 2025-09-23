#include <headers.cl>

// initializes the viewport with the appropriate
// origin and direction of each ray for the pixel
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
    
    float3 pixel = {pixelx*FOV, pixely*FOV, focal_length};

    float3 normal = normalize(pixel);

    // vstoren computes the 3* offset internally
    // so no need to give it rbgoffset
    vstore3(pixel, offset, origin);

    vstore3(normal, offset, direction);

    return;
}
