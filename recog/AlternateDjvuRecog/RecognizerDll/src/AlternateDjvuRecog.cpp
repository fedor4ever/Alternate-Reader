
// INCLUDE FILES
#include <apmrec.h>
#include <apmstd.h>
#include <ImplementationProxy.h>
#include "AlternateDjvuRecog.h"
#include <f32file.h>

// CONSTANTS
const TUid KUidRecognizerEx = {0x101FF1ec};
const TInt KMaxBufferLength = 4;                  // maximum amount of buffer space we will ever use

_LIT(KExtension1, ".djvu");

_LIT8(KDataType, "image/vnd.djvu");

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// RecognizerEx::RecognizerEx()
// constructs the object
// ---------------------------------------------------------
//
CApaRecognizerEx::CApaRecognizerEx()
    : CApaDataRecognizerType(KUidRecognizerEx, CApaDataRecognizerType::ENormal)
    {
		iCountDataTypes = 1;
    }

// ---------------------------------------------------------
// RecognizerEx::~RecognizerEx()
// Destroys the object
// ---------------------------------------------------------
//
CApaRecognizerEx::~CApaRecognizerEx()
    {
    // Do nothing
    }

// ---------------------------------------------------------
// RecognizerEx::PreferredBufSize()
// Returns preferred buffer size
// ---------------------------------------------------------
//
TUint CApaRecognizerEx::PreferredBufSize()
    {
		return KMaxBufferLength;
    }

// ---------------------------------------------------------
// RecognizerEx::SupportedDataTypeL()
// Returns supported mime type
// ---------------------------------------------------------
//
TDataType CApaRecognizerEx::SupportedDataTypeL(TInt /*aIndex*/) const
    {
		return TDataType(KDataType);
    }

// ---------------------------------------------------------
// RecognizerEx::DoRecognizeL()
// Recognizes the file by name and/or head buffer
// ---------------------------------------------------------
//
void CApaRecognizerEx::DoRecognizeL(const TDesC& aName, const TDesC8& /*aBuffer*/)
    {
		
	    TParse parse;
	    parse.Set(aName, NULL, NULL);
	    TPtrC ext = parse.Ext(); // extract the extension from the filename

	    if (ext.CompareF(KExtension1)==0)
		{
			iConfidence = ECertain;
			iDataType = TDataType(KDataType); 
		}
 
    }



// ================= OTHER EXPORTED FUNCTIONS ==============

// ---------------------------------------------------------
// RecognizerEx::CreateRecognizerL()
// Returns pointer to the new object
// ---------------------------------------------------------
//
CApaDataRecognizerType* CApaRecognizerEx::CreateRecognizerL()
    {
		return new (ELeave) CApaRecognizerEx();
    }

const TImplementationProxy ImplementationTable[] =
    {
    	IMPLEMENTATION_PROXY_ENTRY(KRecognizerExImplementationUid, CApaRecognizerEx::CreateRecognizerL)
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

