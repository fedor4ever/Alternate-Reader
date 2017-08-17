
#include "CDjvuReader.h"

_LIT(KExtensionDjvu, ".djvu");
_LIT(KExtensionDjv, ".djv");
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
		iBitmap = new (ELeave) CFbsBitmap();
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

void CDjvuReader::RenderPageWithoutBitmapCopyL(TInt aPage)
	{
		iAbstractReader->RenderPageWithoutBitmapCopyL(aPage);
	}

void CDjvuReader::NextPageWithoutBitmapCopyL()
	{
		if (iAbstractReader->GetCurrentPage() < iAbstractReader->LastPageNumber())
		{
			iAbstractReader->iCurrentPage++;
			iAbstractReader->RenderPageWithoutBitmapCopyL(iAbstractReader->iCurrentPage);
		}
	}

void CDjvuReader::PreviousPageWithoutBitmapCopyL()
	{
		if (iAbstractReader->iCurrentPage > iAbstractReader->FirstPageNumber())
		{
			iAbstractReader->iCurrentPage--;
			iAbstractReader->RenderPageWithoutBitmapCopyL(iAbstractReader->iCurrentPage);
		}
	}

TBool CDjvuReader::OpenL(const TDesC& aFileName)
	{
		if(iAbstractReader)
		{
			delete iAbstractReader; 
		}
		
	    TParse parse;
	    parse.Set(aFileName, NULL, NULL);
	    TPtrC ext = parse.Ext(); // extract the extension from the filename

	    if (ext.CompareF(KExtensionDjvu)==0)
		{
			iAbstractReader = (AbstractReader*)new DjvuReader();
			iAbstractReader->iBitmap = iBitmap;
			iAbstractReader->OpenL(aFileName);
			return ETrue;
		}
	    else if (ext.CompareF(KExtensionDjv)==0)
		{
			iAbstractReader = (AbstractReader*)new DjvuReader();
			iAbstractReader->iBitmap = iBitmap;
			iAbstractReader->OpenL(aFileName);
			return ETrue;
		}
	    else if (ext.CompareF(KExtensionPdf)==0)
		{
			iAbstractReader = (AbstractReader*)new PdfReader();
			iAbstractReader->iBitmap = iBitmap;
			iAbstractReader->OpenL(aFileName);
			return ETrue;
		}	    
		
	    return EFalse;
	    
	}
