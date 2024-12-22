#pragma once

#include "cocos2d.h"

#include "EnemySprite.h"
#include "PlayerSprite.h"

using namespace cocos2d;
class CBulletManager : public CCLayer
{
public:
	/**	初始化函数	*/
    virtual bool init();

	/**	创建子弹	*/
	void createBullet(int iPlaneType, int iType, int iNum, float x, float y);

	/**	创建函数	*/
	CREATE_FUNC(CBulletManager);

private:
	/**	更新函数	*/
	void update(float dt);

	/**	获取空的Tag值	*/
	int getFreeTag();

	CCSpriteBatchNode* m_pBulletList;	///<子弹列表
	int m_iBulletCout;					///<Tag的计数值
};
