#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <cmath>
#include <vector>
#include <functional>
#include <cstdlib>

using namespace std;
int Nframe=2500,Nmax=Nframe-500;
int Npassive=75;
int Nactive=10;
int Ncircle=1;
double Mp=1.0,Mc=3.0,Mt=Mp*Npassive+Mc*Nactive;

main()
{
	int i,j,k,g,dt,type,t,index,index2,dtmax;
	double xu,yu,mux,muy,x,y,fx,fy,vx,vy;
	double sumx,sumy,time;
	double M=Npassive*1+Nactive*3;
	double *xc,*yc;
	double *msdxc,*msdyc;
	int *cnt;
	string temp1,temp2,id;

	xc = (double*)malloc(Nframe*Ncircle*sizeof(double));
	yc = (double*)malloc(Nframe*Ncircle*sizeof(double));
	cnt = (int*)malloc(Nmax*sizeof(int));
	msdxc =(double*)malloc(Nmax*sizeof(double));
	msdyc =(double*)malloc(Nmax*sizeof(double));
	
	for(i=0;i<Nmax;i++)
	{
		cnt[i]=0;
		msdxc[i]=0.0;
		msdyc[i]=0.0;
	}	

	//Á£×Ó×ø±ê¶ÁÈ¡
	ifstream infile1("poly.lammpstrj",ios::in);
	ifstream infile2("actv.lammpstrj",ios::in);

	for(i=0;i<Nframe;i++)
	{
		for(g=0;g<9;g++)
		{
			getline(infile1,temp1);
			getline(infile2,temp2);
		}

		
		for(j=0;j<Ncircle;j++)
		{
			sumx=0.0;
			sumy=0.0;
			for(k=0;k<Npassive;k++)
			{
				getline(infile1,temp1);
				stringstream word(temp1);
				word>>id>>type>>xu>>yu>>x>>y>>fx>>fy>>vx>>vy;
				sumx=sumx+xu*Mp;
				sumy=sumy+yu*Mp;
			}
			for(k=0;k<Nactive;k++)
			{
				getline(infile2,temp2);
				stringstream word(temp2);
				word>>id>>type>>xu>>yu>>mux>>muy>>fx>>fy>>vx>>vy;
				sumx=sumx+xu*Mc;
				sumy=sumy+yu*Mc;
			}
			index=i*Ncircle+j;
			xc[index]=sumx/Mt;
			yc[index]=sumy/Mt;
		}

	}
	infile1.close();
	infile2.close();

    for(t=0;t<Nframe-1;t++)
    {
    	dtmax=Nmax<Nframe-t-1?Nmax:Nframe-t-1;
		for(dt=1;dt<=dtmax;dt++)
    	{
	    	for(i=0;i<Ncircle;i++)
	    	{
		    	cnt[dt-1]++;
		    	index=t*Ncircle+i;
		    	index2=(t+dt)*Ncircle+i;
				msdxc[dt-1] += (xc[index2] - xc[index])*(xc[index2] - xc[index]);
             	msdyc[dt-1] += (yc[index2] - yc[index])*(yc[index2] - yc[index]);
	    	}
     	}
    }

    FILE *fp;
	fp=fopen("msd.dat","w");
    for(t=0;t<Nmax;t++)
    {
    	msdxc[t] /= cnt[t];
        msdyc[t] /= cnt[t];
		time=(t+1)*4000*0.001;
		fprintf(fp,"%.7f,%.11f,%.11f,%.11f\n",time,msdxc[t],msdyc[t],msdxc[t]+msdyc[t]);
    }
    fclose(fp);
	cout<<"Done!"<<endl;
    return 0;
}
