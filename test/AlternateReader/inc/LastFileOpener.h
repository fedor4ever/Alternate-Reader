
#ifndef LASTFILEOPENER_H
#define LASTFILEOPENER_H

#include <e32base.h>	// For CActive, link against: euser.lib
#include <e32std.h>		// For RTimer, link against: euser.lib

#include "CDjvuReader.h"
#include "RenderThreadManager.h"
#include "BookSettings.h"


class CLastFileOpener : public CActive
	{
public:
	// Cancel and destroy
	~CLastFileOpener();

	// Two-phased constructor.
	static CLastFileOpener* NewL();

	// Two-phased constructor.
	static CLastFileOpener* NewLC();

public:
	// New functions
	// Function for making the initial request
	void StartL(TTimeIntervalMicroSeconds32 aDelay);

	// How to cancel me
	void DoCancel();	
	
private:
	// C++ constructor
	CLastFileOpener();

	// Second-phase constructor
	void ConstructL();

private:
	// From CActive
	// Handle completion
	void RunL();

	// Override to handle leaves from RunL(). Default implementation causes
	// the active scheduler to panic.
	TInt RunError(TInt aError);

private:
	enum TLastFileOpenerState
		{
		EUninitialized, // Uninitialized
		EInitialized, // Initalized
		EError
		// Error condition
		};

private:
	TInt iState; // State of the active object
	RTimer iTimer; // Provides async timing service
	
public:
	
	CDjvuReader* iDjVuReader;
	CRenderThreadManager* iRenderThreadManager;
	
	};

#endif // LASTFILEOPENER_H
