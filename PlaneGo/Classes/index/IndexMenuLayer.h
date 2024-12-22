#ifndef IndexMenuLayer_h__
#define IndexMenuLayer_h__
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

class IndexMenuLayer : public CCLayer {
public:
	virtual bool init();
	CREATE_FUNC(IndexMenuLayer);
private:
	virtual void keyBackClicked();
	void ccTouchButton(CCObject *object, CCControlEvent event);
};
#endif // IndexMenuLayer_h__
