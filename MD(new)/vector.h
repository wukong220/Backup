#ifndef VECTOR_H
#define VECTOR_H
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>

struct Vector2D
{

	double x, y;

	Vector2D() { x = y = 0; }
	Vector2D(double a, double b) :x(a), y(b) {}
	Vector2D(const Vector2D &a) :x(a.x), y(a.y) {}

	bool operator == (const Vector2D &a) const { return x == a.x && y == a.y; }
	bool operator != (const Vector2D &a) const { return x != a.x || y != a.y; }

	void zero() { x = y = 0.; }
	Vector2D operator -() const { return Vector2D(-x, -y); }
	void operator =(double a) { x = y = a; }
	
	Vector2D operator +(const Vector2D &a) const { return Vector2D(x + a.x, y + a.y); }
	Vector2D operator +(double a) const { return Vector2D(x + a, y + a); }
	friend Vector2D operator +(double a, const Vector2D &b) { return Vector2D(a + b.x, a + b.y); }
	void operator +=(const Vector2D &a) { x += a.x; y += a.y;}
	void operator +=(double a) { x += a; y += a; }

	Vector2D operator -(const Vector2D &a) const { return Vector2D(x - a.x, y - a.y); }
	Vector2D operator -(double a) const { return Vector2D(x - a, y - a); }
	friend Vector2D operator -(double a, const Vector2D &b) { return Vector2D(a - b.x, a - b.y); }
	void operator -=(const Vector2D &a) { x -= a.x; y -= a.y; }
	void operator -=(double a) { x -= a; y -= a; }

	Vector2D operator *(double a) const { return Vector2D(x * a, y* a); }
	friend Vector2D operator *(double a, const Vector2D &b) { return Vector2D(a*b.x, a * b.y); }
	Vector2D operator *(const Vector2D &a) const { return Vector2D(x * a.x, y * a.y); }
	void operator *=(double a) { x *= a; y *= a; }

	Vector2D operator /(double a) const { return Vector2D(x / a, y / a); }
	void operator /=(double a) { x /= a; y /= a; }

	inline double dot(const Vector2D &a) const { return x * a.x + y * a.y; }
	inline double cross(const Vector2D &a) const { return x * a.y - y * a.x; }
	inline double module() { return std::sqrt(x*x + y*y); }
	inline static double distance(const Vector2D &a, const Vector2D &b)
	{
		double dx = a.x - b.x;
		double dy = a.y - b.y;
		return std::sqrt(dx * dx + dy * dy);
	}
	friend std::ostream& operator <<(std::ostream &output, const Vector2D &a)
	{
		output << "(" << a.x << "," << a.y << ")";
		return output;
	}

	inline void tangle(const Vector2D &L)
	{
		if (x < 0)
			x += L.x;
		else if (x >= L.x)
			x -= L.x;
		if (y < 0)
			y += L.y;
		else if (y >= L.y)
			y -= L.y;
	}
	inline Vector2D untangle(const Vector2D &pre, const Vector2D &L)
	{
		double dx = x - pre.x;
		if (dx > 0.5 * L.x)
			dx -= L.x;
		else if (dx < -0.5 * L.x)
			dx += L.x;
		double dy = y - pre.y;
		if (dy > 0.5 * L.y)
			dy -= L.y;
		else if (dy < -0.5 * L.y)
			dy += L.y;
		return Vector2D(pre.x + dx, pre.y + dy);
	}
};

struct Vector3D
{
	double x, y, z;
	Vector3D() { x = y = z = 0; }
	Vector3D(double a, double b, double c) :x(a), y(b), z(c) {}
	Vector3D(const Vector3D &a) :x(a.x), y(a.y), z(a.z) {}

	Vector3D operator -() const { return Vector3D(-x, -y, -z); }
	void operator =(double a) { x = y = z = a; }

	Vector3D operator +(const Vector3D &a) const { return Vector3D(x + a.x, y + a.y, z + a.z); }
	Vector3D operator +(double a) const { return Vector3D(x + a, y + a, z + a); }
	friend Vector3D operator +(double a, const Vector3D &b) { return Vector3D(a + b.x, a + b.y, a + b.z); }
	void operator +=(const Vector3D &a) { x += a.x; y += a.y; z += a.z; }
	void operator +=(double a) { x += a; y += a; z += a; }

	Vector3D operator -(const Vector3D &a) const { return Vector3D(x - a.x, y - a.y, z - a.z); }
	Vector3D operator -(double a) const { return Vector3D(x - a, y - a, z - a); }
	friend Vector3D operator -(double a, const Vector3D &b) { return Vector3D(a - b.x, a - b.y, a - b.z); }
	void operator -=(const Vector3D &a) { x -= a.x; y -= a.y, z -= a.z; }
	void operator -=(double a) { x -= a; y -= a; z -= a; }

	Vector3D operator *(double a) const { return Vector3D(x * a, y * a, z * a); }
	friend Vector3D operator *(double a, const Vector3D &b) { return Vector3D(a * b.x, a * b.y, a * b.z); }
	Vector3D operator *(const Vector3D &a) const { return Vector3D(x * a.x, y * a.y, z * a.z); }
	void operator *=(double a) { x *= a; y *= a; z *= a; }

	Vector3D operator /(double a) const { return Vector3D(x / a, y / a, z / a); }
	void operator /=(double a) { x /= a; y /= a; z /= a; }

	double dot(const Vector3D &a) const { return x * a.x + y * a.y + z * a.z; }
	Vector3D cross(const Vector3D &a) const { return Vector3D(y * a.z - z * a.y, z * a.x - x * a.z, x * a.y - y * a.x); }
	double square() const { return x * x + y * y + z * z; }
	double module() const { return std::sqrt(square()); }
	void normalize()
	{
		double r = module();
		x /= r;
		y /= r;
		z /= r;
	}
	friend std::ostream& operator <<(std::ostream &output, const Vector3D &a)
	{
		output << a.x << "\t" << a.y << "\t" << a.z;
		return output;
	}
};
#endif
