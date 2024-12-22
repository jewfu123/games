#pragma once
#ifndef __USERDATA_SCENE_H__
#define __USERDATA_SCENE_H__

#include "cocos2d.h"

class UserDataConsistencyCCSprite :public cocos2d::CCSprite
{
public:
	~UserDataConsistencyCCSprite(void);


	virtual    const char*   getName(); //类型名字
	virtual     bool         getDestroy(); //判断是否销毁
	virtual    const char*   getLayerName(); //所在层的名字
};

#endif

