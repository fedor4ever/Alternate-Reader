
#include "CDjvuReader.h"

_LIT(KExtensionDjvu, ".djvu");
_LIT(KExtensionPdf, ".pdf");

/**
 * Symbian two-phase constructor.
 * This creates an instance then calls the second-phase constructor
 * without leaving the instance on the cleanup stack.
 * @return new instance of CSymDjvuContainerView
 */
CDjvuReader* CDjvuReader::NewL()
	{
		CDjvuReader* self = CDjvuReader::NewLC();
		CleanupStack::Pop(self);
		return self;
	}

/**
 * Symbian two-phase constructor.
 * This creates an instance, pushes it on the cleanup stack,
 * then calls the second-phase constructor.
 * @return new instance of CSymDjvuContainerView
 */
CDjvuReader* CDjvuReader::NewLC()
	{
		CDjvuReader* self = new (ELeave) CDjvuReader();
		CleanupStack::PushL(self);
		self->ConstructL();
		return self;
	}

/**
 * Second-phase constructor for view.  
 * Initialize contents from resource.
 */
void CDjvuReader::ConstructL()
	{
		iBitmap = new (ELeave) CFbsBitmap;
	}

CDjvuReader::CDjvuReader()
	{
		iAbstractReader = NULL;
	}

CDjvuReader::~CDjvuReader()
	{
		delete iAbstractReader;
		delete iBitmap;
	}

void CDjvuReader::BitmapCopyL()
	{
		iAbstractReader->BitmapCopyL();
	}

void CDjvuReader::RenderPageL(TInt aPage)
	{
		
		iAbstractReader->RenderPageL(aPage);				
			
	}

void CDjvuReader::RenderPageWithoutBitmapCopyL(TInt aPage)
	{
		
		iAbstractReader->RenderPageWithoutBitmapCopyL(aPage);
		
	}

void CDjvuReader::NextPageL()
	{
		if (iAbstractReader->iCurrentPage + 1 < iAbstractReader->iPageCount)
		{
			iAbstractReader->iCurrentPage++;
			iAbstractReader->RenderPageL(iAbstractReader->iCurrentPage);
		}
	}

void CDjvuReader::NextPageWithoutBitmapCopyL()
	{
		if (iAbstractReader->iCurrentPage + 1 < iAbstractReader->iPageCount)
		{
			iAbstractReader->iCurrentPage++;
			iAbstractReader->RenderPageWithoutBitmapCopyL(iAbstractReader->iCurrentPage);
		}
	}

void CDjvuReader::PreviousPageL()
	{
		if (iAbstractReader->iCurrentPage > 0)
		{
			iAbstractReader->iCurrentPage--;
			iAbstractReader->RenderPageL(iAbstractReader->iCurrentPage);
		}
	}

void CDjvuReader::PreviousPageWithoutBitmapCopyL()
	{
		if (iAbstractReader->iCurrentPage > 0)
		{
			iAbstractReader->iCurrentPage--;
			iAbstractReader->RenderPageWithoutBitmapCopyL(iAbstractReader->iCurrentPage);
		}
	}

void CDjvuReader::OpenL(const TDesC& aFileName)
	{
		if(iAbstractReader)
		{
			delete (PdfReader*)iAbstractReader; 
		}
		
	    TParse parse;
	    parse.Set(aFileName, NULL, NULL);
	    TPtrC ext = parse.Ext(); // extract the extension from the filename

	    if (ext.CompareF(KExtensionDjvu)==0)
		{
			iAbstractReader = (AbstractReader*)new DjvuReader();
			iAbstractReader->iBitmap = iBitmap;
			iAbstractReader->OpenL(aFileName);
		}
	    
	    if (ext.CompareF(KExtensionPdf)==0)
		{
			iAbstractReader = (AbstractReader*)new PdfReader();
			iAbstractReader->iBitmap = iBitmap;
			iAbstractReader->OpenL(aFileName);
		}	    
		
	}
