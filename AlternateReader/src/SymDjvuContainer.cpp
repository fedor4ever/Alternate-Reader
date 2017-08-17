
#include <aknviewappui.h>
#include <eikappui.h>
#include <SymDjvu.rsg>
#include <AknUtils.h>
#include <aknglobalnote.h>
#include <BARSREAD.H> 
#include <AknQueryDialog.h>
#include <stringloader.h>
#include <aknappui.h>
#include <stdlib.h>
#include <eiksbfrm.h>


#include "numberconversion.h"
#include "SymDjvuContainer.h"
#include "SymDjvuContainerView.h"
#include "SymDjvu.hrh"
#include "CDjvuReader.h"
#include "SymDjvuContainer.hrh"

const TUint KMinimalTouchEnabledMajorVersion = 5;

/**
 * First phase of Symbian two-phase construction. Should not 
 * contain any code that could leave.
 */
CSymDjvuContainer::CSymDjvuContainer() : iCursorPosition(TPoint(0,0)),
                                         iEButtonDownLastPositionDrag(TPoint(0,0)),
                                         iEButtonDownPosition(TPoint(0,0))
	{
	
	}
/** 
 * Destroy child controls.
 */
CSymDjvuContainer::~CSymDjvuContainer()
	{
	
	#ifdef _TOUCH_SUPPORT_
		delete iLongTapDetector;
		iLongTapDetector = NULL;
		
		delete iStylusPopupMenu;
		iStylusPopupMenu = NULL;

	#endif
		
		delete iGif;
		iGif = NULL;
		
	}
				
/**
 * Construct the control (first phase).
 *  Creates an instance and initializes it.
 *  Instance is not left on cleanup stack.
 * @param aRect bounding rectangle
 * @param aParent owning parent, or NULL
 * @param aCommandObserver command observer
 * @return initialized instance of CSymDjvuContainer
 */
CSymDjvuContainer* CSymDjvuContainer::NewL( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver,
		CDjvuReader* aDjvuReader )
	{
	CSymDjvuContainer* self = CSymDjvuContainer::NewLC( 
			aRect, 
			aParent, 
			aCommandObserver,
			aDjvuReader );
	CleanupStack::Pop( self );
	return self;
	}

/**
 * Construct the control (first phase).
 *  Creates an instance and initializes it.
 *  Instance is left on cleanup stack.
 * @param aRect The rectangle for this window
 * @param aParent owning parent, or NULL
 * @param aCommandObserver command observer
 * @return new instance of CSymDjvuContainer
 */
CSymDjvuContainer* CSymDjvuContainer::NewLC( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver,
		CDjvuReader* aDjvuReader )
	{
	CSymDjvuContainer* self = new ( ELeave ) CSymDjvuContainer();
	CleanupStack::PushL( self );
	self->ConstructL( aRect, aParent, aCommandObserver, aDjvuReader );
	return self;
	}
			
/**
 * Construct the control (second phase).
 *  Creates a window to contain the controls and activates it.
 * @param aRect bounding rectangle
 * @param aCommandObserver command observer
 * @param aParent owning parent, or NULL
 */ 
void CSymDjvuContainer::ConstructL( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver,
		CDjvuReader* aDjvuReader )
	{
	
		if ( aParent == NULL )
		{
			CreateWindowL();
		}
		else
		{
			SetContainerWindowL( *aParent );
			SetMopParent((MObjectProvider*)aParent);
		}
		iFocusControl = NULL;
		iCommandObserver = aCommandObserver;

		#ifdef _TOUCH_SUPPORT_ 		
			iLongTapDetector = CAknLongTapDetector::NewL(this);
			iLongTapDetector->SetLongTapDelay(1500000);
			iLongTapDetector->SetTimeDelayBeforeAnimation(1000000);
			iLongTapDetector->EnableLongTapAnimation(EFalse);
		#endif
		
		iGif = CAnimatedGif::NewL(_L("c:\\private\\e1a0b4f1\\load.gif"), *(GetWindow()), TPoint(0,0));
			
		iFullScreenMode = false;
		
		iCursorPosition.iX = 0;
		iCursorPosition.iY = 0;
		
		dPixels = 10;
		iMargin = 15; 
		
		iDjVuReader = aDjvuReader;

		SetRect( aRect );
		
		ActivateL();
		
		#ifdef _TOUCH_SUPPORT_
			EnableDragEvents();
		#endif	
			
	}


