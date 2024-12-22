#pragma once

#include "cocos2d.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

using namespace cocos2d;

class CLoadLogo : public CCLayer
{
public:
	/**	初始化函数	*/
    virtual bool init();  

	/**	场景	*/
    static CCScene* scene();

	/**	创建函数	*/
    CREATE_FUNC(CLoadLogo);

private:
	/**	切换场景	*/
	void makeTransition(float dt);
};

