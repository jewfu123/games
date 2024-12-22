#pragma once
#include "cocos2d.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

using namespace cocos2d;

class CUnitSprite : public CCLayer
{
public:
	/**	获得两个点之间的距离	*/
	float getDist(float fPosX, float fPosY);

	/**	得到旋转角度	*/
	float getAngle(float fPosX, float fPosY);

	/**	获取精灵	*/
	CCSprite *getSprite(){ return m_pSprite; }

protected:
	char m_szName[32];		///<精灵图片名
	int m_iHeight;			///<精灵图片的高
	int m_iWidth;			///<精灵图片的宽
	CCSprite *m_pSprite;	///<精灵

};
