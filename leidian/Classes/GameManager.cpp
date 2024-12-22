#include "GameManager.h"
#include "Effect.h"

CPlayerSprite* CGameManager::m_pPlayer = NULL;
CBulletManager* CGameManager::m_pBulletManager = NULL;
CEnemyManager* CGameManager::m_pEnemyManager = NULL;
CItemManager* CGameManager::m_pItemManager = NULL;
CGameMap* CGameManager::m_pGameMap = NULL;

CCScene* CGameManager::scene()
{
	CCScene * scene = NULL;
	do 
	{
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		CGameManager *layer = CGameManager::create();
		CC_BREAK_IF(! layer);

		scene->addChild(layer);
	} while (0);

	return scene;
}


CGameManager* CGameManager::create()
{
	CGameManager *pRet = new CGameManager();
    if (pRet && pRet->init())
    { 
        pRet->autorelease(); 
        return pRet; 
    } 
    else 
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool CGameManager::init()
{
	bool bRet = false;
	do{
		CC_BREAK_IF(! CCLayer::init());

		char szPath[32];
		sprintf(szPath,"Music/Stage%d.mp3", g_iGameLevel);
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(szPath, true);

		CCSize size = CCDirector::sharedDirector()->getWinSize();
		/**	随机种子	*/
		cc_timeval psv;
		CCTime::gettimeofdayCocos2d(&psv, NULL);
		unsigned long int seed = psv.tv_sec*1000 + psv.tv_usec/1000;
		srand(seed);

		/**	创建共享的特效（爆炸和金币闪烁的特效用得多，所以设置共享）	*/
		CEffect::sharedExplosion();
		CEffect::sharedGold();

		/**	创建游戏地图	*/
		m_pGameMap = (CGameMap*)CGameMap::create();
		this->addChild(m_pGameMap, 0, 0);

		/**	添加物品	*/
		m_pItemManager = (CItemManager*)CItemManager::create();
		this->addChild(m_pItemManager, 1, 1);

		/**	添加子弹	*/
		m_pBulletManager = (CBulletManager*)CBulletManager::create();
		this->addChild(m_pBulletManager, 2, 2);

		/**	添加敌人	*/
		m_pEnemyManager = (CEnemyManager*)CEnemyManager::create();
		this->addChild(m_pEnemyManager, 3, 3);

		/**	添加玩家	*/
		m_pPlayer = (CPlayerSprite*)CPlayerSprite::create(0);
		this->addChild(m_pPlayer, 4, 4);

		/**	添加信息框	*/
		m_pGameMessage = (CGameMessage*)CGameMessage::create();
		this->addChild(m_pGameMessage, 5, 5);

		/**	设置返回按钮	*/
		CCMenuItemImage *pBack = CCMenuItemImage::create();       
		pBack->initWithTarget(this, menu_selector(CGameManager::back));      
		pBack->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("Back.png"));  
		pBack->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("Back.png"));
		pBack->setPosition(ccp(size.width - 39, 21));

		/**	添加菜单	*/
		CCMenu *pMenu = CCMenu::create(pBack, NULL);
		pMenu->setPosition(ccp(0, 0));
		this->addChild(pMenu, 6, 6);

		this->scheduleUpdate();

		bRet = true;
	}
	while(0);

	return bRet;
}
void CGameManager::update(float dt)
{
	if(m_pPlayer->isWin() == true && m_pPlayer->getSprite()->isVisible() == true)
	{
		m_pPlayer->setWin(false);
		m_pPlayer->initAttrWithWin();
		m_pPlayer->setTouchEnabled(true);
		g_iGameLevel ++ ;
		if(g_iGameLevel >= 4)
			g_iGameLevel = 1;

		char szPath[32];
		sprintf(szPath,"Music/Stage%d.mp3", g_iGameLevel);
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(szPath, true);

		this->removeChildByTag(0, true);
		m_pGameMap = (CGameMap*)CGameMap::create();
		this->addChild(m_pGameMap,0, 0);

		this->removeChildByTag(1, true);
		m_pItemManager = (CItemManager*)CItemManager::create();
		this->addChild(m_pItemManager, 1, 1);

		this->removeChildByTag(2, true);
		m_pBulletManager = (CBulletManager*)CBulletManager::create();
		this->addChild(m_pBulletManager, 2, 2);

		this->removeChildByTag(3, true);
		m_pEnemyManager = (CEnemyManager*)CEnemyManager::create();
		this->addChild(m_pEnemyManager, 3, 3);
	}
}

void CGameManager::back(CCObject* pSender)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Music/Title.mp3", true);
	CGameEvents::startFlipInterface(GAME_LEVEL);
}