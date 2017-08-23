
#ifndef SYMDJVUCONTAINERVIEW_H
#define SYMDJVUCONTAINERVIEW_H

#include <aknview.h>

#include "Config.h"

class CSymDjvuContainer;
class CLastFileOpener;
class CDjvuReader;
class CRenderThreadManager;

//class CSymDjvuContainer;

/**
 * Avkon view class for SymDjvuContainerView. It is register with the view server
 * by the AppUi. It owns the container control.
 * @class	CSymDjvuContainerView SymDjvuContainerView.h
 */						
			
class CSymDjvuContainerView : public CAknView
	{
	
public:
	
	// constructors and destructor
	CSymDjvuContainerView();
	static CSymDjvuContainerView* NewL();
	static CSymDjvuContainerView* NewLC();        
	void ConstructL();
	virtual ~CSymDjvuContainerView();

	// from base class CAknView
	TUid Id() const;
	void HandleCommandL( TInt aCommand );

	static TInt RunPageNumL(
			TDes& aData,
			TBool aUseDefaults = ETrue,
			const TDesC* aOverridePrompt = NULL );

	TBool HandleUpButtonPressedL();
	TBool HandleDownButtonPressedL();

	void SaveSettings();

protected:
	
	// from base class CAknView
	void DoActivateL(
		const TVwsViewId& aPrevViewId,
		TUid aCustomMessageId,
		const TDesC8& aCustomMessage );
	void DoDeactivate();
	void HandleStatusPaneSizeChange();
	
	#ifdef _TOUCH_SUPPORT_ 
	void DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane);
	#endif
	
	void HandleOpenFileMenuItemSelectedL();
	void HandleGoToPageL();
	void HandleZoomInItemSelectedL();
	void HandleZoomOutItemSelectedL();
	void HandleZoomWidthItemSelectedL();
	void HandleFitActualSizeItemSelectedL();
	void HandleSetFullScreenModeL();
	void HandleAboutItemSelectedL();
	void HandleFindText();
	void DoNothing(){}

private:
	
	void SetupStatusPaneL();

public:
	
	CRenderThreadManager* iRenderThreadManager = nullptr;
	CDjvuReader* iDjVuReader = nullptr;
	CLastFileOpener* iLastFileOpener = nullptr;
	
	};

#endif // SYMDJVUCONTAINERVIEW_H
