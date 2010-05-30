/*
 * DjvuReader.h
 *
 *  Created on: 28.05.2010
 *      Author: 1
 */

#ifndef DJVUREADER_H_
#define DJVUREADER_H_

#include "AbstractReader.h"

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
	void RenderPageL(TInt aPage);
	void RenderPageWithoutBitmapCopyL(TInt aPage);
	void BitmapCopyL();
	
	};

#endif /* DJVUREADER_H_ */
