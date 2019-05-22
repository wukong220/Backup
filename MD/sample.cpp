#include "sample.h"
using namespace std;

char folder[100];
char folder_xyz[100];

void create_folder(char* folder_name)
{
#ifdef _MSC_VER
	if (_access(folder_name, 0) != 0)
#else
	if (access(folder_name, 0) != 0)
#endif
	{
		char command[100];
		snprintf(command, 100, "mkdir %s", folder_name);
		system(command);
	}
	else
		cout << "folder " << folder_name << "/ exists" << endl;
};

void set_output_folder(char *folder)
{
	create_folder(folder);
	snprintf(folder_xyz, 100, "%s/xyz", folder);
	create_folder(folder_xyz);
}

void set_output(int nPar, double phi, double T)
{
	ofstream fout;
	ofstream fout_snap;
	char dir[100];
	char dir_snap[100];
	snprintf(dir, 100, "%d_%.2f_%.2f", nPar, phi, T);
	create_folder(dir);
#ifdef _MSC_VER
	snprintf(dir_snap, 100, "%s\\snap", dir);
#else
	snprintf(dir_snap, 100, "%s/snap", dir);
#endif
	create_folder(dir_snap);
	char file[100];
	snprintf(file, 100, "%s/log.dat", dir);
	fout.open(file);
};

double cal_p_tail(double rho, double rc)
{
	double tmp = pow(1. / rc, 3);
	return 16. / 3 * 3.14159 * rho * rho *(2. / 3 * tmp * tmp * tmp - tmp);
}