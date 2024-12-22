#ifndef PlayerLayer_h__
#define PlayerLayer_h__
#include "cocos2d.h"
#include "PBulletSprite0.h"
USING_NS_CC;

class PlaneSprite;

class PlayerLayer : public CCLayer {
public:
	virtual bool init();
	void removeBullet(PBulletSprite0 *_bullet);
	CREATE_FUNC(PlayerLayer);
	PlaneSprite *getPlayer();
	void again();
	void updateLevel();
	void removerAllBullet();
private:
	int ddTime;
	PlaneSprite *player;
	CCSpriteBatchNode *batchNode;
	void autoCreateBullet(float dt);
	virtual void onEnter();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	void setShoot(CCNode *node);
	void setPlayerAlive();
};

#endif // PlayerLayer_h__
