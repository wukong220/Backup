#ifndef FORCE_H
#define FORCE_H
#include <cmath>
#include "vector.h"

const double PI = 3.14159265358979;

class Force
{
public:
	Force(double eps, double sig, double rc, double theta_m);
	double V_rep(double r) const;
	double V_att(double r) const;
	double phi(double theta) const;
	double d_V_rep(double r) const;
	double d_V_att(double r) const;
	double d_phi(double theta) const;
	Vector3D grad_phi(double theta, double r, const Vector3D &r_hat, const Vector3D &u_hat) const;
	double potential(const Vector3D &R12, const Vector3D &u1, const Vector3D &u2) const;
	double potential(const Vector3D &R12, double theta1, double theta2) const;
	void cal_force(const Vector3D &R12_hat, double r12, const Vector3D &u1, const Vector3D &u2, Vector3D &F1, Vector3D &F2, Vector3D &T1, Vector3D &T2) const;
	void cal_force_numerical(const Vector3D &R12, const Vector3D &u1, const Vector3D &u2, Vector3D &F1, Vector3D &F2, Vector3D &T1, Vector3D &T2) const;
	bool within_range(double r2) const { return (r2 < rCut2); }

private:
	double epsilon;
	double sigma;
	double a;
	double rWCA;
	double rCut;
	double rCut2;
	double theta_max;
	double theta_tail;
};
#endif