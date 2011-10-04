
#ifndef CANIMMANAGER_H
#define CANIMMANAGER_H

#include <e32base.h>	// For CActive, link against: euser.lib
#include <e32std.h>		// For RTimer, link against: euser.lib

#include "SymDjvuContainer.h"
#include "WaitingAnimation.h"
#include "BookSettings.h"

class CRenderThreadManager : public CActive
	{
public:
	// Cancel and destroy
	~CRenderThreadManager();

	// Two-phased constructor.
	static CRenderThreadManager* NewL();

	// Two-phased constructor.
	static CRenderThreadManager* NewLC();

public:
	// New functions
	// Function for making the initial request
	void StartL();
	
	void RenderNextPage();
	void RenderPreviousPage();
	void RenderCurrentPage();
	void RenderOpenFile();
	void RenderIncrementZoom();
	void RenderDecrementZoom();
	void RenderZoomWidth();
	void RenderFitActualSize();
	void RenderSetZoom(const TReal aZoom);
	
	RThread iThread;
	TBool iThreadStarted;
	CSymDjvuContainer* iContainer;
	CWaitingAnimation* iAnimation;
	CBookSettings* iBookSettings;
	TReal iSetZoom;
	
private:
	// C++ constructor
	CRenderThreadManager();

	// Second-phase constructor
	void ConstructL();

private:
	// From CActive
	// Handle completion
	void RunL();

	// How to cancel me
	void DoCancel();

	// Override to handle leaves from RunL(). Default implementation causes
	// the active scheduler to panic.
	TInt RunError(TInt aError);

private:
	enum TCAnimManagerState
		{
			EUninitialized, // Uninitialized
			EInitialized, // Initalized
			EError
			// Error condition
		};
	
	enum TRendering
		{
			EOpenFile = 0,
			EGoToPage,
			ENextPage,
			ERenderCurrentPage,
			ERenderOpenFile,
			EPreviousPage,
			EIncrementZoom,
			EDecrementZoom,
			EZoomWidth,
			EFitActualSize,
			ESetZoom
		};

private:
	TInt iState; // State of the active object
	TInt iRendering;
	};

#endif // CANIMMANAGER_H
