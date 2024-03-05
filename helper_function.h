
#ifndef     HELPER_FUNCTION_H_
#define     HELPER_FUNCTION_H_

#include  <float.h>
#include  <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>    

#define NUM_ITERATIONS 17

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

double generate_normal_random(double mean, double stddev);
double rand_double_range(double min, double max);


#endif /* HELPER_FUNCTION_H_ */
