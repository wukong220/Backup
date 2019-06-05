#ifndef FILES_H
#define FILES_H

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

class LmpFile
{
private:
	//int m_NumFile;
	vector<int> m_files;	//cnt[NumFile, EffFile, cnt]	
	string m_head;
	vector<vector<string> > m_fnamebel;	//filename[ifile][name,label]
	vector<vector<int> > m_frames;		//frames[ifile][Num_frame, Max_frame]
	
public:
	//constructors and destructors
	LmpFile();
	LmpFile(const string &);
	LmpFile(const vector<vector<string> >);
	LmpFile(const vector<vector<string> >, const int);
	LmpFile(const vector<vector<string> >, const vector<vector<int> >);
	~LmpFile();
	friend ostream & operator<<(ostream & os, const LmpFile & file);
	int files(){return m_files[1];};
	//ifile, atom, center
	vector<vector<vector<double> > > read_data(const int, const vector<int>, ofstream &output, const int);	//ifile, closefile[i]
	//ifile, N_chain, atom[ifile][jatom][xu, yu, zu...]
	vector<vector<vector<double> > > center(const int, const int, const vector<vector<vector<double> > > &);	//ifile, N_chain, atom
	//ifile, rCM[ifile][jchain][x, y, z], count[ifile][dframe], msd[iframe][jchain][0, x, y, z]
	vector<vector<vector<double> > > msd_ave(const int, const vector<vector<vector<double> > > , vector<vector<vector<double> > > &);	
	void out_msd(const string, const vector<vector<vector<double> > >);
};

string read_atoms(ifstream &, int, int, vector<vector<vector<double> > > &); 	// fin, iframe, nAtoms, atom[i][j][k]

#endif