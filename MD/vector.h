#ifndef VECTOR_H_
#define VECTOR_H_

#include <iostream>
#include <cstdlib>
#include <cmath>
#include "rand.h"

using namespace std;
const double PI = 3.14159265358979;

namespace VECTOR
{
	namespace VECTOR2D
	{
		class Vector
		{
		private:
			enum { Dimension = 2 };

		public:
			double x;
			double y;
			Vector() { x = y = 0; };
			Vector(double a) { x = y = a; };
			Vector(double a, double b) :x(a), y(b) {};
			Vector(const Vector &a) :x(a.x), y(a.y) {};

			friend int Dimen(Vector &a) { return a.Dimension; };		//访问私有数据
			double X() const { return x; };
			double Y() const { return y; };

			static Vector randf(Rand *myRand) { return Vector(myRand->doub(), myRand->doub()); };
			friend ostream& operator <<(ostream &output, const Vector &a);

			double dot(const Vector &a) const { return x * a.x + y * a.y; };		//基本运算
			double cross(const Vector &a) const { return x * a.y - y * a.x; };
			double square() const { return x * x + y * y; };
			double module() const { return sqrt(square()); };
			friend double distance(const Vector &a, const Vector &b);
			double volume() const { return x * y; };

			bool operator == (const Vector &a) const { return x == a.x && y == a.y; };
			bool operator != (const Vector &a) const { return x != a.x || y != a.y; };

			void zero() { x = y = 0.; };
			Vector operator -() const { return Vector(-x, -y); };
			void operator =(double a) { x = y = a; };

			Vector operator +(const Vector &a) const { return Vector(x + a.x, y + a.y); };	//重载运算符
			Vector operator +(double a) const { return Vector(x + a, y + a); };
			friend Vector operator +(double a, const Vector &b) { return Vector(a + b.x, a + b.y); };
			void operator +=(const Vector &a) { x += a.x; y += a.y; };
			void operator +=(double a) { x += a; y += a; };

			Vector operator -(const Vector &a) const { return Vector(x - a.x, y - a.y); };
			Vector operator -(double a) const { return Vector(x - a, y - a); };
			friend Vector operator -(double a, const Vector &b) { return Vector(a - b.x, a - b.y); };
			void operator -=(const Vector &a) { x -= a.x; y -= a.y; };
			void operator -=(double a) { x -= a; y -= a; };

			Vector operator *(double a) const { return Vector(x * a, y* a); };
			friend Vector operator *(double a, const Vector &b) { return Vector(a*b.x, a * b.y); };
			Vector operator *(const Vector &a) const { return Vector(x * a.x, y * a.y); };
			void operator *=(double a) { x *= a; y *= a; };

			Vector operator /(double a) const { return Vector(x / a, y / a); };
			void operator /=(double a) { x /= a; y /= a; };
		};

		inline double distance(const Vector &a, const Vector &b)
		{
			double dx = a.x - b.x;
			double dy = a.y - b.y;
			return sqrt(dx * dx + dy * dy);
		};

		inline ostream& operator <<(ostream &output, const Vector &a)
		{
			output << a.x << "\t" << a.y;
			return output;
		};
	};

	namespace VECTOR3D
	{
		class Vector
		{
		private:
			enum { Dimension = 3 };
		public:
			double x;
			double y;
			double z;
			Vector() { x = y = z = 0; };			//构造函数
			Vector(double a) { x = y = z = a; };
			Vector(double a, double b, double c) :x(a), y(b), z(c) {};
			Vector(const Vector &a) :x(a.x), y(a.y), z(a.z) {};

			double X() const { return x; };		//访问私有数据
			double Y() const { return y; };
			double Z() const { return z; };
			friend int Dimen(Vector &a) { return a.Dimension; };

			static Vector randf(Rand *myRand) { return Vector(myRand->doub(), myRand->doub(), myRand->doub()); };
			friend ostream& operator <<(ostream &output, const Vector &a);

			double dot(const Vector &a) const { return x * a.x + y * a.y + z * a.z; };		//基本运算
			Vector cross(const Vector &a) const { return Vector(y * a.z - z * a.y, z * a.x - x * a.z, x * a.y - y * a.x); };
			double square() const { return x * x + y * y + z * z; };
			double module() const { return sqrt(square()); };
			void normalize() { double r = module(); x /= r; y /= r; z /= r; };
			friend double distance(const Vector &a, const Vector &b);
			double volume() const { return x * y * z; };

			void zero() { x = y = z = 0.; };
			Vector operator -() const { return Vector(-x, -y, -z); };
			void operator =(double a) { x = y = z = a; };

			Vector operator +(const Vector &a) const { return Vector(x + a.x, y + a.y, z + a.z); };//重载运算符
			Vector operator +(double a) const { return Vector(x + a, y + a, z + a); };
			friend Vector operator +(double a, const Vector &b) { return Vector(a + b.x, a + b.y, a + b.z); };
			void operator +=(const Vector &a) { x += a.x; y += a.y; z += a.z; };
			void operator +=(double a) { x += a; y += a; z += a; };

			Vector operator -(const Vector &a) const { return Vector(x - a.x, y - a.y, z - a.z); };
			Vector operator -(double a) const { return Vector(x - a, y - a, z - a); };
			friend Vector operator -(double a, const Vector &b) { return Vector(a - b.x, a - b.y, a - b.z); };
			void operator -=(const Vector &a) { x -= a.x; y -= a.y, z -= a.z; };
			void operator -=(double a) { x -= a; y -= a; z -= a; };

			Vector operator *(double a) const { return Vector(x * a, y * a, z * a); };
			friend Vector operator *(double a, const Vector &b) { return Vector(a * b.x, a * b.y, a * b.z); };
			Vector operator *(const Vector &a) const { return Vector(x * a.x, y * a.y, z * a.z); };
			void operator *=(double a) { x *= a; y *= a; z *= a; };

			Vector operator /(double a) const { return Vector(x / a, y / a, z / a); };
			void operator /=(double a) { x /= a; y /= a; z /= a; };
		};

		inline double distance(const Vector &a, const Vector &b)
		{
			double dx = a.x - b.x;
			double dy = a.y - b.y;
			double dz = a.z - b.z;
			return sqrt(dx * dx + dy * dy + dz*dz);
		};

		inline ostream& operator <<(ostream &output, const Vector &a)
		{
			output << a.x << "\t" << a.y << "\t" << a.z;
			return output;
		};

	};

	template <class Vector>
	void tangle(Vector &a, const Vector L)
	{
		if (a.x < 0)
			a.x += L.x;
		else if (a.x >= L.x)
			a.x -= L.x;
		if (a.y < 0)
			a.y += L.y;
		else if (a.y >= L.y)
			a.y -= L.y;
		if (Dimen(a) == 3)
		{
			if (a.z < 0)
				a.z += L.z;
			else if (a.z >= L.z)
				a.z -= L.z;
		}
	}

	template <class Vector>
	void untangle(Vector &Rm, Vector R, const Vector L)
	{
		Vector a = Rm - R;
		if (a.x < -0.5 * L.x)
			Rm.x += L.x;
		else if (a.x > 0.5 * L.x)
			Rm.x -= L.x;
		if (a.y < -0.5 * L.y)
			Rm.y += L.y;
		else if (a.y > 0.5 * L.y)
			Rm.y -= L.y;
		if (Dimen(a) == 3)
		{
			if (a.z < -0.5 * L.z)
				Rm.z += L.z;
			else if (a.z > 0.5 * L.z)
				Rm.z -= L.z;
		}
	};
}
#endif // !VECTOR_H_