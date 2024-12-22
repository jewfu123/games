#pragma once

#include "cocos2d.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

// 中文字体转换需要的库，在属性里再加一个：libiconv.lib
//#include "iconv/iconv.h"

#include "GameLevel.h"

using namespace cocos2d;

enum{
	GAME_MENU = 1,  // 游戏菜单,
	GAME_LEVEL,     // 游戏关卡
	GAME_START,     // 游戏开始
	GAME_OVER,      // 游戏结束
};
extern int g_iGameLevel;		// 游戏关卡

class CGameEvents : public CCLayer
{
public:
	/**	开始翻屏	*/
	static void startFlipInterface(int iType);
	
	/**	创建函数	*/
    CREATE_FUNC(CGameEvents);

};