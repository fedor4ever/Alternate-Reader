
#ifndef SYMDJVUCONTAINERVIEW_H
#define SYMDJVUCONTAINERVIEW_H

#include <aknview.h>

#include "Config.h"
#include "CDjvuReader.h"
#include "SymDjvuContainer.h"
#include "RenderThreadManager.h"
#include "LastFileOpener.h"

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
	
	CSymDjvuContainer* CreateContainerL();
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
	
	TBool HandleOpenFileMenuItemSelectedL( TInt aCommand );
	TBool HandleGoToPageItemSelectedL( TInt aCommand );
	TBool HandleZoomInItemSelectedL( TInt aCommand );
	TBool HandleZoomOutItemSelectedL( TInt aCommand );
	TBool HandleZoomWidthItemSelectedL( TInt aCommand );
	TBool HandleFitActualSizeItemSelectedL( TInt aCommand );
	TBool HandleFullscreenItemSelectedL( TInt aCommand );
	TBool HandleExitItemSelectedL( TInt aCommand );
	TBool HandleAboutItemSelectedL( TInt aCommand );

private:
	
	void SetupStatusPaneL();
	void CleanupStatusPane();

public:
	
	CRenderThreadManager* iRenderThreadManager;
	CDjvuReader* iDjVuReader;
	CLastFileOpener* iLastFileOpener;
	
	};

#endif // SYMDJVUCONTAINERVIEW_H
