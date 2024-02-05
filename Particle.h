
#ifndef PARTICLE_H_
#define PARTICLE_H_

#include "helper_function.h"
#include "measurement.h"

#define NUM_PARTICLES  500

struct Particle {

	int id;
	double x;
	double y;
	double weight;
	double velocity;     //if considering Gaussian Random Acceleration
	double acceleration; //if considering Gaussian Random Acceleration
};

void initialize_particles(Particle particles[NUM_PARTICLES]);

void prediction();

void updateWeights();

void resample();

#endif /* PARTICLE_H_ */

