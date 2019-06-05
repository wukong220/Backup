#ifndef CELL_H
#define CELL_H
#include "particle.h"

struct Cell
{
	Particle* head;
	Cell ** neighbor;
	Vector3D* distance;
	int valid_n_neighbor;

	static int n_neighbor;
	static int nCell, nx, ny, nz, nxy;
	static Vector3D lBox;
	static Vector3D lCell;

	Cell() { head = NULL; distance = NULL; neighbor = NULL; valid_n_neighbor = 0; }
	~Cell();
	static void ini(Cell** cell, Particle * par, int N, Vector3D &_lBox,double rcuct, bool PBCx, bool PBCy, bool PBCz);
	void interact(const Force *pf) const;
	void interact(const Cell *c, const Vector3D &DR, const Force *pf) const;

	void find_neighbor(int ix, int iy, int iz, Cell *cell, bool PBCx, bool PBCy, bool PBCz);
	static void find_all_neighbor(Cell *cell, bool PBCx, bool PBCy, bool PBCz);
	static void createCellList(Particle* par, int N, Cell* cell);
	static void removeCellList(Cell* cell);
	static void updateCellList(Particle * par, int N, Cell * cell);
	static void pairForce(Cell* cell, const Force *pf);
};
#endif
