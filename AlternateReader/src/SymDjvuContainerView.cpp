
#include <aknmessagequerydialog.h>
#include <aknviewappui.h>
#include <eikmenub.h>
#include <avkon.hrh>
#include <akncontext.h>
#include <akntitle.h>
#include <stringloader.h>
#include <barsread.h>
#include <eikbtgpc.h>
#include <gdi.h>
#include <eikedwin.h>
#include <eikenv.h>
#include <SymDjvu.rsg>
#include <CAknFileSelectionDialog.h> 
#include <CAknMemorySelectionDialog.h>
#include <AknQueryDialog.h> 

#include "Config.h"
#ifdef _TOUCH_SUPPORT_ 
	#include <caknmemoryselectiondialogmultidrive.h>
#endif 

#include "SymDjvu.hrh"
#include "SymDjvuContainerView.h"
#include "SymDjvuContainer.hrh"
#include "LastFileOpener.h"
#include "CDjvuReader.h"
#include "RenderThreadManager.h"
#include "SymDjvuContainer.h"

#include <numberconversion.h>
#include <e32err.h>



/**
 * First phase of Symbian two-phase construction. Should not contain any
 * code that could leave.
 */
CSymDjvuContainerView::CSymDjvuContainerView()
	{
	}

/** 
 * The view's destructor removes the container from the control
 * stack and destroys it.
 */
CSymDjvuContainerView::~CSymDjvuContainerView()
	{
		delete iRenderThreadManager;
		iRenderThreadManager = NULL;
		
		delete iDjVuReader;
		iDjVuReader = NULL;
		
		delete iLastFileOpener;
		iLastFileOpener = NULL;
	}

/**
 * Symbian two-phase constructor.
 * This creates an instance then calls the second-phase constructor
 * without leaving the instance on the cleanup stack.
 * @return new instance of CSymDjvuContainerView
 */
CSymDjvuContainerView* CSymDjvuContainerView::NewL()
	{
		CSymDjvuContainerView* self = CSymDjvuContainerView::NewLC();
		CleanupStack::Pop( self );
		return self;
	}

/**
 * Symbian two-phase constructor.
 * This creates an instance, pushes it on the cleanup stack,
 * then calls the second-phase constructor.
 * @return new instance of CSymDjvuContainerView
 */
CSymDjvuContainerView* CSymDjvuContainerView::NewLC()
	{
		CSymDjvuContainerView* self = new ( ELeave ) CSymDjvuContainerView();
		CleanupStack::PushL( self );
		self->ConstructL();
		return self;
	}

/**
 * Second-phase constructor for view.  
 * Initialize contents from resource.
 */ 
void CSymDjvuContainerView::ConstructL()
	{
		BaseConstructL( R_SYM_DJVU_CONTAINER_SYM_DJVU_CONTAINER_VIEW );
		iDjVuReader = CDjvuReader::NewL();
	}

/**
 * @return The UID for this view
 */
TUid CSymDjvuContainerView::Id() const
	{
		return TUid::Uid( ESymDjvuContainerViewId );
	}

/**
 * Handle a command for this view (override)
 * @param aCommand command id to be handled
 */
void CSymDjvuContainerView::HandleCommandL( TInt aCommand )
	{

		TBool commandHandled = EFalse;
		switch ( aCommand )
			{
			
			case EOpenFile:
				commandHandled = HandleOpenFileMenuItemSelectedL();
				break;
			case EGoToPage:
				commandHandled = HandleGoToPageItemSelectedL();
				break;		
			case EZoomIn:
				commandHandled = HandleZoomInItemSelectedL();
				break;		
			case EZoomOut:
				commandHandled = HandleZoomOutItemSelectedL();
				break;		
			case EZoomWidth:
				commandHandled = HandleZoomWidthItemSelectedL();
				break;		
			case EFitActualSize:
				commandHandled = HandleFitActualSizeItemSelectedL();
				break;	
			case EExitFullscreen: // FALLTHRU
			case EFullscreen:
				commandHandled = HandleSetFullScreenModeL();
				break;		
			case EButtonDown:
				commandHandled = HandleDownButtonPressedL();
				break;		
			case EButtonUp:
				commandHandled = HandleUpButtonPressedL();
				break;		
			case EExit:
				commandHandled = HandleExitItemSelectedL();
				break;		
			case EAbout:
				commandHandled = HandleAboutItemSelectedL();
				break;
			case EFixVertically:
				if (iRenderThreadManager->iContainer)
					iRenderThreadManager->iContainer->FixVertically();
				break;
			default:
				break;
			}
			
			if ( !commandHandled ) 
			{
				if ( aCommand == EAknSoftkeyExit )
				{
					HandleExitItemSelectedL();
				}
			}
	
	}

