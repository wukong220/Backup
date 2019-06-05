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

#include "string.h"
#include "stdlib.h"
#include "fix_vforce.h"
#include "atom.h"
#include "atom_masks.h"
#include "accelerator_kokkos.h"
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

using namespace LAMMPS_NS;
using namespace FixConst;

enum{NONE,CONSTANT,EQUAL,ATOM};

/* ---------------------------------------------------------------------- */

FixVForce::FixVForce(LAMMPS *lmp, int narg, char **arg) :
  Fix(lmp, narg, arg)
{
  if (narg < 4) error->all(FLERR,"Illegal fix addforce command");

  dynamic_group_allow = 1;
  scalar_flag = 1;
  vector_flag = 1;
  size_vector = 3;
  global_freq = 1;
  extscalar = 1;
  extvector = 1;

  xstr = NULL;

  if (strstr(arg[3],"v_") == arg[3]) {
    int n = strlen(&arg[3][2]) + 1;
    xstr = new char[n];
    strcpy(xstr,&arg[3][2]);
  } else {
    xvalue = force->numeric(FLERR,arg[3]);
    xstyle = CONSTANT;
  }
 
  // optional args

  nevery = 1;
  iregion = -1;
  idregion = NULL;
  estr = NULL;

  int iarg = 4;
  while (iarg < narg) {
    if (strcmp(arg[iarg],"every") == 0) {
      if (iarg+2 > narg) error->all(FLERR,"Illegal fix addforce command");
      nevery = atoi(arg[iarg+1]);
      if (nevery <= 0) error->all(FLERR,"Illegal fix addforce command");
      iarg += 2;
    } else if (strcmp(arg[iarg],"region") == 0) {
      if (iarg+2 > narg) error->all(FLERR,"Illegal fix addforce command");
      iregion = domain->find_region(arg[iarg+1]);
      if (iregion == -1)
        error->all(FLERR,"Region ID for fix addforce does not exist");
      int n = strlen(arg[iarg+1]) + 1;
      idregion = new char[n];
      strcpy(idregion,arg[iarg+1]);
      iarg += 2;
    } else if (strcmp(arg[iarg],"energy") == 0) {
      if (iarg+2 > narg) error->all(FLERR,"Illegal fix addforce command");
      if (strstr(arg[iarg+1],"v_") == arg[iarg+1]) {
        int n = strlen(&arg[iarg+1][2]) + 1;
        estr = new char[n];
        strcpy(estr,&arg[iarg+1][2]);
      } else error->all(FLERR,"Illegal fix addforce command");
      iarg += 2;
    } else error->all(FLERR,"Illegal fix addforce command");
  }

  force_flag = 0;
  foriginal[0] = foriginal[1] = foriginal[2] = foriginal[3] = 0.0;

  maxatom = atom->nmax;
  memory->create(sforce,maxatom,2,"addforce:sforce");
}

/* ---------------------------------------------------------------------- */

FixVForce::~FixVForce()
{
  delete [] xstr;
  delete [] estr;
  delete [] idregion;
  memory->destroy(sforce);
}

/* ---------------------------------------------------------------------- */

int FixVForce::setmask()
{
  datamask_read = datamask_modify = 0;

  int mask = 0;
  mask |= POST_FORCE;
  mask |= THERMO_ENERGY;
  mask |= POST_FORCE_RESPA;
  mask |= MIN_POST_FORCE;
  return mask;
}

/* ---------------------------------------------------------------------- */

void FixVForce::init()
{
  // check variables

  if (xstr) {
    xvar = input->variable->find(xstr);
    if (xvar < 0)
      error->all(FLERR,"Variable name for fix addforce does not exist");
    if (input->variable->equalstyle(xvar)) xstyle = EQUAL;
    else if (input->variable->atomstyle(xvar)) xstyle = ATOM;
    else error->all(FLERR,"Variable for fix addforce is invalid style");
  }
  if (estr) {
    evar = input->variable->find(estr);
    if (evar < 0)
      error->all(FLERR,"Variable name for fix addforce does not exist");
    if (input->variable->atomstyle(evar)) estyle = ATOM;
    else error->all(FLERR,"Variable for fix addforce is invalid style");
  } else estyle = NONE;

  // set index and check validity of region

  if (iregion >= 0) {
    iregion = domain->find_region(idregion);
    if (iregion == -1)
      error->all(FLERR,"Region ID for fix addforce does not exist");
  }

  if (xstyle == ATOM )
    varflag = ATOM;
  else if (xstyle == EQUAL )
    varflag = EQUAL;
  else varflag = CONSTANT;

  if (varflag == CONSTANT && estyle != NONE)
    error->all(FLERR,"Cannot use variable energy with "
               "constant force in fix addforce");
  if ((varflag == EQUAL || varflag == ATOM) &&
      update->whichflag == 2 && estyle == NONE)
    error->all(FLERR,"Must use variable energy with fix addforce");

  if (strstr(update->integrate_style,"respa"))
    nlevels_respa = ((Respa *) update->integrate)->nlevels;
}

