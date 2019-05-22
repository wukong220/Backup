#ifndef INTG_H
#define INTG_H
#include "particle.h"
#include "cell.h"
#include "rand.h"

const double h = 5e-5;
const double hT = sqrt(2.0 * h);
const double hR = sqrt(6.0 * h);
void BD_simple(Particle &p, Normaldev *myran);
void BD_simple(Particle &p, Normaldev *myran, double Pe);
#endif
