#include "intg.h"

using namespace std;

void rand_tangent_vector(const Vector3D &u, Vector3D &rVec, Normaldev *ran)
{
	double vx = ran->dev();
	double vy = ran->dev();

	if (u.z < 1. && u.z > -1.)
	{
		double cos_theta = u.z;
		double sin_theta = sqrt(1 - cos_theta * cos_theta);
		double cos_phi = u.x / sin_theta;
		double sin_phi = u.y / sin_theta;
		rVec.x = vx * cos_phi * cos_theta - vy * sin_phi;
		rVec.y = vx * sin_phi * cos_theta + vy * cos_phi;
		rVec.z = -vx * sin_theta;
	}
	else
	{
		rVec.x = vx;
		rVec.y = vy;
		rVec.z = 0;
	}
}

void periodic_boundary(Vector3D &R)
{
	static Vector3D vecL = Cell::lBox;
	if (R.x < 0) R.x += vecL.x;
	else if (R.x >= vecL.x) R.x -= vecL.x;
	if (R.y < 0) R.y += vecL.y;
	else if (R.y >= vecL.y) R.y -= vecL.y;
	if (R.z < 0) R.z += vecL.z;
	else if (R.z >= vecL.z) R.z -= vecL.z;
}

void BD_simple(Particle & p, Normaldev * myran)
{
	Vector3D du;
	rand_tangent_vector(p.vecN, du, myran);

	p.vecN += p.vecT.cross(p.vecN) * 3 * h + du * hR;
	p.vecN.normalize();

	Vector3D eta_T(myran->dev(), myran->dev(), myran->dev());
	p.vecR += p.vecF * h + eta_T * hT;
	periodic_boundary(p.vecR);

	p.vecF = 0;
	p.vecT = 0;

}

void BD_simple(Particle &p, Normaldev *myran, double Pe)
{
	Vector3D du;
	rand_tangent_vector(p.vecN, du, myran);

	p.vecN += p.vecT.cross(p.vecN) * 3 * h + du * hR;
	p.vecN.normalize();

	Vector3D eta_T(myran->dev(), myran->dev(), myran->dev());
	p.vecR += (p.vecF + Pe * p.vecN) * h + eta_T * hT;
	periodic_boundary(p.vecR);

	p.vecF = 0;
	p.vecT = 0;
}
