#pragma once

#include "cocos2d.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

using namespace cocos2d;

class CMoveTrackArray
{
public:
	/**	获取敌人的移动轨迹数组	*/
	static CCPointArray *getEnemyArray(int iType);

	/**	获取BOSS的移动轨迹数组	*/
	static CCPointArray *getBossArray(int iType, int iHeight, int iWidth);

private:
	/**	敌人的移动轨迹数组1	*/
	static CCPointArray *getEnemyArray1();

	/**	敌人的移动轨迹数组2	*/
	static CCPointArray *getEnemyArray2(int iDirection);

	/**	BOSS的移动轨迹数组1	*/
	static CCPointArray *getBossArray1(int iHeight, int iWidth);

	/**	BOSS的移动轨迹数组2	*/
	static CCPointArray *getBossArray2(int iHeight, int iWidth);

	/**	BOSS的移动轨迹数组3	*/
	static CCPointArray *getBossArray3(int iHeight, int iWidth);

};
