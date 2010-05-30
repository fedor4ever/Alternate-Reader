
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

#include "SymDjvu.hrh"
#include "SymDjvuContainerView.h"
#include "SymDjvuContainer.hrh"
#include "SymDjvuContainer.h"

#include "numberconversion.h"
#include <e32err.h>



/**
 * First phase of Symbian two-phase construction. Should not contain any
 * code that could leave.
 */
CSymDjvuContainerView::CSymDjvuContainerView()
	{
		iRenderThreadManager = NULL;
		iDjVuReader = NULL;
		iLastFileOpener = NULL;
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
			{	// code to dispatch to the AknView's menu and CBA commands is generated here
			
			case EOpenFile:
				commandHandled = HandleOpenFileMenuItemSelectedL( aCommand );
				break;
			case EGoToPage:
				commandHandled = HandleGoToPageItemSelectedL( aCommand );
				break;		
			case EZoomIn:
				commandHandled = HandleZoomInItemSelectedL( aCommand );
				break;		
			case EZoomOut:
				commandHandled = HandleZoomOutItemSelectedL( aCommand );
				break;		
			case EZoomWidth:
				commandHandled = HandleZoomWidthItemSelectedL( aCommand );
				break;		
			case EFitActualSize:
				commandHandled = HandleFitActualSizeItemSelectedL( aCommand );
				break;		
			case EFullscreen:
				commandHandled = HandleFullscreenItemSelectedL( aCommand );
				break;		
			case EButtonDown:
				commandHandled = HandleDownButtonPressedL();
				break;		
			case EButtonUp:
				commandHandled = HandleUpButtonPressedL();
				break;		
			case EExit:
				commandHandled = HandleExitItemSelectedL( aCommand );
				break;		
			default:
				break;
			}
			
			if ( !commandHandled ) 
			{
				if ( aCommand == EAknSoftkeyExit )
				{
					HandleExitItemSelectedL( aCommand );
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
		
		if ( iRenderThreadManager == NULL )
		{
			CSymDjvuContainer* mSymDjvuContainer = CreateContainerL();
			mSymDjvuContainer->SetMopParent( this );
			AppUi()->AddToStackL( *this, mSymDjvuContainer );
			
			iRenderThreadManager = CRenderThreadManager::NewL();
			iRenderThreadManager->iContainer = mSymDjvuContainer;
			iRenderThreadManager->iAnimation->iContainer = mSymDjvuContainer;
			iRenderThreadManager->iBookSettings->iContainer = mSymDjvuContainer;
			
			iSBFrame = new ( ELeave ) CEikScrollBarFrame( iRenderThreadManager->iContainer, NULL );
			iSBFrame->CreateDoubleSpanScrollBarsL( ETrue, EFalse, ETrue, EFalse );
			iSBFrame->SetTypeOfVScrollBar( CEikScrollBarFrame::EDoubleSpan );
			iSBFrame->SetScrollBarVisibilityL(CEikScrollBarFrame::EOn, CEikScrollBarFrame::EOn );
			
			iLastFileOpener = CLastFileOpener::NewL();
			iLastFileOpener->iDjVuReader = iDjVuReader;
			iLastFileOpener->iRenderThreadManager = iRenderThreadManager;
			iLastFileOpener->StartL(1000000);
			
		} 			

	}

/**
 */
void CSymDjvuContainerView::DoDeactivate()
	{

		CleanupStatusPane();
		
		if ( iRenderThreadManager->iContainer != NULL )
		{
			AppUi()->RemoveFromViewStack( *this, iRenderThreadManager->iContainer );
			
			delete iSBFrame;
			iSBFrame = NULL;		
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
			TInt result;
			TRAP( result, SetupStatusPaneL() );
		}
	}

/**
 *	Creates the top-level container for the view.  You may modify this method's
 *	contents and the CSymDjvuContainer::NewL() signature as needed to initialize the
 *	container, but the signature for this method is fixed.
 *	@return new initialized instance of CSymDjvuContainer
 */
CSymDjvuContainer* CSymDjvuContainerView::CreateContainerL()
	{
		return CSymDjvuContainer::NewL( ClientRect(), NULL, this, iDjVuReader);
	}

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

void CSymDjvuContainerView::CleanupStatusPane()
	{
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
TBool CSymDjvuContainerView::HandleOpenFileMenuItemSelectedL( TInt aCommand )
	{
	
		CAknMemorySelectionDialog::TMemory memory = CAknMemorySelectionDialog::EPhoneMemory;
		TFileName fileName;
		TBool result = EFalse;
		TBool SecondIteration = EFalse;
		
		TBool LastDirectory = CBookSettings::LoadLastDirectory(fileName);
		
		do
			{
				
				if(!LastDirectory || SecondIteration)
				{
					
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
				}
				
				// Создание объекта сделано в цикле для обхода ошибки
				// при перевыборе файла (если выбрать память телефона то
				// не понятно как перейти к памяти карты).
				// Create select file dialog
				CAknFileSelectionDialog* dlg = CAknFileSelectionDialog::NewL(ECFDDialogTypeSelect);
				CleanupStack::PushL(dlg);
				// Dialog customizations:
				dlg->SetTitleL(_L("Select file"));
				dlg->SetLeftSoftkeyFileL(_L("Select"));
				dlg->SetLeftSoftkeyFolderL(_L("Open"));
				dlg->SetRightSoftkeyRootFolderL(_L("Back")); // for root folder				
				if (dlg->ExecuteL(fileName))
				{
					iRenderThreadManager->iBookSettings->iBookNameCache = fileName;
					if (iDjVuReader && iDjVuReader->IsOpen())
					{
						iRenderThreadManager->iBookSettings->SaveSettings();
					}
					
					iDjVuReader->OpenL(fileName);
					iRenderThreadManager->iBookSettings->LoadSettings(fileName);			
					iRenderThreadManager->RenderOpenFile();
					result = ETrue;
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

TBool CSymDjvuContainerView::HandleGoToPageItemSelectedL( TInt aCommand )
	{
		if (iDjVuReader && iDjVuReader->IsOpen())
		{
			
			TBuf<25> numOfPage;
			TBuf<25> title;
			_LIT(KTitle, "Choose page (max ");
			_LIT(KTitleEnd, ")");
			title = KTitle;
			title.AppendNum(iDjVuReader->GetPageCount());
			title.Append(KTitleEnd);
			
			HBufC* text = StringLoader::LoadLC( R_SYM_DJVU_CONTAINER_NUM_OF_PAGE );
			numOfPage.Copy( *text );
			CleanupStack::PopAndDestroy( text );
							
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

TBool CSymDjvuContainerView::HandleZoomInItemSelectedL( TInt aCommand )
	{
		if (iDjVuReader && iDjVuReader->IsOpen())
		{
			iRenderThreadManager->RenderIncrementZoom();
		}       	
		
		return ETrue;
	}

TBool CSymDjvuContainerView::HandleZoomOutItemSelectedL( TInt aCommand )
	{
		if (iDjVuReader && iDjVuReader->IsOpen())
		{
			iRenderThreadManager->RenderDecrementZoom();
		}       	
		
		return ETrue;
	}

TBool CSymDjvuContainerView::HandleZoomWidthItemSelectedL( TInt aCommand )
	{
		if (iDjVuReader && iDjVuReader->IsOpen())
		{
			iRenderThreadManager->RenderZoomWidth();
		}       	
		
		return ETrue;
	}

TBool CSymDjvuContainerView::HandleFitActualSizeItemSelectedL( TInt aCommand )
	{
		if (iDjVuReader && iDjVuReader->IsOpen())
		{
			iRenderThreadManager->RenderFitActualSize();
		}       	
		
		return ETrue;
	}

TBool CSymDjvuContainerView::HandleFullscreenItemSelectedL( TInt aCommand )
	{
		iRenderThreadManager->iContainer->SetRect( iAvkonViewAppUi->View( TUid::Uid( ESymDjvuContainerViewId ) )->ClientRect() );
		if (!iRenderThreadManager->iContainer->iFullScreenMode)
		{
			iRenderThreadManager->iContainer->iFullScreenMode = ETrue;
		
			#ifdef _TOUCH_SUPPORT_
			iRenderThreadManager->iContainer->EnableLongTapAnimation(ETrue);
			#endif	
			
			iRenderThreadManager->iContainer->SetExtentToWholeScreen();
		}
		else
		{
		
			#ifdef _TOUCH_SUPPORT_
			iRenderThreadManager->iContainer->EnableLongTapAnimation(EFalse);
			#endif	
		
			iRenderThreadManager->iContainer->iFullScreenMode = EFalse;
		}
		
		iRenderThreadManager->iContainer->CursorPositionCorrection();
		iRenderThreadManager->iContainer->DrawNow();
		
		return ETrue;
	}

TBool CSymDjvuContainerView::HandleExitItemSelectedL( TInt aCommand )
	{
		
		iRenderThreadManager->iBookSettings->SaveSettings();
		AppUi()->HandleCommandL( EEikCmdExit );
	
		return ETrue;
	}

TBool CSymDjvuContainerView::HandleUpButtonPressedL()
	{

		if (iRenderThreadManager->iContainer->iDjVuReader && iRenderThreadManager->iContainer->iDjVuReader->IsOpen())
		{
			
			if (iRenderThreadManager->iContainer->iCursorPosition.iY == iRenderThreadManager->iContainer->iMargin
				|| iRenderThreadManager->iContainer->iDjVuReader->GetBitmap()->SizeInPixels().iHeight <= iRenderThreadManager->iContainer->Rect().Height())
			{
				iRenderThreadManager->RenderPreviousPage();
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
				iRenderThreadManager->RenderNextPage();
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
