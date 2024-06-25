/*
 * File:   application.c
 * Author:
 *
 *
 */

 #include "Particle.h"


//// Add a function to clamp values within a range
//float clamp(float value, float min, float max) {
//    if (value < min) return min;
//    if (value > max) return max;
//    return value;
//}



void initialize_particles(Particle particles[NUM_PARTICLES]) {

    // double mean_x = (max_x + min_x)/2.0; // spread particles in region of measurements (x)
    // double mean_y = (max_y + min_y)/2.0; // spread particles in region of measurements (y)
    // double stddev_x = (max_x - min_x)/4.0; // approximate equation of standard deviation value knowing min and max values (x)
    // double stddev_y = (max_y- min_y)/4.0; // approximate equation of standard deviation value knowing min and max values (y)
    
    uint8_t mean_x = 0; // Assuming the car is centered at origin
    uint8_t mean_y = 0; // Assuming the car is centered at origin
    uint8_t stddev_x = BLE_RANGE; // Broader distribution across the car's width
    uint8_t stddev_y = BLE_RANGE; // Broader distribution as device could be up to 50 meters away
    
    //uint32_t count = 0;
    for (uint32_t i = 0; i < NUM_PARTICLES; i++) {
        do {
            particles[i].x = generate_normal_random(mean_x, stddev_x);
            particles[i].y = generate_normal_random(mean_y, stddev_y);
        }while (fabs(particles[i].x) > BLE_RANGE || fabs(particles[i].y) > BLE_RANGE);

        particles[i].weight = 1.0 / NUM_PARTICLES; // Initialize weights evenly
        particles[i].accelerationX =generate_normal_random(0, 1.5); //as the standard deviation value in prediction step
        particles[i].accelerationY =generate_normal_random(0, 1.5); //as the standard deviation value in prediction step
        particles[i].velocityX =0;
        particles[i].velocityY =0;

        printf("Particle %d: x = %f, y = %f, weight: %f\n", i, particles[i].x, particles[i].y,particles[i].weight);

        // if(particles[i].x >60 || particles[i].y>60){
        //     count++;
        // }

        // FILE *fp = fopen("init.txt", "w");
        // if (fp == NULL) {
        //     printf("Error opening file!\n");
        //     return;
        // }
        // for (int i = 0; i < NUM_PARTICLES; i++) {
        //     fprintf(fp, "%f %f\n", particles[i].x, particles[i].y);
        // }
        // fclose(fp);
    }
    //printf("Out of range particles: %f percentage \n", (count/(float)NUM_PARTICLES)*100);
}


void update_particles(Particle particles[NUM_PARTICLES], Measurement_Type newMeasurement){

    float std_x; //init for standard deviation for particle x value
    float std_y; //init for standard deviation for particle y value
    float weight_sum = 0.0; //used for weight normalizing
    float wt = 1.0;
    for(uint32_t i=0; i<NUM_PARTICLES; i++){
        Particle *p = &particles[i];
        wt = 1.0;
        // float distance_min = FLT_MAX;
        // Measurement_Type newMeasurement;
        
        // for(uint32_t j=0; j<NUM_MEASUREMENTS; j++){
        //     float distance = sqrt(pow((measurements[j].x-particles[i].x),2)+pow((measurements[j].y-particles[i].y),2)); //Euclidean distance equation
        //     if(distance < distance_min){
        //         distance_min= distance;
        //         newMeasurement.x= measurements[j].x;
        //         newMeasurement.y= measurements[j].y;
        //         newMeasurement.type= measurements[j].type;
        //     }
        // }

        /////////////---------------Gaussian PDF Weighting Strategy---------------/////////////
        //newMeasurement.type=0;
        // if(newMeasurement.type==0){
        //     std_x = 1.5;
        //     std_y = 1.5;

        // }else if(newMeasurement.type==1){
        //     std_x = 1.0;
        //     std_y=1.0;

        // }else if(newMeasurement.type==2){
        //     std_x = 0.5;
        //     std_y=0.5;
        // }

        // float numerator = exp(-0.5 * (pow((newMeasurement.x - particles[i].x), 2) / pow(std_x, 2) + pow((newMeasurement.y - particles[i].y), 2) / pow(std_y, 2)));
        // float denominator = 2 * M_PI * std_x * std_y;
        // printf("num=%f, denom = %f, nearestPoint X: %f, nearestPoint Y: %f ",numerator,denominator,newMeasurement.x,newMeasurement.y);

        /////////////---------------Euclidean Distance Weighting Strategy---------------/////////////
        // float numerator =1;
        // float denominator = sqrt(pow((newMeasurement.x - particles[i].x), 2) + pow((newMeasurement.y - particles[i].y), 2));
        // printf("num=%f, denom = %f",numerator,denominator);

        /////////////---------------Inverse Distance Weighting Strategy---------------/////////////
        float power = 0;
//        newMeasurement.type=0;
        if(newMeasurement.type==0){
            power =2.0;

        }else if(newMeasurement.type==1){
            power =1.0;

        }else if(newMeasurement.type==2){
            power =0.5;
        }
         float numerator =1;
         float denominator = pow(sqrt(pow((newMeasurement.x - particles[i].x), 2) + pow((newMeasurement.y - particles[i].y), 2)),power);
         printf("num=%f, denom = %f, nearestPoint X: %f, nearestPoint Y: %f ",numerator,denominator,newMeasurement.x,newMeasurement.y);

        wt *= (numerator/denominator);
        p->weight = wt;
        weight_sum += wt;

        printf("Particle %d: x = %f, y = %f, weight: %f\n", i, particles[i].x, particles[i].y,particles[i].weight);
    }

    //Normalize Weights
    for (uint32_t i = 0; i < NUM_PARTICLES; i++) {
        Particle *p = &particles[i];
        p->weight /= weight_sum;
    }
    float total_weight=0;
    for (uint32_t i = 0; i < NUM_PARTICLES; i++) {

        total_weight+=particles[i].weight;

    }
    //printf("Total weight of particles: %f\n",total_weight);
}


