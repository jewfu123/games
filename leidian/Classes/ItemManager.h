#pragma once

#include "cocos2d.h"

#include "EnemySprite.h"
#include "PlayerSprite.h"

using namespace cocos2d;

class CItemManager : public CCLayer
{
public:
	/**	初始化函数	*/
    virtual bool init();

	/**	创建物品	*/
	void createItem(int iType, float x, float y);
	
	/**	创建函数	*/
	CREATE_FUNC(CItemManager);

private:
	/**	更新函数	*/
	void update(float dt);

	CCSpriteBatchNode* m_pItemList;		///<物品列表
	int m_iItemCout;					///<Tag的计数值
};
