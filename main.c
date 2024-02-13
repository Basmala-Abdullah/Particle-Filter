#include <stdio.h>
#include <stdlib.h>
#include "Particle.h"
int main()
{
    Particle particles[NUM_PARTICLES];
    FILE *file;
    Measurement measurements[NUM_MEASUREMENTS]; // Change 100 to the maximum number of lines you expect
    int count = 0;
    //The init function takes the boundries of measurements values (min and max value in measurements)
    int min_x = INT_MAX;
    int min_y = INT_MAX;
    int max_x = INT_MIN;
    int max_y = INT_MIN;

    // Open the file
    file = fopen("LocalizationData.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    // Read the file
    while (fscanf(file, "%lf %lf", &measurements[count].x, &measurements[count].y) == 2) {
        //measuring the minimum and maximum values of measurements
       if(measurements[count].x > max_x){
            max_x = measurements[count].x;
       }
       if(measurements[count].x < min_x){
            min_x = measurements[count].x;
       }
       if(measurements[count].y > max_y){
            max_y = measurements[count].y;
       }
       if(measurements[count].y < min_y){
            min_y = measurements[count].y;
       }
        count++;
        if (count > NUM_MEASUREMENTS) { // Change 100 to the same number above
            break;
        }
    }

    // Close the file
    fclose(file);
    printf("Init step: \n");
    //spread particles in region of the recieved measurements
    initialize_particles(particles, max_x,min_x,max_y,min_y);
    for(int k=0; k<NUM_ITERATIONS;k++){
        printf("Iteration num %d\n",k);
        prediction(particles);
        printf("Prediction step: \n");
        for (int i = 0; i < NUM_PARTICLES; i++) {
            printf("Particle %d: x = %f, y = %f\n", i, particles[i].x, particles[i].y);
        }
        printf("Update step: \n");
        update_particles(particles, measurements);
        printf("Resample step: \n");
        resample(particles);
            for (int i = 0; i < NUM_PARTICLES; i++) {
                printf("Particle %d: x = %f, y = %f, weight: %f\n", i, particles[i].x, particles[i].y,particles[i].weight);
        }
    }


    printf("Estimation step: \n");
    double device_coordinates[2];
    estimate(particles, device_coordinates);
    printf("Device is located at x = %f, and y = %f\n",device_coordinates[0],device_coordinates[1]);

    return 0;
}
