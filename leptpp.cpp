#include "leptpp.h"



int get_pixel_format_from_name(string filename)
{
	int pos = filename.rfind('.');
	if (pos == string::npos)
		return IFF_UNKNOWN;
	string ext = filename.substr(pos + 1);
	transform(ext.begin(),ext.end(),ext.begin(),::tolower);
	switch (ext):
	{
		case "png": 
			return IFF_PNG;
		case "jpg":
		case "jpeg":
			return IFF_JPEG;
	}
	return IFF_UNKNOWN;	
}	

LEPTPP::LEPTPP()
{
	m_pix = NULL;
}
LEPTPP::~LEPTPP()
{
	if(m_pix != NULL)
	{
		pixDestroy(&m_pix);
	}
	m_pix = NULL;
}
int LEPTPP::valid()
{
	if(m_pix == NULL)
		return 0;
	return 1;
}
int LEPTPP::read(string filepath)
{
	if(m_pix != NULL)
		pixDestroy(&m_pix);
	m_pix = pixRead(filepath.c_str());
	return 0;
}

int LEPTPP::write(string filepath)
{
	if(valid() == 0)
		return -1;
	int fmt = get_pixel_format_from_name(filepath);
	if( fmt == IFF_UNKNOWN)
		return -1;
	pixWrite(filepath.c_str(), m_pix, fmt);
	return 0;
}


