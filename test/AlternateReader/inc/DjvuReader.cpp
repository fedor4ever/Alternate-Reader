/*
 * DjvuReader.cpp
 *
 *  Created on: 28.05.2010
 *      Author: 1
 */

#include "DjvuReader.h"

DjvuReader::DjvuReader()
	{
	
		iDjvuRenderMode = DDJVU_RENDER_COLOR;
		iBitmapCopyWaiting = EFalse;
		iOpen = EFalse;
		
		iDjvuContext = NULL;
		iDjvuDoc     = NULL;
		iDjvuPage    = NULL;
		
		if (!(iDjvuFormat = ddjvu_format_create(DDJVU_FORMAT_BGR24, 0, 0)))
		{
			User::Leave(2);
		}
		ddjvu_format_set_row_order(iDjvuFormat, 1);
	
	}

DjvuReader::~DjvuReader()
	{
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
		if (iDjvuFormat)
		{
			ddjvu_format_release(iDjvuFormat);
			iDjvuFormat = NULL;
		}
		
		//delete iBitmap;
	}

void Handle_ddjvu_messages(ddjvu_context_t* iDjvuContext, TBool aWait)
	{
		if (!iDjvuContext)
			User::Leave(3);
	
		const ddjvu_message_t *msg;
	
		if (aWait)
		{
			ddjvu_message_wait(iDjvuContext);
		}
		
		while ((msg = ddjvu_message_peek(iDjvuContext)))
			{
			
			switch (msg->m_any.tag)
				{
					case DDJVU_ERROR:
						
						User::Leave(4);
						
						break;
					
					default:
						
						break;
				}
			
			ddjvu_message_pop(iDjvuContext);
			}
		delete msg;
	}

void DjvuReader::OpenL(const TDesC& aFileName)
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

		iDjvuContext = static_cast<ddjvu_context_t*>(User::LeaveIfNull(ddjvu_context_create("djview"))) ;
		ddjvu_cache_set_size(iDjvuContext, 2097152);
		
		iDjvuDoc = static_cast<ddjvu_document_t*>(User::LeaveIfNull(ddjvu_document_create_by_filename(iDjvuContext,(char*) utf8name.PtrZ(), true)));
		while (!ddjvu_document_decoding_done(iDjvuDoc))
		{
			Handle_ddjvu_messages(iDjvuContext, ETrue);
		}
		
		iPageCount = ddjvu_document_get_pagenum(iDjvuDoc);
		
		// Default settings
		iZoomK = 1;
		iOpen = ETrue;
		iPageWidth = 360;
		
		iCurrentPage = 0;
		
		//RenderPageL(0);
		
	}

void DjvuReader::BitmapCopyL()
	{
		
		if (iDjvuPage)
		{
		
			ddjvu_rect_t prect;

			iImageWidth = ddjvu_page_get_width(iDjvuPage);
			iImageHeight = ddjvu_page_get_height(iDjvuPage);
			
			// Calculating image size...
			prect.w = (TReal)iPageWidth * iZoomK;
			prect.h = ((TReal)prect.w * (TReal)iImageHeight) / (TReal)iImageWidth;

			iRowsize = prect.w * 3;
			while (iRowsize%4){
				iRowsize += 3;
			}
			
			iRealWidth = prect.w;
			iRealHeight = prect.h;		
			// Calculating image size done...		
			
			TSize new_size(iRealWidth, iRealHeight);
			if (iBitmap->SizeInPixels() != new_size)
			{
				iBitmap->Reset();
				iBitmap->Create(new_size, EColor16M);
			}
			
			iBitmap->LockHeap();
			char *image = (char*) iBitmap->DataAddress();
			int result = ddjvu_page_render(iDjvuPage, iDjvuRenderMode, &prect, &prect, iDjvuFormat, iRowsize, image);
			if (!result)
			{
				memset(image, 0xFF, iRowsize * iRealHeight);
			}
			iBitmap->UnlockHeap();			
			
			ddjvu_page_release(iDjvuPage);
			iDjvuPage = NULL;
					
		}
		
		iBitmapCopyWaiting = ETrue;
	
	}

void DjvuReader::RenderPageL(TInt aPage)
	{
		
		if (aPage >= 0)		
		{
			iCurrentPage =  aPage >= iPageCount?iPageCount-1:aPage;
		}
		
		if (iDjvuPage)
		{
			User::Leave(5);
		}
				
		iDjvuPage = static_cast<ddjvu_page_t*>( User::LeaveIfNull(ddjvu_page_create_by_pageno(iDjvuDoc, iCurrentPage)));
		
		while (!ddjvu_page_decoding_done(iDjvuPage))
		{
			Handle_ddjvu_messages(iDjvuContext, ETrue);
		}
		
		if (ddjvu_page_decoding_error(iDjvuPage))
		{	
			User::Leave(1);
		}
		
		BitmapCopyL();				
			
	}

void DjvuReader::RenderPageWithoutBitmapCopyL(TInt aPage)
	{
		
		if (aPage >= 0)		
		{
			iCurrentPage =  aPage >= iPageCount?iPageCount-1:aPage;
		}
		
		if (iDjvuPage)
		{
			User::Leave(5);
		}
				
		iDjvuPage = static_cast<ddjvu_page_t*>( User::LeaveIfNull(ddjvu_page_create_by_pageno(iDjvuDoc, iCurrentPage)));
		
		while (!ddjvu_page_decoding_done(iDjvuPage))
		{
			Handle_ddjvu_messages(iDjvuContext, ETrue);
		}
		
		if (ddjvu_page_decoding_error(iDjvuPage))
		{	
			User::Leave(1);
		}
		
		iBitmapCopyWaiting = ETrue;
		
	}
