#pragma once

#include "cocos2d.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

#include "EnemyManager.h"
#include "PlayerSprite.h"
#include "BulletManager.h"
#include "ItemManager.h"
#include "GameMap.h"
#include "GameMessage.h"
#include "GameEvents.h"

using namespace cocos2d;

class CGameManager : public CCLayer
{
public:
	/**	更新函数	*/
    virtual bool init();
	
	/**	场景	*/
	static CCScene* scene();

	/**	获取玩家类	*/
	static CPlayerSprite* getPlayer(){return m_pPlayer;}

	/**	获取子弹管理类	*/
	static CBulletManager* getBulletManager(){return m_pBulletManager;}

	/**	获取敌人管理类	*/
	static CEnemyManager* getEnemyManager(){return m_pEnemyManager;}

	/**	获取物品管理类	*/
	static CItemManager* getItemManager(){return m_pItemManager;}

	/**	获取地图类	*/
	static CGameMap* getGameMap(){return m_pGameMap;}

	/**	创建函数	*/
	static CGameManager* create();

private:
	/**	更新函数	*/
	void update(float dt);

	/**	返回按钮的回调函数	*/
	void back(CCObject* pSender);

	static CEnemyManager *m_pEnemyManager;		///<敌人管理
	static CPlayerSprite *m_pPlayer;			///<玩家管理
	static CBulletManager *m_pBulletManager;	///<子弹管理
	static CItemManager *m_pItemManager;		///<物品管理
	static CGameMap *m_pGameMap;				///<游戏地图
	CGameMessage *m_pGameMessage;				///<玩家信息
};
