#ifndef    MEASUREMENT_H_
#define    MEASUREMENT_H_

#include "std_types.h"

#define NUM_MEASUREMENTS 135


enum type_of_ranging_algorithm {
    Rssi,
    Tof,
    ChannelSounding,
};

typedef struct Measurement {
    float x;
    float y;
} Measurement;

typedef struct Measurement_Type {
    float x;
    float y;
    enum type_of_ranging_algorithm type;
} Measurement_Type;



#endif /* MEASUREMENT_H_ */

