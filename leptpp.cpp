#include "leptpp.h"
#include "baseapi.h"

int get_pixel_format_from_name(string filename)
{
	int pos = filename.rfind('.');
	if (pos == string::npos)
		return IFF_UNKNOWN;
	string ext = filename.substr(pos + 1);
	transform(ext.begin(),ext.end(),ext.begin(),::tolower);
	if(ext == "png")
		return IFF_PNG;
	if(ext == "bmp")
		return IFF_BMP;
	if(ext == "PNM")
		return IFF_PNM;
	if(ext == "gif")
		return IFF_GIF;
	if(ext == "jpg" || ext == "jpeg")
		return IFF_JFIF_JPEG;
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
	if (fmt != IFF_JFIF_JPEG)
	{
		if( 0 != pixWrite(filepath.c_str(), m_pix, fmt))
			return -1;
	}
	else
	{
		if(0 != pixWriteImpliedFormat(filepath.c_str(),m_pix,
					75,0))
			return -1;
	}
	return 0;
}

int LEPTPP::cvt2gray()
{
	int depth = pixGetDepth(m_pix);
	if(depth != 8)
	{
		Pix* gray = depth < 8? pixConvertTo8(m_pix,false)
			: pixConvertRGBToLuminance(m_pix);
		pixDestroy(&m_pix);
		m_pix =  gray;
	}
	return 0;
}

int LEPTPP::binarize()
{
	if(valid() == 0)
		return -1;
	Pix* seg = NULL;
	pixOtsuAdaptiveThreshold(m_pix,
		pixGetWidth(m_pix), pixGetHeight(m_pix),
		0, 0, 0,
		NULL, &seg);

	pixDestroy(&m_pix);
	m_pix = seg;
	return 0;
}


