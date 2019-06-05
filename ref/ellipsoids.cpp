#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <time.h>
#include <string.h>
//#include <stdlib.h>
//#include <stdio.h>
//#include <cstdlib>
using namespace std;

#define max(x, y)  ( x >= y? x : y )
#define min(x, y)  ( x <= y? x : y )
int number(double a, double b, double l, double phi);				//Number of ellipsoids with dencity of phi 

const double PI=3.1415926535897;	//pi

const int Type_ellipsoids = 1;				//atom type of ellipsoidstacles
const int MID_ellipsoids = 2 ;				//Mollecule ID of ellipsoids

const double L_box = 70.0;					//Lengh of the box

//ellipsoidstacle setup
const int ellipsoidsflag = 1;			//ellipsoidal finite size 
const int density = 1.0;			//Ellipsoidal density
vector<double> shape(3,1);			//shape of ellipsoids
vector<double> orient(4,0);			//orientation of ellipsoids
const double sa = 3;				//shapex
const double qw = 1;				//quaternion w
const double Phi_ellipsoids = 0.4;												//Area density of the ellipsoids
const int Num_ellipsoids = number(sa, shape[1], L_box, Phi_ellipsoids);		//Number of the ellipsoids

const int Num_atoms = Num_ellipsoids;			//Number of atoms
const int Num_bonds = 0;						//Number of bonds
const int Num_angles = 0;						//Number of angles
const int N_atomtypes = 1;						//Number of atom types
const int N_bondtypes = 0;						//Number of bond types
const int N_angltypes = 0;						//Number of angle types
	
struct Ranq2
{
	unsigned long long v, w;
	Ranq2(unsigned long long j) : v(4101842887655102017LL), w(1) {
		v ^= j;
		w = int64();
		v = int64();
	}
	inline unsigned long long int64() {
		v ^= v >> 17; v ^= v << 31; v ^= v >> 8;
		w = 4294957665U * (w & 0xffffffff) + (w >> 32);
		return v ^ w;
	}
	inline double doub() { return 5.42101086242752217E-20 * int64(); }
	inline unsigned int int32() { return (unsigned int)int64(); }
};
struct Normaldev : Ranq2 {
	double mu, sig;
	Normaldev(double mmu, double ssig, unsigned long long i) : Ranq2(i), mu(mmu), sig(ssig) {}
	double dev();	//	Return a normal deviate.
};


int main()
{
	double dx;
	double dy;
	double dr;
	int t;

	Ranq2 Rand((unsigned int)(time(NULL)));
	
	// 2 dimensional array for particles
	double**  par = new double* [Num_ellipsoids];	
	for (int i = 0;i < Num_ellipsoids;i++)
			par[i] = new double[2];
	shape[0] = sa;
	orient[0] = qw;
	
	
	// Create Num_ellipsoids random particlar position with minimal distance inter
	double l = L_box;
	for (int i = 0; i < Num_ellipsoids;)
	{
		par[i][0] = L_box * Rand.doub();
		par[i][1] = L_box * Rand.doub();
//		if(!checkin(L_box, inter, par, i))
			i++;
	}
	
	//Output file.data
	string filename = "ellipsoids.data";
	ofstream fout(filename);

	fout << "LAMMPS data file from ellipsoids.cpp:\n\n\t\t"
	<< Num_ellipsoids << " atoms\n\n\t\t"
	<< Num_ellipsoids << " ellipsoids\n\n\t\t"
	<< Num_bonds << " bonds\n\n\t\t"
	<< Num_angles <<" angles\n\n\t\t"
	<< N_atomtypes << " atom types\n\n\t\t"
	<< N_bondtypes << " bond types\n\n\t\t"
	<< N_angltypes << " angle types\n\n\t\t"
	<< 0 << " " << L_box << " xlo xhi\n\t\t"
	<< 0 << " " << L_box << " ylo yhi\n\t\t"
	<< -0.5 << " " << 0.5 << " zlo zhi\n\n\tAtoms\n"
	<< endl;
		
	//Atoms
	for (int i = 0; i < Num_ellipsoids; i++) 
	{
		fout << i + 1 << "  "
		<< Type_ellipsoids << "  "
		<< ellipsoidsflag << "  "
		<< density << "  "
		<< par[i][0] << "    "
		<< par[i][1] << "    "
		<< 0 << "  " << endl;

	}
	
	//Ellipsoids
	fout<< "\nEllipsoids\n\n";
	for (int i = 0; i < Num_ellipsoids; i++) 
	{
		fout << i + 1 << "  ";
		for (int j = 0; j < shape.size(); j++)
			fout << shape[j] << "  ";
		for (int k =0; k < orient.size(); k++)
			fout << orient[k] << "  ";
		fout << endl;
	}
	
	fout.close();
		
	for (int i=0; i < Num_ellipsoids; i++)
		delete[] par[i];
	delete[] par;
	
	return 0;
}



//	Return a normal deviate.
double Normaldev::dev()	
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

//Number of ellipsoidstacle with radius of r, with dencity of phi, in box of l
inline int number(double a, double b, double l, double phi)
{
	double s = PI * a * b;
	double S = l * l;
	double num = phi * S/s;
	return int(num);
}