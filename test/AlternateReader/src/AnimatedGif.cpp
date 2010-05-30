
#include "AnimatedGif.h"
#include <COEMAIN.H>


/**
 * Symbian two-phase constructor.
 * This creates an instance then calls the second-phase constructor
 * without leaving the instance on the cleanup stack.
 * @return new instance of CSymDjvuContainerView
 */
CAnimatedGif* CAnimatedGif::NewL(const TFileName& aFileName, RWindow& aWindow,const TPoint& aPoint)
	{
		CAnimatedGif* self = CAnimatedGif::NewLC(aFileName, aWindow, aPoint);
		CleanupStack::Pop(self);
		return self;
	}

/**
 * Symbian two-phase constructor.
 * This creates an instance, pushes it on the cleanup stack,
 * then calls the second-phase constructor.
 * @return new instance of CSymDjvuContainerView
 */
CAnimatedGif* CAnimatedGif::NewLC(const TFileName& aFileName, RWindow& aWindow, const TPoint& aPoint)
	{
		CAnimatedGif* self = new (ELeave) CAnimatedGif();
		CleanupStack::PushL(self);
		self->ConstructL(aFileName, aWindow, aPoint);
		return self;
	}

/**
 * Second-phase constructor for view.  
 * Initialize contents from resource.
 */
void CAnimatedGif::ConstructL(const TFileName& aFileName, RWindow& aWindow, const TPoint& aPoint)
	{
		iGifProvider = new (ELeave) CICLAnimationDataProvider;
		iGifProvider->SetFileL(CCoeEnv::Static()->FsSession(), aFileName);
		iGifImage = CBasicAnimation::NewL(iGifProvider , aPoint , CCoeEnv::Static()->WsSession(), aWindow);
	}

void CAnimatedGif::SetPosition(const TPoint& aPoint)
	{
		iGifImage->SetPosition(aPoint);
	}

const TPoint& CAnimatedGif::Position()
	{
		return iGifImage->Position();
	}


const TSize& CAnimatedGif::Size()
	{
		return iGifImage->Size();
	}


void CAnimatedGif::StartL()
	{
		TAnimationConfig aGifconfig;
		aGifconfig.iFlags = TAnimationConfig::ELoop;
		aGifconfig.iData = 10000;
		
		iGifImage->Start(aGifconfig);
		iStart = ETrue;

	}

void CAnimatedGif::Stop()
	{
		iGifImage->Stop();
		iStart = EFalse;
	}

void CAnimatedGif::Draw(CWindowGc& aGc) const
	{
		iGifImage->Draw(aGc);
	}

CAnimatedGif::CAnimatedGif()
	{

	}

CAnimatedGif::~CAnimatedGif()
	{
		if (iGifImage)
		{
			iGifImage->Stop();
			delete iGifImage;
		}
	}