/**
 *	Handles user actions during activation of the view, 
 *	such as initializing the content.
 */
void CSymDjvuContainerView::DoActivateL( 
		const TVwsViewId& /*aPrevViewId*/,
		TUid /*aCustomMessageId*/,
		const TDesC8& /*aCustomMessage*/ )
	{
		SetupStatusPaneL();
		
		if (!iRenderThreadManager)
		{
			CSymDjvuContainer* mSymDjvuContainer =
					CSymDjvuContainer::NewL( ClientRect(), NULL, this, iDjVuReader);
			mSymDjvuContainer->SetMopParent( this );
			AppUi()->AddToStackL( *this, mSymDjvuContainer );
			
			iRenderThreadManager = CRenderThreadManager::NewL();
			iRenderThreadManager->iContainer = mSymDjvuContainer;
			iRenderThreadManager->iAnimation->iContainer = mSymDjvuContainer;
			iRenderThreadManager->iBookSettings->iContainer = mSymDjvuContainer;
			
			iLastFileOpener = CLastFileOpener::NewL();
			iLastFileOpener->iDjVuReader = iDjVuReader;
			iLastFileOpener->iRenderThreadManager = iRenderThreadManager;
			iLastFileOpener->StartL(1000000);
			
		} else {
//			AppUi()->AddToStackL( *this, iRenderThreadManager->iContainer );
//			iRenderThreadManager->iContainer->SetRect(ClientRect());
		}

	}

/**
 */
void CSymDjvuContainerView::DoDeactivate()
	{
		
		if ( iRenderThreadManager->iContainer != NULL )
		{
			AppUi()->RemoveFromViewStack( *this, iRenderThreadManager->iContainer );
		}
	
	}

/** 
 * Handle status pane size change for this view (override)
 */
void CSymDjvuContainerView::HandleStatusPaneSizeChange()
	{
		CAknView::HandleStatusPaneSizeChange();
		
		// this may fail, but we're not able to propagate exceptions here
		TVwsViewId view;
		AppUi()->GetActiveViewId( view );
		if ( view.iViewUid == Id() )
		{
			TRAPD( err, SetupStatusPaneL() );
		}
	}

#ifdef _TOUCH_SUPPORT_ 
void CSymDjvuContainerView::DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane)
  {
    if (R_SYM_DJVU_CONTAINER_MENU_PANE1_MENU_PANE == aResourceId)
      {
		aMenuPane->SetItemDimmed(EOpenFile,                                iRenderThreadManager->iThreadStarted);
		aMenuPane->SetItemDimmed(EZoomIn,        !iDjVuReader->IsOpen() || iRenderThreadManager->iThreadStarted);
		aMenuPane->SetItemDimmed(EZoomOut,       !iDjVuReader->IsOpen() || iRenderThreadManager->iThreadStarted);
		aMenuPane->SetItemDimmed(EZoomWidth,     !iDjVuReader->IsOpen() || iRenderThreadManager->iThreadStarted);
		aMenuPane->SetItemDimmed(EFitActualSize, !iDjVuReader->IsOpen() || iRenderThreadManager->iThreadStarted);
		aMenuPane->SetItemDimmed(EGoToPage,      !iDjVuReader->IsOpen() || iRenderThreadManager->iThreadStarted);
      }
  }
#endif

void CSymDjvuContainerView::SetupStatusPaneL()
	{
		// reset the context pane
		TUid contextPaneUid = TUid::Uid( EEikStatusPaneUidContext );
		CEikStatusPaneBase::TPaneCapabilities subPaneContext = 
			StatusPane()->PaneCapabilities( contextPaneUid );
		if ( subPaneContext.IsPresent() && subPaneContext.IsAppOwned() )
		{
			CAknContextPane* context = static_cast< CAknContextPane* > ( 
				StatusPane()->ControlL( contextPaneUid ) );
			context->SetPictureToDefaultL();
		}
		
		// setup the title pane
		TUid titlePaneUid = TUid::Uid( EEikStatusPaneUidTitle );
		CEikStatusPaneBase::TPaneCapabilities subPaneTitle = 
			StatusPane()->PaneCapabilities( titlePaneUid );
		if ( subPaneTitle.IsPresent() && subPaneTitle.IsAppOwned() )
		{
			CAknTitlePane* title = static_cast< CAknTitlePane* >( 
				StatusPane()->ControlL( titlePaneUid ) );
			TResourceReader reader;
			iEikonEnv->CreateResourceReaderLC( reader, R_SYM_DJVU_CONTAINER_TITLE_RESOURCE );
			title->SetFromResourceL( reader );
			CleanupStack::PopAndDestroy(); // reader internal state
		}
				
	}


