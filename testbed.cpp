#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include "leptpp.h"
using namespace std;
using namespace leptpp;

int main(int argc, char* argv[])
{
	string inpath = argv[1];
	string outdir = argv[2];
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


	io.ocr_page_layout();	

	io.ocr_by_word();
	ostringstream oss;
	oss<<outdir<<"/output.png";
	if(0 != io.write(oss.str()))
	{
		cout<<"unk format"<<endl;
	}

	CCL* ccl = io.ccl();
	ccl->write(outdir);
	delete ccl;
	return 0;
}