/**
* Return the number of controls in the container (override)
* @return count
*/
TInt CSymDjvuContainer::CountComponentControls() const
	{
	return ( int ) ELastControl;
	}
				
/**
* Get the control with the given index (override)
* @param aIndex Control index [0...n) (limited by #CountComponentControls)
* @return Pointer to control
*/
CCoeControl* CSymDjvuContainer::ComponentControl( TInt aIndex ) const
	{
		return NULL;
	}
				
/**
 *	Handle resizing of the container. This implementation will lay out
 *  full-sized controls like list boxes for any screen size, and will layout
 *  labels, editors, etc. to the size they were given in the UI designer.
 *  This code will need to be modified to adjust arbitrary controls to
 *  any screen size.
 */				
void CSymDjvuContainer::SizeChanged()
	{
		CCoeControl::SizeChanged();
	}

RWindow*
CSymDjvuContainer::GetWindow()
	{
		return &Window();
	}

/**
 *	Handle key events.
 */				
TKeyResponse CSymDjvuContainer::OfferKeyEventL( 
		const TKeyEvent& aKeyEvent, 
		TEventCode aType )
	{
	
		if (aType == EEventKey)
		{
			switch (aKeyEvent.iCode)
			{
				case EKeyDownArrow:
					
					iCommandObserver->ProcessCommandL(EButtonDown);
					return EKeyWasConsumed;
					
				case EKeyUpArrow:
					
					iCommandObserver->ProcessCommandL(EButtonUp); 
					return EKeyWasConsumed;
					
				case EKeyLeftArrow:
					
					if (iDjVuReader && iDjVuReader->IsOpen())
					{
						iCursorPosition.iX += 30;
						CursorPositionCorrection();
						DrawNow();
					} 
					
					return EKeyWasConsumed;
					
				case EKeyRightArrow:
					
					if (iDjVuReader && iDjVuReader->IsOpen())
					{
						iCursorPosition.iX -= 30;
						CursorPositionCorrection();
						DrawNow();
					} 
					
					return EKeyWasConsumed;
				case 50:
					
					iCommandObserver->ProcessCommandL(EZoomIn);
					return EKeyWasConsumed;
				case 56:
					
					iCommandObserver->ProcessCommandL(EZoomOut); 
					return EKeyWasConsumed;
				case 42: // *

					iCommandObserver->ProcessCommandL(EExitFullscreen); 
					return EKeyWasConsumed;
				case EKeyEnter:
					
					break;
				case EKeyOK:
					
					return EKeyWasConsumed;
			}
		}
		
		return EKeyWasNotConsumed;

	}


void CSymDjvuContainer::CursorPositionCorrection()
	{
	
		if (iDjVuReader->GetBitmap()->SizeInPixels().iWidth > Rect().Width())
		{
			if (iCursorPosition.iX > iMargin)
			{
				iCursorPosition.iX = iMargin;
			}
			if (iCursorPosition.iX < Rect().Width() - iDjVuReader->GetBitmap()->SizeInPixels().iWidth - iMargin)
			{
				iCursorPosition.iX = Rect().Width() - iDjVuReader->GetBitmap()->SizeInPixels().iWidth - iMargin;
			}
		}
		else
		{
			iCursorPosition.iX = (Rect().Width() - iDjVuReader->GetBitmap()->SizeInPixels().iWidth) / 2;
		}
		
		if (iDjVuReader->GetBitmap()->SizeInPixels().iHeight > Rect().Height())
		{
			if (iCursorPosition.iY > iMargin)
			{
				iCursorPosition.iY = iMargin;
			}
			if (iCursorPosition.iY < Rect().Height() - iDjVuReader->GetBitmap()->SizeInPixels().iHeight - iMargin)
			{
				iCursorPosition.iY = Rect().Height() - iDjVuReader->GetBitmap()->SizeInPixels().iHeight - iMargin;
			}
		}
		else
		{
			iCursorPosition.iY = (Rect().Height() - iDjVuReader->GetBitmap()->SizeInPixels().iHeight) / 2; 
		}
	
	}

