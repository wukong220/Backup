#ifndef INIT_H_
#define INIT_H_
#include "vector.h"
#include "sample.h"

template <class Vector>
bool check_overlap(Vector *vecR, int cur_index, double sigma)
{
	double r2;
	Vector vec = vecR[cur_index];
	if (cur_index == 0)
		return false;
	for (int i = 0; i < cur_index; i++)
	{
		Vector dR = vec - vecR[i];
		r2 = dR.square();
		if (r2 < sigma*sigma)
			return true;
	}
	return false;
};

template <class Vector>
void init_pos(int nPar, double phi, Vector &lBox, Rand *myRand, Vector *par_R)
{
	double s = PI*0.5*0.5;
	if (Dimen(lBox) == 2)
		lBox = sqrt(s*nPar / phi);
	else if (Dimen(lBox) == 3)
	{
		s = 4. *s* 0.5 / 3.;
		lBox = pow(nPar*s / phi, 1. / 3);
	};

	double a = phi < 0.4 ? 0.5 : 0.3;
	Vector R0(a);
	Vector L_eff = lBox - 2 * R0;
	Vector *vecR = new Vector[nPar];

	int i = 0;
	while (i < nPar)		//initiation of positions
	{
		vecR[i] = R0 + Vector::randf(myRand) * L_eff;
		if (!check_overlap(vecR, i, 2 * a))
		{
			par_R[i] = vecR[i];
			//cout << "par_R[i]: " << par_R[i]<<"\t"<< par_R[i].square() << endl;
			i++;
		};
	};
	delete[] vecR;
};

template <class Vector>
void init_vel(int nPar, Vector lBox, double temp, double delt,
	Rand *myRand, Vector *par_R, Vector *par_Rm, Vector *par_V)
{
	int i = 0;
	double vs2 = 0;
	double fs = 0;
	Vector Vc;

	for (i = 0; i < nPar; i++)		//initiation of velocities
	{
		par_V[i] = Vector::randf(myRand) - 0.5;
		Vc += par_V[i];
		vs2 += par_V[i].square();
	}
	Vc /= nPar;
	vs2 /= nPar;
	fs = sqrt(3 * temp / vs2);
	for (int i = 0; i < nPar; i++)
	{
		par_V[i] = (par_V[i] - Vc)*fs;
		//cout << "par_V[i]" << par_V[i]<<"\t"<<par_V[i].square() << endl;
		par_Rm[i] = par_R[i] - par_V[i] * delt;
		tangle(par_Rm[i], lBox);
		//cout << "par_Rm[i]: " << par_Rm[i] << "\t" << par_Rm[i].square() << endl;
	}
};

template <class Vector>
void init(int nPar, double phi,
	double temp, double delt,
	Vector &lBox, Rand *myRand,
	Vector *par_R, Vector * par_Rm,
	Vector *par_V)
{
	init_pos(nPar, phi, lBox, myRand, par_R);
	init_vel(nPar, lBox, temp, delt, myRand, par_R, par_Rm, par_V);
	set_output(nPar, phi, temp);
};

#endif // !INIT_H_