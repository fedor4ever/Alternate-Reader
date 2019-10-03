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
#include <utf.h>

#include "CDJVUDocument.h"
#include "debug.h"

void handle_ddjvu_messages(ddjvu_context_t* iDjvuContext, TBool aWait);


CDJVUDocument::CDJVUDocument(){}


CDJVUDocument* CDJVUDocument::NewL()
{
    CDJVUDocument* self = new(ELeave) CDJVUDocument();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
}


CDJVUDocument::~CDJVUDocument()
{
	if(iDjvuDoc)
		ddjvu_document_release(iDjvuDoc);
	if(iDjvuContext)
		ddjvu_context_release(iDjvuContext);
	if(iDjvuFormat)
		ddjvu_format_release(iDjvuFormat);
}


void CDJVUDocument::ConstructL()
{
	if(!(iDjvuFormat = ddjvu_format_create(DDJVU_FORMAT_BGR24, 0, 0)))
	{
		//report error
		User::Leave(2);
	}
	ddjvu_format_set_row_order(iDjvuFormat, 1);
}


void CDJVUDocument::Open(const TDesC16& aFileName)
{
	TBuf8<256> utf8name;
	CnvUtfConverter::ConvertFromUnicodeToUtf8(utf8name, aFileName);

	if (iDjvuDoc)
	{
		ddjvu_document_release(iDjvuDoc);
		iDjvuDoc = NULL;
	}
	if (iDjvuContext)
	{
		ddjvu_context_release(iDjvuContext);
		iDjvuContext = NULL;
	}

	if(!(iDjvuContext = ddjvu_context_create("AlternateReader")))
	{
//		error report
		User::Leave(2);
	}
	ddjvu_cache_set_size(iDjvuContext, 2097152);

	iDjvuDoc = ddjvu_document_create_by_filename(iDjvuContext,(char*)utf8name.PtrZ(), true);
	while (!ddjvu_document_decoding_done(iDjvuDoc))
	{
		handle_ddjvu_messages(iDjvuContext, ETrue);
	}

	iPageCount = ddjvu_document_get_pagenum(iDjvuDoc);
}
	
	
void CDJVUDocument::Close()
{
	;
}
		
		
void CDJVUDocument::Search(const TDesC16& str)
{
	;
}
			
			
void CDJVUDocument::FileInfo()
{
	;
}
					

void CDJVUDocument::GetDocIndex()
{
	;
}
					

void CDJVUDocument::Settings()
{
	;
}
					

void CDJVUDocument::PrevPage()
{
	;
}
					

void CDJVUDocument::NextPage()
{
	;
}
				

void CDJVUDocument::GoToPage(const TUint64 page)
{
	;
}
					

void CDJVUDocument::CopyTexOnPage()
{
	;
}
				

void CDJVUDocument::Scaling()
{
	;
}
				

void CDJVUDocument::ScalingInPercent(const TReal32 percent)
{
	;
}
					

void CDJVUDocument::ScalingBorderless()
{
	;
}
				

void CDJVUDocument::ScalingByWith()
{
	;
}


void handle_ddjvu_messages(ddjvu_context_t* iDjvuContext, TBool aWait)
{
	if(!iDjvuContext)
		User::Leave(3);

	const ddjvu_message_t *msg;

	if (aWait)
		ddjvu_message_wait(iDjvuContext);

	while ((msg = ddjvu_message_peek(iDjvuContext)))
	{
		switch (msg->m_any.tag)
		{
			case DDJVU_ERROR:
				User::Leave(DDJVU_ERROR);
				break;
			default:
				break;
		}
		ddjvu_message_pop(iDjvuContext);
	}
	delete msg;
}


