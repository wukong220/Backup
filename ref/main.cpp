#include <ctime>
#include "paras.h"
#include "files.h"

const int Num_info = 15;		//id type xu yu zu vx vy vz c_orient[1] c_orient[2] c_orient[3] c_orient[4] c_shape[1] c_shape[2] c_shape[3]
const int dimension = 2;
const int Num_file = 20;

const double mass = 1.0;
const int N_chain = 30;									//Polarization of single chain
const int Num_chains = 1;								//Number of the chains
const int Num_beeds = N_chain * Num_chains; 			//Number of beeds
//vector<string> type{"1", "2"};								//atom types to read

vector<int> closefiles{};				//closefiles
string finname;// = "003";				//empty or single input file
string foutname = "MSD.000_0.4_2.5_1.0_3.0.txt";		
string logname = "MSD.000.log";

const double md_dt = 0.001;
const int Num_frame = 10000;
const int dNM = 3000;
const int Max_frame = Num_frame - dNM;
const int framestep = 5000;		

int main() 
{
	//atom[iframe] [id] [id,type,xu,yu,zu...]
	vec_doub3 vecAtom(Num_frame, vector<vector<double> >(Num_beeds, vector<double>(Num_info,0))); 
	//rCM[iframe] [jchain] [x,y,z]
	vec_doub3 rCM(Num_frame, vector<vector<double> >(Num_chains, vector<double>(dimension + 1,0)));	//each frame with centers of chain	
	//msd[iframe] [jchain] [0,x,y,z]: 0 for sum of average, (dimension+1)for sum
	vec_doub3 msdCM(Max_frame, vector<vector<double> >(Num_chains, vector<double>((dimension + 1) * Num_file + 1, 0))); 	//msd of CM for each chain
	//cnt[0,ifile] [msd_frame]: 0 for sum of average; (Max_frame+1) for compare average 
	
	string str;
	stringstream ss;
	clock_t start = clock();		//start time
	
	//LmpFile infiles;
	LmpFile inFiles(finname);
	ofstream output(logname);
	int f = inFiles.files();
	//atom[iframe] [id] [id,type,xu,yu,zu...]
	for(int ifile = 0; ifile < f; ifile++)
	{
		vecAtom = inFiles.read_data(ifile, closefiles, output, Num_beeds);		//read atom data from files, exluding closefiles
		//cout << atom;
		rCM = inFiles.center(ifile, N_chain, vecAtom);			//positiion of CM from atom data of files
		//cout << "rCM: \n" << rCM;
		inFiles.msd_ave(ifile, rCM, msdCM);
		//cout << "msd: \n" << msd;
		
	}
	inFiles.out_msd(foutname, msdCM);
	cout << endl << inFiles;
	cout << "\"Writing\": " << foutname << endl << "\"Outputing\": " << logname << endl;
	output << "\"Writing\": " << foutname << endl << "\"Outputing\": " << logname << endl;	
	//time

	clock_t stop = clock();		//#include <ctime>
	double Time = (double)(stop - start)/CLOCKS_PER_SEC;
	vector<int> inter(4,Time);
	inter[1] = inter[0]/3600;
	inter[2] = inter[0]/60%60;
	inter[3] = inter[0]%60;
	string st = "\"Time\": ";    //#include <string>
	for (int i = 0; i < 3; i++)
	{
		ss << inter[i+1];
		ss >> str;  
		if (inter[i+1] < 9)
			st += "0" + str;    //using std::to_string;
		else
			st += str;
		if (i < 2)
			st += ":";
		ss.clear();
	}
	cout << st << endl;
	output << st << endl;
	output.close();
	return 0;
}