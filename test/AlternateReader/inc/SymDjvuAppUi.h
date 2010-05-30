

#ifndef SYMDJVUAPPUI_H
#define SYMDJVUAPPUI_H

#include <aknviewappui.h>

#include <remconcoreapitargetobserver.h>    // link against RemConCoreApi.lib
#include <remconcoreapitarget.h>            // and
#include <remconinterfaceselector.h>        // RemConInterfaceBase.lib

class CSymDjvuContainerView;

/**
 * @class	CSymDjvuAppUi SymDjvuAppUi.h
 * @brief The AppUi class handles application-wide aspects of the user interface, including
 *        view management and the default menu, control pane, and status pane.
 */
class CSymDjvuAppUi : public CAknViewAppUi, public MRemConCoreApiTargetObserver
	{
public: 
	// constructor and destructor
	CSymDjvuAppUi();
	virtual ~CSymDjvuAppUi();
	void ConstructL();

public:
	// from CCoeAppUi
	TKeyResponse HandleKeyEventL(
				const TKeyEvent& aKeyEvent,
				TEventCode aType );

	// from CEikAppUi
	void HandleCommandL( TInt aCommand );
	void HandleResourceChangeL( TInt aType );
		
	// From MRemConCoreApiTargetObserver
	void MrccatoCommand(TRemConCoreApiOperationId aOperationId, TRemConCoreApiButtonAction aButtonAct);
	
	// from CAknAppUi
	void HandleViewDeactivation( 
			const TVwsViewId& aViewIdToBeDeactivated, 
			const TVwsViewId& aNewlyActivatedViewId );

private:
	void InitializeContainersL();
public: 
	
	void LoadFile(const TFileName& filename);
	
private: 
	CSymDjvuContainerView* iSymDjvuContainerView;

	CRemConInterfaceSelector* iInterfaceSelector;
	CRemConCoreApiTarget*     iCoreTarget;
	
protected: 
	
	};

#endif // SYMDJVUAPPUI_H			
