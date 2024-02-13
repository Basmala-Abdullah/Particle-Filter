/*
 * File:   application.c
 * Author:
 *
 *
 */

 #include "Particle.h"
 #define BLE_RANGE 50

//// Add a function to clamp values within a range
//double clamp(double value, double min, double max) {
//    if (value < min) return min;
//    if (value > max) return max;
//    return value;
//}

void initialize_particles(Particle particles[NUM_PARTICLES]) {

    double mean_x = 0; // Assuming the car is centered at origin
    double mean_y = 0; // Assuming the car is centered at origin
    double stddev_x = BLE_RANGE; // Broader distribution across the car's width
    double stddev_y = BLE_RANGE; // Broader distribution as device could be up to 50 meters away

    double count = 0;
    for (int i = 0; i < NUM_PARTICLES; i++) {
        do {
            particles[i].x = generate_normal_random(mean_x, stddev_x);
            particles[i].y = generate_normal_random(mean_y, stddev_y);
        } while (fabs(particles[i].x) > BLE_RANGE || fabs(particles[i].y) > BLE_RANGE);

        particles[i].weight = 1.0 / NUM_PARTICLES; // Initialize weights evenly
        particles[i].accelerationX =generate_normal_random(mean_x, stddev_x);
        particles[i].accelerationY =generate_normal_random(mean_x, stddev_y);
        particles[i].velocityX =0;
        particles[i].velocityY =0;

        printf("Particle %d: x = %f, y = %f, weight: %f\n", i, particles[i].x, particles[i].y,particles[i].weight);

        if(particles[i].x >60 || particles[i].y>60){
            count++;
        }
    }
    printf("Out of range particles: %f percentage \n", (count/NUM_PARTICLES)*100);


}


void update_particles(Particle particles[NUM_PARTICLES], Measurement measurements[NUM_MEASUREMENTS]){

<<<<<<< Updated upstream
     double std_x = 0.2; //standard deviation for particle x value - Not sure
     double std_y = 0.2; //standard deviation for particle y value - Not sure
=======
      double std_x = 0.2; //standard deviation for particle x value - Not sure
      double std_y = 0.2; //standard deviation for particle y value - Not sure
>>>>>>> Stashed changes
     double weight_sum = 0.0; //used for weight normalizing
    double wt =1.0;
     for(int i=0; i<NUM_PARTICLES; i++){
         Particle *p = &particles[i];
         wt = 1.0;
         double distance_min = DBL_MAX;
         Measurement nearestPointToParticle;
         for(int j=0; j<NUM_MEASUREMENTS; j++){
             double distance = sqrt(pow((measurements[j].x-particles[i].x),2)+pow((measurements[j].y-particles[i].y),2)); //Euclidean distance equation
             if(distance < distance_min){
                distance_min= distance;
                nearestPointToParticle = measurements[j];
             }
         }
         //--------------------------Gaussian PDF-------------------

//         double numerator = exp(-0.5 * (pow((nearestPointToParticle.x - particles[i].x), 2) / pow(std_x, 2) + pow((nearestPointToParticle.y - particles[i].y), 2) / pow(std_y, 2)));
//         double denominator = 2 * M_PI * std_x * std_y;
//
//        printf("num=%f, denom = %f",numerator,denominator);

       //--------------------------Euclidean Distance-------------------

//        double numerator =1;
//        double denominator = sqrt(pow((nearestPointToParticle.x - particles[i].x), 2) + pow((nearestPointToParticle.y - particles[i].y), 2));
//        printf("num=%f, denom = %f",numerator,denominator);


//-------------------------------Inverse Distance weighting------------------------------------

        double numerator =1;
<<<<<<< Updated upstream
        double denominator = sqrt(pow((nearestPointToParticle.x - particles[i].x), 2) + pow((nearestPointToParticle.y - particles[i].y), 2));
=======
        double denominator = pow(sqrt(pow((nearestPointToParticle.x - particles[i].x), 2) + pow((nearestPointToParticle.y - particles[i].y), 2)),3);
>>>>>>> Stashed changes
        printf("num=%f, denom = %f",numerator,denominator);
        wt *= (numerator/denominator);
        p->weight = wt;
        weight_sum += wt;

        printf("Particle %d: x = %f, y = %f, weight: %f\n", i, particles[i].x, particles[i].y,particles[i].weight);
     }

     //Normalize Weights
    for (int i = 0; i < NUM_PARTICLES; i++) {
        Particle *p = &particles[i];
        p->weight /= weight_sum;

    }
    double total_weight=0;
    for (int i = 0; i < NUM_PARTICLES; i++) {

        total_weight+=particles[i].weight;

    }
    printf("Total weight of particles: %f\n",total_weight);
}


