
#include "RenderThreadManager.h"
#include "CDjvuReader.h"

const TInt KStackSize = 0x10000;

CRenderThreadManager::CRenderThreadManager() :
	CActive(EPriorityStandard), // Standard priority
	iThreadStarted(EFalse)
	{
	}

CRenderThreadManager* CRenderThreadManager::NewLC()
	{
		CRenderThreadManager* self = new (ELeave) CRenderThreadManager();
		CleanupStack::PushL(self);
		self->ConstructL();
		return self;
	}

CRenderThreadManager* CRenderThreadManager::NewL()
	{
		CRenderThreadManager* self = CRenderThreadManager::NewLC();
		CleanupStack::Pop(); // self;
		return self;
	}

void CRenderThreadManager::ConstructL()
	{
		iAnimation = CWaitingAnimation::NewL();
		iBookSettings = CBookSettings::NewL();
		CActiveScheduler::Add(this); // Add to scheduler
	}

CRenderThreadManager::~CRenderThreadManager()
	{
		if(iAnimation->IsActive()) iAnimation->DoCancel();
		Cancel(); // Cancel any request, if outstanding
		iThread.Close();
		
		delete iAnimation;
		iAnimation = NULL;
		
		delete iBookSettings;
		iBookSettings = NULL;
	}

TInt ThreadRenderNextPage(TAny* aParams)
	{
		CRenderThreadManager* RenderThreadManager = static_cast<CRenderThreadManager*>(aParams);
		RenderThreadManager->iContainer->iDjVuReader->NextPageWithoutBitmapCopyL();
		return KErrNone;
	}
TInt ThreadRenderPreviousPage(TAny* aParams)
	{
		CRenderThreadManager* RenderThreadManager = static_cast<CRenderThreadManager*>(aParams);
		RenderThreadManager->iContainer->iDjVuReader->PreviousPageWithoutBitmapCopyL();
		return KErrNone;
	}
TInt ThreadRenderIncrementZoom(TAny* aParams)
	{
		CRenderThreadManager* RenderThreadManager = static_cast<CRenderThreadManager*>(aParams);
		RenderThreadManager->iContainer->iDjVuReader->IncrementZoomWithoutBitmapCopyL();
		return KErrNone;
	}
TInt ThreadRenderDecrementZoom(TAny* aParams)
	{
		CRenderThreadManager* RenderThreadManager = static_cast<CRenderThreadManager*>(aParams);
		RenderThreadManager->iContainer->iDjVuReader->DecrementZoomWithoutBitmapCopyL();
		return KErrNone;
	}
TInt ThreadRenderZoomWidth(TAny* aParams)
	{
		CRenderThreadManager* RenderThreadManager = static_cast<CRenderThreadManager*>(aParams);
		
		RenderThreadManager->iContainer->iDjVuReader->SetDPIForWidth(RenderThreadManager->iContainer->Rect().Width());
		RenderThreadManager->iContainer->iDjVuReader->SetZoom(1.00);		
		RenderThreadManager->iContainer->iDjVuReader->SetPageWidth(RenderThreadManager->iContainer->Rect().Width());
		
		RenderThreadManager->iContainer->iDjVuReader->RenderPageWithoutBitmapCopyL(RenderThreadManager->iContainer->iDjVuReader->CurrentPage());
		return KErrNone;
	}
TInt ThreadRenderFitActualSize(TAny* aParams)
	{
		CRenderThreadManager* RenderThreadManager = static_cast<CRenderThreadManager*>(aParams);
		
		TReal x1 = RenderThreadManager->iContainer->iDjVuReader->GetImageLeftMargin();
		TReal x2 = RenderThreadManager->iContainer->iDjVuReader->GetImageRightMargin();
		TReal l  = RenderThreadManager->iContainer->iDjVuReader->GetPageWidth() - x1 - x2; 
		
		TReal ww = RenderThreadManager->iContainer->Rect().Width();
		
		TReal w = (ww/l)*(l+x1+x2);

		RenderThreadManager->iContainer->iDjVuReader->SetDPIForWidth((TInt)w);
		RenderThreadManager->iContainer->iDjVuReader->SetZoom(1.00);
		RenderThreadManager->iContainer->iDjVuReader->SetPageWidth((TInt)w);

		RenderThreadManager->iContainer->iDjVuReader->RenderPageWithoutBitmapCopyL(RenderThreadManager->iContainer->iDjVuReader->CurrentPage());
		return KErrNone;
	}
TInt ThreadRenderCurrentPage(TAny* aParams)
	{
		CRenderThreadManager* RenderThreadManager = static_cast<CRenderThreadManager*>(aParams);
		RenderThreadManager->iContainer->iDjVuReader->RenderPageWithoutBitmapCopyL(RenderThreadManager->iContainer->iDjVuReader->CurrentPage());
		return KErrNone;
	}

void CRenderThreadManager::RenderNextPage()
	{
		if(!iThreadStarted)
		{
			
			iRendering = ENextPage;
		
			TInt result = iThread.Create(
				_L("SymThr"),
				ThreadRenderNextPage,
				KStackSize,
				NULL, // Using heap of this process
				this
			);
			
			if(result == KErrNone)
			{
				StartL();
			}
		}
	}

