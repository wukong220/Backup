#include "init.h"
#include "paras.h"

const double xmin_box = -L_box/2.0;
const double xmax_box = L_box/2.0;
const double ymin_box = -L_box/2.0;
const double ymax_box = L_box/2.0;
const double zmin_box = -0.5;
const double zmax_box = 0.5;

bool checkin(Vector* p_Vec, int n)
{ 
	Vector dR;
	if (n == 0)
		return false;
	for(int i = 0; i < n; i++)
		{
			dR = p_Vec[n] - p_Vec[i];
			if(dR.module() < inter)
				return true;	//overlap
		}
	return false;
}

bool overlap(Vector VecR, Particle* par)
{
	return true;
}


struct Ranq2 Rand((unsigned int)(time(NULL)));	//Random numbers
/* create num identical particles with minimal distance rm in the Box of l */
void Particle::creat_random(Particle* par,Ranq2* Rand)
{
	Vector* p_Vec;
	p_Vec = new Vector[Num_obs];
	for(int i = 0;i < Num_obs;)
	{
		p_Vec->x = Rand->doub() * L_box / 2.0;
		p_Vec->y = Rand->doub() * L_box / 2.0;
		p_Vec->z = 0.0;
		if (!checkin(p_Vec, i))
			i++;
	}
	
	for(int i = 0; i < Num_obs; i++)
		par[i].vecR = p_Vec[i];
		
	delete[] p_Vec;
	p_Vec = NULL;
}

void Chain::init_chain(Chain** Chain, Particle* par, Ranq2 *Rand)
{
/*	Vector** pp_Chain = new Vector* [Num_chains];
	for(int i = 0; i < Num_chains; i++)
		Vector[i] = new Vector[N_chain];
		
		
		
	for(int i = 0; i < Num_chains; i++)
	{
		for(int j = 0; j < N_chain;)
			{
				pp_Chain[i][j]
			}
			
	}
		
		for(int i = 0; i < N_chain; i++)
			{
				delete[] pp_Chain[i];
			}
		delete[] pp_Chain;*/
}