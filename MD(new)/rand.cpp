#include "rand.h"

using namespace std;
double Normaldev::dev()	//	Return a normal deviate.
{
	double u, v, x, y, q;
	do {
		u = doub();
		v = 1.7156*(doub() - 0.5);
		x = u - 0.449871;
		y = abs(v) + 0.386595;
		q = x*x + y*(0.19600*y - 0.25472*x);
	} while (q > 0.27597 && (q > 0.27846 || v*v > -4.*log(u)*u*u));
	return mu + sig*v / u;
}