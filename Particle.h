
#ifndef PARTICLE_H_
#define PARTICLE_H_

#include "helper_function.h"
#include "measurement.h"

<<<<<<< Updated upstream
#define NUM_PARTICLES  10
=======
#define NUM_PARTICLES  25
>>>>>>> Stashed changes

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

void initialize_particles(Particle particles[NUM_PARTICLES]);

void update_particles(Particle particles[NUM_PARTICLES], Measurement measurements[NUM_MEASUREMENTS]);

void prediction(Particle particles[NUM_PARTICLES]);

void resample(Particle particles[NUM_PARTICLES]);

void estimate(Particle particles[NUM_PARTICLES],double coordinates[2]);

#endif /* PARTICLE_H_ */


