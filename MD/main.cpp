#include <iostream>
#include <ctime>
#include "init.h"
#include "sample.h"
#include "force.h"

using namespace VECTOR;

//const int nPar = 108;
const int nPar = 10;

void to_phi(double rho,double &phi) { phi=4. * PI * rho / (3 * nPar);};

int main()
{
	using namespace VECTOR3D;


	double phi=0.4;
	//double rho = 0.8442;
	//to_phi(rho,phi);
	double temp = 0.728;
	double vs2 = 0;

	double t = 0;
	double delt = 0.001;
	int interval = 1000;
	double t_max = 200;

	Vector lBox;
	Rand myRand(time(0));    //Rand myRand(1);

	Vector par_R[nPar];
	Vector par_Rm[nPar];
	Vector par_V[nPar];
	init(nPar,phi,temp,delt, lBox, &myRand, par_R, par_Rm, par_V);

	double en_p=0.;
	double en_v2 = 0.;
	ULJ pair;
	Vector par_F[nPar];

	int icycle = 0;
	for(t=0;t<t_max;icycle++)
	{
		force(nPar, en_p, lBox, par_R, par_Rm, par_V, &pair, par_F);		//计算力
		integtate(nPar, delt, en_v2, temp, lBox, par_R, par_Rm, par_V, par_F);//积分
		if (icycle%interval == 0)
		{
			snap_xyz(par_R, nPar, t, lBox);
			cout << en_p << "\t" << en_v2 << "\t" << en_p + en_v2 << endl;
		}
		t += delt;
	}

	cin.get();
	cin.get();
	return 0;
}


