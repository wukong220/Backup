#include "files.h"
#include "paras.h"


//extern const int Num_file, Num_frame
LmpFile::LmpFile()
{
	stringstream ss, sl;
	m_files = vector<int> {Num_file, Num_file, 0};		//cnt[Num_file, files, count]
	m_head = "none";
	m_fnamebel = vector<vector<string> > (m_files[0], vector<string>(2));
	m_frames = vector<vector<int> >(m_files[0] + 1, vector<int>{Num_frame, Num_frame - dNM});
	for (int i = 0; i < m_files[0]; i++)
	{
		if (i < 9)
		{
			ss << "00" << i + 1 << "u.lammpstrj";
			sl << "00" << i + 1;
		}
		else if (i >= 9 && i < 100)
		{
			ss << "0" << i + 1 << "u.lammpstrj";
			sl << "0" << i + 1;
		}
		ss >> m_fnamebel[i][0];
		sl >> m_fnamebel[i][1];
		ss.clear();
		sl.clear();
	}
}

LmpFile::LmpFile(const string & finname)
{
	m_files = vector<int> {Num_file, Num_file, 0};
	m_head = "none";
	stringstream ss, sl;
	if (finname != "\0")
	{
		m_files[0] = 1;
		m_files[1] = 1;
		m_fnamebel = vector<vector<string> > (m_files[0], vector<string>(2));
		m_frames = vector<vector<int> >(m_files[0] + 1, vector<int>{Num_frame, Num_frame - dNM});
		m_fnamebel[0][0] = finname + "u.lammpstrj";
		m_fnamebel[0][1] = "000";
		
	}
	else
	{
		m_fnamebel = vector<vector<string> > (m_files[0], vector<string>(2));
		m_frames = vector<vector<int> >(m_files[0] + 1, vector<int>{Num_frame, Num_frame - dNM});
		
		for (int i = 0; i < m_files[0]; i++)
		{
			if (i < 9)
			{
				ss << "00" << i + 1 << "u.lammpstrj";
				sl << "00" << i + 1;
			}
			else if (i >= 9 && i < 100)
			{
				ss << "0" << i + 1 << "u.lammpstrj";
				sl << "0" << i + 1;
			}
			ss >> m_fnamebel[i][0];
			sl >> m_fnamebel[i][1];
			ss.clear();
			sl.clear();
		}
	}
}

LmpFile::LmpFile(const vector<vector<string> > fnamebel)
{
	int num = fnamebel.size();
	m_files = vector<int> {num, num, 0};
	m_head = "none";
	m_fnamebel = fnamebel;
	m_frames = vector<vector<int> >(m_files[0] + 1, vector<int>{Num_frame, Num_frame - dNM});
}

LmpFile::LmpFile(const vector<vector<string> > fnamebel, const int a)
{
	int num = fnamebel.size();
	m_files = vector<int> {num, num, 0};
	m_head = "none";
	m_fnamebel = fnamebel;
	m_frames = vector<vector<int> >(m_files[0] + 1, vector<int>{a, a - dNM});
}

LmpFile::LmpFile(const vector<vector<string> > fnamebel, const vector<vector<int> > frames)
{
	int num = fnamebel.size();
	m_files = vector<int> {num, num, 0};
	m_head = "none";
	m_fnamebel = fnamebel;
	m_fnamebel.insert(m_fnamebel.begin(), m_fnamebel[0]);
	m_frames = frames;
	m_frames.insert(m_frames.begin(), m_frames[0]);
}

LmpFile::~LmpFile()
{
	
}

ostream & operator<<(ostream & os, const LmpFile & file)
{
	//os.setf(ios::adjustfield);
	cout << "Information: \n";
	for(int i = 0; i < file.m_head.size(); i++)
	{
		if ((file.m_head[i] >= 'A' && file.m_head[i] <= 'Z')|| file.m_head[i] == ':')
			continue;
		else
			cout << file.m_head[i];
	}
	cout << "\n\n";
	
	cout << "Effective Files: " << file.m_files[1] << endl;
	os << setw(25) << left << "Name ";
	os << setw(5) << "Label";
	os << " ";
	os << setw(7) << left << "Num";
	os << " ";
	os << setw(7) << left << "Max" << endl;
	for (int i = 0; i < file.m_files[0]; i++)
	{
		os << setw(25) << left << file.m_fnamebel[i][0] << " ";
		os << setw(5) << file.m_fnamebel[i][1];
		for (int k = 0; k < file.m_frames[i + 1].size(); k++)
			os << setw(7) << left << file.m_frames[i + 1][k] << " ";
		os << endl;
	}
	os << endl;
	
	return os;
}

