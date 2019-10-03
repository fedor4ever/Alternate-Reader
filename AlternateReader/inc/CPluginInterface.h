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

#ifndef CPLUGININTERFACE_H_
#define CPLUGININTERFACE_H_

#include <e32def.h>
#include <e32base.h>
class TDesC16;
class TDesC8;

class CPluginInterface: public CBase
	{
public:
	virtual void Open(const TDesC16& file) = 0;
	virtual void Close() = 0;
	virtual void Search(const TDesC16& str) = 0;
	virtual void FileInfo() = 0;
	virtual void GetDocIndex() = 0;
	virtual void Settings() = 0;
	virtual void PrevPage() = 0;
	virtual void NextPage() = 0;
	virtual void GoToPage(const TUint64 page) = 0;
	virtual void CopyTexOnPage() = 0;
	virtual void Scaling() = 0;
	virtual void ScalingInPercent(const TReal32 percent) = 0;
	virtual void ScalingBorderless() = 0;
	virtual void ScalingByWith() = 0;
	};

#endif /* CPLUGININTERFACE_H_ */
