#pragma once
#ifndef __USERDATA_SCENE_H__
#define __USERDATA_SCENE_H__

#include "cocos2d.h"

class UserDataConsistencyCCSprite :public cocos2d::CCSprite
{
public:
	~UserDataConsistencyCCSprite(void);


	virtual    const char*   getName(); //��������
	virtual     bool         getDestroy(); //�ж��Ƿ�����
	virtual    const char*   getLayerName(); //���ڲ������
};

#endif

