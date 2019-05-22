#ifndef FORCE_H_
#define FORCE_H_ 

class ULJ
{
private:
	const double epsilon;
	double rCut;
	double rCut2;
	double ecut;

public:
	ULJ() :epsilon(1.) { rCut = 2.5; rCut2 = rCut*rCut; double r6i = pow(rCut2, -3.); ecut = 4 * epsilon*r6i*(r6i - 1); };
	ULJ(double rc) :epsilon(1.) { rCut = rc; rCut2 = rc*rc; double r6i = pow(rCut2, -3.), ecut = 4 * epsilon*r6i*(r6i - 1); };
	double rC2() { return rCut2; };
	double potencial(double r2) const { double r6i = pow(r2, -3.); return 4 * epsilon*r6i*(r6i - 1) - ecut; };
	double force(double r2)const
	{
		double r2i = pow(r2, -1.);
		double r6i = pow(r2, -3.);
		return 48 * epsilon * r2i * r6i * (r6i - 0.5);
	};
};

template <class Vector, class ULJ>
void force(int nPar, double &en_p,
	Vector lBox, Vector *par_R, Vector *par_Rm, Vector *par_V,
	ULJ *pair, Vector *par_F)
{
	Vector dR;
	double r2;
	int i = 0;
	int j = 0;
	for (; i < nPar; i++)
		par_F[i] = 0;
	for (i = 0; i < nPar; i++)
		for (j = i + 1; j < nPar; j++)
		{
			untangle(par_R[i], par_R[j], lBox);
			dR = par_R[i] - par_R[j];
			tangle(par_R[i], lBox);
			//cout << "dR: " << dR<<"\t"<<dR.square() << endl;
			r2 = dR.square();
			if (r2<pair->rC2())
			{
				//cout << "r2: " << r2 << endl;
				par_F[i] = par_F[i] + pair->force(r2)*dR;
				par_F[j] = par_F[j] - pair->force(r2)*dR;
				en_p += pair->potencial(r2);
			}
		}
	en_p /= nPar;
};

template <class Vector>
void integtate(int nPar, double delt, double &en_v2, double &temp,
	Vector lBox, Vector *par_R, Vector *par_Rm, Vector *par_V, Vector *par_F)
{
	Vector *par_Rn = new Vector[nPar];
	for (int i = 0; i < nPar; i++)
	{
		untangle(par_Rm[i], par_R[i], lBox);
		par_Rn[i] = 2 * par_R[i] - par_Rm[i] + par_F[i] * delt*delt;
		tangle(par_Rn[i], lBox);
		//cout << "par_Rn[i]: "<<par_Rn[i]<<"\t"<<par_Rn[i].square()<< endl;
		untangle(par_Rm[i], par_Rn[i], lBox);
		par_V[i] = (par_Rn[i] - par_Rm[i]) / (2 * delt);
		//cout << "par_V[i]: "<<par_V[i]<<"\t"<<par_V[i].square()<< endl;

		en_v2 += par_V[i].square();
		par_Rm[i] = par_R[i];
		par_R[i] = par_Rn[i];
	}
	temp = en_v2 / (3 * nPar);
	en_v2 = en_v2 / (2 * nPar);
	delete[] par_Rn;
}
#endif // !FORCE_H_