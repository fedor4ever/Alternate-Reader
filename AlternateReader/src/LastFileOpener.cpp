
#include "LastFileOpener.h"

CLastFileOpener::CLastFileOpener() :
	CActive(EPriorityStandard) // Standard priority
	{
	}

CLastFileOpener* CLastFileOpener::NewLC()
	{
	CLastFileOpener* self = new (ELeave) CLastFileOpener();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CLastFileOpener* CLastFileOpener::NewL()
	{
	CLastFileOpener* self = CLastFileOpener::NewLC();
	CleanupStack::Pop(); // self;
	return self;
	}

void CLastFileOpener::ConstructL()
	{
	User::LeaveIfError(iTimer.CreateLocal()); // Initialize timer
	CActiveScheduler::Add(this); // Add to scheduler
	}

CLastFileOpener::~CLastFileOpener()
	{
	Cancel(); // Cancel any request, if outstanding
	iTimer.Close(); // Destroy the RTimer object
	// Delete instance variables if any
	}

void CLastFileOpener::DoCancel()
	{
	iTimer.Cancel();
	}

void CLastFileOpener::StartL(TTimeIntervalMicroSeconds32 aDelay)
	{
	Cancel(); // Cancel any request, just to be sure
	iState = EUninitialized;
	iTimer.After(iStatus, aDelay); // Set for later
	SetActive(); // Tell scheduler a request is active
	}

void CLastFileOpener::RunL()
	{
		if (iState == EUninitialized)
		{
			if(!iDjVuReader->IsOpen())
			{
				TFileName LastOpenFile;
				if(CBookSettings::LoadLastOpenFile(LastOpenFile))
				{
					iRenderThreadManager->iBookSettings->iBookNameCache = LastOpenFile;
					iDjVuReader->OpenL(LastOpenFile);
					iRenderThreadManager->iBookSettings->LoadSettings(LastOpenFile);			
					iRenderThreadManager->RenderOpenFile();
				}
			}
	
			iState = EInitialized;
		}
		else if (iState != EError)
		{
		}
		//iTimer.After(iStatus, 1000000); // Set for 1 sec later
		//SetActive(); // Tell scheduler a request is active
	}

TInt CLastFileOpener::RunError(TInt aError)
	{
	return aError;
	}
