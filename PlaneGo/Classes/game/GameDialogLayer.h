#ifndef GameDialogLayer_h__
#define GameDialogLayer_h__
#include "cocos2d.h"
USING_NS_CC;
class GameDialogLayer : public CCLayer {
public:
	/**_state:0游戏失败，1游戏成功*/
	static GameDialogLayer *create(int _state, int _coin, int _score);
	bool init(int _state, int _coin, int _score);
private:
	int state;
	bool isMenu;
	CCMenu *menu;
	virtual void onEnter();
	virtual void onExit();
	virtual bool ccTouchBegan(CCTouch *touch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	void menuCall(CCObject *object);
};
#endif // GameDialogLayer_h__
