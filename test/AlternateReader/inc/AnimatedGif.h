
#ifndef ANIMATEDGIF_H_
#define ANIMATEDGIF_H_

#include <e32base.h>
#include <ICLAnimationDataProvider.h>
#include <basicanimation.h>
#include <AnimationConfig.h>


class CAnimatedGif : public CBase
	{
public:
	static CAnimatedGif* NewL(const TFileName& aFileName, RWindow& aWindow, const TPoint& aPoint);
	static CAnimatedGif* NewLC(const TFileName& aFileName, RWindow& aWindow, const TPoint& aPoint);   
	CAnimatedGif();
	virtual ~CAnimatedGif();
	void StartL();
	void Stop();
	void Draw(CWindowGc& aGc) const;
	
	void SetPosition(const TPoint& aPoint);
	const TPoint& Position(void);
	const TSize& Size(void);
	
	TBool IsStart()
		{
		return iStart;
		}
private:
	void ConstructL(const TFileName& aFileName, RWindow& aWindow, const TPoint& aPoint);
	
	CICLAnimationDataProvider* iGifProvider;
	CBasicAnimation* iGifImage;
	TBool iStart;
	};

#endif /* ANIMATEDGIF_H_ */
