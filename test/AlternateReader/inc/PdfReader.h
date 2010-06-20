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
	void RenderPageWithoutBitmapCopyL(TInt aPage);
	void BitmapCopyL();
	
	void SetPageWidth(TInt aPageWidth)
	{
		iPageWidth = aPageWidth;
	} 
	
	void SetDPIForWidth(TInt aPageWidth)
	{
		iDPI = ((TReal)aPageWidth*iZoomK/iImageWidth)*iDPI;
	}
	
	TInt FirstPageNumber()
	{
		return 1;
	}
	TInt LastPageNumber()
	{
		return iPageCount;
	}
	
	};

#endif /* PDFREADER_H_ */
