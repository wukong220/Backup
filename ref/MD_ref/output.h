#ifndef OUTPUT_H
#define OUTPUT_H
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <io.h>			//for Windows
//#include <unistd.h>   //for Linux
#include "particle.h"
#include "intg.h"

void set_output_folder(char * folder);

void set_output(std::ofstream &fout, double theta_max, double epsilon, double Pe, int N, double phi, int seed);

void snapshot_xyz(Particle * par, int N, int step, const Vector3D & lBox);

#endif
