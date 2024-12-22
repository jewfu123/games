#pragma once

#include "cocos2d.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

// 这一行别忘记加了
using namespace cocos2d;

enum{
	LEVEL_ONE = 1, 
	LEVEL_TWO,  
	LEVEL_THREE,    
};

class CGameLevel : public CCLayer
{
public:
	/**	初始化函数	*/
    virtual bool init();  

	/**	场景	*/
    static CCScene* scene();

	/**	创建函数	*/
	CREATE_FUNC(CGameLevel);

private:
	/**	选择关卡的回调函数	*/
	void setLevelMode(CCObject* pSender);

	/**	开始按钮的回调函数	*/
	void setLevelStart(CCObject* pSender);

	/**	返回按钮的回调函数	*/
	void back(CCObject* pSender);

	CCParticleSun* m_pSun;		///<太阳特效

};