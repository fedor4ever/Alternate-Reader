/*
 * PdfReader.h
 *
 *  Created on: 28.05.2010
 *      Author: 1
 */

#ifndef PDFREADER_H_
#define PDFREADER_H_

#include "AbstractReader.h"

class PdfReader : public AbstractReader
	{
public:
	PdfReader();
	virtual ~PdfReader();
	
	PDFDoc* iPdfDoc;
	Catalog* iPdfCatalog;
	Page* iPdfPage;
	SplashOutputDev* iOutputDevice;
	SplashColor iSplashColor;
	
	void OpenL(const TDesC& aFileName);
	void RenderPageL(TInt aPage);
	void RenderPageWithoutBitmapCopyL(TInt aPage);
	void BitmapCopyL();
	
	};

#endif /* PDFREADER_H_ */
