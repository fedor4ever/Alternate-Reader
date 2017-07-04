/*
 *  Copyright: BSD license
 * ============================================================================
 */

#ifndef ALTERNATERECOG_H
#define ALTERNATERECOG_H

// INCLUDES
#include <apmrec.h>

// CONSTANTS
const TInt KRecognizerExImplementationUid = 0x101FF1ed;

class CAlternateRecog : public CApaDataRecognizerType
    {
public:
    // constructor and destructor
    CAlternateRecog();
    virtual ~CAlternateRecog();

    // creates and returns recognizer
    static CApaDataRecognizerType* CreateRecognizerL();

    // returns preferred buffer size
    TUint PreferredBufSize();

    // returns supported mime type
    TDataType SupportedDataTypeL(TInt aIndex) const;

private:
    // recognises the file by name and/or buffer
    void DoRecognizeL(const TDesC& aName, const TDesC8& aBuffer);
    bool CheckForDjvu(const TDesC8 & aBuffer);
    bool CheckForPdf(const TDesC8 & aBuffer);
    };

#endif //ALTERNATERECOG_H
