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

class AbstractReader
	{
public:
	AbstractReader(){}
	virtual ~AbstractReader(){}

	// Rendering
	virtual void OpenL(const TDesC& aFileName) = 0;
	virtual void RenderPageWithoutBitmapCopyL(TInt aPage) = 0;
	virtual void BitmapCopyL() = 0;
	virtual void FindText(const TDesC& str) = 0;
	virtual void LoadTOC() = 0;
	
	// Manages
	virtual TInt FirstPageNumber() = 0;
	virtual TInt LastPageNumber() = 0;
	
	virtual void SetPageWidth(TInt aPageWidth) = 0;
	virtual void SetDPIForWidth(TInt aPageWidth) = 0;
	
	TInt GetCurrentPage()
	{
		return iCurrentPage;
	}
	void SetCurrentPage(TInt aPage)
	{
		iCurrentPage = aPage;
	}
	
	CFbsBitmap*	iBitmap;
	TBool iBitmapCopyWaiting;
	
	TBool iOpen;
    
    TInt iCurrentPage;
	TInt iPageCount;
			
	// Ўирина и высота рисунка без учета масштабировани€
	TInt iImageWidth;
	TInt iImageHeight;
	
	// Ўирина и высота рисунка c учетом масштабировани€
	TInt iRealWidth;
	TInt iRealHeight;
	
	TInt iPageWidth;
	TReal iZoomK;
	TReal iDPI;
	
	};

#endif /* ABSTRACTREADER_H_ */
