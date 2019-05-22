#ifndef RAND_H_
#define RAND_H_
#include <cstdlib>
#include <cmath>

class Rand
{
private:
	unsigned long long v;
	unsigned long long w;

public:
	Rand(unsigned long long j);

	unsigned long long int64();
	unsigned int int32() { return (unsigned int)int64(); };
	double doub() { return 5.42101086242752217E-20 * int64(); };
};

inline Rand::Rand(unsigned long long j)
{
	v = 4101842887655102017LL;
	w = 1;
	v ^= j;
	w = int64();
	v = int64();
}

inline unsigned long long Rand::int64()
{
	v ^= v >> 17; v ^= v << 31; v ^= v >> 8;
	w = 4294957665U * (w & 0xffffffff) + (w >> 32);
	return v ^ w;
};

struct Normaldev : Rand {
	double mu, sig;
	Normaldev(double mmu, double ssig, unsigned long long i) : Ranq2(i), mu(mmu), sig(ssig) {}
	double dev();	//	Return a normal deviate.
};
#endif // !RAND_H_