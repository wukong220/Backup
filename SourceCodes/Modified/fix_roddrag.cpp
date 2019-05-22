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

#include "math.h"
#include "stdlib.h"
#include "string.h"
#include "fix_roddrag.h"
#include "atom.h"
#include "update.h"
#include "respa.h"
#include "error.h"
#include "force.h"
#include "stdio.h"
#include "math_extra.h"
#include "comm.h"
#include "domain.h"
#include "atom_vec.h"

using namespace LAMMPS_NS;
using namespace FixConst;

/* ---------------------------------------------------------------------- 
 F = - gamma * velocity. 
 �������ڸ������ĵ��Ǹ�������
 Ҳ����˵group��ֻ�и������� 
*/

FixRoddrag::FixRoddrag(LAMMPS *lmp, int narg, char **arg) :
  Fix(lmp, narg, arg)
{
  if (narg < 5) error->all(FLERR,"Illegal fix cmdrag command");

  double gamma_one = force->numeric(FLERR,arg[3]); //�� 
  double cw = force->numeric(FLERR,arg[4]); //����� 
  gamma = new double[atom->ntypes+1];
  uuxy  = new double[atom->ntypes+1];
  for (int i = 1; i <= atom->ntypes; i++){
     gamma[i] = gamma_one;
     uuxy[i] = cw*gamma_one;
  } 

  // optional args
  /*
  int iarg = 4;
  while (iarg < narg) {
    if (strcmp(arg[iarg],"n_d") == 0) { //drage of normal direction was increased by scale(long/wide)  
      if (iarg+3 > narg) error->all(FLERR,"Illegal fix cmdrag command");
      int itype = force->inumeric(FLERR,arg[iarg+1]);
      double scale = force->numeric(FLERR,arg[iarg+2]);
      if (itype <= 0 || itype > atom->ntypes)
        error->all(FLERR,"Illegal fix cmdrag command");
      uuxy[itype] = uuxy[itype] * scale;
      iarg += 3;
    } else error->all(FLERR,"Illegal fix cmdrag command");
  }
  */
}

/* ---------------------------------------------------------------------- */

FixRoddrag::~FixRoddrag()
{
  delete [] gamma;
  delete [] uuxy; 
}

/* ---------------------------------------------------------------------- */

int FixRoddrag::setmask()
{
  int mask = 0;
  mask |= POST_FORCE;
  mask |= POST_FORCE_RESPA;
  mask |= MIN_POST_FORCE;
  return mask;
}

/* ---------------------------------------------------------------------- */

void FixRoddrag::init()
{
  if (strstr(update->integrate_style,"respa"))
    nlevels_respa = ((Respa *) update->integrate)->nlevels;
}

/* ---------------------------------------------------------------------- */

void FixRoddrag::setup(int vflag)
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

void FixRoddrag::min_setup(int vflag)
{
  post_force(vflag);
}

/* ---------------------------------------------------------------------- */

void FixRoddrag::post_force(int vflag)
{
  // apply drag force to atoms in group
  // direction is opposed to velocity vector
  // magnitude depends on atom type

  double **v = atom->v;
  double **f = atom->f;
  int *mask = atom->mask;
  int *type = atom->type;
  int nlocal = atom->nlocal;

  double dragp,drage;
  double **mu = atom->mu;
  
  for (int i = 0; i < nlocal; i++){
	if (mask[i] & groupbit) {
  	   if (mu[i][3] > 0.0) { //mu�ǵ�λʸ�� 
  	   	   double uvp[3],uve[3];  // uvp ƽ�з�����uve ��ֱ���� 
  	   	   double vp = v[i][0] * mu[i][0] + v[i][1] * mu[i][1] + v[i][2] * mu[i][2]; //ƽ��ͶӰ��С 
  	   	   uvp[0] = mu[i][0] * vp; 
		   uvp[1] = mu[i][1] * vp; 
		   uvp[2] = mu[i][2] * vp; //ƽ�з��� 
  	   	   uve[0] = v[i][0] - uvp[0]; 
  	   	   uve[1] = v[i][1] - uvp[1];
  	   	   uve[2] = v[i][2] - uvp[2];  //��ֱ����
  	   	   
           dragp = gamma[type[i]]; //ƽ�з���Ħ��ϵ�� 
		   drage = uuxy[type[i]];  //��ֱ����Ħ��ϵ�� 
		   
           f[i][0] -= dragp*uvp[0]+ drage*uve[0];  //���������xyz������� 
           f[i][1] -= dragp*uvp[1]+ drage*uve[1];
           f[i][2] -= dragp*uvp[2]+ drage*uve[2];
       }
  	}
  }
    
}

/* ---------------------------------------------------------------------- */

void FixRoddrag::post_force_respa(int vflag, int ilevel, int iloop)
{
  if (ilevel == nlevels_respa-1) post_force(vflag);
}

/* ---------------------------------------------------------------------- */

void FixRoddrag::min_post_force(int vflag)
{
  post_force(vflag);
}
