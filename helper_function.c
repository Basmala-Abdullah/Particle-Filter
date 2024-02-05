
#include "helper_function.h"

bool FIRST_TIME = true;

double generate_normal_random(double mean, double stddev) {
    if(FIRST_TIME){
            srand(time(NULL));
            FIRST_TIME = false;
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
