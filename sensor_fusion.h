#ifndef SENSOR_FUSION_H_
#define SENSOR_FUSION_H_

#include "helper_function.h"
#include "measurement.h"

#define NUM_OF_ANCHORS 3

typedef unsigned char deviceId_t;

//strcture that represents single element of array that is recieved from CAN_DB
typedef struct object_list
{
    deviceId_t deviceId:4; 
    uint32_t cycleId:25;
	enum type_of_ranging_algorithm type:2;
	uint8_t angle;
	uint16_t distance:11;
	uint8_t accuracy: 2;
	uint8_t confidence:4;
	uint8_t mesgId;
}object_list;

Measurement calculateDevicePosition(float anchorX, float anchorY, float distance, float angleRadians);
void calculateAvgDevicePosition(object_list arr_object_list[NUM_OF_ANCHORS]);


//For RSSI and ToF
void matrix_multiply(double *A, double *B, double *result, int m, int n, int p);
void matrix_transpose(double *A, double *result, int n, int m);
int matrix_inverse(double *A, double *result);
Measurement_Type trilaterate_position(object_list arr_object_list[NUM_OF_ANCHORS]);


#endif /* SENSOR_FUSION_H_ */