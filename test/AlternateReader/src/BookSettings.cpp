
#include "BookSettings.h"
#include "CDjvuReader.h"

CBookSettings::~CBookSettings()
	{

	}


CBookSettings* CBookSettings::NewLC()
	{
		CBookSettings* self = new (ELeave) CBookSettings();
		CleanupStack::PushL(self);
		self->ConstructL();
		return self;
	}

CBookSettings* CBookSettings::NewL()
	{
		CBookSettings* self = CBookSettings::NewLC();
		CleanupStack::Pop(); // self;
		return self;
	}

void CBookSettings::ConstructL()
	{

	}

void 
CBookSettings::ExternalizeL(RWriteStream& aStream) const
	{
	
		aStream << iBookName;
			
		aStream << iCurrentPage;
		
		aStream << iCursorPositionX;
		aStream << iCursorPositionY;
		aStream << iFullScreenMode;
		
		aStream << iZoom;
		aStream << iPageWidth;
	
	}

void
CBookSettings::InternalizeL(RReadStream& aStream)
	{
	
		aStream >> iBookName;
		
		aStream >> iCurrentPage;
		
		aStream >> iCursorPositionX;
		aStream >> iCursorPositionY;
		aStream >> iFullScreenMode;
		
		aStream >> iZoom;
		aStream >> iPageWidth;
	
	}


void CBookSettings::LoadSettings(TFileName fileName)
	{
		TParse parse;
	
		// Read book settings
		RFs fs;
		User::LeaveIfError(fs.Connect());
		CleanupClosePushL(fs);
	
		// File name
		parse.Set(fileName, NULL, NULL);
		TBuf<KMaxFileName> buf;
		buf.Append(parse.Name());
		buf.Append(_L(".dat"));
		
		// Catalog with drive
		RProcess process;
		TPtrC drive = process.FileName().Left(2);
		TBuf<KMaxFileName> name;
		fs.PrivatePath(name);
		//name.Insert(0, drive);
		parse.Set(name, NULL, NULL);
		parse.AddDir(_L("Books"));
		fs.MkDirAll(parse.FullName());
		name.Zero();
		name.Append(parse.FullName());
		name.Append(buf);
		
		
		RFileReadStream frs;
		TInt result = frs.Open(fs, name, EFileRead);
		CleanupClosePushL(frs);
				
		if(result == KErrNone)
		{
			InternalizeL(frs); 
		}
		else
		{
			
			RFileWriteStream fws;
			result = fws.Replace(fs, name, EFileWrite);
			CleanupClosePushL(fws);
			if(result == KErrNone)
			{
				iBookName = fileName;
				iCurrentPage = 1;
				iCursorPositionX = 0;
				iCursorPositionY = 0;
				iFullScreenMode = EFalse;
				iPageWidth = 360;
				iZoom = 1;
				
				ExternalizeL(fws);
				
			}				
			CleanupStack::PopAndDestroy(&fws);
			
		}
		
		CleanupStack::PopAndDestroy(&frs);
		CleanupStack::PopAndDestroy(&fs);
	
		iContainer->iDjVuReader->SetCurrentPage(iCurrentPage);
		iContainer->iDjVuReader->SetPageWidth(iPageWidth);
		iContainer->iDjVuReader->SetZoom(iZoom);
		
	}

void CBookSettings::ApplyCurrentSettings()
	{
		iContainer->iCursorPosition.iX = iCursorPositionX;
		iContainer->iCursorPosition.iY = iCursorPositionY;
		iContainer->SetFullScreenMode(iFullScreenMode);
	}

