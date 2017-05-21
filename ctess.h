#pragma once

#include "baseapi.h"
using namespace tesseract;

class CTESS : public TessBaseAPI
{
	public:
		CTESS()
		{
		}
		~CTESS()
		{
		}
	public:
		const PAGE_RES* extGetPageRes()
		{
			return GetPageRes();
		}
};
