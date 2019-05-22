#include "output.h"

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
}

void set_output_folder(char *folder)
{
	create_folder(folder);
	snprintf(folder_xyz, 100, "%s/xyz", folder);
	create_folder(folder_xyz);
}

void set_output(std::ofstream & fout, double theta_max, double epsilon, double Pe, int N, double phi, int seed)
{
	snprintf(folder, 100, "%d_%.1f_%.1f_%d_%.3f_%d", int(theta_max), epsilon, Pe, N, phi, seed);
	create_folder(folder);
#ifdef _MSC_VER
	snprintf(folder_xyz, 100, "%s\\xyz", folder);
#else
	snprintf(folder_xyz, 100, "%s/xyz", folder);
#endif
	create_folder(folder_xyz);
	char file[100];
	snprintf(file, 100, "%s/log.dat", folder);
	fout.open(file);
}

void snapshot_xyz(Particle * par, int N, int step, const Vector3D & lBox)
{
	char file[100];
	snprintf(file, 100, "%s/%08d.extxyz", folder_xyz, step);
	ofstream fout(file);
	fout << N * 2 << endl;
	fout << "Lattice=\"" << lBox.x << " 0 0 0 " << lBox.y << " 0 0 0 " << lBox.z << "\" Properties=species:S:1:pos:R:3 Time=" << step * h << endl;
	for (int i = 0; i < N; i++)
	{
		Vector3D d = par[i].vecN * 0.01;
		fout << fixed << setprecision(5);
		fout << "O\t" << par[i].vecR << endl;
		fout << "N\t" << par[i].vecR + d << endl;
	}
	fout.close();
}