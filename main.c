#include <stdio.h>
#include <stdlib.h>
#include "Particle.h"
#include "sensor_fusion.h"


object_list arr_object_list[NUM_OF_ANCHORS];


void plot_particles(Particle particles[], int num_particles, const char* title) {
    FILE *gnuplotPipe = popen("gnuplot -persistent", "w");
    if (gnuplotPipe == NULL) {
        fprintf(stderr, "Error opening pipe to GNU plot.\n");
        exit(1);
    }

    fprintf(gnuplotPipe, "set title '%s'\n", title);
    fprintf(gnuplotPipe, "plot '-' with points pointtype 7 notitle\n");
    for (int i = 0; i < num_particles; i++) {
        fprintf(gnuplotPipe, "%f %f\n", particles[i].x, particles[i].y);
    }
    fprintf(gnuplotPipe, "e\n");
    fflush(gnuplotPipe);
    pclose(gnuplotPipe);
}

void plot_final_estimation(double x, double y, const char* title) {
    FILE *gnuplotPipe = popen("gnuplot -persistent", "w");
    if (gnuplotPipe == NULL) {
        fprintf(stderr, "Error opening pipe to GNU plot.\n");
        exit(1);
    }

    // Set plot title and labels
    fprintf(gnuplotPipe, "set title '%s'\n", title);
    fprintf(gnuplotPipe, "set xlabel 'X Coordinate'\n");
    fprintf(gnuplotPipe, "set ylabel 'Y Coordinate'\n");

    // Set point style
    fprintf(gnuplotPipe, "plot '-' u 1:2 with points pt 7 lc rgb 'red' title 'Final Estimation'\n");

    // Plot the final estimation point
    fprintf(gnuplotPipe, "%f %f\n", x, y);
    
    // Tell GNU Plot that you're done sending data
    fprintf(gnuplotPipe, "e\n");

    // Flush the pipe to make sure GNU Plot receives all commands
    fflush(gnuplotPipe);

    // Close the pipe to GNU Plot
    pclose(gnuplotPipe);
}

int main()
{
    //First Anchor
    arr_object_list[0].deviceId = 15;
//    arr_object_list[0].anchorId =0x123;
    arr_object_list[0].cycleId = 0;
    arr_object_list[0].type = 0;
    arr_object_list[0].angle= 1.2;
    arr_object_list[0].distance = 5;
    arr_object_list[0].confidence=0.5;
    arr_object_list[0].mesgId=0;

    //Second Anchor
    arr_object_list[1].deviceId = 15;
//    arr_object_list[1].anchorId =0x123;
    arr_object_list[1].cycleId = 0;
    arr_object_list[1].type = 0;
    arr_object_list[1].angle= 1.0;
    arr_object_list[1].distance = 10;
    arr_object_list[1].confidence=0.5;
    arr_object_list[1].mesgId=0;

    //Third Anchor
    arr_object_list[2].deviceId = 15;
//    arr_object_list[2].anchorId =0x123;
    arr_object_list[2].cycleId = 0;
    arr_object_list[2].type = 0;
    arr_object_list[2].angle= 1.6;
    arr_object_list[2].distance = 15;
    arr_object_list[2].confidence=0.5;
    arr_object_list[2].mesgId=0;

    calculateAvgDevicePosition(arr_object_list);

    Particle particles[NUM_PARTICLES];
    FILE *file;
    Measurement_Type measurements[NUM_MEASUREMENTS]; 
    uint32_t count = 0;
    //The init function takes the boundries of measurements values (min and max value in measurements)
    uint32_t min_x = 51;
    uint32_t min_y = 51;
    uint32_t max_x = -51;
    uint32_t max_y = -51;

    // Open the file
    file = fopen("LocalizationData2.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    // Read the file
    while (fscanf(file, "%f %f %d", &measurements[count].x, &measurements[count].y,  &measurements[count].type) == 3) {
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
        // if (count > NUM_MEASUREMENTS) { // Change 100 to the same number above
        //     break;
        // }
    }

    // Close the file
    fclose(file);
    printf("Init step: \n");
    //spread particles in region of the recieved measurements
    initialize_particles(particles);
#if PLOT_GRAPH
    plot_particles(particles, NUM_PARTICLES, "Initial Particle Distribution");
#endif


    for(uint32_t k=0; k<NUM_ITERATIONS;k++){
        printf("Iteration num %d\n",k);
        prediction(particles);
#if PLOT_GRAPH
        // Plot after prediction
        char title[50];
        sprintf(title, "Particle Distribution After Prediction %d", k);
        plot_particles(particles, NUM_PARTICLES, title);
#endif


        printf("Prediction step: \n");
        for (uint32_t i = 0; i < NUM_PARTICLES; i++) {
            printf("Particle %d: x = %f, y = %f\n", i, particles[i].x, particles[i].y);
        }
        printf("Update step: \n");
        update_particles(particles, measurements);
#if PLOT_GRAPH
        // Plot after update
        sprintf(title, "Particle Distribution After Update %d", k);
        plot_particles(particles, NUM_PARTICLES, title);
#endif

        printf("Resample step: \n");
        resample(particles);
#if PLOT_GRAPH
        // Plot after resampling
        sprintf(title, "Particle Distribution After Resampling %d", k);
        plot_particles(particles, NUM_PARTICLES, title);
#endif

            for (uint32_t i = 0; i < NUM_PARTICLES; i++) {
                printf("Particle %d: x = %f, y = %f, weight: %f\n", i, particles[i].x, particles[i].y,particles[i].weight);
        }
    }


    printf("Estimation step: \n");
    float device_coordinates[2];
    estimate(particles, device_coordinates);
#if PLOT_GRAPH
    // Plot final estimation
    plot_final_estimation(device_coordinates[0], device_coordinates[1], "Estimation");
#endif
    printf("Device is located at x = %f, and y = %f\n",device_coordinates[0],device_coordinates[1]);
    return 0;
}