#ifndef LEPTPP_H
#define LEPTPP_H
#include <string>
#include <vector>
#include <algorithm>
#include "allheaders.h"
using namespace std;
class LEPTPP
{
	private:
		PIX * m_pix;
	public:
		LEPTPP();
		~LEPTPP();
	public:
		int valid();
		int read(string filepath);
		int write(string filepath);
};


#endif
