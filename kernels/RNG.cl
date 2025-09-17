#include "headers.cl"

float xorshift(int state){
    state ^= state << 10;
	state ^= state >> 16;
	state ^= state << 5;
    return (float)state/(float)0xffffffff;
}
