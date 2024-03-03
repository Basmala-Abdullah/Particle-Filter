#ifndef SENSOR_FUSION_H_
#define SENSOR_FUSION_H_

#include "helper_function.h"
#include "measurement.h"

#define NUM_OF_ANCHORS 3

//strcture that represents single element of array that is recieved from CAN_DB
typedef struct object_list
{
    int deviceId;
	int anchorId;
    int cycleId;
	int type;
	double angle;
	double distance;
	double confidence;
	int mesgId;
}object_list;

Measurement calculateDevicePosition(double anchorX, double anchorY, double distance, double angleRadians);
void calculateAvgDevicePosition(object_list arr_object_list[NUM_OF_ANCHORS]);


#endif /* SENSOR_FUSION_H_ */