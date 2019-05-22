#include "particle.h"
using namespace std;

/* static member variables */
double Particle::radius;
double Particle::sigma;


bool check_overlap(Vector3D *vecR, int cur_index, double sigma)
{
	if (cur_index == 0)
		return false;
	for (int i = 0; i < cur_index; i++)
	{
		Vector3D dR = vecR[cur_index] - vecR[i];
		if (dR.square() < sigma * sigma)
			return true;	//overlap
	}
	return false;
}

/* create N identical particles with radius a in the domain Box */
void Particle::create_random(Particle *par, int N, Vector3D &Box, double _radius, Normaldev *myran)
{
	radius = _radius;
	sigma = 2 * radius;
	Vector3D R0 = Vector3D(radius, radius, radius);
	Vector3D L_eff = Box - 2 * R0;
	Vector3D *vecR = new Vector3D[N];
	int i = 0;
	while (i < N)
	{
		vecR[i] = R0 + Vector3D(myran->doub(), myran->doub(), myran->doub()) * L_eff;
		if (!check_overlap(vecR, i, sigma))
			i++;
	}
	for (int i = 0; i < N; i++)
	{
		par[i].vecR = vecR[i];
		par[i].vecF = 0;
		double theta = myran->doub() * PI;
		double phi   = myran->doub() * 2 * PI;
		par[i].vecN = Vector3D(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
		par[i].vecT = 0;
		par[i].next = NULL;
	}
	delete[]vecR;
	vecR = NULL;
}

void Particle::pair_force(Particle *p2, const Force *pf)
{
	Vector3D R12 = p2->vecR - vecR;
	double r_square = R12.square();
	if (pf->within_range(r_square))
	{
		double r12 = sqrt(r_square);
		Vector3D R12_hat = R12 / r12;
		pf->cal_force(R12_hat, r12, vecN, p2->vecN, vecF, p2->vecF, vecT, p2->vecT);
	}
}

void Particle::pair_force(Particle *p2, const Force *pf, const Vector3D &DR)
{
	Vector3D R12 = p2->vecR - vecR + DR;
	double r_square = R12.square();
	if (pf->within_range(r_square))
	{
		double r12 = sqrt(r_square);
		Vector3D R12_hat = R12 / r12;
		pf->cal_force(R12_hat, r12, vecN, p2->vecN, vecF, p2->vecF, vecT, p2->vecT);
	}
}


