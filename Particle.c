/*
 * File:   application.c
 * Author:
 *
 *
 */

 #include "Particle.h"



void initialize_particles(Particle particles[NUM_PARTICLES]) {

    float mean_x = 0; // Assuming the car is centered at origin
    float mean_y = 0; // Assuming the car is centered at origin
    float stddev_x = 50; // Broader distribution across the car's width
    float stddev_y = 50; // Broader distribution as device could be up to 50 meters away


    for (int i = 0; i < NUM_PARTICLES; i++) {
        // Generate particles around the center with normal distribution
        particles[i].x = generate_normal_random(mean_x, stddev_x);
        particles[i].y = generate_normal_random(mean_y, stddev_y);
        particles[i].weight = 1.0 / NUM_PARTICLES; // Initialize weights evenly

        particles[i].acceleration =generate_normal_random(mean_x, stddev_x);
        particles[i].velocity =0;

        printf("Particle %d: x = %f, y = %f\n", i, particles[i].x, particles[i].y);
    }


}


void update_particles(Particle particles[NUM_PARTICLES], Measurement measurements[NUM_MEASUREMENTS]){

     double std_x = 0.2; //standard deviation for particle x value - Not sure
     double std_y = 0.2; //standard deviation for particle y value - Not sure
     double weight_sum = 0.0; //used for weight normalizing

     for(int i=0; i<NUM_PARTICLES; i++){
         Particle *p = &particles[i];
         double wt = 1.0;
         double distance_min = DBL_MAX;
         Measurement nearestPointToParticle;
         for(int j=0; j<NUM_MEASUREMENTS; j++){
             double distance = sqrt(pow((measurements[j].x-particles[i].x),2)+pow((measurements[j].y-particles[i].y),2)); //Euclidean distance equation
             if(distance < distance_min){
                distance_min= distance;
                nearestPointToParticle = measurements[j];
             }
         }
         double numerator = exp(-0.5 * (pow((nearestPointToParticle.x - particles[i].x), 2) / pow(std_x, 2) + pow((nearestPointToParticle.y - particles[i].y), 2) / pow(std_y, 2)));
         double denominator = 2 * M_PI * std_x * std_y;

        wt *= (numerator/denominator); //Multivariate Gaussian Distribution
        p->weight = wt;
        weight_sum += wt;
     }

     //Normalize Weights
    for (int i = 0; i < NUM_PARTICLES; i++) {
        Particle *p = &particles[i];
        p->weight /= weight_sum;
    }
}


void prediction(Particle particles[NUM_PARTICLES]){
    double std_x = 1.5; //standard deviation for particle x value
    double std_y = 1.5; //standard deviation for particle y value
    int time =1; //time = 1 sec as we got the std_dev value based on human speed that is measured in seconds
        for(int i=0; i<NUM_PARTICLES; ++i){
            Particle *p = &particles[i]; // get address of particle to update

            double errorX = generate_normal_random(0, std_x); //we can enter std_x or std_y
            double new_acc_x = p->acceleration + error;
            double new_vel = p->velocity + new_acc * time;
            double new_x = p->x +
        }


}