void CBookSettings::SaveSettings()
	{
		TParse parse;
	
		// Connecting to file system
		RFs fs;
		User::LeaveIfError(fs.Connect());
		CleanupClosePushL(fs);
	
		// File name
		parse.Set(iBookName, NULL, NULL);
		TBuf<KMaxFileName> buf;
		buf.Append(parse.Name());
		buf.Append(_L(".dat"));
		
		// Catalog with drive
		RProcess process;
		TPtrC drive = process.FileName().Left(2);
		TBuf<KMaxFileName> name;
		fs.PrivatePath(name);
		//name.Insert(0, drive);
		parse.Set(name, NULL, NULL);
		parse.AddDir(_L("Books"));
		fs.MkDirAll(parse.FullName());
		name.Zero();
		name.Append(parse.FullName());
		name.Append(buf);
		
		RFileWriteStream fws;
		TInt result = fws.Replace(fs, name, EFileWrite);
		CleanupClosePushL(fws);
		if(result == KErrNone)
		{
			iCurrentPage = iContainer->iDjVuReader->CurrentPage();
			iCursorPositionX = iContainer->iCursorPosition.iX;
			iCursorPositionY = iContainer->iCursorPosition.iY;
			iPageWidth = iContainer->iDjVuReader->GetPageWidth();
			iFullScreenMode = iContainer->iFullScreenMode;
			iZoom = iContainer->iDjVuReader->GetZoom();
			
			ExternalizeL(fws);
			
		}				
		CleanupStack::PopAndDestroy(&fws);
		CleanupStack::PopAndDestroy(&fs);
		
	}

void CBookSettings::SaveLastOpenFile(TFileName aFileName)
	{
		// Connecting to file system
		RFs fs;
		User::LeaveIfError(fs.Connect());
		CleanupClosePushL(fs);

		// Private catalog with drive
		RProcess process;
		TPtrC drive = process.FileName().Left(2);
		TBuf<KMaxFileName> name;
		fs.PrivatePath(name);
		//name.Insert(0, drive);
		fs.MkDir(name);
		name.Append(_L("LastDirectory.dat"));
		
		RFileWriteStream fws;
		User::LeaveIfError(fws.Replace(fs, name, EFileWrite));
		CleanupClosePushL(fws);
		fws << aFileName; 
		CleanupStack::PopAndDestroy(&fws);
		CleanupStack::PopAndDestroy(&fs);
	}

TBool CBookSettings::LoadLastDirectory(TFileName& aLastDirectory)
	{
		// Connecting to file system
		RFs fs;
		User::LeaveIfError(fs.Connect());
		CleanupClosePushL(fs);
	
		// Private catalog with drive
		RProcess process;
		TPtrC drive = process.FileName().Left(2);
		TBuf<KMaxFileName> name;
		fs.PrivatePath(name);
		//name.Insert(0, drive);
		fs.MkDir(name);
		name.Append(_L("LastDirectory.dat"));
		
		RFileReadStream frs;
		TInt result = frs.Open(fs, name, EFileRead);
		CleanupClosePushL(frs);
				
		if(result == KErrNone)
		{
			TFileName LastOpenFile;
			frs >> LastOpenFile;
			
			// Parse a directory name
			TParse parse;
			parse.Set(LastOpenFile, NULL, NULL);
	
			aLastDirectory.Zero();
			aLastDirectory.Append(parse.DriveAndPath());
			
			CleanupStack::PopAndDestroy(&frs);
			CleanupStack::PopAndDestroy(&fs);
			
			parse.Set(aLastDirectory, NULL, NULL);
			return parse.PathPresent();
			
		}
		else
		{
			CleanupStack::PopAndDestroy(&frs);
			CleanupStack::PopAndDestroy(&fs);
			
			return EFalse;
		}
	
	}

TBool CBookSettings::LoadLastOpenFile(TFileName& aLastFile)
	{
		// Connecting to file system
		RFs fs;
		User::LeaveIfError(fs.Connect());
		CleanupClosePushL(fs);
	
		// Private catalog with drive
		RProcess process;
		TPtrC drive = process.FileName().Left(2);
		TBuf<KMaxFileName> name;
		fs.PrivatePath(name);
		//name.Insert(0, drive);
		fs.MkDir(name);
		name.Append(_L("LastDirectory.dat"));
		
		RFileReadStream frs;
		TInt result = frs.Open(fs, name, EFileRead);
		CleanupClosePushL(frs);
				
		if(result == KErrNone)
		{
			frs >> aLastFile;
			
			CleanupStack::PopAndDestroy(&frs);
			CleanupStack::PopAndDestroy(&fs);
			
			// Parse a file name
			TParse parse;
			parse.Set(aLastFile, NULL, NULL);
			return parse.PathPresent();
			
		}
		else
		{
			CleanupStack::PopAndDestroy(&frs);
			CleanupStack::PopAndDestroy(&fs);
			
			return EFalse;
		}
	
	}
