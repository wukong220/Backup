/*************************************************************************
	> File Name: test_01.cpp
	> Author: 悟空
	> Mail: wusong220@outlook.com 
	> Created Time: Thu Apr 11 21:31:44 2019
 ************************************************************************/

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

int main()
{
		using namespace std;
		
		stringstream ss;
		string str;
		
		string finname = "random.cpp";
		string foutname = "test01.cpp";
		ifstream fin(finname);
		ofstream fout(foutname);
		if(!fin.is_open())
			cout << "Error!" << endl;
		while(getline(fin,str))
			fout << str << endl;
		cout << str;
		fin.close();
		fout.close();
		return 0;
}
