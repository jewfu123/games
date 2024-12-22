#ifndef __HELPLAYER_SCENE_H__
#define __HELPLAYER_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"

class HelpLayer :public cocos2d::CCLayer
{
public:
	virtual bool init();  
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
	virtual void menuCloseCallback(CCObject* pSender);
	//·µ»Ø£¬ÍË³ö
	virtual void  keyBackClicked();
	 CREATE_FUNC(HelpLayer);
};

#endif