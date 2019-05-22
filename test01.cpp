#include <iostream>
#include <time.h>
#include <random>
#include <string>

int main()
{
	using namespace std;
	clock_t start = clock();
	
	unsigned int seed = time(NULL);
	default_random_engine generator(seed);
	normal_distribution<double> dis(4, 1.5);	//mean = 0; stddev = 1.0
	uniform_real_distribution<double> u(0, 1);	//[0,1]
	
	vector<int> vec1(10, 1);
	vector<int> vec2(10, 2);
	cout << vec2 - vec1 << endl;	
	for (int i = 0; i < vec1.size(); i++)
	{
		cout << i << ": " << string(vec1[i], '#') << endl;
	}
	
	clock_t end = clock();
	clock_t inter = end - start;
	cout << "Start: " << start << endl
	<< "End: " << end << endl
	<< "Continue: " << inter << endl;
	return 0;
}