vec_doub3 LmpFile::read_data(const int ifile, const vector<int> closefiles, ofstream &output, const int nAtoms)
{
	//input	
	//extern const int Num_info = 15;
	vec_doub3 atom(m_frames[0][0], vector<vector<double> >(nAtoms, vector<double>(Num_info,0)));
	string error = "Right";
	int timestep = 0;
	string temp;
	stringstream ss;
	int Num_atoms = 1;
	
	ifstream fin(m_fnamebel[ifile][0]);
	if (m_fnamebel[0][1] == "000")
		m_files[1] = 1;
	
	for (int j = 0; j < closefiles.size(); j++)
	{
		if (ifile == closefiles[j] - 1)
			error = "\"CLOSE\": ";
	}
	
	if(!fin.is_open())
		error = "\"ERROR\": Cannot open ";
	
	//error information
	if(error != "Right")
	{
		cout << error << m_fnamebel[ifile][0] << endl;
		output << error << m_fnamebel[ifile][0] << endl;		//for output
		
		m_files[1]--;
		m_fnamebel[ifile][1] = "  ";
		
		m_frames[ifile + 1][0] = 0;
		m_frames[ifile + 1][1] = 0;
		
		error = "Right";
		return atom;
	}
	else
	{
		cout << "\"Opening\": " << m_fnamebel[ifile][0] << "……" << endl;
		output << "\"Opening\": " << m_fnamebel[ifile][0] << "……" << endl;
	}
	
	
	for (int i = 0; i < m_frames[0][0]; i++)
	{
		for (int clear = 0; clear < 2; clear++)			//the head
			getline(fin, temp);
		ss << temp;
		ss >> timestep;
		if (timestep != i * framestep)		//extern const int framestep = 5000;
		{
			error = "\"ERROR\": TIMESTEP/FRAME(files.cpp:198) -> ";
			cout << error << timestep << " != " << i * framestep << " (" << i << " * " << framestep << ")" << endl;
			output << error << timestep << " != " << i * framestep << " (" << i << " * " << framestep << ")" << endl;
			m_files[1]--;	
			m_fnamebel[ifile][1]= "000";
			m_frames[ifile + 1][0] = i - 1 ;					//frames[Num_frames, max_frames]
			m_frames[ifile + 1][1] = m_frames[ifile + 1][0] - dNM;
			break;
		}
		ss.clear();
		m_head = read_atoms(fin, i, nAtoms, atom);
		//read_type(fin, i, type, atom);
	}
	fin.close();
	//extern const int dNM = 3000;
	//m_frames[ifile + 1][1] = m_frames[ifile + 1][0] - dNM;	//frames[Num_frames, max_frames]
	if (m_files[1] == 0 && error == "Right")
		m_files[1] = 1;								//single file	
	return atom;
}

//calcucate position of the center of mass 
vec_doub3 LmpFile::center(const int ifile, const int nChain, const vec_doub3 &vec)
{
	//extern int dimension = 2
	//extern double mass = 1.0
	int NumChains = vec[0].size() / nChain;
	vec_doub3 rCM(m_frames[0][0], vector<vector<double> >(NumChains, vector<double>(dimension + 1,0)));
	for (int i = 0; i < m_frames[ifile + 1][0]; i++)			
	{
		for (int j = 0; j < NumChains; j++)
		{
			for (int k = nChain * j; k < nChain * (j + 1); k++)
			{
				rCM[i][j][0] += vec[i][k][2] * mass;		//rCm[iframe][jchain][x, y, z]
				rCM[i][j][1] += vec[i][k][3] * mass;
				rCM[i][j][2] += vec[i][k][4] * mass;	
				//cout << vec[i][k][2] << " " << vec[i][k][3] << " " << endl;
			}
			double TotMass = nChain * mass;
			rCM[i][j][0] /= TotMass;
			rCM[i][j][1] /= TotMass;
			rCM[i][j][2] /= TotMass;
			//cout << rCM[i][j][0] << " " << rCM[i][j][1] << " " << rCM[i][j][2] << endl << endl;
		}
	}
	return rCM;
}