#ifdef _TOUCH_SUPPORT_
void CSymDjvuContainer::EnableLongTapAnimation(TBool aAnimation)
	{
		iLongTapDetector->EnableLongTapAnimation(aAnimation);
	}

void CSymDjvuContainer::HandlePointerEventL(const TPointerEvent& aPointerEvent)
    {
		// Check if touch is enabled or not
		if( !AknLayoutUtils::PenEnabled() )
		{
			return;
		}
		
		// Pass the pointer event to Long tap detector component
		iLongTapDetector->PointerEventL(aPointerEvent);
		
		switch (aPointerEvent.iType)
			{
			case TPointerEvent::EButton1Down:

				iDragEnabled = EFalse;
				iEButtonDownPosition = aPointerEvent.iPosition;
				
				break;
			case TPointerEvent::EDrag:	
				
				if (!(iDjVuReader && iDjVuReader->GetBitmap()))
					break;

				if(!iDragEnabled)
				{
					dX = iEButtonDownPosition.iX - aPointerEvent.iPosition.iX;	
					dY = iEButtonDownPosition.iY - aPointerEvent.iPosition.iY;
					if(dX<0) dX = (-dX);
					if(dY<0) dY = (-dX);
					
					if( dX > dPixels || dY > dPixels )
					{
						iDragEnabled = ETrue;
					}
					iEButtonDownLastPositionDrag = aPointerEvent.iPosition;
				}
				
				if(!iDragEnabled)
					break;
				
				dX = iEButtonDownLastPositionDrag.iX - aPointerEvent.iPosition.iX;	
				dY = iEButtonDownLastPositionDrag.iY - aPointerEvent.iPosition.iY;
				
				if (iVertFix)
					iCursorPosition.iX -= dX;

				iCursorPosition.iY -= dY;
				
				CursorPositionCorrection();
				
				iEButtonDownLastPositionDrag = aPointerEvent.iPosition;

				DrawNow();
					
				break;
			case TPointerEvent::EButton1Up:	
				
				if ( !iDragEnabled )
				{
					if (aPointerEvent.iPosition.iY > (Rect().Height() / 2))
					{
						iCommandObserver->ProcessCommandL(EButtonDown);
					}
					else
					{
						iCommandObserver->ProcessCommandL(EButtonUp);
					}
				}
				
				break;
			default:
				break;
		}
		
		// Call base class HandlePointerEventL()
		CCoeControl::HandlePointerEventL(aPointerEvent);
    }
#endif


void CSymDjvuContainer::ProcessCommandL(TInt aCommand)
{
	iCommandObserver->ProcessCommandL(aCommand);
}

void CSymDjvuContainer::EnableFullScreenMode(bool aMode)
{
	iFullScreenMode = aMode;
	if (iFullScreenMode == true)
	{
#ifdef _TOUCH_SUPPORT_
		EnableLongTapAnimation(ETrue);
#endif	
		SetExtentToWholeScreen();
	}
	else
	{
#ifdef _TOUCH_SUPPORT_
		EnableLongTapAnimation(EFalse);
#endif
		SetRect(iAvkonViewAppUi->View( TUid::Uid( ESymDjvuContainerViewId ) )->ClientRect());
	}
	CursorPositionCorrection();
	DrawNow();
}


