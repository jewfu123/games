#ifndef GameMenuLayer_h__
#define GameMenuLayer_h__
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

class GameMenuLayer : public CCLayer {
public:
	virtual bool init();
	CREATE_FUNC(GameMenuLayer);
	CC_SYNTHESIZE(int , coinValue, CoinValue);
	CC_SYNTHESIZE(int , scoreValue, ScoreValue);
	void reduceLife();
	void addBomb(int value);
	void updateCoin(int _coin);
private:
	int level;
	int numBomb;
	bool isHalfway;
	bool isArrive;
	float speed;
	int coinTime, scoreTime;
	CCSprite *thumb;
	CCLabelTTF *coinLabel, *scoreLabel, *bombLabel;
	CCArray *lifeArray;
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	void ccTouchButton(CCObject *object, CCControlEvent event);
	virtual void keyBackClicked();
	void toScene(int _state);
	void toBack();
	virtual void update(float td);
	void togglemenuMusic(CCObject *pSender);
};


#endif // GameMenuLayer_h__
