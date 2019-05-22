/* ----------------------------------------------------------------------
   LAMMPS - Large-scale Atomic/Molecular Massively Parallel Simulator
   http://lammps.sandia.gov, Sandia National Laboratories
   Steve Plimpton, sjplimp@sandia.gov

   Copyright (2003) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under
   the GNU General Public License.

   See the README file in the top-level LAMMPS directory.
   
   
   Please use the current fix comment:
   
   fix ID group_ID activeforce  Positive_Force_value  keyword region =region_ID 
   
------------------------------------------------------------------------- */


#include "string.h"
#include "stdlib.h"
#include "fix_activeforce.h"
#include "atom.h"
#include "update.h"
#include "modify.h"
#include "domain.h"
#include "region.h"
#include "respa.h"
#include "input.h"
#include "variable.h"
#include "memory.h"
#include "error.h"
#include "force.h"
#include "math.h"
#include "stdio.h"
#include "atom_vec.h"


using namespace LAMMPS_NS;
using namespace FixConst;

enum{NONE,CONSTANT,EQUAL,ATOM};

/* ---------------------------------------------------------------------- */

FixActiveForce::FixActiveForce(LAMMPS *lmp, int narg, char **arg) :
  Fix(lmp, narg, arg)
{
  if (narg < 4) error->all(FLERR,"Illegal fix activeforce command");

  dynamic_group_allow = 1;
  scalar_flag = 1;
  vector_flag = 1;
  size_vector = 3;
  global_freq = 1;
  extscalar = 1;
  extvector = 1;

  pstr = NULL;

  if (strstr(arg[3],"v_") == arg[3]) {
    int n = strlen(&arg[3][2]) + 1;
    pstr = new char[n];
    strcpy(pstr,&arg[3][2]);
  } else {
    pvalue = force->numeric(FLERR,arg[3]);
    pstyle = CONSTANT;
  }

  // optional args

  iregion = -1;
  idregion = NULL;
  estr = NULL;

  int iarg = 4;
  while (iarg < narg) {
    if (strcmp(arg[iarg],"region") == 0) {
      if (iarg+2 > narg) error->all(FLERR,"Illegal fix activeforce command");
      iregion = domain->find_region(arg[iarg+1]);
      if (iregion == -1)
        error->all(FLERR,"Region ID for fix activeforce does not exist");
      int n = strlen(arg[iarg+1]) + 1;
      idregion = new char[n];
      strcpy(idregion,arg[iarg+1]);
      iarg += 2;
    } else if (strcmp(arg[iarg],"energy") == 0) {
      if (iarg+2 > narg) error->all(FLERR,"Illegal fix activeforce command");
      if (strstr(arg[iarg+1],"v_") == arg[iarg+1]) {
        int n = strlen(&arg[iarg+1][2]) + 1;
        estr = new char[n];
        strcpy(estr,&arg[iarg+1][2]);
      } else error->all(FLERR,"Illegal fix activeforce command");
      iarg += 2;
    } else error->all(FLERR,"Illegal fix activeforce command");
  }

  force_flag = 0;
  foriginal[0] = foriginal[1] = foriginal[2] = foriginal[3] = 0.0;

  maxatom = atom->nmax;
  memory->create(sforce,maxatom,2,"activeforce:sforce");
}

/* ---------------------------------------------------------------------- */

FixActiveForce::~FixActiveForce()
{
  delete [] pstr;
  delete [] estr;
  delete [] idregion;
  memory->destroy(sforce);
}

/* ---------------------------------------------------------------------- */

int FixActiveForce::setmask()
{
  int mask = 0;
  mask |= POST_FORCE;
  mask |= THERMO_ENERGY;
  mask |= POST_FORCE_RESPA;
  mask |= MIN_POST_FORCE;
  return mask;
}

/* ---------------------------------------------------------------------- */

void FixActiveForce::init()
{
  // check variables

  if (pstr) {
    pvar = input->variable->find(pstr);
    if (pvar < 0)
      error->all(FLERR,"Variable name for fix activeforce does not exist");
    if (input->variable->equalstyle(pvar)) pstyle = EQUAL;
    else error->all(FLERR,"Variable for fix activeforce is invalid style");
  }
  if (estr) {
    evar = input->variable->find(estr);
    if (evar < 0)
      error->all(FLERR,"Variable name for fix activeforce does not exist");
    if (input->variable->atomstyle(evar)) estyle = ATOM;
    else error->all(FLERR,"Variable for fix activeforce is invalid style");
  } else estyle = NONE;

  // set index and check validity of region

  if (iregion >= 0) {
    iregion = domain->find_region(idregion);
    if (iregion == -1)
      error->all(FLERR,"Region ID for fix activeforce does not exist");
  }

  if (pstyle == ATOM)
    varflag = ATOM;
  else if (pstyle == EQUAL)
    varflag = EQUAL;
  else varflag = CONSTANT;

  if (varflag == CONSTANT && estyle != NONE)
    error->all(FLERR,"Cannot use variable energy with "
               "constant force in fix activeforce");
  if ((varflag == EQUAL || varflag == ATOM) &&
      update->whichflag == 2 && estyle == NONE)
    error->all(FLERR,"Must use variable energy with fix activeforce");

  if (strstr(update->integrate_style,"respa"))
    nlevels_respa = ((Respa *) update->integrate)->nlevels;
}

/* ---------------------------------------------------------------------- */

