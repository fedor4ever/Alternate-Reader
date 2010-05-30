
#include "SymDjvuApplication.h"
#include "SymDjvuDocument.h"
#ifdef EKA2
#include <eikstart.h>
#endif

/**
 * @brief Returns the application's UID (override from CApaApplication::AppDllUid())
 * @return UID for this application (KUidSymDjvuApplication)
 */
TUid CSymDjvuApplication::AppDllUid() const
	{
	return KUidSymDjvuApplication;
	}

/**
 * @brief Creates the application's document (override from CApaApplication::CreateDocumentL())
 * @return Pointer to the created document object (CSymDjvuDocument)
 */
CApaDocument* CSymDjvuApplication::CreateDocumentL()
	{
	return CSymDjvuDocument::NewL( *this );
	}

#ifdef EKA2

/**
 *	@brief Called by the application framework to construct the application object
 *  @return The application (CSymDjvuApplication)
 */	
LOCAL_C CApaApplication* NewApplication()
	{
	return new CSymDjvuApplication;
	}

/**
* @brief This standard export is the entry point for all Series 60 applications
* @return error code
 */	
GLDEF_C TInt E32Main()
	{
	return EikStart::RunApplication( NewApplication );
	}
	
#else 	// Series 60 2.x main DLL program code

/**
* @brief This standard export constructs the application object.
* @return The application (CSymDjvuApplication)
*/
EXPORT_C CApaApplication* NewApplication()
	{
	return new CSymDjvuApplication;
	}

/**
* @brief This standard export is the entry point for all Series 60 applications
* @return error code
*/
GLDEF_C TInt E32Dll(TDllReason /*reason*/)
	{
	return KErrNone;
	}

#endif // EKA2
