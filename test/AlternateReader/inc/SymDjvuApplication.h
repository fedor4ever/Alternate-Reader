/*
========================================================================
 Name        : SymDjvuApplication.h
 Author      : midday
 Copyright   : All right reserved by Midday ЬРав 2009
 Description : 
========================================================================
*/
#ifndef SYMDJVUAPPLICATION_H
#define SYMDJVUAPPLICATION_H

// [[[ begin generated region: do not modify [Generated Includes]
#include <aknapp.h>
// ]]] end generated region [Generated Includes]

// [[[ begin generated region: do not modify [Generated Constants]
const TUid KUidSymDjvuApplication = { 0xE1A0B4F1 };
// ]]] end generated region [Generated Constants]

/**
 *
 * @class	CSymDjvuApplication SymDjvuApplication.h
 * @brief	A CAknApplication-derived class is required by the S60 application 
 *          framework. It is subclassed to create the application's document 
 *          object.
 */
class CSymDjvuApplication : public CAknApplication
	{
private:
	TUid AppDllUid() const;
	CApaDocument* CreateDocumentL();
	
	};
			
#endif // SYMDJVUAPPLICATION_H		
