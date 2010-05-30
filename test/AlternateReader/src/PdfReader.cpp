/*
 * PdfReader.cpp
 *
 *  Created on: 28.05.2010
 *      Author: 1
 */

#include "PdfReader.h"
#include <bitdev.h>

PdfReader::PdfReader()
	{
		iPdfDoc       = NULL;
		iPdfCatalog   = NULL;
		iPdfPage      = NULL;
		iOutputDevice = NULL;
	
		iSplashColor[0] = 255;
		iSplashColor[1] = 255;
		iSplashColor[2] = 255;
		
		globalParams  = new GlobalParams();
		iOutputDevice = new SplashOutputDev(splashModeRGB8, 4, gFalse, iSplashColor);
	}

PdfReader::~PdfReader()
	{
		delete iOutputDevice;
		delete iPdfDoc;
		delete globalParams;
	}

void PdfReader::OpenL(const TDesC& aFileName)
	{
		
		TBuf8<256> utf8name;
		CnvUtfConverter::ConvertFromUnicodeToUtf8(utf8name, aFileName);

		GooString* filename_g = new GooString ((char*) utf8name.PtrZ());
		iPdfDoc = new PDFDoc(filename_g);
		//delete filename_g; // filename_g will deleted within PDFDoc 
		
		if (!iPdfDoc->isOk())
		{
			TInt err = iPdfDoc->getErrorCode();
			delete iPdfDoc;
		}
		else
		{
	
			iPdfCatalog = iPdfDoc->getCatalog();
			iOutputDevice->startDoc(iPdfDoc->getXRef());
			iPageCount = iPdfCatalog->getNumPages();
			
			iZoomK = 1;
			iOpen = ETrue;
			iPageWidth = 360;
			
			iCurrentPage = 1;
			
		}
			
	}

void PdfReader::BitmapCopyL()
	{
		
		SplashBitmap* bitmap = iOutputDevice->getBitmap();
	
		iImageWidth = bitmap->getWidth();
		iImageHeight = bitmap->getHeight();
		
		iRealWidth = iImageWidth;
		iRealHeight = iImageHeight;		
		// Calculating image size done...		
		
		TSize new_size(iRealWidth, iRealHeight);
		if (iBitmap->SizeInPixels() != new_size)
		{
			iBitmap->Reset();
			iBitmap->Create(new_size, EColor16M);
		}
		
		// create an off-screen device and context
		CGraphicsContext* bitmapContext=NULL;
		CFbsBitmapDevice* bitmapDevice = CFbsBitmapDevice::NewL(iBitmap);
		CleanupStack::PushL(bitmapDevice);
		User::LeaveIfError(bitmapDevice->CreateContext(bitmapContext));
		CleanupStack::PushL(bitmapContext);

		// draw something on the bitmap
		bitmapContext->SetBrushStyle(CGraphicsContext::ESolidBrush);
		bitmapContext->SetPenSize(TSize(1,1));
		
		iBitmap->LockHeap();
		SplashColorPtr pixel = new Guchar[4];
		for (int y = 0; y < iRealHeight; y++)
		{
			for (int x = 0; x < iRealWidth; x++)
			{
				iOutputDevice->getBitmap()->getPixel(x, y, pixel);
				bitmapContext->SetPenColor(TRgb(pixel[0], pixel[1], pixel[2]));
				bitmapContext->Plot(TPoint(x, y));
			}
		}
		delete[] pixel;			
		iBitmap->UnlockHeap();			

		CleanupStack::PopAndDestroy(2);
		
		iBitmapCopyWaiting = ETrue;
	
	}

void PdfReader::RenderPageL(TInt aPage)
	{
		
		if (aPage >= 0)		
		{
			if(aPage >= iPageCount)
			{
				iCurrentPage = iPageCount;
			}
			else
			{
				iCurrentPage = aPage;
			}
		
		}
		else
		{
			iCurrentPage = 1;
		}
		
		if(iCurrentPage == 0)
		{
			iCurrentPage = 1;
		}
		
		iPdfPage = iPdfCatalog->getPage(iCurrentPage);
		iPdfPage->display(iOutputDevice, 75*iZoomK, 75*iZoomK, 0, gFalse, gFalse, gFalse, iPdfCatalog);
		
		BitmapCopyL();				
			
	}

void PdfReader::RenderPageWithoutBitmapCopyL(TInt aPage)
	{
		
		if (aPage >= 0)		
		{
			iCurrentPage =  aPage >= iPageCount?iPageCount-1:aPage;
		}
		if(iCurrentPage == 0)
		{
			iCurrentPage = 1;
		}
		
		iPdfPage = iPdfCatalog->getPage(iCurrentPage);
		iPdfPage->display(iOutputDevice, 75*iZoomK, 75*iZoomK, 0, gFalse, gFalse, gFalse, iPdfCatalog);
		
		iBitmapCopyWaiting = ETrue;
		
	}