/* ---------------------------------------------------------------------- */

void FixVForce::setup(int vflag)
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

void FixVForce::min_setup(int vflag)
{
  post_force(vflag);
}

/* ---------------------------------------------------------------------- */

void FixVForce::post_force(int vflag)
{
  double **x = atom->x;
  double **f = atom->f;
  double **v = atom->v;
  int *mask = atom->mask;
  imageint *image = atom->image;
  int nlocal = atom->nlocal;

  if (update->ntimestep % nevery) return;

  if (lmp->kokkos)
    atom->sync_modify(Host, (unsigned int) (F_MASK | MASK_MASK),
                      (unsigned int) F_MASK);

  // update region if necessary

  Region *region = NULL;
  if (iregion >= 0) {
    region = domain->regions[iregion];
    region->prematch();
  }

  // reallocate sforce array if necessary

  if ((varflag == ATOM || estyle == ATOM) && nlocal > maxatom) {
    maxatom = atom->nmax;
    memory->destroy(sforce);
    memory->create(sforce,maxatom,2,"addforce:sforce");
  }

  // foriginal[0] = "potential energy" for added force
  // foriginal[123] = force on atoms before extra force added

  foriginal[0] = foriginal[1] = foriginal[2] = foriginal[3] = 0.0;
  force_flag = 0;

  // constant force
  // potential energy = - x dot f in unwrapped coords

  if (varflag == CONSTANT) {
    double unwrap[3];
    for (int i = 0; i < nlocal; i++)
      if (mask[i] & groupbit) {
        if (region && !region->match(x[i][0],x[i][1],x[i][2])) continue;
        
        domain->unmap(x[i],image[i],unwrap);
        double xf,yf,zf;
        double vv = sqrt(v[i][0] * v[i][0] + v[i][1] * v[i][1] + v[i][2] * v[i][2]);
        if( vv != 0.0){
        	xf = xvalue * v[i][0]/vv;
        	yf = xvalue * v[i][1]/vv;
        	zf = xvalue * v[i][2]/vv;
        }else{
		    xf = yf = zf =0.0;        	
        }        
        foriginal[0] -= xf*unwrap[0] + yf*unwrap[1] + zf*unwrap[2];
        foriginal[1] += f[i][0];
        foriginal[2] += f[i][1];
        foriginal[3] += f[i][2];
        f[i][0] += xf;
        f[i][1] += yf;
        f[i][2] += zf;
      }

  // variable force, wrap with clear/add
  // potential energy = evar if defined, else 0.0
  // wrap with clear/add

  } else {

    modify->clearstep_compute();

    if (xstyle == EQUAL) xvalue = input->variable->compute_equal(xvar);
    else if (xstyle == ATOM)
      input->variable->compute_atom(xvar,igroup,&sforce[0][0],2,0);
    if (estyle == ATOM)
      input->variable->compute_atom(evar,igroup,&sforce[0][1],2,0);

    modify->addstep_compute(update->ntimestep + 1);

    for (int i = 0; i < nlocal; i++)
      if (mask[i] & groupbit) {
        if (region && !region->match(x[i][0],x[i][1],x[i][2])) continue;
        if (estyle == ATOM) foriginal[0] += sforce[i][1];
        foriginal[1] += f[i][0];
        foriginal[2] += f[i][1];
        foriginal[3] += f[i][2];
        
        double xf,yf,zf;
        xf = yf = zf = 0.0;
        double vv = sqrt(v[i][0] * v[i][0] + v[i][1] * v[i][1] + v[i][2] * v[i][2]);
        if (xstyle == ATOM){
            if( vv != 0.0){ 
        	   xf = sforce[i][0] * v[i][0]/vv;
        	   yf = sforce[i][0] * v[i][1]/vv;
        	   zf = sforce[i][0] * v[i][2]/vv;
           }           
           f[i][0] += xf;
		   f[i][1] += yf;
		   f[i][2] += zf;
        } 
        else if (xstyle){
        	if( vv != 0.0){ 
        	   xf = xvalue * v[i][0]/vv;
        	   yf = xvalue * v[i][1]/vv;
        	   zf = xvalue * v[i][2]/vv;
            }
        	f[i][0] += xf;
			f[i][1] += yf;
			f[i][2] += zf;
        } 
      }
  }
}

/* ---------------------------------------------------------------------- */

void FixVForce::post_force_respa(int vflag, int ilevel, int iloop)
{
  if (ilevel == nlevels_respa-1) post_force(vflag);
}

/* ---------------------------------------------------------------------- */

void FixVForce::min_post_force(int vflag)
{
  post_force(vflag);
}

/* ----------------------------------------------------------------------
   potential energy of added force
------------------------------------------------------------------------- */

double FixVForce::compute_scalar()
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

double FixVForce::compute_vector(int n)
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

double FixVForce::memory_usage()
{
  double bytes = 0.0;
  if (varflag == ATOM) bytes = atom->nmax*4 * sizeof(double);
  return bytes;
}
