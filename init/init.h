#ifndef INIT_H
#define INIT_H

#include <time.h>

#include "rand.h"
#include "vector.h"
#include "paras.h"

bool check_overlap(Vector * p_Vec,int n);	//check overlap

//2 Dimension
class Particle		//obstacle
{
private:
	int type;
	Vector vecR;
	double radius;
	double diameter;
	Particle* next;
public:
	Particle() { type = Type_obs; vecR = 0; radius = rad_obs; diameter = R_obs; next = NULL; }
	~Particle();
	void creat_random(Particle *par, Ranq2 *Rand);
};

class Chain		//chains 
{
private:
	int type;
	Vector vecR;
	double radius;
	double diameter;
	Chain* next;
public:	
	Chain(){type = Type_chain; vecR = 0; radius = rad; diameter = Sigma; next = NULL;}
	~Chain();
	void init_chain(Chain** Chain, Particle* par, Ranq2* Rand);	
};


#endif