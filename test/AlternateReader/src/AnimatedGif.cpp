
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
		#ifdef _S603xF1_
		iGifProvider = new (ELeave) CICLAnimationDataProvider;
		iGifProvider->SetFileL(CCoeEnv::Static()->FsSession(), aFileName);
		iGifImage = CBasicAnimation::NewL(iGifProvider , aPoint , CCoeEnv::Static()->WsSession(), aWindow);
		#endif
	}

void CAnimatedGif::SetPosition(const TPoint& aPoint)
	{
		#ifdef _S603xF1_
		iGifImage->SetPosition(aPoint);
		#endif
	}

const TPoint& CAnimatedGif::Position()
	{
		#ifdef _S603xF1_
		return iGifImage->Position();
		#else
		return TPoint(0,0);
		#endif
	}


const TSize& CAnimatedGif::Size()
	{
		#ifdef _S603xF1_
		return iGifImage->Size();
		#else
		return TSize(0,0);
		#endif
	}


void CAnimatedGif::StartL()
	{
		#ifdef _S603xF1_
		TAnimationConfig aGifconfig;
		aGifconfig.iFlags = TAnimationConfig::ELoop;
		aGifconfig.iData = 10000;
		
		iGifImage->Start(aGifconfig);
		#endif
		
		iStart = ETrue;
	
	}

void CAnimatedGif::Stop()
	{
		#ifdef _S603xF1_
		iGifImage->Stop();
		#endif
		
		iStart = EFalse;
		
	}

void CAnimatedGif::Draw(CWindowGc& aGc) const
	{
		#ifdef _S603xF1_
		iGifImage->Draw(aGc);
		#endif
	}

CAnimatedGif::CAnimatedGif()
	{

	}

CAnimatedGif::~CAnimatedGif()
	{
		#ifdef _S603xF1_
		if (iGifImage)
		{
			iGifImage->Stop();
			delete iGifImage;
		}
		#endif
	}
