/*
 * AbstractReader.h
 *
 *  Created on: 28.05.2010
 *      Author: 1
 */

#ifndef ABSTRACTREADER_H_
#define ABSTRACTREADER_H_

#include <e32base.h>
#include <fbs.h>

// DjVu
#include "ddjvuapi.h"
#include "UTF.h"

// PDF
#include "PDFDoc.h"
#include "SplashOutputDev.h"
#include "goo/GooString.h"
#include "GlobalParams.h"
#include "SplashBitmap.h"

class AbstractReader
	{
public:
	AbstractReader();
	virtual ~AbstractReader();

	virtual void OpenL(const TDesC& aFileName) = 0;
	virtual void RenderPageL(TInt aPage) = 0;
	virtual void RenderPageWithoutBitmapCopyL(TInt aPage) = 0;
	virtual void BitmapCopyL() = 0;
	
	CFbsBitmap*	iBitmap;
	TBool iBitmapCopyWaiting;
	
	TBool iOpen;
    
    TInt iCurrentPage;
	TInt iPageCount;
			
	// Ўирина и высота рисунка без учета масштабировани€
	TInt iImageWidth;
	TInt iImageHeight;
	TInt iRowsize;
	
	// Ўирина и высота рисунка c учетом масштабировани€
	TInt iRealWidth;
	TInt iRealHeight;
	
	TInt iPageWidth;
	TReal iZoomK;
	
	};

#endif /* ABSTRACTREADER_H_ */
