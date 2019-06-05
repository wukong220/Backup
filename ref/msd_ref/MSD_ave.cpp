//msd
//ave 
//files

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>

#define max(x, y)  ( x >= y? x : y )	//#include <cmath>
#define min(x, y)  ( x <= y? x : y )	

const int Num_info = 15;		//id type xu yu zu vx vy vz c_orient[1] c_orient[2] c_orient[3] c_orient[4] c_shape[1] c_shape[2] c_shape[3]

const int N_chain = 30;									//Polarization of single chain
const int Num_chains = 1;								//Number of the chains
const int Num_beeds = N_chain * Num_chains; 			//Number of beeds

const int dimension = 2;
const int Num_file = 8;
std::vector<int> closefiles{2,3,4,6};				//closefiles
std::string foutname = "msd_ave.txt";

const double md_dt = 0.001;
const int Num_frame = 1000;
const int Max_frame = Num_frame - 50;
const int framestep = 100000;

const int len = 2;
using namespace std;

int main() 
{

	vector<vector<vector<double> > > chain(Num_frame, vector<vector<double> >(Num_beeds, vector<double>(Num_info,0))); //iframe, id , type,xu,yu,zu...
	//vector< vector<double> > position(Num_beeds, vector<double>(3, 0));			//xu, yu, zu

	vector<vector<vector<double> > > center(Num_frame, vector<vector<double> >(Num_chains, vector<double>(dimension,0)));			//each frame with centers of chain
	
	vector<int>count(Max_frame);		//count
	vector<vector<vector<double> > > msd(Max_frame, vector<vector<double> >(Num_chains, vector<double>(dimension*Num_file+1, 0))); //mean squared displacement of each chain
	
	vector<string> filename(Num_file + 1);				//input filename
	vector<string> label(Num_file);						//label
	vector<int> files(3, Num_file);						//Num_file, max_file, files
	
	stringstream ss;
	stringstream sl;
	string str;
	string temp;
	string error = "Right";
	int Num_atoms = 1;
	int Num_obs = 1;

	int t = 0;
	int dt = 1;
	int max_dt = 0;
	int Tstart = 0;
	int Tstop = 0;
	double time;
	
	for (int i = 0; i < Num_file; i++)
	{

		if (i < 9)
		{
			ss << "00" << i + 1 << "u.lammpstrj";
			sl << "00" << i + 1;
		}
		else if (i >= 9)
		{
			ss << "0" << i + 1 << "u.lammpstrj";
			sl << "0" << i + 1;
		}
		ss >> filename[i];
		sl >> label[i];
		ss.clear();
		sl.clear();
	}
	
	ofstream fout(foutname);	
	for (int i = 0; i < foutname.size(); i++)
	{
		if (foutname[i] >= '0' && foutname[i] <= '9')
		{
			filename[Num_file] += foutname[i];
			files[1] = 1;
			files[2] = 1;
		}
	}
	
	for(int ifile = 0; ifile < files[1]; ifile++)
	{
		//input
		ifstream fin;	
		if (filename[Num_file] != "\0")
		{
			filename[Num_file] += "u.lammpstrj";
			fin.open(filename[Num_file]);
			str = filename[Num_file];
		}
		else 
		{
			fin.open(filename[ifile]);
			str = filename[ifile];
		}
		
		for (int j = 0; j < closefiles.size(); j++)
		{
			if (ifile == closefiles[j] - 1)
				error = "\"CLOSE\": ";
		}
		
		if(!fin.is_open())
			error = "\"ERROR\": Cannot open ";
		else if(error != "Right")
		{
			cout << error << str << endl;
			files[2]--;
			label[ifile] = "000";
			error = "Right";
			continue;
		}
		else
			cout << "\"Opening\"" << str << "……" << endl;
			
		int timestep = 0;
		for (int i =0; i < Num_frame; i++)
		{
			for (int clear = 0; clear < 2; clear++)			//the head
				getline(fin, temp);
			ss << temp;
			ss >> timestep;
			if (timestep != i * framestep)
			{
				error = "\"ERROR\": TIMESTEP -> ";
				//fout << error << endl;
				cout << error << timestep << " != " << i * framestep 
				<< " (" << i << " * " << framestep << ")" << endl;
				files[2]--;	
				label[ifile] = "000";
				break;
			}
			ss.clear();
			
			for (int clear = 0; clear < 2; clear++)			//the head
				getline(fin, temp);
			ss << temp;
			ss >> Num_atoms;								//Number of atoms
			Num_obs = Num_atoms - Num_beeds;		
			ss.clear();
			
			for (int clear = 0; clear < 5; clear++)			//the head
				getline(fin, temp);
			
			for (int j = 0; j < Num_beeds; j++)
				{
					getline(fin, temp);
					stringstream input(temp);
					for (int k = 0; k < Num_info; k++)
						input >> chain[i][j][k];
				}

			for (int clear = 0; clear < Num_obs; clear ++)	//the tail
				getline(fin, temp);
		}
		//temp = " ";
		fin.close();
		
		if (files[2] == 0)
			return 1;			//single file
		else if (error != "Right")
		{
			error = "Right";
			continue;
		}
		
		for (int i = 0; i < Num_frame; i++)			//position of the center of mass
		{
			for (int j = 0; j < Num_chains; j++)
			{
				for (int k = N_chain * j; k < N_chain * (j + 1); k++)
				{
					center[i][j][0] += chain[i][k][2];		//i frame j chain x or y
					center[i][j][1] += chain[i][k][3];
					//cout << chain[i][k][2] << " " << chain[i][k][3] << " " << endl;
				}
				center[i][j][0] /= N_chain;
				center[i][j][1] /= N_chain;
				//cout << center[i][j][0] << " " << center[i][j][1] << endl << endl;
			}
		}
			
		for (dt = 1; dt <= Max_frame; dt++)
		{
			for(Tstart = 0; Tstart < min(Max_frame, Num_frame - dt); Tstart++)
				{
					Tstop = Tstart + dt;
					count[dt-1]++;
					for(int i = 0; i < Num_chains; i++)
					{
						int k = dimension * ifile + 1;
						msd[dt-1][i][k] += (center[Tstop][i][0] - center[Tstart][i][0])*(center[Tstop][i][0] - center[Tstart][i][0]); 
						msd[dt-1][i][k+1] += (center[Tstop][i][1] - center[Tstart][i][1])*(center[Tstop][i][1] - center[Tstart][i][1]);
						if(Tstart == (min(Max_frame, Num_frame - dt)-1))
							msd[dt-1][i][0] += msd[dt-1][i][k] + msd[dt-1][i][k+1];
						/*cout << Tstart << " " << dt << " " << Tstop << " " 
						<< (center[Tstop][i][0] - center[Tstart][i][0])*(center[Tstop][i][0] - center[Tstart][i][0])+(center[Tstop][i][1] - center[Tstart][i][1])*(center[Tstop][i][1] - center[Tstart][i][1]) << " "
						<< msd[dt-1][i][2] << " " << msd[dt-1][i][2]/count[dt-1] << endl;*/
					}
				}
				//cout << msd[dt-1][0][2]/count[dt-1] << endl;
		}
		for (int i = 0; i < Num_frame; i++)
		{
			for (int j = 0; j < Num_chains; j++)
			{
				center[i][j][0] = 0;
				center[i][j][1] = 0;
			}
		}
	}
	
	//output 
	//cout << 1 * framestep * md_dt<< " " << msd[0][0][2]/count[0] << endl;
	fout << "time ";
	cout << "time "; 
	for (int i = 0; i < files[1]; i++)
	{
		fout << "msd[" << label[i] << "] ";
		cout << "msd[" << label[i] << "] ";
	}
	fout << "ave\n";	
	cout << "ave\n";
	
	for (int iframe = 0; iframe < Max_frame; iframe++)
	{
		time = (iframe + 1) * framestep * md_dt;
		fout << time << " ";
		cout << time << " ";
		for (int ifile = 0; ifile < files[1]; ifile++)
		{
			int j = ifile * dimension + 1; 
			for(int i = 0; i < Num_chains; i++)
			{	
				//if (label[ifile] != "000")
					//fout << files[2] * (msd[iframe][i][j] + msd[iframe][i][j+1])/(count[iframe])<< " ";
				//cout << msd[iframe][i][j] << " " << msd[iframe][i][j+1] << " " 
					cout << setw(len) << files[2] * (msd[iframe][i][j] + msd[iframe][i][j+1])/(count[iframe])<< " ";
				if (ifile == files[1] - 1)
				{
					fout << msd[iframe][i][0] / count[iframe] << " ";
					cout << setw(len) << msd[iframe][i][0] / count[iframe] << " ";
				}
			}
		}
		fout << endl;
		cout << endl;
	}
	fout.close();
}