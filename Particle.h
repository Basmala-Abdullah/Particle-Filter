
#ifndef PARTICLE_H_
#define PARTICLE_H_

#include "helper_function.h"
#include "measurement.h"

#define NUM_PARTICLES  50
 #define BLE_RANGE 20
 #define PLOT_GRAPH 0

typedef struct Particle{

	int id;
	double x;
	double y;
	double weight;
	double velocityX;     //if considering Gaussian Random Acceleration
	double accelerationX; //if considering Gaussian Random Acceleration
	double velocityY;     //if considering Gaussian Random Acceleration
	double accelerationY; //if considering Gaussian Random Acceleration
}Particle;

void initialize_particles(Particle particles[NUM_PARTICLES],int max_x,int min_x,int max_y,int min_y);

void update_particles(Particle particles[NUM_PARTICLES], Measurement measurements[NUM_MEASUREMENTS]);

void prediction(Particle particles[NUM_PARTICLES]);

void resample(Particle particles[NUM_PARTICLES]);

void estimate(Particle particles[NUM_PARTICLES],double coordinates[2]);

#endif /* PARTICLE_H_ */


