#define ASPECT_RATIO 2
#define VRES 80
#define APERTURE 100

__kernel void adder(global int* a){
    int block = get_global_id(0);
    int y = get_global_id(1);
    int x = get_global_id(2);
    
    // determine the pixel
    size_t blocky = block / (VRES*ASPECT_RATIO/8);
    size_t blockx = block % (VRES*ASPECT_RATIO/8);
    int offset = (blocky * 8 * VRES*ASPECT_RATIO) + (VRES*ASPECT_RATIO*y) + (blockx * 8) + (x);
    a[offset] = offset;

}
