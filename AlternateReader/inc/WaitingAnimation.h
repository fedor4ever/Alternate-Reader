/*
 ============================================================================
 Name		: WaitingAnimation.h
 Author	  : 
 Version	 : 1.0
 Copyright   : Vasily Kharitonov
 Description : CWaitingAnimation declaration
 ============================================================================
 */

#ifndef WAITINGANIMATION_H
#define WAITINGANIMATION_H

#include <e32base.h>	// For CActive, link against: euser.lib
#include <e32std.h>		// For RTimer, link against: euser.lib

#include "SymDjvuContainer.h"

class CWaitingAnimation : public CActive
	{
public:
	// Cancel and destroy
	~CWaitingAnimation();

	// Two-phased constructor.
	static CWaitingAnimation* NewL();

	// Two-phased constructor.
	static CWaitingAnimation* NewLC();

public:
	// New functions
	// Function for making the initial request
	void StartL(TTimeIntervalMicroSeconds32 aDelay);
	void StopL(TTimeIntervalMicroSeconds32 aDelay);

	// How to cancel me
	void DoCancel();
	
	CSymDjvuContainer* iContainer;
	
private:
	// C++ constructor
	CWaitingAnimation();

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
	enum TWaitingAnimationState
		{
			EStarted, // Uninitialized
			EStopped, // Initalized
			EError
			// Error condition
		};

private:
	
	TInt iState; // State of the active object
	RTimer iTimer; // Provides async timing service
	
	};

#endif // WAITINGANIMATION_H
