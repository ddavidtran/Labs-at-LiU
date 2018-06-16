#ifndef _physics_h
#define _physics_h

#include "coordinate.h"

#define STEP_SIZE 1.0 /* the step size use in the integration */

int feuler(Particle *a,
	   float time_step) ;

float wall_collide(Particle *p,
		   cord_t wall) ;


float collide(Particle *p1,
	      Particle *p2) ;

void interact(Particle *p1,
	      Particle *p2,
	      float t) ;


#endif
