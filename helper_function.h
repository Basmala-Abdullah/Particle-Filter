
#ifndef     HELPER_FUNCTION_H_
#define     HELPER_FUNCTION_H_

#include  <float.h>
#include  <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>    
#include "std_types.h"
#define NUM_ITERATIONS 17

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

float generate_normal_random(float mean, float stddev);
uint32_t rand_double_range(uint32_t min, uint32_t max);


#endif /* HELPER_FUNCTION_H_ */
