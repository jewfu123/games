#ifndef GameSettingLayer_h__
#define GameSettingLayer_h__
#include "cocos2d.h"
USING_NS_CC;

class GameSettingLayer : public CCLayer {
public:
	virtual bool init();
	CREATE_FUNC(GameSettingLayer);
private:
	bool isMenu;
	CCMenu *menu;
	void menuCall(CCObject *object);
	virtual void onEnter();
	virtual void onExit();
	virtual bool ccTouchBegan(CCTouch *touch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
};


#endif // GameSettingLayer_h__
