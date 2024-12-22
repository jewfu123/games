#ifndef SelectMenuLayer_h__
#define SelectMenuLayer_h__
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

class SelectMenuLayer : public CCLayer {
public:
	virtual bool init();
	CREATE_FUNC(SelectMenuLayer);
private:
	int num; 
	float beganX;
	int tempX;
	bool isButtonMenu, isNumMenu, isScroll, isMove;
	CCSprite *kuang;
	CCMenu *buttonMenu, *numMenu;
	CCMenuItemImage *back, *start;
	CCScrollView *scroll;
	virtual void keyBackClicked();
	void ccMenuCall(CCObject *object);
	void ccMenuCallNum(CCObject *object);
	virtual void onEnter();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	virtual bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	/**自动调节页面*/
	void adjustScrollView(CCScrollView *view);
	/**更新页数标签*/
	void updatePageTab(int page);
	/**跳转到另一个情景*/
	void toScene(int state);
	/**跳转到首页情景*/
	void toIndex();
	/**跳转到选飞机界面情景*/
	void toPlane();
};

#endif // SelectMenuLayer_h__