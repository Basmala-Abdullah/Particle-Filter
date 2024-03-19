#include "helper_function.h"

int FIRST_TIME = 1;

// uint32_t generate_normal_random(uint32_t mean, uint32_t  stddev) {
//     if(FIRST_TIME){
//             srand(time(NULL));
//             FIRST_TIME = 0;
//     }
//     uint32_t  u = ((uint32_t )rand() / (RAND_MAX)) * 2 - 1;
//     uint32_t  v = ((uint32_t )rand() / (RAND_MAX)) * 2 - 1;
//     uint32_t  s = u * u + v * v;
//     while (s >= 1 || s == 0) {
//         u = ((uint32_t)rand() / (RAND_MAX)) * 2 - 1;
//         v = ((uint32_t)rand() / (RAND_MAX)) * 2 - 1;
//         s = u * u + v * v;
        
//     }
//     uint32_t mul = sqrt(-2.0 * log(s) / s);
//     return mean + stddev * u * mul;
// }

// uint32_t generate_normal_random(uint32_t mean, uint32_t stddev) {
//     if (FIRST_TIME) {
//         srand(time(NULL));
//         FIRST_TIME = 0;
//     }

//     double u, v, s;
//     do {
//         // Generate u and v in the range [-1, 1]
//         u = ((double)rand() / RAND_MAX) * 2 - 1;
//         v = ((double)rand() / RAND_MAX) * 2 - 1;

//         // Calculate the squared radius
//         s = u * u + v * v;
//         //printf("infinite loop");
//     } while (s >= 1 || s == 0);

//     // Box-Muller transform
//     double mul = sqrt(-2.0 * log(s) / s);
//     double z = mean + stddev * u * mul;

//     // Return the rounded value as uint32_t
//     return (uint32_t)round(z);
// }
float generate_normal_random(float mean, float stddev) {
    if(FIRST_TIME){
            srand(time(NULL));
            FIRST_TIME = 0;
    }
    float u = ((float)rand() / (RAND_MAX)) * 2 - 1;
    float v = ((float)rand() / (RAND_MAX)) * 2 - 1;
    float s = u * u + v * v;
    while (s >= 1 || s == 0) {
        u = ((float)rand() / (RAND_MAX)) * 2 - 1;
        v = ((float)rand() / (RAND_MAX)) * 2 - 1;
        s = u * u + v * v;
    }
    float mul = sqrt(-2.0 * log(s) / s);
    return mean + stddev * u * mul;
}


uint32_t rand_double_range(uint32_t min, uint32_t max) {
    return min + (rand() / (RAND_MAX / (max - min)));
}


