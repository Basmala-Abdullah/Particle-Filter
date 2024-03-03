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
    Measurement_Type deviceAvgPosition;
    deviceAvgPosition.x=0;
    deviceAvgPosition.y=0;

    double totalConfidence = 0; // Total confidence for normalization

    //Dummy Values of recieved angles and distances
    //each index referes to anchor 1,2,3 respectively
    // double distances[3] = {3,4,5};
    // double angles[3] = {0.9,1,1.2};

    // Anchor coordinates
    Measurement anchorCoordinates[NUM_OF_ANCHORS] = {
        {-0.2704, 0.26},    // First anchor coordinates
        {-0.2704, -0.26},   // Second anchor coordinates
        {-0.2704, 0}        // Third anchor coordinates
    };

    for(int i=0;i<NUM_OF_ANCHORS; i++){
        object_list *p = &arr_object_list[i];
        if(p!=NULL){
            
            double weight = p->confidence; // Weight based on confidence

            Measurement devicePosition = calculateDevicePosition(anchorCoordinates[i].x,anchorCoordinates[i].y,arr_object_list[i].distance,arr_object_list[i].angle);

            // Update total confidence
            totalConfidence += weight;

            deviceAvgPosition.x+=devicePosition.x;
            deviceAvgPosition.y+=devicePosition.y;
            numberOfMeasurements++;

            deviceAvgPosition.type = p->type;
        }
    }

    // Normalize the average position by the total confidence
    if (totalConfidence > 0) {
        deviceAvgPosition.x /= totalConfidence;
        deviceAvgPosition.y /= totalConfidence;
    }

    // Open the file in append mode ("a")
    FILE *fptr = fopen("LocalizationData2.txt", "a");

    if (fptr == NULL) {
        printf("Error opening file!\n");
        return;
    }

    // Write the value to the file
    fprintf(fptr, "%f %f %d\n", deviceAvgPosition.x, deviceAvgPosition.y,deviceAvgPosition.type);

    // Close the file
    fclose(fptr);
}


