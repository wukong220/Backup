#include "force.h"
using namespace std;

Force::Force(double eps, double sig, double rc, double theta_m) :epsilon(eps), sigma(sig), rCut(rc)
{
	a = sigma * 0.5;
	rWCA = sigma * pow(2, 1 / 6.0);
	rCut2 = rCut * rCut;
	theta_max = theta_m / 180.0 * PI;
	theta_tail = 10 / 180.0 * PI;
}

double Force::V_rep(double r) const
{
	if (r < rWCA)
	{
		double tmp = pow(sigma / r, 6);
		return 4 * epsilon * (tmp * tmp - tmp + 0.25);
	}
	else
		return 0;
}

double Force::V_att(double r) const
{
	if (r < rCut)
	{
		double rs = abs(r - sigma);
		double tmp = a / (rs + a * pow(2, 1 / 6.0));
		tmp = pow(tmp, 6);
		return 4 * epsilon * (tmp * tmp - 2 * tmp);
	}
	else
		return 0;
}

double Force::phi(double theta) const
{
	static double k = PI / (2 * theta_tail);

	if (theta >= theta_max + theta_tail)
		return 0.;
	else if (theta <= theta_max)
		return 1.;
	else
	{
		double tmp = cos(k * (theta - theta_max));
		return tmp * tmp;
	}
}

double Force::d_V_rep(double r) const
{
	double tmp = pow(sigma / r, 6);
	return -24 * epsilon * (2 * tmp * tmp - tmp) / r;
}

double Force::d_V_att(double r) const
{
	double rs = abs(r - sigma);
	double u = a / (rs + 0.5 * rWCA);
	double u6 = pow(u, 6);
	double res = 48 * epsilon / a * u * (u6 * u6 - u6);
	if (r < sigma)
		return res;
	else
		return -res;
}

double Force::d_phi(double theta) const
{
	static double k = PI / (2 * theta_tail);
	if (theta >= theta_max + theta_tail || theta <= theta_max)
		return 0.;
	else
		return -k * sin(2 * k * (theta - theta_max));
}

double Force::potential(const Vector3D &R12, const Vector3D &u1, const Vector3D &u2) const
{
	double r12 = R12.module();
	Vector3D R12_hat = R12 / r12;
	double theta1 = acos(u1.dot(R12_hat));
	double theta2 = acos(u2.dot(-R12_hat));
	return V_rep(r12) + V_att(r12) * phi(theta1) * phi(theta2);
}

Vector3D Force::grad_phi(double theta, double r, const Vector3D &r_hat, const Vector3D &u_hat) const
{
	double sin_theta = sin(theta);
	if (sin_theta != 0.)
		return -d_phi(theta) / sin_theta / r * (u_hat - u_hat.dot(r_hat) * r_hat);
	else
		return Vector3D(0., 0., 0.);
}
double Force::potential(const Vector3D &R12, double theta1, double theta2) const
{
	double r12 = R12.module();
	Vector3D R12_hat = R12 / r12;
	return V_rep(r12) + V_att(r12) * phi(theta1) * phi(theta2);
}


void Force::cal_force(const Vector3D &R12_hat, double r12, const Vector3D &u1, const Vector3D &u2, Vector3D &F1, Vector3D &F2, Vector3D &T1, Vector3D &T2) const
{
	double theta1 = acos(u1.dot(R12_hat));
	double theta2 = acos(u2.dot(-R12_hat));
	double phi1 = phi(theta1);
	double phi2 = phi(theta2);
	Vector3D F12;

	//anisotropic attractive force
	if (phi1 > 0.0 && phi2 > 0.0)
	{
		double Vatt = V_att(r12);
		Vector3D grad1 = d_V_att(r12) * phi1 * phi2 * R12_hat;
		Vector3D grad2 = Vatt * grad_phi(theta1, r12, R12_hat, u1) * phi2;
		Vector3D grad3 = -Vatt * phi1 * grad_phi(theta2, r12, R12_hat, u2);
		F12 = -(grad1 + grad2 + grad3);
		T1 += -grad2.cross(R12_hat) * r12;
		T2 += -grad3.cross(R12_hat) * r12;
	}
	//isotropic repulsive force
	if (r12 <= rWCA)
	{
		F12 += -d_V_rep(r12) * R12_hat;
	}
	F1 -= F12;
	F2 += F12;
}

void Force::cal_force_numerical(const Vector3D &R12, const Vector3D &u1, const Vector3D &u2, Vector3D &F1, Vector3D &F2, Vector3D &T1, Vector3D &T2) const
{
	double delta = 1e-5;
	double V0 = potential(R12, u1, u2);
	Vector3D Rx(R12.x + delta, R12.y, R12.z);
	Vector3D Ry(R12.x, R12.y + delta, R12.z);
	Vector3D Rz(R12.x, R12.y, R12.z + delta);
	double Vx = potential(Rx, u1, u2);
	double Vy = potential(Ry, u1, u2);
	double Vz = potential(Rz, u1, u2);


	double r12 = R12.module();
	Vector3D R12_hat = R12 / r12;
	double theta1 = acos(u1.dot(R12_hat));
	double theta2 = acos(u2.dot(-R12_hat));

	double V1 = potential(R12, theta1 + delta, theta2);
	double V2 = potential(R12, theta1, theta2 + delta);

	Vector3D vecF;
	vecF.x = -(Vx - V0) / delta;
	vecF.y = -(Vy - V0) / delta;
	vecF.z = -(Vz - V0) / delta;

	F1 -= vecF;
	F2 += vecF;

	Vector3D vecN1 = R12.cross(u1);
	vecN1.normalize();
	Vector3D vecN2 = -R12.cross(u2);
	vecN2.normalize();

	T1 += -(V1 - V0) / delta * vecN1;
	T2 += -(V2 - V0) / delta * vecN2;
}

