//
//  CCRadioMenu.h
//  SceneHello
//
//  Created by Orangef on 12-12-24.
//
//

#ifndef __SceneHello__CCRadioMenu__
#define __SceneHello__CCRadioMenu__

#include "cocos2d.h"
class CCRadioMenu : public cocos2d::CCMenu{
public:
    CCRadioMenu():cocos2d::CCMenu(){}
    cocos2d::CCMenuItem *_curHighlighted;
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch,cocos2d::CCEvent *pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch,cocos2d::CCEvent *pEvent);
    virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch,cocos2d::CCEvent *pEvent);
    virtual void ccTouchMoved(cocos2d::CCTouch *pTouch,cocos2d::CCEvent *pEvent);
    void setSelectedItem_(cocos2d::CCMenuItem *item);
    
	static CCRadioMenu* create();
    static CCRadioMenu* create(cocos2d::CCMenuItem* item, ...);
	static CCRadioMenu* createWithItems(cocos2d::CCMenuItem *firstItem, va_list args);
	static CCRadioMenu* createWithArray(cocos2d::CCArray* pArrayOfItems);
};


#endif /* defined(__SceneHello__CCRadioMenu__) */
