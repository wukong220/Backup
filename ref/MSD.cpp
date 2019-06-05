#include <iostream>
#include <vector>

#include "paras.h"

using namespace std;

int main() 
{
	vector< vector<double> > chains(Num_beeds, vector<double>(4, 0));								//ach beed with position and velocity
	vector< vector<double> > centers(Num_frame, vector<double>(Num_chains * dimension, 0));			//each frame with centers of chain
	
	
}