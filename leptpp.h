#ifndef LEPTPP_H
#define LEPTPP_H
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include "allheaders.h"
#include "baseapi.h"
using namespace std;
using namespace tesseract;

namespace leptpp
{
class CCL
{
	Pixa* m_pics;
	Boxa* m_boxs;
	public:
		CCL();
		~CCL();
	public:
		int write(string outdir);
		Pixa*& pics();
		Boxa*& boxs();
};

class LEPTPP
{
	private:
		Pix * m_pix;
	public:
		LEPTPP();
		~LEPTPP();
	public:
		int valid();
		int read(string filepath);
		int write(string filepath);
		int binarize();
		int cvt2gray();
		CCL* ccl();
		int ocr_by_word();
};

};

#endif
