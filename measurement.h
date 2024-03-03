#ifndef    MEASUREMENT_H_
#define    MEASUREMENT_H_

#define NUM_MEASUREMENTS 136


typedef struct Measurement {
    double x;
    double y;
} Measurement;

typedef struct Measurement_Type {
    double x;
    double y;
    int type;
} Measurement_Type;



#endif /* MEASUREMENT_H_ */

