#include <stdio.h>
#include <stdlib.h>
#include "Particle.h"
int main()
{
    Particle particles[NUM_PARTICLES];
    FILE *file;
    Measurement measurements[NUM_MEASUREMENTS]; // Change 100 to the maximum number of lines you expect
    int count = 0;

    // Open the file
    file = fopen("LocalizationData.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    // Read the file
    while (fscanf(file, "%lf %lf", &measurements[count].x, &measurements[count].y) == 2) {
        count++;
        if (count >= 100) { // Change 100 to the same number above
            break;
        }
    }

    // Close the file
    fclose(file);
    printf("Init step: \n");
    initialize_particles(particles);
    for(int k=0; k<NUM_ITERATIONS;k++){
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