void prediction(Particle particles[NUM_PARTICLES]){
    /////////////---------------Gaussian Random Acceleration---------------/////////////
    float std_x = 1.5; //standard deviation for particle x value
    float std_y = 1.5; //standard deviation for particle y value
    float time =0.1; //time = 1 sec as we got the std_dev value based on human speed that is measured in seconds
        for(uint32_t i=0; i<NUM_PARTICLES; ++i){
            Particle *p = &particles[i]; // get address of particle to update

            //X-direction
            float errorX = generate_normal_random(0, std_x);
            float new_acc_x = p->accelerationX + errorX;
            float new_vel_x = p->velocityX + new_acc_x * time;
            float new_x = p->x + new_vel_x * time + new_acc_x * time * time;

            //Y-direction
            float errorY = generate_normal_random(0, std_y);
            float new_acc_y = p->accelerationY + errorY;
            float new_vel_y = p->velocityY + new_acc_y * time;
            float new_y = p->y + new_vel_y * time + new_acc_y * time * time;


            p->x = new_x;
            p->y = new_y;
            p->velocityX = new_vel_x;
            p->velocityY = new_vel_y;
            p->accelerationX = new_acc_x;
            p->accelerationY = new_acc_y;

        }

    /////////////---------------Random Walk---------------/////////////
    //Parameters for Random Walk
    // float step_size_max_x = 1.0; // Max step size in x direction
    // float step_size_max_y = 1.0; // Max step size in y direction

    // for(int i = 0; i < NUM_PARTICLES; ++i) {
    //     Particle *p = &particles[i]; // Get address of particle to update

    //     // Generate random step in both directions
    //     float step_x = rand_double_range(-step_size_max_x, step_size_max_x);
    //     float step_y = rand_double_range(-step_size_max_y, step_size_max_y);

    //     // Update positions with random step
    //     p->x += step_x;
    //     p->y += step_y;
    // }


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

}

void resample(Particle particles[NUM_PARTICLES]) {
    Particle resampled_particles[NUM_PARTICLES];
    float weights[NUM_PARTICLES];
    float sum_of_weights = 0;

    // Calculate sum of weights
    for (uint32_t i = 0; i < NUM_PARTICLES; i++) {
        weights[i] = particles[i].weight;
        sum_of_weights += particles[i].weight;
    }

    // Generate cumulative weights for sampling
    for (uint32_t i = 1; i < NUM_PARTICLES; i++) {
        weights[i] += weights[i - 1];
    }

    // Resample
    // srand(time(NULL)); // Seed the random number generator.
    for (uint32_t i = 0; i < NUM_PARTICLES; i++) {
        float pick = (float)rand() / RAND_MAX * sum_of_weights;
        for (uint32_t j = 0; j < NUM_PARTICLES; j++) {
            if (pick <= weights[j]) {
                resampled_particles[i] = particles[j];
                break;
            }
        }
    }

    // Copy the resampled particles back
    for (uint8_t i = 0; i < NUM_PARTICLES; i++) {
        particles[i] = resampled_particles[i];
    }


    //Normalize Weights
    sum_of_weights = 0;
    // Calculate sum of weights
    for (uint32_t i = 0; i < NUM_PARTICLES; i++) {
        weights[i] = particles[i].weight;
        sum_of_weights += particles[i].weight;
    }
    for (uint32_t i = 0; i < NUM_PARTICLES; i++) {
        Particle *p = &particles[i];
        p->weight /= sum_of_weights;

    }
    float total_weight=0;
    for (uint32_t i = 0; i < NUM_PARTICLES; i++) {

        total_weight+=particles[i].weight;

    }
    //printf("Total weight of particles: %f\n",total_weight);
}


void estimate(Particle particles[NUM_PARTICLES],float coordinates[2]){

    //Maximum Weight Particle

    /*float highest_weight = -1.0;
    Particle best_particle;
    for (uint32_t i = 0; i < NUM_PARTICLES; ++i){
        if (particles[i].weight > highest_weight) {
            highest_weight = particles[i].weight;
            best_particle = particles[i];
		}
    }
    coordinates[0] = best_particle.x;
    coordinates[1] = best_particle.y;

    */

    //Weighted Average
    float sum_weights_x = 0;
    float sum_weights_y = 0;
    float total_weight = 0;

    for (uint32_t i = 0; i < NUM_PARTICLES; i++) {
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





