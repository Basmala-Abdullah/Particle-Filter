
#ifndef PARTICLE_H_
#define PARTICLE_H_

#include "helper_function.h"
#include "measurement.h"

#define NUM_PARTICLES  50
 #define BLE_RANGE 20
 #define PLOT_GRAPH 0

typedef struct Particle{

	float x;
	float y;
	float weight;
	float velocityX;     //if considering Gaussian Random Acceleration
	float accelerationX; //if considering Gaussian Random Acceleration
	float velocityY;     //if considering Gaussian Random Acceleration
	float accelerationY; //if considering Gaussian Random Acceleration
}Particle;

void initialize_particles(Particle particles[NUM_PARTICLES]);

void update_particles(Particle particles[NUM_PARTICLES], Measurement_Type measurements[NUM_MEASUREMENTS]);

void prediction(Particle particles[NUM_PARTICLES]);

void resample(Particle particles[NUM_PARTICLES]);

void estimate(Particle particles[NUM_PARTICLES],float coordinates[2]);

#endif /* PARTICLE_H_ */