void prediction(Particle particles[NUM_PARTICLES]){
    double std_x = 1.5; //standard deviation for particle x value
    double std_y = 1.5; //standard deviation for particle y value
    double time =0.1; //time = 1 sec as we got the std_dev value based on human speed that is measured in seconds

    //-----------------------Gaussian Random Acceleration-----------------------------------------

        for(int i=0; i<NUM_PARTICLES; ++i){
            Particle *p = &particles[i]; // get address of particle to update

            //X-direction
            double errorX = generate_normal_random(0, std_x);
            double new_acc_x = p->accelerationX + errorX;
            double new_vel_x = p->velocityX + new_acc_x * time;
            double new_x = p->x + new_vel_x * time + new_acc_x * time * time;

            //Y-direction
            double errorY = generate_normal_random(0, std_y);
            double new_acc_y = p->accelerationY + errorY;
            double new_vel_y = p->velocityY + new_acc_y * time;
            double new_y = p->y + new_vel_y * time + new_acc_y * time * time;


            p->x = new_x;
            p->y = new_y;
            p->velocityX = new_vel_x;
            p->velocityY = new_vel_y;
            p->accelerationX = new_acc_x;
            p->accelerationY = new_acc_y;

        }
    //--------------------basic form of the Newtonian motion-------------------------

//            for (int i = 0; i < NUM_PARTICLES; ++i) {
//                    Particle *p = &particles[i];
//
//                    // Predict new state with added noise
//                    p->accelerationX += generate_normal_random(0, std_x);
//                    p->accelerationY += generate_normal_random(0, std_y);
//                    p->velocityX += p->accelerationX * time;
//                    p->velocityY += p->accelerationY * time;
//                    p->x += p->velocityX * time + 0.5 * p->accelerationX * pow(time, 2);
//                    p->y += p->velocityY * time + 0.5 * p->accelerationY * pow(time, 2);
//
//                    // Boundary check and clamp to BLE range
//                    p->x = clamp(p->x, -BLE_RANGE, BLE_RANGE);
//                    p->y = clamp(p->y, -BLE_RANGE, BLE_RANGE);
//                }

//---------------------------------Random walk----------------------------------------

//        for (int i = 0; i < NUM_PARTICLES; ++i) {
//                Particle *p = &particles[i];
//
//                // Generate a random step from Gaussian distribution for both x and y
//                double step_x = generate_normal_random(0, std_x);
//                double step_y = generate_normal_random(0, std_y);
//
//                // Update the particle's positio    n
//                p->x += step_x;
//                p->y += step_y;
//            }

}

void resample(Particle particles[NUM_PARTICLES]) {
    Particle resampled_particles[NUM_PARTICLES];
    double weights[NUM_PARTICLES];
    double sum_of_weights = 0;

    // Calculate sum of weights
    for (int i = 0; i < NUM_PARTICLES; i++) {
        weights[i] = particles[i].weight;
        sum_of_weights += particles[i].weight;
    }

    // Generate cumulative weights for sampling
    for (int i = 1; i < NUM_PARTICLES; i++) {
        weights[i] += weights[i - 1];
    }

    // Resample
    // srand(time(NULL)); // Seed the random number generator.
    for (int i = 0; i < NUM_PARTICLES; i++) {
        double pick = (double)rand() / RAND_MAX * sum_of_weights;
        for (int j = 0; j < NUM_PARTICLES; j++) {
            if (pick <= weights[j]) {
                resampled_particles[i] = particles[j];
                break;
            }
        }
    }

    // Copy the resampled particles back
    for (int i = 0; i < NUM_PARTICLES; i++) {
        particles[i] = resampled_particles[i];
    }


    //Normalize Weights
    sum_of_weights = 0;
    // Calculate sum of weights
    for (int i = 0; i < NUM_PARTICLES; i++) {
        weights[i] = particles[i].weight;
        sum_of_weights += particles[i].weight;
    }
    for (int i = 0; i < NUM_PARTICLES; i++) {
        Particle *p = &particles[i];
        p->weight /= sum_of_weights;

    }
    double total_weight=0;
    for (int i = 0; i < NUM_PARTICLES; i++) {

        total_weight+=particles[i].weight;

    }
    printf("Total weight of particles: %f\n",total_weight);
}


void estimate(Particle particles[NUM_PARTICLES],double coordinates[2]){

    //Maximum Weight Particle

    /*double highest_weight = -1.0;
    Particle best_particle;
    for (int i = 0; i < NUM_PARTICLES; ++i){
        if (particles[i].weight > highest_weight) {
            highest_weight = particles[i].weight;
            best_particle = particles[i];
		}
    }
    coordinates[0] = best_particle.x;
    coordinates[1] = best_particle.y;

    */

    //Weighted Average
    double sum_weights_x = 0;
    double sum_weights_y = 0;
    double total_weight = 0;

    for (int i = 0; i < NUM_PARTICLES; i++) {
        sum_weights_x += particles[i].x * particles[i].weight;
        sum_weights_y += particles[i].y * particles[i].weight;
        total_weight += particles[i].weight;
    }

    Measurement estimated_position;
    if (total_weight > 0) {
        estimated_position.x = sum_weights_x / total_weight;
        estimated_position.y = sum_weights_y / total_weight;
    } else {
        // Handle the case where total weight is zero
        estimated_position.x = 0;
        estimated_position.y = 0;
    }

    coordinates[0] = estimated_position.x ;
    coordinates[1] = estimated_position.y ;

}







