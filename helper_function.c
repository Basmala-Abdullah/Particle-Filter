
#include "helper_function.h"

int FIRST_TIME = 1;

double generate_normal_random(double mean, double stddev) {
    if(FIRST_TIME){
            srand(time(NULL));
            FIRST_TIME = 0;
    }
    double u = ((double)rand() / (RAND_MAX)) * 2 - 1;
    double v = ((double)rand() / (RAND_MAX)) * 2 - 1;
    double s = u * u + v * v;
    while (s >= 1 || s == 0) {
        u = ((double)rand() / (RAND_MAX)) * 2 - 1;
        v = ((double)rand() / (RAND_MAX)) * 2 - 1;
        s = u * u + v * v;
    }
    double mul = sqrt(-2.0 * log(s) / s);
    return mean + stddev * u * mul;
}

double rand_double_range(double min, double max) {
    return min + (rand() / (RAND_MAX / (max - min)));
}


