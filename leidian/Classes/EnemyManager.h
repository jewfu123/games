#pragma once

#include "cocos2d.h"

#include "EnemySprite.h"
#include "BossSprite.h"


using namespace cocos2d;

class CEnemyManager : public CCLayer
{
public:
	/**	初始化函数	*/
    virtual bool init();

	/**	获取怪物列表	*/
	CCSpriteBatchNode* getEnemyList(){return m_pEnemyList;}
	
	/**	获取Tag计数值	*/
	int getCount(){return m_iEnemyCout;}

	/**	创建函数	*/
	CREATE_FUNC(CEnemyManager);

private:
	/**	更新函数	*/
	void update(float dt);

	/**	添加一只敌人	*/
	void addEnemy();

	/**	创建BOSS	*/
	void createBoss(float dt);

	/**	创建新敌人	*/
	void createNewEnemys(float dt);

	CCSpriteBatchNode* m_pEnemyList;	///<敌人列表（不含BOSS）
	int	m_iEnemyCout;					///<Tag的计数值
};
