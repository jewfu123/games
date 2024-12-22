#pragma once

#include "cocos2d.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

using namespace cocos2d;

class CGameMenu : public CCLayer
{
public:
	/**	初始化函数	*/
    virtual bool init();  

	/**	场景	*/
    static CCScene* scene();

    CREATE_FUNC(CGameMenu);

private:
	/**	开始按钮的回调函数	*/
    void menuStartCallback(CCObject* pSender);

	/**	配置按钮的回调函数	*/
	void menuOptionCallback(CCObject* pSender);

	/**	退出按钮的回调函数	*/
	void menuExitCallback(CCObject* pSender);

	/**	陨石移动	*/
	void move(float dt);

	CCParticleSun* m_pMeteorolite;		///<陨石特效
};
