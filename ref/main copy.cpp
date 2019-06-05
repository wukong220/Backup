// Chain with Obstacles

#include <fstream>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "rand.h"
#include "paras.h"
#include "init.h"

#define PI 3.1415926535897;	//pi
bool check_in(double**  pp, int n, double r_min);
Ranq2 Rand((unsigned int)(time(NULL)));
using namespace std;
int main() 
{
	double dx, dy, dr = 0;
	Ranq2 Rand((unsigned int)(time(NULL)));
	// 2 dimensional array for particles
	double**  par = new double* [Num_obs];	
	for (int i = 0;i < Num_obs;i++)
		par[i] = new double[2];
	
	int t;
	// Create Num_obs random particlar position with minimal distance inter
	for (int i = 0; i < 10;)
	{
		par[i][0] = L_box * Rand.doub() - 0.5 * L_box;
		par[i][1] = L_box * Rand.doub() - 0.5 * L_box;
		t = 0;
		for(int j = 0; j < i; j++)
		{
			//period boundary
			dx = abs(par[i][0] - par[j][0]);
			dx = min(dx, L_box - dx);
			dy = abs(par[i][1] - par[j][1]);
			dy = min(dy, L_box - dy);
			dr = dx * dx + dy * dy;
			if(dr > (50 * 50)) 
				t++;
			else
				break;
		}
		if (t == i ) 
		{
			cout<< sqrt(dr) << endl;
			i++;
		}
			
	}
	
	return 0;
}