
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

const TReal KZoomFactor = 0.1;

class CDjvuReader : public CBase
	{
public:
	
	static CDjvuReader* NewL();
	static CDjvuReader* NewLC();   
	void ConstructL();
	CDjvuReader();
	virtual ~CDjvuReader();
	
	TBool OpenL(const TDesC& aFileName);
	void RenderPageWithoutBitmapCopyL(TInt aPage);
	void BitmapCopyL();
	virtual void FindText(const TDesC& str);
	virtual void LoadTOC();
	
	void NextPageWithoutBitmapCopyL();
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
    
    void SetDPI(TReal aDPI)
	{
		iAbstractReader->iDPI = aDPI;
	} 
    
    TReal GetDPI()
   	{
		if(!iAbstractReader)
		{	
			return 75;
		}		
    	
    	return iAbstractReader->iDPI;
  	} 
    
    TReal GetZoom() const
	{
		if(!iAbstractReader)
		{	
			return 1;
		}		
    	
    	return iAbstractReader->iZoomK;
	}
    
    void SetPageWidth(TInt aPageWidth)
	{
    	iAbstractReader->SetPageWidth(aPageWidth);
	} 
    
    void SetDPIForWidth(TInt aPageWidth)
   	{
       	iAbstractReader->SetDPIForWidth(aPageWidth);
   	} 
    
    TInt FirstPageNumber()
   	{
       	if(!iAbstractReader)
		{
			return 0;
		}
    	return iAbstractReader->FirstPageNumber();
   	} 
    
    TInt LastPageNumber()
	{
		if(!iAbstractReader)
		{
			return 0;
		}
		return iAbstractReader->LastPageNumber();
	}
    
    TInt GetPageWidth() const
   	{
		if(!iAbstractReader)
		{	
			return 0;
		}		
 
    	return iAbstractReader->iPageWidth;
   	}

    void DecrementZoomWithoutBitmapCopyL()
    {
    	if (iAbstractReader->iZoomK > KZoomFactor)
    		iAbstractReader->iZoomK -= KZoomFactor;
    	iAbstractReader->RenderPageWithoutBitmapCopyL(iAbstractReader->iCurrentPage);
    }    
    
    void IncrementZoomWithoutBitmapCopyL()
	{
    	iAbstractReader->iZoomK += KZoomFactor;
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
		return KErrGeneral;
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
    		return KErrGeneral;
    	}
    
private:
	
    AbstractReader* iAbstractReader;
    CFbsBitmap* iBitmap;
    
	};

#endif
