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
#include "fix_browmv.h"
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

//fix 1 all browdyn coeff  Temp  Dr  seed;   
//v(t) = F(t)/coeff + sqrt(2*D*delt_t)*gauss
//r(t+dt) = r(t)+ v(t)*dt
// oumic = sqrt(2*Dr*delt_t) * gauss;
// sita(t+dt) = sita(t) + oumic*dt
/* ---------------------------------------------------------------------- */

FixBrowMv::FixBrowMv(LAMMPS *lmp, int narg, char **arg) :
  Fix(lmp, narg, arg)
{
  if (strcmp(style,"bd") != 0 && narg < 7)
    error->all(FLERR,"Illegal fix bd command");
    
  ivcoeff = force->numeric(FLERR,arg[3]);
  temp  = force->numeric(FLERR,arg[4]);  
  diffusion_rot  = force->numeric(FLERR,arg[5]);   
  int seed = force->inumeric(FLERR,arg[6]);
  if(ivcoeff <=0.0 || temp<0.0 || diffusion_rot <0.0 || seed <=0)  //rot = 3.0 * kbT/coeff
     error->all(FLERR," coeff,temp, rot, and seed should be non-negative");
  random = new RanMars(lmp,seed + comm->me);  
  ivcoeff = 1.0/ivcoeff;
  dynamic_group_allow = 1;
  time_integrate = 1;
}

/* ---------------------------------------------------------------------- */

int FixBrowMv::setmask()
{
  int mask = 0;
  mask |= INITIAL_INTEGRATE;
  mask |= FINAL_INTEGRATE;
 // mask |= INITIAL_INTEGRATE_RESPA;
 // mask |= FINAL_INTEGRATE_RESPA;
  return mask;
}

/* ---------------------------------------------------------------------- */

void FixBrowMv::init()
{
  dtv = update->dt;
  //dtf = 0.5 * update->dt * force->ftm2v;
}

/* ----------------------------------------------------------------------
   allow for both per-type and per-atom mass
------------------------------------------------------------------------- */

void FixBrowMv::initial_integrate(int vflag)
{
  // update v and x of atoms in group

  double **x = atom->x;
  double **v = atom->v;
  double **f = atom->f;
  double ref_t ;
  int *type = atom->type;
  int *mask = atom->mask;
  int nlocal = atom->nlocal;
  if (igroup == atom->firstgroup) nlocal = atom->nfirst;
  
  if(temp > 1e-30 )  ref_t = sqrt(2.0*force->boltz*temp*ivcoeff/dtv);
  else  ref_t = 0.0;
  
  double frand[3]; 
  for (int i = 0; i < nlocal; i++){
  	if (mask[i] & groupbit) {  		
  	      if(ref_t != 0.0) {
	          frand[0] = ref_t*random->gaussian();
		      frand[1] = ref_t*random->gaussian();
		      frand[2] = ref_t*random->gaussian();
  	      }else{
	          frand[0] = 0.0;
		      frand[1] = 0.0;
		      frand[2] = 0.0;
  	      }
	    if (domain->dimension == 2){
  			frand[2] = 0.0; 
  		} 

        v[i][0] = ivcoeff * f[i][0] + frand[0]; 
        v[i][1] = ivcoeff * f[i][1] + frand[1];
        v[i][2] = ivcoeff * f[i][2] + frand[2];
        
        x[i][0] += dtv * v[i][0];
        x[i][1] += dtv * v[i][1];
        x[i][2] += dtv * v[i][2];
    }  	
  	
  }
  
  if(diffusion_rot != 0.0){
  double **mu = atom->mu;
  double omg[3],g[3],msq,scale;
  for (int i = 0; i < nlocal; i++){
    if (mask[i] & groupbit){
      if (mu[i][3] > 0.0) {
          omg[0] = sqrt(2.0*diffusion_rot/dtv)*random->gaussian();  // d_sita/dt = sqrt(2*Dr*delt_t)*gauss
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
}

/* ---------------------------------------------------------------------- */

void FixBrowMv::final_integrate()
{
  //do nothing 
}

/* ---------------------------------------------------------------------- */

void FixBrowMv::reset_dt()
{
  dtv = update->dt;
  //dtf = 0.5 * update->dt * force->ftm2v;
}
