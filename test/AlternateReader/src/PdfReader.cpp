/*
 * PdfReader.cpp
 *
 *  Created on: 28.05.2010
 *      Author: 1
 */

#include "PdfReader.h"
#include <bitdev.h>
#include "SplashTypes.h"

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
			//TInt err = iPdfDoc->getErrorCode();
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
			iDPI = 30;
			
			iCurrentPage = FirstPageNumber();
			
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
		
		/*
		 * It works, but works very slow...
		 *
		
		// create an off-screen device and context
		CGraphicsContext* bitmapContext=NULL;
		CFbsBitmapDevice* bitmapDevice = CFbsBitmapDevice::NewL(iBitmap);
		CleanupStack::PushL(bitmapDevice);
		User::LeaveIfError(bitmapDevice->CreateContext(bitmapContext));
		CleanupStack::PushL(bitmapContext);

		// draw something on the bitmap
		bitmapContext->SetBrushStyle(CGraphicsContext::ESolidBrush);
		bitmapContext->SetPenSize(TSize(1,1));
		
		TRgb color;
		TPoint point;
		iBitmap->LockHeap();
		SplashColorPtr pixel = new Guchar[4];
		for (int y = 0; y < iRealHeight; y++)
		{
			for (int x = 0; x < iRealWidth; x++)
			{
				bitmap->getPixel(x, y, pixel);
				color.SetRed(pixel[0]);
				color.SetGreen(pixel[1]);
				color.SetBlue(pixel[2]);
				bitmapContext->SetPenColor(color);
				
				point.iX = x;
				point.iY = y;
				bitmapContext->Plot(point);
			}
		}
		delete[] pixel;			
		iBitmap->UnlockHeap();			
		
		CleanupStack::PopAndDestroy(2);
		*/
		
		
		TInt outputdatastride = iBitmap->DataStride();
		
		TInt inputbytesInRow = iImageWidth * 3;
		while (inputbytesInRow%4){
			inputbytesInRow += 1;
		}		
		
		SplashColorPtr inputdata = bitmap->getDataPtr();
		char* outputdata = (char*)iBitmap->DataAddress();
		
		iBitmap->LockHeap();
		for (TInt y = 0; y < iImageHeight; y++)
		{
			char* adr = (char*)&inputdata[y*inputbytesInRow];
			
			//memcpy(outputdata, adr, iImageWidth*3);
			for (TInt x = 0; x < iImageWidth; x++)
			{
				*(outputdata + 3 * x + 0) = (char)*(adr + 3 * x + 2);
				*(outputdata + 3 * x + 1) = (char)*(adr + 3 * x + 1);
				*(outputdata + 3 * x + 2) = (char)*(adr + 3 * x + 0);
			}
			
			outputdata = (char*)outputdata + outputdatastride; 
		}		
		iBitmap->UnlockHeap();
		
		iOutputDevice->startPage( 0, NULL );
		
		iBitmapCopyWaiting = ETrue;
	
	}

void PdfReader::RenderPageWithoutBitmapCopyL(TInt aPage)
	{
		
		if(iCurrentPage < FirstPageNumber())
		{
			iCurrentPage = FirstPageNumber();
		}
		if(iCurrentPage > LastPageNumber())
		{
			iCurrentPage = LastPageNumber();
		}
		
		iPdfPage = iPdfCatalog->getPage(iCurrentPage);
		
		double DPI = ((TReal)iPageWidth*iZoomK/iPdfPage->getCropWidth())*72;
		
		iPdfPage->display(iOutputDevice, DPI, DPI, 0, gFalse, gFalse, gFalse, iPdfCatalog);
		
		iBitmapCopyWaiting = ETrue;
		
	}
