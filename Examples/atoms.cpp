#include <math.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define PI 3.1415926535897
#define SR2 1.4142135623731
#define aiInputNumber 23451
#define max(x,y)  ( x>=y?x:y )
#define min(x,y)  ( x<=y?x:y )

//#define DX 1.9537635124064

#define Type_active 1
#define Type_passive 2
#define Type_wall 3
#define Type_chain 4
#define N_atomtype	4	//atom types
#define N_bondtype	1	//bond types
#define N_angletype	1	//angle types

//const double Den_active=0.1;	// Dencity of active paticles , half of actually
//const double Den_passive=0.1;		//Dencity of passive particles
//const int Num_passive=300;
//const int Num_act=2000;
//const int Num_passive_x=12;
//const int Num_passive_y=60;
//const int Num_wall_left=200;
//const int Num_wall_right=200;
//const double Boundary_x=100.0;		//Length of the box
//const double Boundary_y=50.0;
//const double boundary_ll=-61;
//const double boundary_rr=61;
const int Num_act=30;
const int Num_chain=21;
const int Num_wall_circle=360;
const double rou=20;
const int Num_wall_bid_up=2*rou-2;
const int Num_wall_bid_down=2*rou-2;
const double MASS=1.0;
const double Soft=1.0;
double ran2(int *idum);
FILE *fp;

int main()
{
	int ir,is,il,ii,ie,i;
	
	/**************************************size of the box****************************************/
	double boundary_xl,boundary_xr,boundary_yl,boundary_yr;
	const double boundary_zl=-0.500; 
	const double boundary_zr=0.500;
	double sa,sr,se;
	
/*	boundary_xl=0.0-Boundary_x;
	boundary_xr=0.0+Boundary_x;
	boundary_yl=0.0-Boundary_y;
	boundary_yr=0.0+Boundary_y;
	se=(boundary_ll-boundary_xl)*(boundary_yr-boundary_yl)+(boundary_xr-boundary_rr)*(boundary_yr-boundary_yl);*/
	boundary_xl=0.0-rou*2;
	boundary_xr=0.0+rou*2;
	boundary_yl=0.0-rou*2;
	boundary_yr=0.0+rou*2;
		
	/**********************************DPD position of particles*************************************/	
	int ia,iw,ib,runt,t0,ja,jb;
	int tem1,aiNum_ini,aiNum_ini1,aiNum_ini2;
	double s0,s1,dx0,dy0,dx1,dy1,dx,dy;
	srand(time(0)+aiInputNumber);
	tem1=rand();
	aiNum_ini=(int)(0.2*rand());
	aiNum_ini1=(int)(0.5*rand());
	aiNum_ini2=(int)(0.6*rand());
	ran2(&tem1);
	ran2(&aiNum_ini);
	ran2(&aiNum_ini1);
	ran2(&aiNum_ini2);
	
//	const int Num_act=(int)(se*Den_active*4/PI);
//	const int Num_passive=(int)(se*Den_passive);
//    const int Num_passive=Num_passive_x*Num_passive_y;
//	const int Num_wall_left=(int)(se*Den_wall_left);
//	const int Num_wall_right=(int)(se*Den_wall_right);
//	const int Num_dpd=Num_act+Num_passive;  //+Num_wall_left+Num_wall_right;
//	double coor_dpd_x[Num_dpd],coor_dpd_y[Num_dpd];
//	double coor_act_x1[Num_act],coor_act_y1[Num_act]; 
//	double coor_act_x2[Num_act],coor_act_y2[Num_act]; 
    double coor_act_x[Num_act],coor_act_y[Num_act]; 
	double coor_passive_x1[Num_act],coor_passive_y1[Num_act];
	double coor_passive_x2[Num_act],coor_passive_y2[Num_act];
//	double coor_wall_left_x[Num_wall_left],coor_wall_left_y[Num_wall_left];
//	double coor_wall_right_x[Num_wall_right],coor_wall_right_y[Num_wall_right];
/*	for (ia=0;ia<Num_act;ia++)
	{		
		for(runt=0;;runt++)
		{
			t0=0;
			coor_act_x1[ia]=(-99.5)+ran2(&aiNum_ini)*((boundary_ll)-(-99.5));
			coor_act_y1[ia]=boundary_yl+ran2(&aiNum_ini1)*((boundary_yr)-(boundary_yl));
			/******************active to boundary condition*******************/
/*			for(ib=0;ib<ia;ib++)
			{
				dx0=fabs(coor_act_x1[ia]-coor_act_x1[ib]);
				dy0=fabs(coor_act_y1[ia]-coor_act_y1[ib]);
			if (dx0>((boundary_xr)-(boundary_xl)/2))
				{
					dx0=fabs(min(coor_act_x1[ia],coor_act_x1[ib])+((boundary_xr)-(boundary_xl))-max(coor_act_x1[ia],coor_act_x1[ib]));
				}
				if (dy0>((boundary_yr)-(boundary_yl)/2))
				{
					dy0=fabs(min(coor_act_y1[ia],coor_act_y1[ib])+((boundary_yr)-(boundary_yl))-max(coor_act_y1[ia],coor_act_y1[ib]));
				}				s0=sqrt(dx0*dx0+dy0*dy0);
				if(s0<=Soft)    t0=t0+1;
			}
		
			if(t0==0)  break;		//if t0=0,the particle is needed
		}
	}*/


    for(ia=0;ia<Num_act;ia++)
    {
    	coor_act_x[ia]=1;
    	coor_act_y[ia]=Num_act/2-2*ia;
    	if(coor_act_y[ia]<-15)
    	{
    		coor_act_x[ia]=5;
    		coor_act_y[ia]=Num_act/2-2*(ia-Num_act/2);
		}
	}	
    
	
	
	/*****************************init of dipolar*****************************/
	int ui,aiNum_ini3,aiNum_ini4,aiNum_ini5;
	double uz=0.0; 
	double uux,uuy,ua,ub;
	double ux[Num_act],uy[Num_act];
	srand(time(0)+aiInputNumber);
	tem1=-rand();
	aiNum_ini3=(int)(0.3*rand());
	aiNum_ini4=(int)(0.7*rand());
	aiNum_ini5=(int)(0.9*rand());
	ran2(&tem1);
	ran2(&aiNum_ini3);
	ran2(&aiNum_ini4);
	ran2(&aiNum_ini5);
	
	for(ui=0;ui<Num_act;ui++)
	{
		uux=ran2(&aiNum_ini3);
		uuy=sqrt(1.0-uux*uux);
		ua=ran2(&aiNum_ini4);
		ub=ran2(&aiNum_ini5);	
		ux[ui]=(ua>0.5?uux:(-1*uux));
		uy[ui]=(ub>0.5?uuy:(-1*uuy));
	}
	
	
	/*****************************data file  output******************************/ 
	const int sum_active=Num_act*3;
//	const int sum_passive=Num_passive*2;
    const int sum_chain=Num_chain;
    const int sum_wall=Num_wall_circle+Num_wall_bid_up+Num_wall_bid_down;
	const int sum_all=sum_active+sum_wall+sum_chain;
	const int sum_bond=Num_chain-1;
	const int sum_angle=Num_chain-2;
	const double coor_z=0.0;
	static int id=0;
	static int q=0;
	static double diameter=2,density=MASS/(diameter*diameter);
	static int type;
	char Name[512];

	fp=fopen("atoms.data","a");		// filename
	
	fprintf(fp,"LAMMPS FENE chain data file\n");
	fprintf(fp,"\n");
	fprintf(fp,"      %6d  atoms\n",sum_all);
	fprintf(fp,"      %6d  bonds\n",sum_bond);
	fprintf(fp,"      %6d  angles\n",sum_angle);		
	fprintf(fp,"\n");
	fprintf(fp,"      %6d  atom types\n",N_atomtype);
	fprintf(fp,"      %6d  bond types\n",N_bondtype);
	fprintf(fp,"      %6d  angle types\n",N_angletype);
	fprintf(fp,"\n");
	fprintf(fp,"%f %f xlo xhi\n",boundary_xl,boundary_xr);
	fprintf(fp,"%f %f ylo yhi\n",boundary_yl,boundary_yr);
	fprintf(fp,"%.1f %.1f zlo zhi\n",boundary_zl,boundary_zr);
	fprintf(fp,"\n");
	fprintf(fp,"Masses\n");
	fprintf(fp,"\n");
	fprintf(fp,"	1 %4.6f\n",MASS);
	fprintf(fp,"	2 %4.6f\n",MASS);
	fprintf(fp,"	3 %4.6f\n",MASS);
	fprintf(fp,"	4 %4.6f\n",MASS);
	fprintf(fp,"\n");
	fprintf(fp,"Atoms\n");
	fprintf(fp,"\n");			
	/***************************** active particle output******************************/
	for(ia=0;ia<Num_act;ia++)
	{
		id+=1;
		type=Type_active;
		fprintf(fp,"%d	%d	%f	%f	%f	%f	%f  %d  %d	%f	%f	%f\n",id,type,coor_act_x[ia],coor_act_y[ia],coor_z,diameter,density,ia+1,q,ux[ia],uy[ia],uz);
//      fprintf(fp,"%d	%d	%f	%f	%f	%d	%d  %d  %d    -1    	0    	0\n",id,type,coor_act_x[ia],coor_act_y[ia],coor_z,diameter,density,ia+1,q);
	}
		
	/***************************** passive particle output******************************/
/*	iw=0;
    for(ja=0;ja<Num_passive_y;ja++)
	{
		for(ia=0;ia<Num_passive_x;ia++)
		{
		id+=1;
		iw+=1; 
		type=Type_passive;
		fprintf(fp,"%d	%d	%f	%f	%f	%d	%d  %d  0  0\n",id,type,coor_passive_x[ia],coor_passive_y[ja],coor_z,ia+1,ja+1,q);
        }
	}*/


	return 0;
}

