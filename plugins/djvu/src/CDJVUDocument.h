/*
* ============================================================================
*  Name        : CDJVUDocument.h
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


#ifndef C_CDJVUDOCUMENT_H
#define C_CDJVUDOCUMENT_H

#include <e32base.h>

#include "CPluginInterface.h"
#include "ddjvuapi.h"

class DjVuDebug;
class CFbsBitmap;

NONSHARABLE_CLASS(CDJVUDocument): public CPluginInterface
    {
public:

    static CDJVUDocument* NewL();
    virtual ~CDJVUDocument();

// from base class
	void Open(const TDesC16& file) final;
	void Close() final;
	void Search(const TDesC16& str) final;
	void FileInfo() final;
	void GetDocIndex() final;
	void Settings() final;
	void PrevPage() final;
	void NextPage() final;
	void GoToPage(const TUint64 page) final;
	void CopyTexOnPage() final;
	void Scaling() final;
	void ScalingInPercent(const TReal32 percent) final;
	void ScalingBorderless() final;
	void ScalingByWith() final;

private:
    CDJVUDocument();
    void ConstructL();

private: // data
	ddjvu_document_s*	iDjvuDoc = nullptr;
	ddjvu_context_s*	iDjvuContext = nullptr;
	ddjvu_page_s*	    iDjvuPage = nullptr;
	ddjvu_format_s*     iDjvuFormat = nullptr;
	ddjvu_render_mode_t iDjvuRenderMode = DDJVU_RENDER_COLOR;
	DjVuDebug*          iDebug = nullptr;
	CFbsBitmap*			iBitmap = nullptr;
    int iPageNo = 0;
    int iPageCount = 0;
    };



#endif // C_CDJVUDOCUMENT_H
