
#include "WaitingAnimation.h"

CWaitingAnimation::CWaitingAnimation() :
	CActive(EPriorityStandard) // Standard priority
	{
	}

CWaitingAnimation* CWaitingAnimation::NewLC()
	{
		CWaitingAnimation* self = new (ELeave) CWaitingAnimation();
		CleanupStack::PushL(self);
		self->ConstructL();
		return self;
	}

CWaitingAnimation* CWaitingAnimation::NewL()
	{
		CWaitingAnimation* self = CWaitingAnimation::NewLC();
		CleanupStack::Pop(); // self;
		return self;
	}

void CWaitingAnimation::ConstructL()
	{
		User::LeaveIfError(iTimer.CreateLocal()); // Initialize timer
		CActiveScheduler::Add(this); // Add to scheduler
	}

CWaitingAnimation::~CWaitingAnimation()
	{
		Cancel(); // Cancel any request, if outstanding
		iTimer.Close(); // Destroy the RTimer object
		// Delete instance variables if any
	}

void CWaitingAnimation::DoCancel()
	{
		iTimer.Cancel();
		
		if(!iContainer->iGif->IsStart())
		{
			iContainer->iGif->Stop();
			iContainer->DrawNow();
		}
		
		iStatus = KErrAbort;
		
	}

void CWaitingAnimation::StartL(TTimeIntervalMicroSeconds32 aDelay)
	{
		Cancel(); // Cancel any request, just to be sure
		iState = EStarted;
		iTimer.After(iStatus, aDelay); // Set for later
		SetActive(); // Tell scheduler a request is active
	}

void CWaitingAnimation::StopL(TTimeIntervalMicroSeconds32 aDelay)
	{
		Cancel(); // Cancel any request, just to be sure
		iState = EStopped;
		iTimer.After(iStatus, aDelay); // Set for later
		SetActive(); // Tell scheduler a request is active
	}

void CWaitingAnimation::RunL()
	{
		switch (iState)
		{
			case EStarted:
			
				if(!iContainer->iGif->IsStart())
				{
					iContainer->iGif->StartL();
					iContainer->DrawNow();
				}
				
			break;
			case EStopped:
			
				if(iContainer->iGif->IsStart())
				{
					iContainer->iGif->Stop();
					iContainer->DrawNow();
				}
				
			break;
			default:
			break;
		}
	}

TInt CWaitingAnimation::RunError(TInt aError)
	{
		return aError;
	}
