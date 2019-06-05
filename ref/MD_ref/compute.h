#ifndef COMPUTE_H
#define COMPUTE_H
#include "particle.h"
#include "cell.h"
#include "rand.h"
#include "intg.h"
#include "output.h"

void ini(int N, double phi, double eps, double Pe0, double theta_max, int nStep, int seed);
void run(int nStep);
void end();

#endif
