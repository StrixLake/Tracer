#include "headers.cl"

float xorshift(uint* state){
    *state ^= *state << 10;
	*state ^= *state >> 16;
	*state ^= *state << 5;
    return (float)*state/(float)0xffffffff;
}

float3 marsaglia(uint state){

    while(true){
        float x1 = xorshift(&state);
        float x2 = xorshift(&state);

        // transform between -1 and 1
        x1 = 2*x1 -1;
        x2 = 2*x2 -1;

        if ((x1*x1 + x2*x2) >= 1){
            continue;
        }
        else{
            float d = sqrt(1- x1*x1 - x2*x2);
            float3 point = {2*x1*d, 2*x2*d, 1-2*(x1*x1 + x2*x2)};
            return point;
        }
        ++state;
    }

}

void randomLight(float* light){

    uint seed1 = get_offset();
    uint seed2 = 2*get_offset();
    uint seed3 = 3*get_offset();
    for(int i = 0; i < NSAMPLE; ++i){
        // generate the 3 random points
        float3 random = {xorshift(&seed1), xorshift(&seed2), xorshift(&seed3)};

        // transforms them between -1 and 1
        random = 2*random -1;

        //float3 random = marsaglia(i*offset + 1);
        
        vstore3(random, i, light);
    }

}
