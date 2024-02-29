#include "sensor_fusion.h"

Measurement calculateDevicePosition(double anchorX, double anchorY, double distance, double angleRadians) {

    Measurement devicePosition;

    // Calculate device's coordinates
    devicePosition.x = anchorX + (distance * cos(angleRadians));
    devicePosition.y = anchorY + (distance * sin(angleRadians));
    return devicePosition;
}


void calculateAvgDevicePosition(object_list arr_object_list[NUM_OF_ANCHORS]){
    int numberOfMeasurements =0;
    Measurement deviceAvgPosition;
    deviceAvgPosition.x=0;
    deviceAvgPosition.y=0;

    //Dummy Values of recieved angles and distances
    //each index referes to anchor 1,2,3 respectively
    // double distances[3] = {3,4,5};
    // double angles[3] = {0.9,1,1.2};

    Measurement anchorCoordinates[NUM_OF_ANCHORS];

    //First anchor coordinates
    anchorCoordinates[0].x=-0.2704;
    anchorCoordinates[0].y=0.26;

    //Second anchor coordinates
    anchorCoordinates[1].x=-0.2704;
    anchorCoordinates[1].y=-0.26;

    //Third anchor coordinates
    anchorCoordinates[2].x=-0.2704;
    anchorCoordinates[2].y=0;

    for(int i=0;i<NUM_OF_ANCHORS; i++){
        object_list *p = &arr_object_list[i];
        if(p!=NULL){
            Measurement devicePosition = calculateDevicePosition(anchorCoordinates[i].x,anchorCoordinates[i].y,arr_object_list[i].distance,arr_object_list[i].angle);
            deviceAvgPosition.x+=devicePosition.x;
            deviceAvgPosition.y+=devicePosition.y;
            numberOfMeasurements++;
        }
   
    }
    deviceAvgPosition.x/=(double)numberOfMeasurements;
    deviceAvgPosition.y/=(double)numberOfMeasurements;

    // Open the file in append mode ("a")
    FILE *fptr = fopen("output.txt", "a");

    if (fptr == NULL) {
        printf("Error opening file!\n");
        return;
    }

    // Write the value to the file
    fprintf(fptr, "%f, %f\n", deviceAvgPosition.x, deviceAvgPosition.y);

    // Close the file
    fclose(fptr);
}