void FixActiveForce::setup(int vflag)
{
  if (strstr(update->integrate_style,"verlet"))
    post_force(vflag);
  else {
    ((Respa *) update->integrate)->copy_flevel_f(nlevels_respa-1);
    post_force_respa(vflag,nlevels_respa-1,0);
    ((Respa *) update->integrate)->copy_f_flevel(nlevels_respa-1);
  }
}

/* ---------------------------------------------------------------------- */

void FixActiveForce::min_setup(int vflag)
{
  post_force(vflag);
}

/* ---------------------------------------------------------------------- */

void FixActiveForce::post_force(int vflag)
{
  double **x = atom->x;
  double **f = atom->f;
  int *mask = atom->mask;
  imageint *image = atom->image;
  int nlocal = atom->nlocal;
  //声明力变量和dipole变量 
  double xvalue,yvalue,zvalue;
  double **mu = atom->mu; 

  // update region if necessary

  Region *region = NULL;
  if (iregion >= 0) {
    region = domain->regions[iregion];
    region->prematch();
  }

  // reallocate sforce array if necessary

  if (varflag == ATOM ) {
  	error->all(FLERR,"Current fix activeforce does not support the variable of atom type");
    maxatom = atom->nmax;
    memory->destroy(sforce);
    memory->create(sforce,maxatom,2,"activeforce:sforce");
  }

  // foriginal[0] = "potential energy" for added force
  // foriginal[123] = force on atoms before extra force added

  foriginal[0] = foriginal[1] = foriginal[2] = foriginal[3] = 0.0;
  force_flag = 0;

  // constant force
  // potential energy = - x dot f in unwrapped coords


  if (varflag == CONSTANT) {
    double unwrap[3];
    for (int i = 0; i < nlocal; i++){
    
      if (mask[i] & groupbit) {
        if (region && !region->match(x[i][0],x[i][1],x[i][2])) continue;
        domain->unmap(x[i],image[i],unwrap);
        ////////////////////////////////////////////////////////////////////
        ////计算沿dipole方向力  
        ///d_mu/dt = omega cross mu
        ///renormalize mu to unit dipole
        if(mu[i][3] > 0.0){
        	xvalue = pvalue *  mu[i][0]/mu[i][3];
			yvalue = pvalue *  mu[i][1]/mu[i][3];      //力在dipole取向上分解 
			zvalue = pvalue *  mu[i][2]/mu[i][3];
            ///////////////////////////////////////
            foriginal[0] -= xvalue*unwrap[0] + yvalue*unwrap[1] + zvalue*unwrap[2];
            foriginal[1] += f[i][0];
            foriginal[2] += f[i][1];
            foriginal[3] += f[i][2];
            
			f[i][0] += xvalue;        
			f[i][1] += yvalue;        
			f[i][2] += zvalue;
        }
        //else{
        //	error->all(FLERR,"Make sure you have set dipole moment for each particle when using fix activeforce");
        //}
      }
 
   }
  // variable force, wrap with clear/add
  // potential energy = evar if defined, else 0.0
  // wrap with clear/add

}else {

    modify->clearstep_compute();
    if (pstyle == EQUAL) pvalue = input->variable->compute_equal(pvar);
    modify->addstep_compute(update->ntimestep + 1);
    
    double un_wrap[3];
    
    for (int i = 0; i < nlocal; i++)
      if (mask[i] & groupbit) {
        if (region && !region->match(x[i][0],x[i][1],x[i][2])) continue;
        domain->unmap(x[i],image[i],un_wrap);
        if(mu[i][3] > 0.0){        	
        	xvalue = pvalue *  mu[i][0]/mu[i][3];
			yvalue = pvalue *  mu[i][1]/mu[i][3];      //力在dipole取向上分解 
			zvalue = pvalue *  mu[i][2]/mu[i][3];
            ///////////////////////////////////////
            foriginal[0] -= xvalue*un_wrap[0] + yvalue*un_wrap[1] + zvalue*un_wrap[2];
            foriginal[1] += f[i][0];
            foriginal[2] += f[i][1];
            foriginal[3] += f[i][2];
            
			f[i][0] += xvalue;        
			f[i][1] += yvalue;        
			f[i][2] += zvalue;
        }
        
      }
  }
}

/* ---------------------------------------------------------------------- */

void FixActiveForce::post_force_respa(int vflag, int ilevel, int iloop)
{
  if (ilevel == nlevels_respa-1) post_force(vflag);
}

/* ---------------------------------------------------------------------- */

void FixActiveForce::min_post_force(int vflag)
{
  post_force(vflag);
}

/* ----------------------------------------------------------------------
   potential energy of added force
------------------------------------------------------------------------- */

double FixActiveForce::compute_scalar()
{
  // only sum across procs one time

  if (force_flag == 0) {
    MPI_Allreduce(foriginal,foriginal_all,4,MPI_DOUBLE,MPI_SUM,world);
    force_flag = 1;
  }
  return foriginal_all[0];
}

/* ----------------------------------------------------------------------
   return components of total force on fix group before force was changed
------------------------------------------------------------------------- */

double FixActiveForce::compute_vector(int n)
{
  // only sum across procs one time

  if (force_flag == 0) {
    MPI_Allreduce(foriginal,foriginal_all,4,MPI_DOUBLE,MPI_SUM,world);
    force_flag = 1;
  }
  return foriginal_all[n+1];
}

/* ----------------------------------------------------------------------
   memory usage of local atom-based array
------------------------------------------------------------------------- */

double FixActiveForce::memory_usage()
{
  double bytes = 0.0;
  if (varflag == ATOM) bytes = atom->nmax*2 * sizeof(double);
  return bytes;
}