void CRenderThreadManager::RenderCurrentPage()
	{
		if(!iThreadStarted)
		{
			
			iRendering = ERenderCurrentPage;
		
			TInt result = iThread.Create(
				_L("SymThr"),
				ThreadRenderCurrentPage,
				KStackSize,
				NULL, // Using heap of this process
				this
			);
			
			if(result == KErrNone)
			{
				StartL();
			}
		}
	}

void CRenderThreadManager::RenderOpenFile()
	{
		if(!iThreadStarted)
		{
			
			iRendering = ERenderOpenFile;
		
			TInt result = iThread.Create(
				_L("SymThr"),
				ThreadRenderCurrentPage,
				KStackSize,
				NULL, // Using heap of this process
				this
			);
			
			if(result == KErrNone)
			{
				StartL();
			}
		}
	}

void CRenderThreadManager::RenderPreviousPage()
	{
		if(!iThreadStarted)
		{
			iRendering = EPreviousPage;
			
			TInt result = iThread.Create(
				_L("SymThr"),
				ThreadRenderPreviousPage,
				KStackSize,
				NULL, // Using heap of this process
				this
			);
			
			if(result == KErrNone)
			{
				StartL();
			}
		}
	}

void CRenderThreadManager::RenderIncrementZoom()
	{
		if(!iThreadStarted)
		{
			iRendering = EIncrementZoom;
			
			TInt result = iThread.Create(
				_L("SymThr"),
				ThreadRenderIncrementZoom,
				KStackSize,
				NULL, // Using heap of this process
				this
			);
			
			if(result == KErrNone)
			{
				StartL();
			}
		}
	}

void CRenderThreadManager::RenderDecrementZoom()
	{
		if(!iThreadStarted)
		{
			iRendering = EDecrementZoom;
			
			TInt result = iThread.Create(
				_L("SymThr"),
				ThreadRenderDecrementZoom,
				KStackSize,
				NULL, // Using heap of this process
				this
			);
			
			if(result == KErrNone)
			{
				StartL();
			}
		}
	}

void CRenderThreadManager::RenderZoomWidth()
	{
		if(!iThreadStarted)
		{
			iRendering = EZoomWidth;
			
			TInt result = iThread.Create(
				_L("DjvuRnd"),
				ThreadRenderZoomWidth,
				KStackSize,
				NULL, // Using heap of this process
				this
			);
			
			if(result == KErrNone)
			{
				StartL();
			}
		}
	}

void CRenderThreadManager::RenderFitActualSize()
	{
		if(!iThreadStarted)
		{
			iRendering = EFitActualSize;
			
			TInt result = iThread.Create(
				_L("DjvuRnd"),
				ThreadRenderFitActualSize,
				KStackSize,
				NULL, // Using heap of this process
				this
			);
			
			if(result == KErrNone)
			{
				StartL();
			}
		}
	}

void CRenderThreadManager::DoCancel()
	{
		iThread.LogonCancel(iStatus);
	}

void CRenderThreadManager::StartL()
	{
		iThreadStarted = ETrue;
		iThread.SetPriority(EPriorityLess);
		iThread.Resume();
	
		Cancel(); // Cancel any request, just to be sure
		iThread.Logon(iStatus);
		iAnimation->StartL(100);
		SetActive(); // Tell scheduler a request is active
	}

void CRenderThreadManager::RunL()
	{
		if(iStatus == KErrNone)	
		{
			iContainer->iDjVuReader->BitmapCopyL();
			switch (iRendering)
			{
				case ENextPage:
					iContainer->iCursorPosition.iY = 0;
					iContainer->CursorPositionCorrection();
				break;
				
				case EPreviousPage:
					iContainer->iCursorPosition.iY = (iContainer->Rect().Height() - iContainer->iDjVuReader->GetBitmap()->SizeInPixels().iHeight - iContainer->iMargin);
					iContainer->CursorPositionCorrection();
				break;

				case ERenderCurrentPage:
					iContainer->CursorPositionCorrection();
				break;
				
				case ERenderOpenFile:
					iBookSettings->ApplyCurrentSettings();
					CBookSettings::SaveLastOpenFile(iBookSettings->iBookNameCache);
					iContainer->CursorPositionCorrection();
				break;
				
				case EIncrementZoom:
					iContainer->CursorPositionCorrection();
				break;
				
				case EDecrementZoom:
					iContainer->CursorPositionCorrection();
				break;
				
				case EZoomWidth:
					iContainer->CursorPositionCorrection();
				break;
				
				case EFitActualSize:
					iContainer->CursorPositionCorrection();
					iContainer->iCursorPosition.iX = -iContainer->iDjVuReader->GetImageLeftMargin(); 
				break;
				
				default:
				
				break;
			}
			
			iThread.Close();
			iThreadStarted = EFalse;
			
			iContainer->DrawNow();
			iAnimation->StopL(1000);
			
		}
	}

TInt CRenderThreadManager::RunError(TInt aError)
	{
		if(iAnimation->IsActive()) iAnimation->DoCancel();
		return aError;
	}
