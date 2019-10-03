/*
* ============================================================================
*  Name        : CDJVUDocument.cpp
*  Part of     : plugins / Alternate-Reader
*  Description : ?Description
*  Version     : %version: %
*
*  Copyright © 2019 LGPL.
*  All rights reserved.
*  This component and the accompanying materials are made available
*  under the terms of the License "Symbian Foundation License v1.0"
*  which accompanies this distribution, and is available
*  at the URL "http://www.symbianfoundation.org/legal/sfl-v10.html".
*
*  Initial Contributors:
*  Fedor Strizhniou - initial contribution.
*
*  Contributors:
*  Fedor Strizhniou
* ============================================================================
*
*/

#include <e32base.h>
#include "CPDFDocument.h"

CPDFDocument::CPDFDocument()
{
}


CPDFDocument* CPDFDocument::NewL()
{
	CPDFDocument* self = new( ELeave ) CPDFDocument;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
}


CPDFDocument::~CPDFDocument()
{
}


void CPDFDocument::ConstructL()
{
}


// from base class MDocument
void CPDFDocument::Open(const TDesC16& file)
{
	;
}
	
	
void CPDFDocument::Close()
{
	;
}
		
		
void CPDFDocument::Search(const TDesC16& str)
{
	;
}
			
			
void CPDFDocument::FileInfo()
{
	;
}
					

void CPDFDocument::GetDocIndex()
{
	;
}
					

void CPDFDocument::Settings()
{
	;
}
				

void CPDFDocument::PrevPage()
{
	;
}
				

void CPDFDocument::NextPage()
{
	;
}
					

void CPDFDocument::GoToPage(const TUint64 page)
{
	;
}
				

void CPDFDocument::CopyTexOnPage()
{
	;
}
					

void CPDFDocument::Scaling()
{
	;
}
					

void CPDFDocument::ScalingInPercent(const TReal32 percent)
{
	;
}
				

void CPDFDocument::ScalingBorderless()
{
	;
}
				

void CPDFDocument::ScalingByWith()
{
	;
}
					

	
	
	
