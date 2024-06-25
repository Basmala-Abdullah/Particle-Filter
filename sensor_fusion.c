#include "sensor_fusion.h"

Measurement calculateDevicePosition(float anchorX, float anchorY, float distance, float angleRadians) {

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

    float totalConfidence = 0; // Total confidence for normalization

    //Dummy Values of recieved angles and distances
    //each index referes to anchor 1,2,3 respectively
    // double distances[3] = {3,4,5};
    // double angles[3] = {0.9,1,1.2};

    // Anchor coordinates
    Measurement anchorCoordinates[NUM_OF_ANCHORS] = {
        {-0.2704, 0.26},    // First anchor coordinates
        {-0.2704, -0.26},   // Second anchor coordinates
        {0.2704, 0}        // Third anchor coordinates
    };

    for(int i=0;i<NUM_OF_ANCHORS; i++){
        object_list *p = &arr_object_list[i];
        if(p!=NULL){
            
            float weight = p->confidence; // Weight based on confidence

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

// Function to multiply two matrices A[m x n] and B[n x p] to result[m x p]
void matrix_multiply(double *A, double *B, double *result, int m, int n, int p) {
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < p; j++) {
            double sum = 0;
            for(int k = 0; k < n; k++) {
                sum += A[i*n + k] * B[k*p + j];
            }
            result[i*p + j] = sum;
        }
    }
}

// Function to transpose a matrix A[n x m] to result[m x n]
void matrix_transpose(double *A, double *result, int n, int m) {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            result[j*n + i] = A[i*m + j];
        }
    }
}

// Function to invert a 2x2 matrix
int matrix_inverse(double *A, double *result) {
    double det = A[0] * A[3] - A[1] * A[2];
    //printf("A[0]=%f, A[3]=%f, A[1]=%f, A[2]=%f, det = %f\n", A[0], A[3], A[1], A[2], det);

    if(det == 0) return -1; // Singular matrix, can't invert

    double invDet = 1.0 / det;
    result[0] = A[3] * invDet;
    result[1] = -A[1] * invDet;
    result[2] = -A[2] * invDet;
    result[3] = A[0] * invDet;
    return 0;
}

// Function to solve the least squares problem for trilateration
Measurement_Type trilaterate_position(object_list arr_object_list[NUM_OF_ANCHORS]) {
    Measurement_Type devicePosition;
    for(int i=0;i<NUM_OF_ANCHORS; i++){
        object_list *p = &arr_object_list[i];
        if(p==NULL){
            devicePosition.x=-1;
            devicePosition.y=-1;
            devicePosition.type=-1;
            return devicePosition;
        }
    }
    double A[NUM_OF_ANCHORS*2], B[NUM_OF_ANCHORS], AT[2*NUM_OF_ANCHORS], ATA[4], ATB[2], ATA_inv[4];

    // Anchor coordinates
    Measurement anchorCoordinates[NUM_OF_ANCHORS] = {
        {-0.2704-2, 0},    // First anchor coordinates
        {0-2, 0.2704},   // Second anchor coordinates
        {0-2, 0.2704}
    };

    // Anchor coordinates
    //  Measurement anchorCoordinates[NUM_OF_ANCHORS] = {
    //     {-0.2704 - 3, 0.26},    // Master Node coordinates
    //     {-0.2704 + 3, -0.26 - 3},   // Second anchor coordinates
    //     {0.2704 + 3, 0}        // Third anchor coordinates
    // };

    //(x-x1)^2+(y-y1^2)=r^2
    // Construct matrices A and B
    for(int i = 0; i < NUM_OF_ANCHORS; i++) {
        A[i*2] = 2 * (anchorCoordinates[i].x - anchorCoordinates[NUM_OF_ANCHORS-1].x);
        A[i*2 + 1] = 2 * (anchorCoordinates[i].y - anchorCoordinates[NUM_OF_ANCHORS-1].y);
        B[i] = pow(arr_object_list[i].distance, 2) - pow(arr_object_list[NUM_OF_ANCHORS-1].distance, 2)
               - pow(anchorCoordinates[i].x, 2) - pow(anchorCoordinates[i].y, 2)
               + pow(anchorCoordinates[NUM_OF_ANCHORS-1].x, 2) + pow(anchorCoordinates[NUM_OF_ANCHORS-1].y, 2);
    }

    // for(int i=0;i<NUM_OF_ANCHORS*2;i++){
    //     printf("A[%d]=%f ",i,A[i]);
    // }
    // printf("\n");
    // Compute transposition of A to AT
    matrix_transpose(A, AT, NUM_OF_ANCHORS, 2);
    // for(int i=0;i<2*NUM_OF_ANCHORS;i++){
    //     printf("AT[%d]=%f ",i,AT[i]);
    // }
    // printf("\n");
    // Compute ATA and ATB for normal equations
    matrix_multiply(AT, A, ATA, 2, NUM_OF_ANCHORS, 2);
    matrix_multiply(AT, B, ATB, 2, NUM_OF_ANCHORS, 1);
    // for(int i=0;i<4;i++){
    //     printf("ATA[%d]=%f ",i,ATA[i]);
    // }
    // printf("\n");
    // for(int i=0;i<2;i++){
    //     printf("ATB[%d]=%f ",i,ATB[i]);
    // }
    // printf("\n");
    // Invert ATA
    if(matrix_inverse(ATA, ATA_inv) != 0) {
        fprintf(stderr, "Matrix inversion failed. Cannot compute location.\n");
            devicePosition.x=-1;
            devicePosition.y=-1;
            devicePosition.type=-1;
            return devicePosition;
    }

    // Compute least squares solution
    double solution[2];
    matrix_multiply(ATA_inv, ATB, solution, 2, 2, 1);

    devicePosition.x=solution[0]*-1;
    devicePosition.y=solution[1]*-1;
    devicePosition.type=arr_object_list[0].type;

    /////...............Writting in file for testing.............../////

    // Open the file in append mode ("a")
    FILE *fptr = fopen("LocalizationData2.txt", "a");

    // if (fptr == NULL) {
    //     printf("Error opening file!\n");
    //     devicePosition.x=-1;
    //     devicePosition.y=-1;
    //     devicePosition.type=-1;
    // }

    // Write the value to the file
    printf("--------------------------------------------------------------------------writing in the file\n");
    fprintf(fptr, "%f %f %d\n", devicePosition.x, devicePosition.y,devicePosition.type);

    // Close the file
    fclose(fptr);
    /////////////////////////////////////////////////////////////////////

    return devicePosition;
}