#ifdef _TOUCH_SUPPORT_
void CSymDjvuContainer::HandleLongTapEventL( const TPoint& aPenEventLocation, 
											 const TPoint& aPenEventScreenLocation )
    {
		if (!iStylusPopupMenu)
		{
			iStylusPopupMenu = CAknStylusPopUpMenu::NewL( this , aPenEventLocation);
			TResourceReader reader;
			iCoeEnv->CreateResourceReaderLC(reader,R_STYLUS_POPUP_MENU);
			iStylusPopupMenu->ConstructFromResourceL(reader);
			CleanupStack::PopAndDestroy();
		}
		if (iFullScreenMode)
		{
			iStylusPopupMenu->SetItemDimmed(EOpenFile,                                iGif->IsStart());
			iStylusPopupMenu->SetItemDimmed(EZoomIn,        !iDjVuReader->IsOpen() || iGif->IsStart());
			iStylusPopupMenu->SetItemDimmed(EZoomOut,       !iDjVuReader->IsOpen() || iGif->IsStart());
			iStylusPopupMenu->SetItemDimmed(EZoomWidth,     !iDjVuReader->IsOpen() || iGif->IsStart());
			iStylusPopupMenu->SetItemDimmed(EFitActualSize, !iDjVuReader->IsOpen() || iGif->IsStart());
			iStylusPopupMenu->SetItemDimmed(EGoToPage,      !iDjVuReader->IsOpen() || iGif->IsStart());
		
			iStylusPopupMenu->SetPosition(aPenEventLocation);
			iStylusPopupMenu->ShowMenu();
		}
		
    }
#endif

/** 
 * Handle global resource changes, such as scalable UI or skin events (override)
 */
void CSymDjvuContainer::HandleResourceChange( TInt aType )
{
	CCoeControl::HandleResourceChange( aType );
	
	if (iFullScreenMode)
	{
		SetExtentToWholeScreen();
	} else {
		SetRect(iAvkonViewAppUi->View( TUid::Uid( ESymDjvuContainerViewId ) )->ClientRect());
	}

	CursorPositionCorrection();
	DrawNow();

}
				
/**
 *	Draw container contents.
 */				
void CSymDjvuContainer::Draw( const TRect& aRect ) const
	{

		CWindowGc& gc = SystemGc();
		gc.Clear( aRect );
	
		if (iDjVuReader && iDjVuReader->GetBitmap() && iDjVuReader->IsOpen())
		{
				
			CFbsBitmap* pBitmap = iDjVuReader->GetBitmap();
			
			// Drawing background		
			gc.SetBrushColor(darkgray);
			gc.SetPenColor(darkgray);
			gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
			gc.DrawRect(
					TRect(0,
						  0,
						  Rect().Width(),
						  Rect().Height())
			);
		
			// Drawing image shadow
			gc.SetBrushColor(ligthgray);
			gc.SetPenColor(ligthgray);
			gc.DrawRect(
					TRect(iCursorPosition.iX + 5,
						  iCursorPosition.iY + 5,
						  iCursorPosition.iX + pBitmap->SizeInPixels().iWidth + 5,
						  iCursorPosition.iY + pBitmap->SizeInPixels().iHeight + 5)
			);
			
			// Drawing image
			gc.BitBlt(iCursorPosition, pBitmap);
			
			// Drawing image border
			gc.SetBrushStyle(CGraphicsContext::ENullBrush);
			gc.SetPenColor(black);
			gc.DrawRect(
					TRect(iCursorPosition.iX - 1,
						  iCursorPosition.iY - 1,
						  iCursorPosition.iX + pBitmap->SizeInPixels().iWidth + 1,
						  iCursorPosition.iY + pBitmap->SizeInPixels().iHeight + 1)
			);
			
			if(iGif)
			{
				if(iGif->IsStart())
				{
					TPoint Position(Rect().Width()/2 - iGif->Size().iWidth/2, Rect().Height()/2 - iGif->Size().iHeight/2);
					if(iGif->Position() != Position)
					{
						iGif->SetPosition(Position);
					}
					iGif->Draw(gc);
				}
			}
			
		}
	}
