/*
========================================================================
 Name        : SymDjvuDocument.cpp
 Author      : midday
 Copyright   : All right reserved by Midday ЬРав 2009
 Description : 
========================================================================
*/
// [[[ begin generated region: do not modify [Generated User Includes]
#include "SymDjvuDocument.h"
#include "SymDjvuAppUi.h"
// ]]] end generated region [Generated User Includes]

/**
 * @brief Constructs the document class for the application.
 * @param anApplication the application instance
 */
CSymDjvuDocument::CSymDjvuDocument( CEikApplication& anApplication )
	: CAknDocument( anApplication )
	{
	}

/**
 * @brief Completes the second phase of Symbian object construction. 
 * Put initialization code that could leave here.  
 */ 
void CSymDjvuDocument::ConstructL()
	{
	}
	
/**
 * Symbian OS two-phase constructor.
 *
 * Creates an instance of CSymDjvuDocument, constructs it, and
 * returns it.
 *
 * @param aApp the application instance
 * @return the new CSymDjvuDocument
 */
CSymDjvuDocument* CSymDjvuDocument::NewL( CEikApplication& aApp )
	{
	CSymDjvuDocument* self = new ( ELeave ) CSymDjvuDocument( aApp );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	return self;
	}

/**
 * @brief Creates the application UI object for this document.
 * @return the new instance
 */	
CEikAppUi* CSymDjvuDocument::CreateAppUiL()
	{
		return new ( ELeave ) CSymDjvuAppUi;
	}

void CSymDjvuDocument::OpenFileL(CFileStore*& aFileStore, RFile& aFile)
  {
	  aFileStore = NULL;
	  
	  TFileName iFileName;
	  aFile.FullName(iFileName);
	 
	  Process()->SetMainDocFileName(iFileName);
	  SetAppFileMode(EFileRead|EFileShareReadersOnly);
	 
	  aFile.Close();
	  
	  CSymDjvuAppUi* mAppUi = static_cast<CSymDjvuAppUi*>(iAppUi);
	  mAppUi->LoadFile(iFileName);
	  
  }

CFileStore* CSymDjvuDocument::OpenFileL( TBool aDoOpen, const TDesC& aFilename, RFs& aFs )
    {
		if (aDoOpen)
		{
			TFileName iFileName = aFilename;
			CSymDjvuAppUi* mAppUi = static_cast<CSymDjvuAppUi*>(iAppUi);
			mAppUi->LoadFile(iFileName);
		}
		
		return NULL;
    }
