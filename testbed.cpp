#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include "leptpp.h"
using namespace std;

int main(int argc, char* argv[])
{
	string inpath = argv[1];
	string outpath = argv[2];
	LEPTPP io;
	io.read(inpath);
	if(io.valid() == 0)
	{
		cout<<"invalid leptpp"<<endl;
		return 0;
	}
	if(io.cvt2gray() != 0)
	{
		cout<<"error: cvt2gray()"<<endl;
		return 0;
	}
	if(io.binarize() != 0)
	{
		cout<<"error: binarize()"<<endl;
		return 0;
	}
	if(0 != io.write(outpath))
	{
		cout<<"unk format"<<endl;
	}
	return 0;
}
