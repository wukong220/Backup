#include "compute.h"
#include <ctime>
using namespace std;

double Pe;

int nPar;
Particle *par = NULL;
Cell *cell = NULL;
Vector3D lBox;

Normaldev *myran = NULL;
Force *pf = NULL;
clock_t t1, t2;
ofstream fout;

void ini(int N, double phi, double eps, double Pe0, double theta_max, int nStep, int seed)
{
	Pe = Pe0;
	nPar = N;
	double radius = 0.5;
	double sigma = 2 * radius;
	double rCut = 1.5 * sigma;
	pf = new Force(eps, sigma, rCut, theta_max);
	
	myran = new Normaldev(0, 1, seed);
	double Vs = 4. / 3. * PI * radius * radius * radius;
	double L = pow(N * Vs / phi, 1.0 / 3);
	lBox = Vector3D(L, L, L);
	par = new Particle[nPar];

	set_output(fout, theta_max, eps, Pe, nPar, phi, seed);

	Particle::create_random(par, nPar, lBox, radius, myran);
	Cell::ini(&cell, par, nPar, lBox, rCut, true, true, true);

	fout << "N = " << nPar << endl;
	fout << "L = " << L << endl;
	fout << "phi = " << phi << endl;
	fout << "Pe = " << Pe << endl;
	fout << "epsilon = " << eps << endl;
	fout << "theta_max = " << theta_max << endl;
	fout << "total step = " << nStep << endl;
	fout << "simulation time = " << nStep * h << endl;

	t1 = clock();
}

void computeForce(Particle *par, int nPar, Cell *cell, int nCell)
{
	Cell::pairForce(cell, pf);
}

void onestep()
{
	computeForce(par, nPar, cell, Cell::nCell);
	for (int i = 0; i < nPar; i++)
		BD_simple(par[i], myran, Pe);
	Cell::updateCellList(par, nPar, cell);
}

void run(int nStep)
{
	for (int i = 0; i <= nStep; i++)
	{
		onestep();
		if ((i < 100000 && i % 1000 == 0) || (i < 10000000 && i % 5000 == 0) || (i % 50000 == 0))
		{
			cout << "step = " << i << endl;
			snapshot_xyz(par, nPar, i, lBox);
		}
	}
}

void end()
{
	delete[]par;
	par = NULL;
	delete[]cell;
	cell = NULL;
	t2 = clock();
	double dt = double(t2 - t1) / CLOCKS_PER_SEC / 3600;
	fout << "time consumed : " << dt << " hours" << endl;
	fout.close();
}
