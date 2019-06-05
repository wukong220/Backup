#ifndef VECTOR_H
#define VECTOR_H
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>

struct Vector
{
	double x, y, z;
	Vector() { x = y = z = 0; }
	Vector(double a, double b, double c) :x(a), y(b), z(c) {}
	Vector(const Vector &a) :x(a.x), y(a.y), z(a.z) {}

	Vector operator -() const { return Vector(-x, -y, -z); }
	void operator =(double a) { x = y = z = a; }

	Vector operator +(const Vector &a) const { return Vector(x + a.x, y + a.y, z + a.z); }
	Vector operator +(double a) const { return Vector(x + a, y + a, z + a); }
	friend Vector operator +(double a, const Vector &b) { return Vector(a + b.x, a + b.y, a + b.z); }
	void operator +=(const Vector &a) { x += a.x; y += a.y; z += a.z; }
	void operator +=(double a) { x += a; y += a; z += a; }

	Vector operator -(const Vector &a) const { return Vector(x - a.x, y - a.y, z - a.z); }
	Vector operator -(double a) const { return Vector(x - a, y - a, z - a); }
	friend Vector operator -(double a, const Vector &b) { return Vector(a - b.x, a - b.y, a - b.z); }
	void operator -=(const Vector &a) { x -= a.x; y -= a.y, z -= a.z; }
	void operator -=(double a) { x -= a; y -= a; z -= a; }

	Vector operator *(double a) const { return Vector(x * a, y * a, z * a); }
	friend Vector operator *(double a, const Vector &b) { return Vector(a * b.x, a * b.y, a * b.z); }
	Vector operator *(const Vector &a) const { return Vector(x * a.x, y * a.y, z * a.z); }
	void operator *=(double a) { x *= a; y *= a; z *= a; }

	Vector operator /(double a) const { return Vector(x / a, y / a, z / a); }
	void operator /=(double a) { x /= a; y /= a; z /= a; }

	double dot(const Vector &a) const { return x * a.x + y * a.y + z * a.z; }
	Vector cross(const Vector &a) const { return Vector(y * a.z - z * a.y, z * a.x - x * a.z, x * a.y - y * a.x); }
	double square() const { return x * x + y * y + z * z; }
	double module() const { return std::sqrt(square()); }
	void normalize()
	{
		double r = module();
		x /= r;
		y /= r;
		z /= r;
	}
	friend std::ostream& operator <<(std::ostream &output, const Vector &a)
	{
		output << a.x << "\t" << a.y << "\t" << a.z;
		return output;
	}
};

#endif