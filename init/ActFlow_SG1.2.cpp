#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <functional>
#include <sstream>
#define PI 3.1415926535897

const int Num_act=4231;
//const double R_ratchet=14.0;
//const double Den_active=0.01;	//活性粒子面密度
const double ll=1.5;	//格子大小
const double Extend=30.0;		//流边界 
const int Lx=(int)ceil(Extend/ll)*2.0;
const int Ly=Lx;
const int Num_ltc=Lx*Ly;
const int Frame_trj=5000;
const int Frame_start_trj=300;
const int Frame_end_trj=5000;
double *rx,*ry,**rvx,**rvy,*RVx,*RVy;
using namespace std;
FILE *fp;

int main()
{
	
	printf("Start at trj: %d\n",Frame_start_trj);
	printf("End at trj: %d\n",Frame_end_trj);
	const int Frame=Frame_trj;
	const int Frame_start=Frame_start_trj;
	const int Frame_end=Frame_end_trj;
	const int Frame_cal=Frame_end-Frame_start+1;
	//const double Boundary=20.0;		//棘轮外径到盒子边界的距离
	static double id,x,y,vx,vy;	
	const double dx=Extend;					//原点坐标移动距离 
	const double dy=Extend;
	const double tx=-Extend;				//坐标系移动 
	const double ty=-Extend;
	int f,g,na,nl;
	int nx,ny;
	double cx,cy;
	double lx=ll;
	double ly=ll;
	double square=lx*ly;
	int Nx=int(Lx/lx);
	int Ny=int(Ly/ly);
	rx=(double*)calloc(Lx*Ly,sizeof(double));
	ry=(double*)calloc(Lx*Ly,sizeof(double));
	rvx=(double**)calloc(Lx*Ly*Frame,sizeof(double));
	rvy=(double**)calloc(Lx*Ly*Frame,sizeof(double));
	RVx=(double*)calloc(Lx*Ly,sizeof(double));
	RVy=(double*)calloc(Lx*Ly,sizeof(double));
	//printf("Num active particle: %d\n",Num_act);
	//printf("Num_ltc: %d\n",Num_ltc);
	ifstream file;
	file.open("Act.lammpstrj",ios::in);
	for(f=0;f<Frame;f++)
	{
		string temp;
		for(g=0;g<9;g++)
		{
			getline(file,temp);
		}
		for(na=0;na<Num_act;na++)
		{
			getline(file,temp);
			stringstream word(temp);
			word>>id>>x>>y>>vx>>vy;	
			cx=x+dx;
			cy=y+dy;
			if(cx>=0&&cx<=Lx&&cy>=0&&cy<=Ly)
			{
				nx=floor(cx/lx);
				ny=floor(cy/ly);
				nl=int(ny*Nx+nx);
				rvx[f][nl]=rvx[f][nl]+vx;
				rvy[f][nl]=rvy[f][nl]+vy;
			}					
		}
		for(nl=0;nl<Num_ltc;nl++)
		{
			rvx[f][nl]=rvx[f][nl]/square;
			rvy[f][nl]=rvy[f][nl]/square; 
		}
		//printf("%d %f\n",f,rvx[f][0]);
	}
	file.close();
	
	fp=fopen("Velocity_g.out","w");
	fprintf(fp,"i x y vx vy\n");	
	for(nl=0;nl<Num_ltc;nl++)
	{
		ny=floor(nl/Nx);
		nx=nl-ny*Nx;
		rx[nl]=nx*lx+lx/2+tx;
		ry[nl]=ny*ly+ly/2+ty;
		for(f=Frame_start;f<Frame_end;f++)
		{
			RVx[nl]=RVx[nl]+rvx[f][nl];
			RVy[nl]=RVy[nl]+rvy[f][nl];
		}
		RVx[nl]=RVx[nl]/Frame_cal;
		RVy[nl]=RVy[nl]/Frame_cal;
		fprintf(fp,"%d %f %f %lf %lf\n",nl,rx[nl],ry[nl],RVx[nl],RVy[nl]);
	}
	fclose(fp);
	return 0;
}
