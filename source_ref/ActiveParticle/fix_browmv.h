/* ----------------------------------------------------------------------
   LAMMPS - Large-scale Atomic/Molecular Massively Parallel Simulator
   http://lammps.sandia.gov, Sandia National Laboratories
   Steve Plimpton, sjplimp@sandia.gov

   Copyright (2003) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under
   the GNU General Public License.

   See the README file in the top-level LAMMPS directory.
------------------------------------------------------------------------- */

#ifdef FIX_CLASS

FixStyle(browmv,FixBrowMv)

#else

#ifndef LMP_FIX_BROWMV_H
#define LMP_FIX_BROWMV_H

#include "fix.h"

namespace LAMMPS_NS {

class FixBrowMv : public Fix {
 public:
  FixBrowMv(class LAMMPS *, int, char **);
  virtual ~FixBrowMv() {}
  int setmask();
  virtual void init();
  virtual void initial_integrate(int);
  virtual void final_integrate();
  virtual void reset_dt();

 protected:
  double dtv;
  int mass_require;  
  double ivcoeff,temp,diffusion_rot;
  int seed;
  class RanMars *random;
  
};

}

#endif
#endif

/* ERROR/WARNING messages:

E: Illegal ... command

Self-explanatory.  Check the input script syntax and compare to the
documentation for the command.  You can use -echo screen as a
command-line option when running LAMMPS to see the offending line.

*/
