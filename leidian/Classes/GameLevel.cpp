#include "GameLevel.h"
#include "CCRadioMenu.h"
#include "GameEvents.h"


CCScene* CGameLevel::scene()
{
	CCScene * scene = NULL;
	do 
	{
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		CGameLevel *layer = CGameLevel::create();
		CC_BREAK_IF(! layer);

		scene->addChild(layer);
	} while (0);

	return scene;
}

bool CGameLevel::init()
{
	bool bRet = false;
	do{
		CC_BREAK_IF(! CCLayer::init());
		CCSize size = CCDirector::sharedDirector()->getWinSize();

		// 选关列表
		CCSpriteFrameCache *pCache = CCSpriteFrameCache::sharedSpriteFrameCache();
		pCache->addSpriteFramesWithFile("Image/GameLevel.plist");

		// 选关背景
		CCSpriteFrame* pBG = pCache->spriteFrameByName("GameLevelBG.png");
		CCSprite* pSprite = CCSprite::createWithSpriteFrame(pBG);
		pSprite->setPosition(ccp(size.width/2, size.height/2));
		this->addChild(pSprite, 0);
		
		/**背景特效	*/
		CCParticleSystemQuad *emitter = new CCParticleSystemQuad();
		emitter->autorelease();
		emitter->initWithFile("Image/Phoenix.plist");
		emitter->setPosition(ccp(size.width/2, size.height/2));
		this->addChild(emitter, 1);

		/**	关卡一按钮	*/
		cocos2d::CCMenuItemImage *pLevelOne = cocos2d::CCMenuItemImage::create();
		pLevelOne->initWithTarget(this, menu_selector(CGameLevel::setLevelMode));
		pLevelOne->setNormalSpriteFrame(pCache->spriteFrameByName("StartLevel1Normal.png")); 
		pLevelOne->setSelectedSpriteFrame(pCache->spriteFrameByName("StartLevel1Selected.png"));
		pLevelOne->setPosition(ccp(size.width*0.14f, size.height/2));
		pLevelOne->m_uID = LEVEL_ONE;

		/**	关卡二按钮	*/
		cocos2d::CCMenuItemImage *pLevelTwo = cocos2d::CCMenuItemImage::create();
		pLevelTwo->initWithTarget(this, menu_selector(CGameLevel::setLevelMode));
		pLevelTwo->setNormalSpriteFrame(pCache->spriteFrameByName("StartLevel2Normal.png")); 
		pLevelTwo->setSelectedSpriteFrame(pCache->spriteFrameByName("StartLevel2Selected.png"));
		pLevelTwo->setPosition(ccp(size.width/2, size.height*0.8f));
		pLevelTwo->m_uID = LEVEL_TWO;

		/**	关卡三按钮	*/
		cocos2d::CCMenuItemImage *pLevelThree = cocos2d::CCMenuItemImage::create();
		pLevelThree->initWithTarget(this, menu_selector(CGameLevel::setLevelMode));
		pLevelThree->setNormalSpriteFrame(pCache->spriteFrameByName("StartLevel3Normal.png")); 
		pLevelThree->setSelectedSpriteFrame(pCache->spriteFrameByName("StartLevel3Selected.png"));
		pLevelThree->setPosition(ccp(size.width*0.86f, size.height/2));
		pLevelThree->m_uID = LEVEL_THREE;

		/**	游戏开始按钮	*/
		cocos2d::CCMenuItemImage *pStart = cocos2d::CCMenuItemImage::create();
		pStart->initWithTarget(this, menu_selector(CGameLevel::setLevelStart));
		pStart->setNormalSpriteFrame(pCache->spriteFrameByName("GameStartNormal.png")); 
		pStart->setSelectedSpriteFrame(pCache->spriteFrameByName("GameStartSelected.png"));
		pStart->setPosition(ccp(size.width/2, size.height*0.2f));

		/**	返回按钮	*/
		CCMenuItemImage *pBack = CCMenuItemImage::create();       
		pBack->initWithTarget(this, menu_selector(CGameLevel::back));      
		pBack->setNormalSpriteFrame(pCache->spriteFrameByName("Back.png"));  
		pBack->setSelectedSpriteFrame(pCache->spriteFrameByName("Back.png"));
		pBack->setAnchorPoint(ccp(1,0));
		//pBack->setPosition(ccp(size.width - 39, 21));
		pBack->setPosition(ccp(size.width, 0));

		/** 创建单项按钮管理对象 CCMenu	*/
		CCRadioMenu *pRadioMenu = CCRadioMenu::create(pLevelOne, pLevelTwo, pLevelThree, NULL);
		pRadioMenu->setPosition(ccp(0,0));
		pRadioMenu->setSelectedItem_(pLevelOne);
		pLevelOne->selected();   
		g_iGameLevel = LEVEL_ONE;
		this->addChild(pRadioMenu,2);

		/**	菜单	*/
		CCMenu *pMenu = CCMenu::create(pStart, pBack, NULL);
		pMenu->setPosition(ccp(0, 0));
		this->addChild(pMenu, 3);

		/**	选中特效	*/
		m_pSun = CCParticleSun::create();
		pSprite->addChild(m_pSun, 1);
		m_pSun->setTexture(CCTextureCache::sharedTextureCache()->addImage("Image/Fire.png") );
		m_pSun->setPosition(ccp(45, size.height/2));
		m_pSun->setStartSize(110);
	
		bRet = true;
	}
	while(0);

	return bRet;
}

void CGameLevel::setLevelMode(CCObject* pSender)
{
	g_iGameLevel = pSender->m_uID;
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	switch(g_iGameLevel)
	{
	case LEVEL_ONE:
		m_pSun->setPosition(ccp(45, size.height/2));
		break;
	case LEVEL_TWO:
		m_pSun->setPosition(ccp(size.width/2, size.height - 100));
		break;
	case LEVEL_THREE:
		m_pSun->setPosition(ccp(size.width - 45, size.height/2));
		break;
	default:
		break;
	}
}

void CGameLevel::setLevelStart(CCObject* pSender)
{
	CGameEvents::startFlipInterface(GAME_START);
}

void CGameLevel::back(CCObject* pSender)
{
	CGameEvents::startFlipInterface(GAME_MENU);
}