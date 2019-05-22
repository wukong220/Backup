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

// fix ID group-ID pass id xcoord file filename

#include "stdlib.h"
#include "string.h"
#include "fix_pass.h"
#include "update.h"
#include "input.h"
#include "modify.h"
#include "variable.h"
#include "memory.h"
#include "error.h"
#include "force.h"
#include "atom.h" 

double const smallnum = 1.0e-20;

using namespace LAMMPS_NS;
using namespace FixConst;

/* ---------------------------------------------------------------------- */
// fix ID group-ID pass N left_id right_id check xcoord file filename
//check = 1, left;check = 2, middle; check = 3 right
FixPass::FixPass(LAMMPS *lmp, int narg, char **arg) :
  Fix(lmp, narg, arg)
{
  if (narg < 8) error->all(FLERR,"Illegal fix pass command");
  nevery = force->inumeric(FLERR,arg[3]);
  lid = force->inumeric(FLERR,arg[4]);
  rid = force->inumeric(FLERR,arg[5]);
  check = force->inumeric(FLERR,arg[6]);
  if (nevery < -smallnum || lid < -smallnum ||rid < -smallnum) error->all(FLERR,"Illegal fix print command");  
  xcoord = force->numeric(FLERR,arg[7]);  
  MPI_Comm_rank(world,&me);

  // parse optional args

  fp = NULL;
  //screenflag = 1;
  //char *title = NULL;

  int iarg = 8;
  while (iarg < narg) {
    if (strcmp(arg[iarg],"file") == 0 || strcmp(arg[iarg],"append") == 0) {
      if (iarg+2 > narg) error->all(FLERR,"Illegal fix print command");
      if (me == 0) {
        if (strcmp(arg[iarg],"file") == 0) fp = fopen(arg[iarg+1],"w");
        else fp = fopen(arg[iarg+1],"a");
        if (fp == NULL) {
          char str[128];
          sprintf(str,"Cannot open fix print file %s",arg[iarg+1]);
          error->one(FLERR,str);
        }
      }
      iarg += 2;
    } else error->all(FLERR,"Illegal fix print command");
  }

  // print file comment line

  if (fp && me == 0) {
    //if (title) fprintf(fp,"%s\n",title);
    fprintf(fp,"# Fix print output for fix %s\n",id);
  }

  //delete [] title;

  // add nfirst to all computes that store invocation times
  // since don't know a priori which are invoked via variables by this fix
  // once in end_of_step() can set timestep for ones actually invoked

  int nfirst = (update->ntimestep/nevery)*nevery + nevery;
  modify->addstep_compute_all(nfirst);
}

/* ---------------------------------------------------------------------- */

FixPass::~FixPass()
{
  //delete [] string;
  //memory->sfree(copy);
  //memory->sfree(work);

  if (fp && me == 0) fclose(fp);
}

/* ---------------------------------------------------------------------- */

int FixPass::setmask()
{
  int mask = 0;
  mask |= END_OF_STEP;
  return mask;
}

/* ---------------------------------------------------------------------- */

void FixPass::end_of_step()
{
  // make a copy of string to work on
  // substitute for $ variables (no printing)
  // append a newline and print final copy
  // variable evaluation may invoke computes so wrap with clear/add

  double **x = atom->x;
  //tagint *tag = atom->tag;  
  //int *mask = atom->mask;
  int index1 = atom->map(lid);
  int index2 = atom->map(rid);
  
// fix ID group-ID pass N left_id right_id check xcoord file filename
//check = 1, left;check = 2, middle; check = 3 right

  if (index1 >= 0 && index1 < atom->nlocal && index2 >= 0 && index2 < atom->nlocal ) {
  	  if(check == 1){
  	  	  if(x[index1][0] > xcoord+smallnum &&  x[index2][0] > xcoord+smallnum	){
  	  	  	  if (me == 0) { 
		          if (fp) {
		          fprintf(fp,"%d , %f\n",update->ntimestep,update->atime);
		          fflush(fp);
	              }
	          }
	          error->all(FLERR,"The polymer has passed through the pore");  
		  }  	
	  }
	  else if(check == 2){
	  	
  	  	  if(x[index1][0] > xcoord+smallnum ){
  	  	  	  if (me == 0) { 
		          if (fp) {
		          fprintf(fp,"+1,  %d , %f\n",update->ntimestep,update->atime);
		          fflush(fp);
	              }
	          }
	          error->all(FLERR,"The polymer has passed through the pore");  
		  }		  
  	  	  else if(  x[index2][0] < xcoord-smallnum	){
  	  	  	  if (me == 0) { 
		          if (fp) {
		          fprintf(fp,"-1,  %d , %f\n",update->ntimestep,update->atime);
		          fflush(fp);
	              }
	          }
	          error->all(FLERR,"The polymer has passed through the pore");  
		  } 
	  	
	  }
	  else if(check == 3){
	  	
  	  	  if(x[index1][0] < xcoord-smallnum &&  x[index2][0] < xcoord-smallnum	){
  	  	  	  if (me == 0) { 
		          if (fp) {
		          fprintf(fp,"%d , %f\n",update->ntimestep,update->atime);
		          fflush(fp);
	              }
	          }
	          error->all(FLERR,"The polymer has passed through the pore");  
		  }  
	  }

  }
  
}
