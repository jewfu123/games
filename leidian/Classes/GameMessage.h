#pragma once
#include "cocos2d.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

using namespace cocos2d;

class CGameMessage : public CCLayer
{
public:
	/**	初始化函数	*/
	virtual bool init();

	/**	创建函数	*/
	CREATE_FUNC(CGameMessage);

private:
	/**	更新函数	*/
	void update(float dt);

	/**	显示分数	*/
	void showScore();

	/**	显示HP	*/
	void showHp();

	/**	显示生命	*/
	void showLife();

	/**	显示导弹	*/
	void showMissile();

	/**	发射导弹*/
	void shootMissile(CCObject* pSender);

	CCSprite *m_pHp;						///<HP血条精灵
	CCLabelAtlas *m_pScore;					///<分数标签
	CCLabelAtlas *m_pLife;					///<生命数标签
	CCMenuItemImage *m_pMissileItem;		///<导弹按钮的菜单项
	CCLabelAtlas *m_pMissileNum;			///<导弹数标签
	int m_iHeight;							///<HP血条精灵的高
	int m_iWidth;							///<HP血条精灵的宽
	float m_fMinX;							///<HP血条精灵帧在图片里的最小X值
	float m_fMinY;							///<HP血条精灵帧在图片里的最小Y值
};