/**
 * Show the popup dialog for PageNum
 * @param aData in-out TDes data
 * @param aUseDefaults TBool use designer default editor data if ETrue
 * @param aOverridePrompt optional override prompt text
 * @return EAknSoftkeyOk (left soft key id) or 0
 */
TInt CSymDjvuContainerView::RunPageNumL( 
		TDes& aData, 
		TBool aUseDefaults, 
		const TDesC* aOverridePrompt )
	{
		if ( aUseDefaults )
		{
			HBufC* text = StringLoader::LoadLC( R_SYM_DJVU_CONTAINER_NUM_OF_PAGE );
			aData.Copy( *text );
			CleanupStack::PopAndDestroy( text );
		}
					
		CAknTextQueryDialog* queryDialog = CAknTextQueryDialog::NewL( aData );	
		
		if ( aOverridePrompt != NULL )
		{
			CleanupStack::PushL( queryDialog );
			queryDialog->SetPromptL( *aOverridePrompt );
			CleanupStack::Pop(); // queryDialog
		}
		
		return queryDialog->ExecuteLD( R_SYM_DJVU_CONTAINER_PAGE_NUM );
	}
			
/** 
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CSymDjvuContainerView::HandleOpenFileMenuItemSelectedL()
	{
	
		#ifdef _TOUCH_SUPPORT_ 
					
		#else 
			CAknMemorySelectionDialog::TMemory memory = CAknMemorySelectionDialog::EPhoneMemory;
		#endif
		
		TFileName fileName;
		TBool result = EFalse;
		TBool SecondIteration = EFalse;
		
		TBool LastDirectory = CBookSettings::LoadLastDirectory(fileName);
		
		do
			{
				
				if(!LastDirectory || SecondIteration)
				{
					
					#ifdef _TOUCH_SUPPORT_ 
					
						CAknMemorySelectionDialogMultiDrive* memDlg = CAknMemorySelectionDialogMultiDrive::NewL(ECFDDialogTypeSelect, EFalse);
						CleanupStack::PushL(memDlg);
						TDriveNumber drive;
						CAknCommonDialogsBase::TReturnKey Key = memDlg->ExecuteL(drive);
						CleanupStack::PopAndDestroy();
						if (Key == CAknFileSelectionDialog::ERightSoftkey)
						{
							// cancel selection
							break;
						}
						
						RFs fs;
						fs.SetHandle(CCoeEnv::Static()->FsSession().Handle());
						TChar DriveLetter;
						User::LeaveIfError(fs.DriveToChar(drive, DriveLetter));
						fileName.Zero();
						fileName.Append(DriveLetter);
						fileName.Append(_L(":\\"));
					
					#else 
					
						CAknMemorySelectionDialog* memDlg = CAknMemorySelectionDialog::NewL(ECFDDialogTypeSelect, ETrue);
						CleanupStack::PushL(memDlg);
						CAknMemorySelectionDialog::TReturnKey Key = memDlg->ExecuteL(memory);
						CleanupStack::PopAndDestroy();
						if (Key == CAknFileSelectionDialog::ERightSoftkey)
						{
							// cancel selection
							break;
						}
				
						if (memory == CAknMemorySelectionDialog::EMemoryCard)
						{
							//	aFileName = PathInfo::MemoryCardRootPath();
							fileName.Copy(_L("E:\\"));
						}
						else
						{
							//	aFileName = PathInfo::PhoneMemoryRootPath();
							fileName.Copy(_L("C:\\"));
						}
				
					#endif 
				
				}
				
				// Create select file dialog
				CAknFileSelectionDialog* dlg = CAknFileSelectionDialog::NewL(ECFDDialogTypeSelect);
				CleanupStack::PushL(dlg);
				
				// Dialog customizations:
				HBufC* msg1 = iEikonEnv->AllocReadResourceLC(R_SELECT_FILE_SELECT_FILE);
				dlg->SetTitleL(*msg1);
				CleanupStack::PopAndDestroy(msg1);
				
				HBufC* msg2 = iEikonEnv->AllocReadResourceLC(R_SELECT_FILE_SELECT);
				dlg->SetLeftSoftkeyFileL(*msg2);
				CleanupStack::PopAndDestroy(msg2);
				
				HBufC* msg3 = iEikonEnv->AllocReadResourceLC(R_SELECT_FILE_OPEN);
				dlg->SetLeftSoftkeyFolderL(*msg3);
				CleanupStack::PopAndDestroy(msg3);
				
				HBufC* msg4 = iEikonEnv->AllocReadResourceLC(R_SELECT_FILE_BACK);
				dlg->SetRightSoftkeyRootFolderL(*msg4);
				CleanupStack::PopAndDestroy(msg4);
				
				if (dlg->ExecuteL(fileName))
				{
					iRenderThreadManager->iBookSettings->iBookNameCache = fileName;
					if (iDjVuReader && iDjVuReader->IsOpen())
					{
						iRenderThreadManager->iBookSettings->SaveSettings();
					}
					
					if(iDjVuReader->OpenL(fileName))
					{
						iRenderThreadManager->iBookSettings->LoadSettings(fileName);			
						iRenderThreadManager->RenderOpenFile();
						result = ETrue;
					}
				}
				else
				{
					if(LastDirectory)
					{
						TParse parse;
						parse.Set(fileName, NULL, NULL);
						if(parse.IsRoot())
						{
							SecondIteration = ETrue;
						}
						else
						{
							parse.PopDir();
							fileName.Zero();
							fileName.Append(parse.FullName());
						}
					}
			
				}
				CleanupStack::PopAndDestroy();
				
			} while (!result);
	
		return result;
	
	}

TBool CSymDjvuContainerView::HandleGoToPageItemSelectedL()
	{
		if (iDjVuReader && iDjVuReader->IsOpen())
		{
			
			TBuf<25> numOfPage;
			TBuf<50> title;
			
			title.Zero();
			
			HBufC* msg = iEikonEnv->AllocReadResourceLC(R_CHOOSE_PAGE);
			title.Append(*msg);
			CleanupStack::PopAndDestroy(msg);
			
			title.Append(' ');
			title.Append('(');
			
			HBufC* msg1 = iEikonEnv->AllocReadResourceLC(R_CHOOSE_PAGE_MAXIMUM);
			title.Append(*msg1);
			CleanupStack::PopAndDestroy(msg1);
			
			title.Append(' ');
			title.AppendNum(iDjVuReader->GetPageCount());
			title.Append(' ');
			
			HBufC* msg2 = iEikonEnv->AllocReadResourceLC(R_CHOOSE_PAGE_PAGES);
			title.Append(*msg2);
			CleanupStack::PopAndDestroy(msg2);
			
			title.Append(')');
			
			numOfPage.Zero();
			numOfPage.AppendNum(iDjVuReader->CurrentPage()+1);
							
			CAknTextQueryDialog* queryDialog = CAknTextQueryDialog::NewL( numOfPage );	
			
			CleanupStack::PushL( queryDialog );
			queryDialog->SetPromptL( title );
			CleanupStack::Pop(); // queryDialog
				
			if(queryDialog->ExecuteLD( R_SYM_DJVU_CONTAINER_PAGE_NUM ))
			{
				TInt textLength;
				TDigitType digitType;
			
				iDjVuReader->SetCurrentPage(NumberConversion::ConvertFirstNumber(numOfPage,textLength,digitType) - 1);
				iRenderThreadManager->RenderCurrentPage();
		
			}

		}       	
		
		return ETrue;
	}

TBool CSymDjvuContainerView::HandleZoomInItemSelectedL()
	{
		if (iDjVuReader && iDjVuReader->IsOpen())
		{
			iRenderThreadManager->RenderIncrementZoom();
		}       	
		
		return ETrue;
	}

TBool CSymDjvuContainerView::HandleZoomOutItemSelectedL()
	{
		if (iDjVuReader && iDjVuReader->IsOpen())
		{
			iRenderThreadManager->RenderDecrementZoom();
		}       	
		
		return ETrue;
	}

TBool CSymDjvuContainerView::HandleZoomWidthItemSelectedL()
	{
		if (iDjVuReader && iDjVuReader->IsOpen())
		{
			iRenderThreadManager->RenderZoomWidth();
		}
		
		return ETrue;
	}

TBool CSymDjvuContainerView::HandleFitActualSizeItemSelectedL()
	{
		if (iDjVuReader && iDjVuReader->IsOpen())
		{
			iRenderThreadManager->RenderFitActualSize();
		}
		return ETrue;
	}

TBool CSymDjvuContainerView::HandleSetFullScreenModeL()
{
	if (!iRenderThreadManager->iContainer->iFullScreenMode)
	{
		iRenderThreadManager->iContainer->EnableFullScreenMode(true);
	}
	else
	{
		iRenderThreadManager->iContainer->EnableFullScreenMode(false);
	}
	return ETrue;
}

TBool CSymDjvuContainerView::HandleExitItemSelectedL()
	{
		AppUi()->HandleCommandL( EEikCmdExit );
		return ETrue;
	}

TBool CSymDjvuContainerView::HandleAboutItemSelectedL()
	{
		
		CAknMessageQueryDialog* dlg = new (ELeave) CAknMessageQueryDialog();
		dlg->PrepareLC(R_ABOUT_QUERY_DIALOG);
		HBufC* title = iEikonEnv->AllocReadResourceLC(R_ABOUT_DIALOG_TITLE);
		dlg->QueryHeading()->SetTextL(*title);
		CleanupStack::PopAndDestroy(); //title
		HBufC* msg = iEikonEnv->AllocReadResourceLC(R_ABOUT_DIALOG_TEXT);
		dlg->SetMessageTextL(*msg);
		CleanupStack::PopAndDestroy(); //msg
		dlg->RunLD();
	
		return ETrue;
	}

TBool CSymDjvuContainerView::HandleUpButtonPressedL()
	{

		if (iRenderThreadManager->iContainer->iDjVuReader && iRenderThreadManager->iContainer->iDjVuReader->IsOpen())
		{

			if (iRenderThreadManager->iContainer->iCursorPosition.iY == iRenderThreadManager->iContainer->iMargin
				|| iRenderThreadManager->iContainer->iDjVuReader->GetBitmap()->SizeInPixels().iHeight <= iRenderThreadManager->iContainer->Rect().Height())
			{
				if(iRenderThreadManager->iContainer->iDjVuReader->CurrentPage() > iRenderThreadManager->iContainer->iDjVuReader->FirstPageNumber())
				{
					iRenderThreadManager->RenderPreviousPage();
				}

			}
			else
			{
				iRenderThreadManager->iContainer->iCursorPosition.iY += iRenderThreadManager->iContainer->Rect().Height();
				iRenderThreadManager->iContainer->CursorPositionCorrection();
				iRenderThreadManager->iContainer->DrawNow();						
			}
		}
		return ETrue;
	}

TBool CSymDjvuContainerView::HandleDownButtonPressedL()
	{
	
		if (iRenderThreadManager->iContainer->iDjVuReader && iRenderThreadManager->iContainer->iDjVuReader->IsOpen())
		{
			
			if (iRenderThreadManager->iContainer->iCursorPosition.iY == (iRenderThreadManager->iContainer->Rect().Height() - iRenderThreadManager->iContainer->iDjVuReader->GetBitmap()->SizeInPixels().iHeight - iRenderThreadManager->iContainer->iMargin)
				|| iRenderThreadManager->iContainer->iDjVuReader->GetBitmap()->SizeInPixels().iHeight <= iRenderThreadManager->iContainer->Rect().Height())
			{
				if(iRenderThreadManager->iContainer->iDjVuReader->CurrentPage() < iRenderThreadManager->iContainer->iDjVuReader->LastPageNumber())
				{
					iRenderThreadManager->RenderNextPage();
				}
			}
			else
			{
				iRenderThreadManager->iContainer->iCursorPosition.iY -= iRenderThreadManager->iContainer->Rect().Height();
				iRenderThreadManager->iContainer->CursorPositionCorrection();
				iRenderThreadManager->iContainer->DrawNow();						
			}
			
		} 
		
		return ETrue;
	}

void CSymDjvuContainerView::SaveSettings()
	{
		iRenderThreadManager->iBookSettings->SaveSettings();
	}
