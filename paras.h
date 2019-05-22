#ifndef PARAS_H
#define PARAS_H

#define SR2 1.4142135623731
#define max(x, y)  ( x >= y? x : y )
#define min(x, y)  ( x <= y? x : y )
int number(double r, double l, double phi);				//Number of obstacles with dencity of phi 

const double PI = 3.1415926535897;	//pi
const int dimension = 2;			//dimension
const int Type_chain = 1;			//atom type of chains
const int Type_obs = 2;				//atom type of obstacles

const int N_atomtypes = 2;		//Number of atom types
const int N_bondtypes = 1;		//Number of bond types
const int N_angltypes = 1;		//Number of angle types

const double L_box = 200.0;					//Lengh of the box

//chain setup
const double rad = 0.5;						//Radius of the chain
const double Sigma = 2 * rad;				//2 * radius of the chain * 2
const int N_chain = 30;						//Polarization of single chain
const int Num_chains = 1;					//Number of the chains

//obstacle setup
const double rad_obs = 5.0;									//Radius of the obstacle
const double R_obs = 2 * rad_obs ;							//2 * radius of the obstacle * 2
const double Phi_obs = 0.58;									//Area density of the obstacles
const double inter = Sigma+2.5 * rad_obs;					//Minimal distace between beeds and obstacles
const int Num_obs = number(rad_obs, L_box, Phi_obs);		//Number of the obstacles

const int Num_beeds = N_chain * Num_chains; 			//Number of beeds
const int Num_atoms = N_chain * Num_chains+Num_obs;		//Number of atoms
const int Num_bonds = Num_chains * (N_chain-1);			//Number of bonds
const int Num_angles = Num_chains * (N_chain - 2);		//Number of angles

const int Num_frame = 1000;

inline int number(double r, double l, double phi)
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