vec_doub3 LmpFile::msd_ave(const int ifile, const vec_doub3 vec, vec_doub3 &msd)
{
	//extern int Max_frame; 
	//Num_chains
	int num = vec[0].size();	//Num_chains
	vector<int> cnt(m_frames[0][1], 0);
	for (int dt = 1; dt <= m_frames[ifile + 1][1]; dt++)
	{
		for(int Tstart = 0; Tstart < min(m_frames[ifile + 1][1], m_frames[ifile + 1][0] - dt); Tstart++)
			{
				int Tstop = Tstart + dt;
				for(int i = 0; i < num; i++)
				{
					//extern int dimension = 2
					int j = (dimension + 1) * ifile + 1;
					//count[ifile+1][dt-1]++;
					cnt[dt-1]++;
					msd[dt-1][i][j] += (vec[Tstop][i][0] - vec[Tstart][i][0])*(vec[Tstop][i][0] - vec[Tstart][i][0]); 
					msd[dt-1][i][j+1] += (vec[Tstop][i][1] - vec[Tstart][i][1])*(vec[Tstop][i][1] - vec[Tstart][i][1]);
					msd[dt-1][i][j+2] = msd[dt-1][i][j] + msd[dt-1][i][j+1];
					if (m_fnamebel[ifile][1] != "000")
					{
						//if (m_fnamebel[ifile][1] != "  ")
						//	count[0][dt-1]++;
						if (Tstart == (min(m_frames[ifile + 1][1], m_frames[ifile + 1][0] - dt)-1))
						{
							if (m_frames[ifile + 1][1] == (m_frames[0][1]) && m_frames[ifile + 1][0] == m_frames[0][0])
							{	
								msd[dt-1][i][j] /= cnt[dt-1];
								msd[dt-1][i][j+1] /= cnt[dt-1];
								msd[dt-1][i][j+2] = msd[dt-1][i][j] + msd[dt-1][i][j+1];
								//cout << "files[2]" << m_files[2] << endl;
								msd[dt-1][i][0] = (msd[dt-1][i][0] * (m_files[2]) + msd[dt-1][i][j+2]) / (m_files[2] + 1);
							}
							if (dt == m_frames[0][1])
								m_files[2]++;
						}
					}
					else if (m_fnamebel[ifile][1] != "  " && Tstart == (min(m_frames[ifile + 1][1], m_frames[ifile + 1][0] - dt)-1))
					{
						msd[dt-1][i][j] /= cnt[dt-1];
						msd[dt-1][i][j+1] /= cnt[dt-1];
						msd[dt-1][i][j+2] = msd[dt-1][i][j] + msd[dt-1][i][j+1];
					}
				}
			}
	}
	return msd;
}

//output
void LmpFile::out_msd(const string foutname, const vec_doub3 vec)
{
	double time;
	//num_chain
	int num = vec[0].size();
	ofstream fout(foutname);
	fout << "time ";
	cout << "time "; 
	//output << "time ";
	for (int ifile = 0; ifile < m_files[0]; ifile++)
	{
		for (int j = 0; j < num; j++)
		{
			cout << "msd[" << j + 1 << "][" << m_fnamebel[ifile][1] << "] ";
			//output << "msd[" << j + 1 << "][" << m_fnamebel[ifile][1] << "] ";
			fout << "msd[" << j + 1 << "][" << m_fnamebel[ifile][1] << "] ";
			if (ifile == m_files[0] - 1)
			{
				cout << "ave[" << j + 1 << "] ";
				//output << "ave[" << j + 1 << "] ";
				fout << "ave[" << j + 1 << "] ";
			}
		}
	}
	cout << endl;
	fout << endl;
	
	//extern int Max_frame = 20000;int framestep = 5000; double md_dt = 0.001;
	for (int dt = 1; dt <= m_frames[0][1]; dt++)
	{
		time = dt * framestep * md_dt;
		cout << time << " ";
		//output << time << " ";
		fout << time << " ";
		
		for (int ifile = 0; ifile < m_files[0]; ifile++)
		{
			if (m_fnamebel[ifile][1] == "  ")
				continue;
			for(int i = 0; i < num; i++)
			{	
				int j = (dimension + 1) * ifile + 1; 
				if (m_fnamebel[ifile][1] == "000" && dt > m_frames[ifile + 1][1])	//suplement
				{
					cout << "nan ";
					//output << "nan ";
					fout << "nan ";
				}
				else
				{
					cout << vec[dt-1][i][j+2] << " ";
					//output << vec[dt-1][i][j+2] << " ";
					fout << vec[dt-1][i][j+2] << " ";
				}	
				if (ifile == m_files[0] - 1)
				{
					cout << vec[dt-1][i][0];		// << " " << vec[dt-1][i][(dimension + 1) * Num_file + 1]/count[0][dt-1];
					//output << vec[dt-1][i][0];		// << " " << vec[dt-1][i][(dimension + 1) * Num_file + 1]/count[0][dt-1];
					if (m_files[1] != 1)
						fout << vec[dt-1][i][0]; 	// << " " << vec[dt-1][i][(dimension + 1) * Num_file + 1]/count[0][dt-1];
				}
			}
		}
		cout << endl;
		//output << endl;
		fout << endl;
	}
	fout.close();
}

//read atom information -> atom[iframe][jatom][kinfo]
string read_atoms(ifstream &fin, int iframe, int nAtoms, vec_doub3 &vec)
{
	string temp;
	stringstream ss;
	string head;
	int Num_atoms = 1;
	int Num_obs = 1;
	
	for (int clear = 0; clear < 2; clear++)		//clear
		getline(fin, temp);
	ss << temp;
	ss >> Num_atoms;							//Number of atoms
	ss.clear();
	Num_obs = Num_atoms - nAtoms;				//for clearing obstacles
	
	for (int clear = 0; clear < 5; clear++)			
		getline(fin, temp);
	head = temp;								//the head
	
	for (int j = 0; j < nAtoms; j++)
	{
		getline(fin, temp);
		ss << temp;
		//extern const int Num_info = 15
		for (int k = 0; k < Num_info; k++)
			ss >> vec[iframe][j][k];
		ss.clear();
	}

	for (int clear = 0; clear < Num_obs; clear ++)	//clear information of obstacles
		getline(fin, temp);
	return head;
}
