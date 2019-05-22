#include "cell.h"
using namespace std;

/* offset value for 3d */
int offset_3d[13][3] = {
	{ 1, 0, 0},
	{ 1, 1, 0},
	{ 0, 1, 0},
	{-1, 1, 0},
	{ 0, 0, 1},
	{ 1, 0, 1},
	{ 1, 1, 1},
	{ 0, 1, 1},
	{-1, 1, 1},
	{-1, 0, 1},
	{-1,-1, 1},
	{ 0,-1, 1},
	{ 1,-1, 1} 
};

/* static member variables */
int Cell::n_neighbor;
Vector3D Cell::lBox;
Vector3D Cell::lCell;
int Cell::nCell;
int Cell::nx;
int Cell::ny;
int Cell::nz;
int Cell::nxy;


Cell::~Cell()
{
	delete[] neighbor;
	neighbor = NULL;
	delete[] distance;
	distance = NULL;
}

/* initialization */
void Cell::ini(Cell** cell, Particle* par, int N, Vector3D &_lBox, double rcut, bool PBCx, bool PBCy, bool PBCz)
{
	lBox = _lBox;
	nx = int(lBox.x / rcut);
	ny = int(lBox.y / rcut);
	nz = int(lBox.z / rcut);
	nCell = nx * ny * nz;
	nxy = nx * ny;
	lCell = Vector3D(lBox.x / nx, lBox.y / ny, lBox.z /nz);

	cout << "(lx, ly, lz) = " << lCell << endl;
	cout << "nx, ny, nz = " << nx << " ," << ny << " ," << nz << endl;
	*cell = new Cell[nCell];

	n_neighbor = 13;
	if (n_neighbor == 13)
		find_all_neighbor(*cell, PBCx, PBCy, PBCz);

	createCellList(par, N, *cell);
}

void check(int &i, int n, bool PBC, double L, bool &flag, double &d)
{
	if (i >= 0 && i < n) flag = true;
	else if (PBC)
	{
		if (i >= n)
		{
			flag = true;
			d = L;
			i -= n;
		}
		else if (i < 0)
		{
			flag = true;
			d = -L;
			i += n;
		}
	}
}

/* find neighbors of one cell */
void Cell::find_neighbor(int ix0, int iy0, int iz0, Cell *cell, bool PBCx, bool PBCy, bool PBCz)
{
	neighbor = new Cell *[n_neighbor];
	distance = new Vector3D[n_neighbor];
	for (int i = 0; i < n_neighbor; i++)
	{
		neighbor[i] = NULL;
		distance[i] = 0;
	}
	int count = 0;
	for (int i = 0; i < n_neighbor; i++)
	{
		int ix = ix0 + offset_3d[i][0];
		int iy = iy0 + offset_3d[i][1];
		int iz = iz0 + offset_3d[i][2];

		bool flag_x = false;
		bool flag_y = false;
		bool flag_z = false;

		double dx = 0;
		double dy = 0;
		double dz = 0;
		
		check(ix, nx, PBCx, lBox.x, flag_x, dx);
		check(iy, ny, PBCy, lBox.y, flag_y, dy);
		check(iz, nz, PBCz, lBox.z, flag_z, dz);

		//judge whether the neighbor is valid
		if (flag_x && flag_y && flag_z)
		{
			int index = ix + iy * nx + iz * nxy;
			neighbor[count] = cell + index;
			distance[count] = Vector3D(dx, dy, dz);
			count++;
		}
	}
	valid_n_neighbor = count;
}

/* find neighbors of each cell */
void Cell::find_all_neighbor(Cell* cell, bool PBCx, bool PBCy, bool PBCz)
{
	for (int iz = 0; iz < nz; iz++)
	{
		for (int iy = 0; iy < ny; iy++)
		{
			for (int ix = 0; ix < nx; ix++)
			{
				int index = ix + iy * nx + iz * nxy;
				cell[index].find_neighbor(ix, iy, iz, cell, PBCx, PBCy, PBCz);
			}
		}
	}
}

/* 遍历cell list, 计算同一条list上各个粒子的相互作用*/
void Cell::interact(const Force *pf) const
{
	Particle* p1 = head;
	Particle* p2;
	while (p1->next)
	{
		p2 = p1->next;
		do {
			p1->pair_force(p2, pf);
			p2 = p2->next;
		} while (p2);
		p1 = p1->next;
	}
}

/* 遍历两条cell list, 计算不同list上粒子间的相互作用, DR是计算距离时，由于周期性边界条件而引入的修正*/
void Cell::interact(const Cell *c, const Vector3D &DR, const Force *pf) const
{
	Particle* p1 = head;
	do {
		Particle* p2 = c->head;
		do {
			p1->pair_force(p2, pf, DR);
			p2 = p2->next;
		} while (p2);
		p1 = p1->next;
	} while (p1);
}

/* create cell list */
void Cell::createCellList(Particle * par, int N, Cell * cell)
{
	for (int i = 0; i < N; i++)
	{
		int ix = int(par[i].vecR.x / lCell.x);
		if (ix >= nx) ix = 0;
		int iy = int(par[i].vecR.y / lCell.y);
		if (iy >= ny) iy = 0;
		int iz = int(par[i].vecR.z / lCell.z);
		if (iz >= nz) iz = 0;

		int j = ix + iy * nx + iz * nxy;
		par[i].next = cell[j].head;
		cell[j].head = par + i;
	}
}

/* unlink cell list */
void Cell::removeCellList(Cell * cell)
{
	for (int i = 0; i < nCell; i++)
		cell[i].head = NULL;
}

/* update cell list */
void Cell::updateCellList(Particle * par, int N, Cell * cell)
{
	removeCellList(cell);
	createCellList(par, N, cell);
}

void Cell::pairForce(Cell * cell, const Force *pf)
{
	for (int i = 0; i < nCell; i++)
	{
		if (cell[i].head)
		{
			cell[i].interact(pf);
			for (int k = 0; k < cell[i].valid_n_neighbor; k++)
			{
				if (cell[i].neighbor[k]->head)
					cell[i].interact(cell[i].neighbor[k], cell[i].distance[k], pf);
			}
		}
	}
}
