
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#if 1
#define NUM_INPUT 1
unsigned long time[NUM_INPUT] = {
    53916768
};

unsigned long distance[NUM_INPUT] = {
    250133010811025
};
#else
#define NUM_INPUT 1
unsigned long time[NUM_INPUT] = {
    71530
};

unsigned long distance[NUM_INPUT] = {
    940200
};
#endif


int main(void) {
    unsigned long result = 1;
    for(unsigned int i = 0; i < NUM_INPUT; i++) {
        printf("Col: %d\n", i);
        unsigned long duration = time[i];
        unsigned long target = distance[i];
        unsigned long wins = 0;
        for(unsigned int n = 0; n <= duration; n++) {
            unsigned long dist = (duration - n) * n;
            if (dist > target) ++wins;
            //printf("\tCharging %d, distaAnce: %lu, target: %lu wins: %lu\n", n, dist, target, wins );
        }
        result *= wins;
    }
    printf("Result: %lu\n", result);
    return (0);
}
