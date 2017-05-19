#include "leptpp.h"
#include "ctess.h"
#include "pageres.h"
using namespace tesseract;
namespace leptpp
{

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

CCL* LEPTPP::ccl()
{
	if(valid() == 0)
		return NULL;
	Pixa* pics;
	Boxa* boxs;
	pics = pixaCreate(0);
	boxs = pixConnComp(m_pix, &pics, 8);

	CCL* ccl = new CCL();
	ccl->boxs() = boxs;
	ccl->pics() = pics;
	return ccl;
}


int LEPTPP::ocr_page_layout()
{
	CTESS* api = new CTESS();
	if( api->Init(NULL,"eng") != 0)
	{
		fprintf(stderr,"could not initialize tesseract.\n");
		return 0;
	}
	api->SetPageSegMode(PSM_AUTO);
	api->SetImage(m_pix);
	api->Recognize(NULL);
	const PAGE_RES* pages = api->extGetPageRes();
	const BLOCK_RES_LIST* blocks = &(pages->block_res_list);
	BLOCK_RES_IT block_it( (BLOCK_RES_LIST*) blocks );
	block_it.move_to_first();
	for(int blockI = 0; blockI < blocks->length(); blockI++)
	{
		BLOCK_RES* block = block_it.data();
		
	}

	return 0;
}

int LEPTPP::ocr_by_word()
{
#if 0
	return 0;
#else
	//TessBaseAPI* api = new TessBaseAPI();
	CTESS* api = new CTESS();
	if( api->Init(NULL,"eng") != 0)
	{
		fprintf(stderr,"could not initialize tesseract.\n");
		return 0;
	}
	api->SetPageSegMode(PSM_AUTO);
	//api->SetVariable("save_blob_choices","true");
	api->SetImage(m_pix);
	api->Recognize(NULL);
	ResultIterator* ri = api->GetIterator();
	PageIteratorLevel level = RIL_WORD;
	if(ri != NULL)
	{
		do{
			const char* word = ri->GetUTF8Text(level);
			float conf = ri->Confidence(level);
			int x0,x1,y0,y1;
			ri->BoundingBox(level,&x0,&y0,&x1,&y1);
			printf("word:'%s';\tconf:%.2f;\tbbox:%d,%d,%d,%d\r\n", word, conf, x0,x1,y0,y1);
			delete[] word;
		}while(ri->Next(level));
	}
	return 0;
#endif
}




/////////////////////////////////////////////////////////////////////
//-------------------------CCL-------------------------------------

CCL::CCL()
{
	m_pics = NULL;
	m_boxs = NULL;
}
CCL::~CCL()
{
	if(m_pics != NULL)
	{
		pixaDestroy(&m_pics);
	}
	if(m_boxs != NULL)
	{
		boxaDestroy(&m_boxs);
	}
}

Pixa*& CCL::pics()
{
	return m_pics;
}

Boxa*& CCL::boxs()
{
	return m_boxs;
}


int CCL::write(string outdir )
{
	if(m_pics == NULL || m_boxs == NULL)
		return -1;
	for(int k = 0; k < m_pics->n; k++)
	{
		PIX* pic = m_pics->pix[k];
		BOX* box = m_boxs->box[k];
		ostringstream oss;
		oss<<outdir<<"/ccl"<<k<<"["<<box->x<<","<<box->y<<","<<box->x + box->w - 1<<","<<box->y + box->h - 1<<"].png";
		if( 0 != pixWrite(oss.str().c_str(), pic, IFF_PNG))
			return -1;
	}
	return 0;
}

};
