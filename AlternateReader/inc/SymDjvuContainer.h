
#ifndef SYMDJVUCONTAINER_H
#define SYMDJVUCONTAINER_H

#include "Config.h"
#include <coecntrl.h>		
#include <stdlib.h>
#include <eiksbfrm.h>
#include <eikmobs.h>

#ifdef _TOUCH_SUPPORT_
#include <aknlongtapdetector.h> 
#include <aknstyluspopupmenu.h>
#endif

#include "AnimatedGif.h"

class MEikCommandObserver;
class CDjvuReader;

const TRgb darkgray  = TRgb(192,192,192);
const TRgb ligthgray = TRgb(127,127,127);
const TRgb black     = TRgb(0,0,0);

/**
 * Container class for SymDjvuContainer
 * 
 * @class	CSymDjvuContainer SymDjvuContainer.h
 */
#ifdef _TOUCH_SUPPORT_
class CSymDjvuContainer : public CCoeControl, MAknLongTapDetectorCallBack, MEikMenuObserver
	{
public:
	// constructors and destructor
	CSymDjvuContainer();
	static CSymDjvuContainer* NewL( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver,
		CDjvuReader* aDjvuReader );
	static CSymDjvuContainer* NewLC( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver,
		CDjvuReader* aDjvuReader );
	void ConstructL( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver,
		CDjvuReader* aDjvuReader );
	virtual ~CSymDjvuContainer();

public:
	// from base class CCoeControl
	TInt CountComponentControls() const;
	CCoeControl* ComponentControl( TInt aIndex ) const;
	TKeyResponse OfferKeyEventL( 
			const TKeyEvent& aKeyEvent, 
			TEventCode aType );
	void HandleResourceChange( TInt aType );
	
	RWindow* GetWindow();
	void SetFullScreenMode(TBool aMode);
	
	virtual void HandlePointerEventL(const TPointerEvent& aPointerEvent);
	virtual void HandleLongTapEventL(const TPoint& aPenEventLocation, 
									 const TPoint& aPenEventScreenLocation);

protected:
	// from base class CCoeControl
	void SizeChanged();

private:
	// from base class CCoeControl
	void Draw( const TRect& aRect ) const;

private:
	
	void InitializeControlsL();
	void LayoutControls();
	
	void ProcessCommandL(TInt aCommandId);
	void SetEmphasis(CCoeControl* /*aMenuControl*/,TBool /*aEmphasis*/)
	{
	}
	
	CCoeControl* iFocusControl;
	MEikCommandObserver* iCommandObserver;
	
	CAknLongTapDetector* iLongTapDetector;
	CAknStylusPopUpMenu* iStylusPopupMenu;
	
public:
	
	TPoint iCursorPosition;
	TPoint iEButtonDownLastPositionDrag;
	TPoint iEButtonDownPosition;
	
	TBool iDragEnabled;
	TBool iFullScreenMode;
	
	TInt dX;
	TInt dY;

	TInt dPixels;
	TInt iMargin;
	
	CDjvuReader* iDjVuReader;
	CAnimatedGif* iGif;
	
	void Rotate();
	
	void CursorPositionCorrection();
	
	void EnableLongTapAnimation(TBool aAnimation);
	
	enum TControls
		{
			ELastControl
		};
	};

#else

/**
 * Container class for SymDjvuContainer
 * 
 * @class	CSymDjvuContainer SymDjvuContainer.h
 */
class CSymDjvuContainer : public CCoeControl, MEikMenuObserver
	{
public:
	// constructors and destructor
	CSymDjvuContainer();
	static CSymDjvuContainer* NewL( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver,
		CDjvuReader* aDjvuReader );
	static CSymDjvuContainer* NewLC( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver,
		CDjvuReader* aDjvuReader );
	void ConstructL( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver,
		CDjvuReader* aDjvuReader );
	virtual ~CSymDjvuContainer();

public:
	// from base class CCoeControl
	TInt CountComponentControls() const;
	CCoeControl* ComponentControl( TInt aIndex ) const;
	TKeyResponse OfferKeyEventL( 
			const TKeyEvent& aKeyEvent, 
			TEventCode aType );
	void HandleResourceChange( TInt aType );
	
	RWindow* GetWindow();
	void SetFullScreenMode(TBool aMode);
	
protected:
	// from base class CCoeControl
	void SizeChanged();

private:
	// from base class CCoeControl
	void Draw( const TRect& aRect ) const;

private:
	
	void InitializeControlsL();
	void LayoutControls();
	
	void ProcessCommandL(TInt aCommandId);
	void SetEmphasis(CCoeControl* /*aMenuControl*/,TBool /*aEmphasis*/)
	{
	}
	
	CCoeControl* iFocusControl;
	MEikCommandObserver* iCommandObserver;
	
public:
	
	TPoint iCursorPosition;
	TPoint iEButtonDownLastPositionDrag;
	TPoint iEButtonDownPosition;
	
	TBool iDragEnabled;
	TBool iFullScreenMode;
	
	TInt dX;
	TInt dY;

	TInt dPixels;
	TInt iMargin;
	
	CDjvuReader* iDjVuReader;
	CAnimatedGif* iGif;
	
	void Rotate();
	
	void CursorPositionCorrection();
	
	enum TControls
		{
			ELastControl
		};
	};

#endif

#endif // SYMDJVUCONTAINER_H
