#ifndef PARTICLE_H
#define PARTICLE_H
#include "vector.h"
#include "force.h"
#include "rand.h"

struct Particle
{
	Vector3D vecR;
	Vector3D vecF;
	Vector3D vecN;
	Vector3D vecT;
	Particle* next;
	static double radius;
	static double sigma;

	Particle() { vecR = 0; vecF = 0; vecN = 0; vecT = 0; next = NULL; }
	void pair_force(Particle *p2, const Force *pf);
	void pair_force(Particle *p2, const Force *pf, const Vector3D &DR);
	static void create_random(Particle *par, int N, Vector3D &Box, double _radius, Normaldev *myran);
};

#endif
