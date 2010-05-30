/*
========================================================================
 Name        : SymDjvuDocument.h
 Author      : midday
 Copyright   : All right reserved by Midday ЬРав 2009
 Description : 
========================================================================
*/
#ifndef SYMDJVUDOCUMENT_H
#define SYMDJVUDOCUMENT_H

#include <akndoc.h>
		
class CEikAppUi;

/**
* @class	CSymDjvuDocument SymDjvuDocument.h
* @brief	A CAknDocument-derived class is required by the S60 application 
*           framework. It is responsible for creating the AppUi object. 
*/
class CSymDjvuDocument : public CAknDocument
	{
public: 
	// constructor
	static CSymDjvuDocument* NewL( CEikApplication& aApp );

private: 
	// constructors
	CSymDjvuDocument( CEikApplication& aApp );
	void ConstructL();
	
public: 
	
	// from base class CEikDocument
	CEikAppUi* CreateAppUiL();
	
	void OpenFileL(CFileStore*& aFileStore, RFile& aFile);
	CFileStore* OpenFileL( TBool aDoOpen, const TDesC& aFilename, RFs& aFs );
	
	};
#endif // SYMDJVUDOCUMENT_H
