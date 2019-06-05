#ifndef PARAS_H
#define PARAS_H

#define PI 3.1415926535897
#define SR2 1.4142135623731
#define Type_chain 1
#define Type_obs 2

int number(double phi, double l, double r);
//Setting box
const double L_box = 200.0;				//Lengh of the box

const double rad = 0.5;				//radius of the chain
const double Sigma = 2 * rad;			//2  *  radius of the chain  *  2
const int N_chain = 50;					//polarization of single chain
const int Num_chains = 1;					//Number of the chains

const double rad_obs = 5.0;				//radius of the obstacle
const double R_obs = 2  *  rad_obs ;		//2  *  radius of the obstacle  *  2
const double Phi_obs = 0.6;				//Area density of the obstacles
const double inter = Sigma+2.5 * rad_obs;	//Minimal distace between beeds and obstacles
const int Num_obs = number(Phi_obs, L_box, rad_obs);//Number of the obstacles





#define max(x, y)  ( x >= y? x : y )
#define min(x, y)  ( x <= y? x : y )

inline int number(double phi, double l, double r)
{
	double s = PI * r * r;
	double S = l * l;
	double num = phi * S/s;
	return int(num);
}
//#define Type_active 1
//#define Type_passive 2
//#define Type_wall 3
//#define Type_chain 4
//#define N_atomtype	4	//atome types
//#define N_bondtype	1	//bond types
//#define N_angletype	1	//angle types

//const double Den_active = 0.1;	// Dencity of active paticles ,  half of actually
//const double Den_passive = 0.1;		//Dencity of passive particles
//const int Num_passive = 300;
//const int Num_act = 2000;
//const int Num_passive_x = 12;
//const int Num_passive_y = 60;
//const int Num_wall_left = 200;
//const int Num_wall_right = 200;
//const double Boundary_x = 100.0;		//Length of the box
//const double Boundary_y = 50.0;
//const double boundary_ll = -61;
//const double boundary_rr = 61;
//const int Num_act = 30;
//const int Num_chain = 21;
//const int Num_wall_circle = 360;
//const double rou = 20;
//const int Num_wall_bid_up = 2 * rou-2;
//const int Num_wall_bid_down = 2 * rou-2;
//const double MASS = 1.0;
//const double Soft = 1.0;



#endif
