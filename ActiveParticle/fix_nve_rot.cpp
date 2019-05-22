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

#include "stdio.h"
#include "string.h"
#include "fix_nve_rot.h"
#include "atom.h"
#include "force.h"
#include "update.h"
#include "respa.h"
#include "error.h"
#include "random_mars.h"
#include "math_extra.h"
#include "comm.h"
#include "domain.h"
#include "atom_vec.h"
#include "math.h"

using namespace LAMMPS_NS;
using namespace FixConst;

/* ---------------------------------------------------------------------- */

FixNVERot::FixNVERot(LAMMPS *lmp, int narg, char **arg) :
  FixNVE(lmp, narg, arg)
{
  if (strcmp(style,"nve/rot") != 0 && narg < 5)
    error->all(FLERR,"Illegal fix nve command");
  diffusion_rot = force->numeric(FLERR,arg[3]);
  
  int seed = force->inumeric(FLERR,arg[4]);
  random = new RanMars(lmp,seed + comm->me);
  
  dynamic_group_allow = 1;
  time_integrate = 1;
  
  if(!atom->mu_flag)
    error->all(FLERR,"Fix nve/rot requires atom attribute mu");
}

/* ---------------------------------------------------------------------- */

/* ---------------------------------------------------------------------- */

void FixNVERot::init()
{
  dtv = update->dt;
  dtf = 0.5 * update->dt * force->ftm2v;

  if (strstr(update->integrate_style,"respa"))
    error->all(FLERR,"Illegal fix nve/rot command doesnot support respa");

  // check that all particles are non-zero mu(dipole)
  // no point particles allowed

  int *mask = atom->mask;
  int nlocal = atom->nlocal;
  double **mu = atom->mu;
  for (int i = 0; i < nlocal; i++)
    if (mask[i] & groupbit)
      if (mu[i][3]== 0.0)
        error->one(FLERR,"Fix nve/rot requires dipole type with non-zero mu"); 

}

/* ----------------------------------------------------------------------
   allow for both per-type and per-atom mass
------------------------------------------------------------------------- */

void FixNVERot::initial_integrate(int vflag)
{
  double dtfm,msq,scale;
  double g[3];
  // update v and x of atoms in group

  double **x = atom->x;
  double **v = atom->v;
  double **f = atom->f;
  
  double *mass = atom->mass;
  int *type = atom->type;
  int *mask = atom->mask;
  
  int nlocal = atom->nlocal;  
  if (igroup == atom->firstgroup) nlocal = atom->nfirst;
  
  for (int i = 0; i < nlocal; i++){
    if (mask[i] & groupbit) {
        dtfm = dtf / mass[type[i]];
        
        v[i][0] += dtfm * f[i][0];
        v[i][1] += dtfm * f[i][1];
        v[i][2] += dtfm * f[i][2];
        x[i][0] += dtv * v[i][0];
        x[i][1] += dtv * v[i][1];
        x[i][2] += dtv * v[i][2];
    }
  }
  
  double **mu = atom->mu;
  double omg[3]; 
  for (int i = 0; i < nlocal; i++){
    if (mask[i] & groupbit){
      if (mu[i][3] > 0.0) {
          omg[0] = sqrt(2.0*diffusion_rot/dtv)*random->gaussian();  // d_sita/dt = sqrt(2Dr)*gauss
          omg[1] = sqrt(2.0*diffusion_rot/dtv)*random->gaussian(); 
          omg[2] = sqrt(2.0*diffusion_rot/dtv)*random->gaussian();
		  if (domain->dimension == 2){
              omg[0] = 0.0;
              omg[1] = 0.0;
		  }
          g[0] = mu[i][0] + dtv * (omg[1]*mu[i][2]-omg[2]*mu[i][1]);
          g[1] = mu[i][1] + dtv * (omg[2]*mu[i][0]-omg[0]*mu[i][2]);
          g[2] = mu[i][2] + dtv * (omg[0]*mu[i][1]-omg[1]*mu[i][0]);
          msq = g[0]*g[0] + g[1]*g[1] + g[2]*g[2];          
          scale = mu[i][3]/sqrt(msq);
          mu[i][0] = g[0]*scale;
          mu[i][1] = g[1]*scale;
          mu[i][2] = g[2]*scale;
      }	
	}
  }
}

/* ---------------------------------------------------------------------- */

void FixNVERot::final_integrate()
{
  double dtfm;

  // update v of atoms in group

  double **v = atom->v;
  double **f = atom->f;
  double *mass = atom->mass;
  int *type = atom->type;
  int *mask = atom->mask;
  int nlocal = atom->nlocal;
  if (igroup == atom->firstgroup) nlocal = atom->nfirst;
    for (int i = 0; i < nlocal; i++)
      if (mask[i] & groupbit) {
        dtfm = dtf / mass[type[i]];
        v[i][0] += dtfm * f[i][0];
        v[i][1] += dtfm * f[i][1];
        v[i][2] += dtfm * f[i][2];
      }
}


