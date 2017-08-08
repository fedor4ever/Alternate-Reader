/*
 * DjvuReader.h
 *
 *  Created on: 28.05.2010
 *      Author: 1
 */

#ifndef DJVUREADER_H_
#define DJVUREADER_H_

#include "AbstractReader.h"
#include "ddjvuapi.h"

class DjvuReader : public AbstractReader
	{
public:
	
	DjvuReader();
	virtual ~DjvuReader();
	
	ddjvu_document_t*	iDjvuDoc;
	ddjvu_context_t*	iDjvuContext;
	ddjvu_page_t*	    iDjvuPage;
	ddjvu_format_t*     iDjvuFormat;
	ddjvu_render_mode_t iDjvuRenderMode;
	
	void OpenL(const TDesC& aFileName);
	void RenderPageWithoutBitmapCopyL(TInt aPage);
	void BitmapCopyL();
	
	void SetPageWidth(TInt aPageWidth)
	{
		iPageWidth = aPageWidth;
	} 
	
	void SetDPIForWidth(TInt aPageWidth)
	{
		
	}
	
	TInt FirstPageNumber()
	{
		return 0;
	}
	TInt LastPageNumber()
	{
		return iPageCount - 1;
	}
	
	};

#endif /* DJVUREADER_H_ */
