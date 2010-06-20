
#ifndef BOOKSETTINGS_H_
#define BOOKSETTINGS_H_

#include <e32def.h>
#include <e32cmn.h>
#include <e32base.h>
#include <s32strm.h>
#include "SymDjvuContainer.h"

#include <S32FILE.H>
//#include <s32strm.h>

class CBookSettings : CBase
	{
public:
	
	~CBookSettings();
	
	// Two-phased constructor.
	static CBookSettings* NewL();

	// Two-phased constructor.
	static CBookSettings* NewLC();
	
	void ExternalizeL(RWriteStream& aStream) const; 
	void InternalizeL(RReadStream& aStream);
	
	void SaveSettings();
	void LoadSettings(TFileName fileName);
	void ApplyCurrentSettings();
	
	static void SaveLastOpenFile(TFileName aFileName);
	static TBool LoadLastDirectory(TFileName& aLastDirectory);
	static TBool LoadLastOpenFile(TFileName& aLastFile);
	
	CSymDjvuContainer* iContainer;
	TFileName iBookNameCache;
	
private:
	
	TFileName iBookName;
	
	// CDjvuReader settings 
	TInt32 iCurrentPage;
	TInt32 iPageWidth;
	TReal  iZoom;
	TReal  iDPI;

	// CSymDjvuContainer settings 
	TInt32  iCursorPositionX;
	TInt32  iCursorPositionY;
	TReal   iFullScreenMode;
	
private:

	// Second-phase constructor
	void ConstructL();	
	
	};

#endif /* BOOKSETTINGS_H_ */
