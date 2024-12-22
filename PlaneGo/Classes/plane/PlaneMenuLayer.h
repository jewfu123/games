#ifndef PlaneMenuLayer_h__
#define PlaneMenuLayer_h__
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

class PlaneMenuLayer : public CCLayer {
public:
	static PlaneMenuLayer *create(int _num);
	bool init(int _num);
private:
	int maxNum;
	int num;
	int tempX;
	bool isTouchLeftOrRight;
	CCMenu *buttonMenu, *numMenu;
	CCMenuItemImage *back, *start;
	CCScrollView *scroll;
	virtual void keyBackClicked();
	virtual void onEnter();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	virtual bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
	virtual void ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
	virtual void ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
	void ccMenuCall(CCObject *object);
	/**自动调节页面*/
	void adjustScrollView(CCScrollView *view);
	/**更新页数标签*/
	void updatePageTab(int page);
	/**跳转到另一个情景*/
	void toScene(int state);
	/**跳转到首页情景*/
	void toSelect();
	/**跳转到游戏主界面情景*/
	void toGame();
};

#endif // PlaneMenuLayer_h__
