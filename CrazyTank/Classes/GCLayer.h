#ifndef __GCLAYER_LAYER_H__
#define __GCLAYER_LAYER_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "Tool.h"
#include "cocos-ext.h"

class GCLayer :public cocos2d::CCLayer
{
public:
	GCLayer(void);
	~GCLayer(void);

	int loadingIndex;

	static cocos2d::CCScene* scene();
	virtual bool init();  
	void goEnd();

	void skipCallback(CCObject* pSender);

	void yesCallback(CCObject* pSender);
	void noCallback(CCObject* pSender);

	 void registerWithTouchDispatcher(void);
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	void logoEnd();
	void LoadData(float time);
	void update(float time);
	CREATE_FUNC(GCLayer);

};

#endif