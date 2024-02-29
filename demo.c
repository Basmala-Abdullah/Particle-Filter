/*
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_PARTICLES 100
#define CAR_LENGTH 4.5
#define CAR_WIDTH 1.8
#define NUM_ANCHORS 4 // Example: 4 anchors

typedef struct {
    float x;
    float y;
    float weight;
} Particle;

typedef struct {
    float x;
    float y;
} Anchor;

// Placeholder for the anchors' positions
Anchor anchors[NUM_ANCHORS] = {
    {0.0, 0.0}, // Anchor 1 position
    {4.5, 0.0}, // Anchor 2 position
    {4.5, 1.8}, // Anchor 3 position
    {0.0, 1.8}  // Anchor 4 position
};
// Convert RSSI to distance (meters)
float rssi_to_distance(float rssi) {
    // Placeholder: replace with your actual RSSI-to-distance conversion
    return 10.0; // Example fixed value
}

// Calculate expected angle of arrival (AoA)
float calculate_expected_aoa(Particle particle, Anchor anchor) {
    // Placeholder: calculate expected AoA based on particle and anchor positions
    return 45.0; // Example fixed angle in degrees
}

// Update particle weights based on RSSI and AoA measurements
void update_particles(Particle particles[NUM_PARTICLES], float rssi[NUM_ANCHORS], float aoa[NUM_ANCHORS]) {
    int i, j;
    for (i = 0; i < NUM_PARTICLES; i++) {
        particles[i].weight = 1.0; // Reset weight
        for (j = 0; j < NUM_ANCHORS; j++) {
            float distance = rssi_to_distance(rssi[j]);
            float expected_aoa = calculate_expected_aoa(particles[i], anchors[j]);
            // Update weight based on distance and AoA
            // This is simplified; in reality, you'd compare expected and measured values
            // For example, using Gaussian probability for the distance and AoA differences
            particles[i].weight *= exp(-pow(distance - expected_aoa, 2)); // Simplified update
        }
    }
    // Normalize weights (not shown here)
}


void initialize_particles(Particle particles[NUM_PARTICLES]) {
    int i;
    float center_x = CAR_LENGTH / 2.0;
    float center_y = CAR_WIDTH / 2.0;

    // Seed the random number generator to get different results each run
    srand(time(NULL));

    for (i = 0; i < NUM_PARTICLES; i++) {
        // Randomly distribute particles around the center
        particles[i].x = ((float)rand() / (float)(RAND_MAX)) * CAR_LENGTH;
        particles[i].y = ((float)rand() / (float)(RAND_MAX)) * CAR_WIDTH;

        // Print initial positions for verification
        printf("Particle %d: x = %f, y = %f\n", i, particles[i].x, particles[i].y);
    }
}

void normalize_weights(Particle particles[NUM_PARTICLES]) {
    float weight_sum = 0.0;
    for (int i = 0; i < NUM_PARTICLES; i++) {
        weight_sum += particles[i].weight;
    }
    for (int i = 0; i < NUM_PARTICLES; i++) {
        particles[i].weight /= weight_sum;
    }
}

void resample_particles(Particle particles[NUM_PARTICLES]) {
    Particle new_particles[NUM_PARTICLES];
    float weights[NUM_PARTICLES];
    float cumulative_weights[NUM_PARTICLES];
    float index = (float)rand() / RAND_MAX * (1.0 / NUM_PARTICLES);
    float step = 1.0 / NUM_PARTICLES;
    int i, j = 0;

    // Normalize weights
    normalize_weights(particles);

    // Compute cumulative weights
    cumulative_weights[0] = particles[0].weight;
    for (i = 1; i < NUM_PARTICLES; i++) {
        cumulative_weights[i] = cumulative_weights[i - 1] + particles[i].weight;
    }

    for (i = 0; i < NUM_PARTICLES; i++) {
        while (index > cumulative_weights[j]) {
            j++;
        }
        new_particles[i] = particles[j];
        index += step;
    }

    // Replace old particles with the resampled particles
    for (i = 0; i < NUM_PARTICLES; i++) {
        particles[i] = new_particles[i];
    }
}

void estimate_position(Particle particles[NUM_PARTICLES], float *estimated_x, float *estimated_y) {
    float sum_weights = 0.0;
    float x = 0.0;
    float y = 0.0;

    for (int i = 0; i < NUM_PARTICLES; i++) {
        x += particles[i].x * particles[i].weight;
        y += particles[i].y * particles[i].weight;
        sum_weights += particles[i].weight;
    }

    // Compute the estimated position
    *estimated_x = x / sum_weights;
    *estimated_y = y / sum_weights;
}
int main() {
    Particle particles[NUM_PARTICLES];
    float estimated_x, estimated_y;

    initialize_particles(particles);
    // Example RSSI and AoA readings from anchors
    float rssi[NUM_ANCHORS] = {-70.0, -65.0, -60.0, -75.0};
    float aoa[NUM_ANCHORS] = {45.0, 90.0, 135.0, 180.0};

    update_particles(particles, rssi, aoa);
    resample_particles(particles);

    // Estimate the position after resampling
    estimate_position(particles, &estimated_x, &estimated_y);

    printf("Estimated Position: x = %f, y = %f\n", estimated_x, estimated_y);
    return 0;
}


///////////////////////////////////////////////////////////

// Function to generate normally distributed random numbers
double generate_normal_random(double mean, double stddev) {
    double u = ((double)rand() / (RAND_MAX)) * 2 - 1;
    double v = ((double)rand() / (RAND_MAX)) * 2 - 1;
    double s = u * u + v * v;
    while (s >= 1 || s == 0) {
        u = ((double)rand() / (RAND_MAX)) * 2 - 1;
        v = ((double)rand() / (RAND_MAX)) * 2 - 1;
        s = u * u + v * v;
    }
    double mul = sqrt(-2.0 * log(s) / s);
    return mean + stddev * u * mul;
}

void initialize_particles(Particle particles[NUM_PARTICLES]) {

    float mean_x = CAR_LENGTH / 2.0; // Assuming the car is centered at this point
    float mean_y = MAX_DISTANCE / 2.0; // Assuming device approaches from the front
    float stddev_x = CAR_LENGTH / 2.0; // Broader distribution across the car's width
    float stddev_y = MAX_DISTANCE / 3.0; // Broader distribution as device could be up to�50�meters�away



    for (int i = 0; i < NUM_PARTICLES; i++) {
        // Generate particles around the center with normal distribution
        particles[i].x = generate_normal_random(mean_x, stddev_x);
        particles[i].y = generate_normal_random(mean_y, stddev_y);
        particles[i].weight = 1.0 / NUM_PARTICLES; // Initialize weights evenly

        printf("Particle %d: x = %f, y = %f\n", i, particles[i].x, particles[i].y);
    }
}}
*/
///////////////////


