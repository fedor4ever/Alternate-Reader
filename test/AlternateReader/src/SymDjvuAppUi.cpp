
#include <eikmenub.h>
#include <akncontext.h>
#include <akntitle.h>
#include <SymDjvu.rsg>

#include "SymDjvuAppUi.h"
#include "SymDjvu.hrh"
#include "SymDjvuContainer.hrh"
#include "SymDjvuContainerView.h"

/**
 * Construct the CSymDjvuAppUi instance
 */ 
CSymDjvuAppUi::CSymDjvuAppUi()
	{
	
	}

/** 
 * The appui's destructor removes the container from the control
 * stack and destroys it.
 */
CSymDjvuAppUi::~CSymDjvuAppUi()
	{
	
	}

void CSymDjvuAppUi::InitializeContainersL()
	{
		iSymDjvuContainerView = CSymDjvuContainerView::NewL();
		AddViewL( iSymDjvuContainerView );
		SetDefaultViewL( *iSymDjvuContainerView );
		
		iInterfaceSelector = CRemConInterfaceSelector::NewL();
		iCoreTarget = CRemConCoreApiTarget::NewL(*iInterfaceSelector, *this);
		iInterfaceSelector->OpenTargetL();
	}

// ----------------------------------------------------------------------------
  // MrccatoCommand()
  // Receives events (press/click/release) from the following buttons:
  // ’Play/Pause’, ’Volume Up’, ’Volume Down’, ’Stop’, ’Rewind’, ’Forward’
  // ----------------------------------------------------------------------------
  void CSymDjvuAppUi::MrccatoCommand(TRemConCoreApiOperationId aOperationId,
                                        TRemConCoreApiButtonAction aButtonAct)
    {
		  TRequestStatus status;
		  switch( aOperationId )
		  {
			  case ERemConCoreApiVolumeUp:
			  {   
				  iSymDjvuContainerView->HandleUpButtonPressedL();   
				  break;
			  }       
			  case ERemConCoreApiVolumeDown:
			  {
				  iSymDjvuContainerView->HandleDownButtonPressedL();
				  break;
			  }
			  default:
				  break;
		  }
    }

/**
 * Handle a command for this appui (override)
 * @param aCommand command id to be handled
 */
void CSymDjvuAppUi::HandleCommandL( TInt aCommand )
	{

		TBool commandHandled = EFalse;
		switch ( aCommand )
		{ // code to dispatch to the AppUi's menu and CBA commands is generated here
			default:
				break;
		}
		
		if ( !commandHandled ) 
		{
			if ( aCommand == EAknSoftkeyExit || aCommand == EEikCmdExit )
			{
				iSymDjvuContainerView->SaveSettings();
				Exit();
			}
		}

	}

/** 
 * Override of the HandleResourceChangeL virtual function
 */
void CSymDjvuAppUi::HandleResourceChangeL( TInt aType )
	{
		CAknViewAppUi::HandleResourceChangeL( aType );
	}
				
/** 
 * Override of the HandleKeyEventL virtual function
 * @return EKeyWasConsumed if event was handled, EKeyWasNotConsumed if not
 * @param aKeyEvent 
 * @param aType 
 */
TKeyResponse CSymDjvuAppUi::HandleKeyEventL(
		const TKeyEvent& aKeyEvent,
		TEventCode aType )
	{
		// The inherited HandleKeyEventL is private and cannot be called
		return EKeyWasNotConsumed;
	}

/** 
 * Override of the HandleViewDeactivation virtual function
 *
 * @param aViewIdToBeDeactivated 
 * @param aNewlyActivatedViewId 
 */
void CSymDjvuAppUi::HandleViewDeactivation( 
		const TVwsViewId& aViewIdToBeDeactivated, 
		const TVwsViewId& aNewlyActivatedViewId )
	{
		CAknViewAppUi::HandleViewDeactivation( 
			aViewIdToBeDeactivated, 
			aNewlyActivatedViewId);
	}

/**
 * @brief Completes the second phase of Symbian object construction. 
 * Put initialization code that could leave here. 
 */ 
void CSymDjvuAppUi::ConstructL()
	{
		BaseConstructL( EAknEnableSkin  | 
						 EAknEnableMSK ); 
		InitializeContainersL();
	}


void CSymDjvuAppUi::LoadFile(const TFileName& filename)
	{
	
		iSymDjvuContainerView->iRenderThreadManager->iBookSettings->iBookNameCache = filename;
		iSymDjvuContainerView->iDjVuReader->OpenL(filename);
		iSymDjvuContainerView->iRenderThreadManager->iBookSettings->LoadSettings(filename);			
		iSymDjvuContainerView->iRenderThreadManager->RenderOpenFile();
	
	}
	