double ran2(int *idum)
{
	const int IM1=2147483563,IM2=2147483399;
	const int IA1=40014,IA2=40692,IQ1=53668,IQ2=52774;
	const int IR1=12211,IR2=3791,NTAB=32,IMM1=IM1-1;
	const int NDIV=1+IMM1/NTAB;
	const double EPS=3.0e-16,RNMX=1.0-EPS,AM=1.0/(double)(IM1);
	static int idum2=123456789,iy=0;
	static int iv[32];
	int j,k;
	double temp;
	
	if(*idum<=0)
	{
		*idum=(*idum==0?1:-*idum);
		idum2=*idum;
		for (j=NTAB+7;j>=0;j--)
		{
			k=*idum/IQ1;
			*idum=IA1*(*idum-k*IQ1)-k*IR1;
			if(*idum<0) *idum+=IM1;
			if(j<NTAB) iv[j]=*idum;
		}
		iy=iv[0];
	}
	
	k=*idum/IQ1;
	*idum=IA1*(*idum-k*IQ1)-k*IR1;
	if(*idum<0) *idum+=IM1;
	k=idum2/IQ2;
	idum2=IA2*(idum2-k*IQ2)-k*IR2;
	if(idum2<0) idum2+=IM2;
	j=iy/NDIV;
	iy=iv[j]-idum2;
	iv[j]=*idum;
	if(iy<1) iy+=IMM1;
	if((temp=AM*iy)>RNMX) return RNMX;
	else return temp;
} 


