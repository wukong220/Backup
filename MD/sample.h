#ifndef SAMPLE_H_
#define SAMPLE_H_

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>

#ifdef _MSC_VER
#include <io.h>
#else
#include <unistd.h>
#endif

#include "vector.h"
#include "force.h"

void set_output_folder(char * folder); 
void set_output(int nPar, double rho, double temp);

template <class Vector>
void snap_xyz(Vector *par, int N, int t, const Vector &lBox)
{
	ofstream fout;
	ofstream fout_snap;
	char dir_snap[100];

	char file[100];
	snprintf(file, 100, "%s/%08d.extxyz", dir_snap, t);
	fout_snap.open(file);
	fout_snap << N << endl;
	if (Dimen(*par) == 2)
		fout_snap << "Lattice=\"" << lBox.x << " 0 0 0 " << lBox.y << " 0 0 0 1\" Properties=species:S:1:pos:R:2 Time=" << t << endl;
	else if (Dimen(*par) == 3)
		fout_snap << "Lattice=\"" << lBox.x << " 0 0 0 " << lBox.y << " 0 0 0 " << lBox.z << "\" Properties=species:S:1:pos:R:3 Time=" << t << endl;
	for (int i = 0; i < N; i++)
	{
		fout_snap << fixed << setprecision(4); 
		fout_snap << "N\t" << par[i] << endl;
	}
	fout_snap.close();
};

template<class Vector, class Force>
void sample(Vector *par, int nPar, int t, const Vector &lBox, const Force *pair, double rho, double beta)
{
	static double p_tail = cal_p_tail(rho, lBox.x * 0.5);
	snap_xyz(par, nPar, t, lBox);
	double p;
	cout << p << endl;
};

#endif // !SAMPLE_H_

