
#ifndef CDJVUREADER_H_
#define CDJVUREADER_H_

#include <e32base.h>
#include <fbs.h>
#include <CAknFileSelectionDialog.h> 
#include <CAknMemorySelectionDialog.h>

// DjVu
#include "ddjvuapi.h"
#include "UTF.h"

// PDF
#include "PDFDoc.h"
#include "SplashOutputDev.h"
#include "goo/GooString.h"
#include "GlobalParams.h"
#include "SplashBitmap.h"

#include "AbstractReader.h"
#include "DjvuReader.h"
#include "PdfReader.h"

class CDjvuReader : public CBase
	{
public:
	
	static CDjvuReader* NewL();
	static CDjvuReader* NewLC();   
	void ConstructL();
	CDjvuReader();
	virtual ~CDjvuReader();
	
	void OpenL(const TDesC& aFileName);
	void RenderPageL(TInt aPage);
	void RenderPageWithoutBitmapCopyL(TInt aPage);
	void BitmapCopyL();
	
	void NextPageL();
	void NextPageWithoutBitmapCopyL();
	void PreviousPageL();
	void PreviousPageWithoutBitmapCopyL();
	
public:

	TInt CurrentPage() const
	{
		if(!iAbstractReader)
		{	
			return 0;
		}		
	
		return iAbstractReader->iCurrentPage;
	}
	
	TInt GetPageCount() const
	{
		if(!iAbstractReader)
		{	
			return 0;
		}		
		
		return iAbstractReader->iPageCount; 
	}

    void SetCurrentPage(TInt aCurrentPage)
    {
		if(iAbstractReader)
		{	
			iAbstractReader->iCurrentPage = aCurrentPage;
		}
    }
    
    CFbsBitmap* GetBitmap() const
	{
		return iBitmap;
	}
   
    TBool IsOpen() const
    {
    	if(iAbstractReader)
		{	
			return iAbstractReader->iOpen;
		}
    	else
    	{
			return false;
    	}
    }
    
    void SetZoom(TReal aZoom)
    {
    	iAbstractReader->iZoomK = aZoom;
    }    
    
    TReal GetZoom() const
	{
		if(!iAbstractReader)
		{	
			return 0;
		}		
    	
    	return iAbstractReader->iZoomK;
	}
    
    void SetPageWidth(TInt aPageWidth)
	{
    	iAbstractReader->iPageWidth = aPageWidth;
	} 
    
    TInt GetPageWidth() const
   	{
		if(!iAbstractReader)
		{	
			return 0;
		}		
 
    	
    	return iAbstractReader->iPageWidth;
   	}
    
    void IncrementZoomL()
    {
    	iAbstractReader->iZoomK += 0.2;
    	iAbstractReader->RenderPageL(iAbstractReader->iCurrentPage);
    }
    
    void IncrementZoomWithoutBitmapCopyL()
	{
    	iAbstractReader->iZoomK += 0.2;
		iAbstractReader->RenderPageWithoutBitmapCopyL(iAbstractReader->iCurrentPage);
	}
    
    void DecrementZoomL()
    {
    	if (iAbstractReader->iZoomK > 0.2)
    		iAbstractReader->iZoomK -= 0.2;
    	iAbstractReader->RenderPageL(iAbstractReader->iCurrentPage);
    }
    
    void DecrementZoomWithoutBitmapCopyL()
    {
    	if (iAbstractReader->iZoomK > 0.2)
    		iAbstractReader->iZoomK -= 0.2;
    	iAbstractReader->RenderPageWithoutBitmapCopyL(iAbstractReader->iCurrentPage);
    }    
    
    TInt GetImageWidth() const
	{
		if(!iAbstractReader)
		{	
			return 0;
		}		

    	
    	return iAbstractReader->iImageWidth;
	}
    
    TInt GetBitmapWidth() const
   	{
		if(!iAbstractReader)
		{	
			return 0;
		}		
 		
    	
    	return iAbstractReader->iRealWidth;
   	} 
    
    TInt GetImageLeftMargin() const
	{
 		TRgb white(255, 255, 255);
		TRgb color;
		TPoint point(0, 0);
		TInt h = iAbstractReader->iBitmap->SizeInPixels().iHeight;
		TInt w = iAbstractReader->iBitmap->SizeInPixels().iWidth;
		
		for(TInt j=0; j<w-1; j++)
		{
			for(TInt i=0; i<h-1; i++)
			{
				point.iX = j;
				point.iY = i;
				iAbstractReader->iBitmap->GetPixel(color, point);
				if(color != white)
				{
					return j;
				}
			}
		}
	}
    
    TInt GetImageRightMargin() const
    	{
    		TRgb white(255, 255, 255);
    		TRgb color;
    		TPoint point(0, 0);
    		TInt h = iAbstractReader->iBitmap->SizeInPixels().iHeight;
    		TInt w = iAbstractReader->iBitmap->SizeInPixels().iWidth;

    		for(TInt j=w-1; j>0; j--)
    		{
    			for(TInt i=0; i<h-1; i++)
    			{
					point.iX = j;
					point.iY = i;
					iAbstractReader->iBitmap->GetPixel(color, point);
    				if(color != white)
    				{
    					return w - j;
    				}
    			}
    		}

    	}
    
private:
	
    AbstractReader* iAbstractReader;
    CFbsBitmap* iBitmap;
    
	};

#endif
