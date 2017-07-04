/*
* ============================================================================
*  Name     : CApaSampleRecognizer from RecognizerEx.h
*  Part of  : RecognizerEx
*  Created  : 27/06/2006 by Forum Nokia
*  Version  : 2.0
*  Copyright: Nokia Corporation
* ============================================================================
*/

#ifndef SAMPLERECOG_H
#define SAMPLERECOG_H

// INCLUDES
#include <apmrec.h>

// CONSTANTS
const TInt KRecognizerExImplementationUid = 0x101FF1ed;

/**
*  CApaSampleRecognizer.
*  Recognizer plug-in for .djvu.
*/
class CApaRecognizerEx: public CApaDataRecognizerType
    {
    public:
        // constructor and destructor
        CApaRecognizerEx();
        virtual ~CApaRecognizerEx();

        // creates and returns recognizer
        static CApaDataRecognizerType* CreateRecognizerL();

        // returns preferred buffer size
        TUint PreferredBufSize();

        // returns supported mime type
        TDataType SupportedDataTypeL(TInt aIndex) const;

    private:
        // recognises the file by name and/or buffer
        void DoRecognizeL(const TDesC& aName, const TDesC8& aBuffer);
    };

#endif //SAMPLERECOG_H
