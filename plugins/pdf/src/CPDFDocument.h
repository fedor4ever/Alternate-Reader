/*
* ============================================================================
*  Name        : CPDFDocument.h
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

NONSHARABLE_CLASS(CPDFDocument) : public CPluginInterface
    {
public:

    static CPDFDocument* NewL();
    virtual ~CPDFDocument();

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

//private, non-derived functions go here, starting with the C++
//constructor and the Symbian second-phase constructor
	CPDFDocument();
    void ConstructL();

private: // data


    };

#endif // C_CDJVUDOCUMENT_H
