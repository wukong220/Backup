#include "compute.h"
using namespace std;

int main(int argc, char* argv[])
{
#ifdef _MSC_VER
	double theta_max = 100;
	double epsilon = 5.5;
	double Pe = 0;
	double phi = 0.01;
	int seed = 321;
#else
	double theta_max = atof(argv[1]);
	double epsilon = atof(argv[2]);
	double Pe = atof(argv[3]);
	double phi = atof(argv[4]);
	int seed = atoi(argv[5]);
#endif
	int N = 1000;
	int nStep = 1e8;
	ini(N, phi, epsilon, Pe, theta_max, nStep, seed);
	run(nStep);
	end();
	return 0;
}


