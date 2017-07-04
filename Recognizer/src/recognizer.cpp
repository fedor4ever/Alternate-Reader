
// INCLUDE FILES
#include <apmrec.h>
#include <apmstd.h>
#include <ImplementationProxy.h>
#include "recognizer.h"
#include <f32file.h>

// CONSTANTS
const TUid KUidRecognizerEx = {0x101FF1ec};
const TInt KMaxBufferLength = 8;                  // maximum amount of buffer space we will ever use

_LIT(KExtensionDjvu, ".djvu");
_LIT(KExtensionDjv, ".djv");
_LIT(KExtensionPdf, ".pdf");

_LIT8(KDataTypeDjvu, "AT&TFORM");
_LIT8(KDataTypePdf, "%PDF");

_LIT8(KMimeTypeDjvu, "image/vnd.djvu");
_LIT8(KMimeTypePdf, "application/pdf");
_LIT8(KMimeTypeXPdf, "application/x-pdf");
_LIT8(KMimeTypePdfPlain, "pdf/plain");

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// RecognizerEx::RecognizerEx()
// constructs the object
// ---------------------------------------------------------
//
CAlternateRecog::CAlternateRecog()
    : CApaDataRecognizerType(KUidRecognizerEx, CApaDataRecognizerType::ENormal)
{
    iCountDataTypes = 4;
}

// ---------------------------------------------------------
// RecognizerEx::~RecognizerEx()
// Destroys the object
// ---------------------------------------------------------
//
CAlternateRecog::~CAlternateRecog()
{
// Do nothing
}

// ---------------------------------------------------------
// RecognizerEx::PreferredBufSize()
// Returns preferred buffer size
// ---------------------------------------------------------
//
TUint CAlternateRecog::PreferredBufSize()
{
    return KMaxBufferLength;
}

// ---------------------------------------------------------
// RecognizerEx::SupportedDataTypeL()
// Returns supported mime type
// ---------------------------------------------------------
//
TDataType CAlternateRecog::SupportedDataTypeL(TInt aIndex) const
{
    switch (aIndex)
    {
        case 1:
            return TDataType(KMimeTypeDjvu);
            break;
        case 2:
            return TDataType(KMimeTypePdf);
            break;
        case 3:
            return TDataType(KMimeTypeXPdf);
            break;
        case 4:
            return TDataType(KMimeTypePdfPlain);
            break;
        default:
            User::Leave(KErrArgument);
            break;
    }
    return TDataType();
}

bool CAlternateRecog::CheckForDjvu(const TDesC8 & aBuffer)
    {
    if(aBuffer.Size()>KMaxBufferLength)
        {
        return false;
        }
    if(aBuffer.FindF(KDataTypeDjvu)!= KErrNotFound)
        {
        return true;
        }
    return false;
    }

bool CAlternateRecog::CheckForPdf(const TDesC8 & aBuffer)
    {
    if(aBuffer.Size()>KMaxBufferLength)
        {
        return false;
        }
    if(aBuffer.FindF(KDataTypePdf)!= KErrNotFound)
        {
        return true;
        }
    return false;
    }

// ---------------------------------------------------------
// RecognizerEx::DoRecognizeL()
// Recognizes the file by name and/or head buffer
// ---------------------------------------------------------
//
void CAlternateRecog::DoRecognizeL(const TDesC& /*aName */, const TDesC8& aBuffer)
{
    if(CheckForDjvu( aBuffer ))
    {
        iConfidence = ECertain;
        iDataType = TDataType(KMimeTypeDjvu);
    }
    if(CheckForPdf( aBuffer ))
    {
        iConfidence = ECertain;
        iDataType = TDataType(KMimeTypePdf);
    }
}


// ================= OTHER EXPORTED FUNCTIONS ==============

// ---------------------------------------------------------
// RecognizerEx::CreateRecognizerL()
// Returns pointer to the new object
// ---------------------------------------------------------
//
CApaDataRecognizerType* CAlternateRecog::CreateRecognizerL()
{
    return new (ELeave) CAlternateRecog();
}

const TImplementationProxy ImplementationTable[] =
{
    IMPLEMENTATION_PROXY_ENTRY(KRecognizerExImplementationUid, CAlternateRecog::CreateRecognizerL)
};

// ---------------------------------------------------------
// ImplementationGroupProxy
// Provides access to the implementationtable
// ---------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
{
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
}

