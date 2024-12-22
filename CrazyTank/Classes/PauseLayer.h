#pragma once
#ifndef __PAUSESceneSCENE_SCENE_H__
#define __PAUSESceneSCENE_SCENE_H__


#include "cocos2d.h"
#include "SimpleAudioEngine.h"

class PauseLayer : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
	void registerWithTouchDispatcher();
    static cocos2d::CCScene* scene();
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	

    CREATE_FUNC(PauseLayer);
};
#endif