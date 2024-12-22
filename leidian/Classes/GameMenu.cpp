#include "GameMenu.h"
#include "GameEvents.h"
#include <stdlib.h>
CCScene* CGameMenu::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        CGameMenu *layer = CGameMenu::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool CGameMenu::init()
{
    bool bRet = false;
    do{
		CC_BREAK_IF(! CCLayer::init());
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		
		/**	菜单列表	*/
		CCSpriteFrameCache *pCache = CCSpriteFrameCache::sharedSpriteFrameCache();
		pCache->addSpriteFramesWithFile("Image/GameMenu.plist");

		/**	菜单背景	*/
		CCSprite* m_pBackground = CCSprite::createWithSpriteFrame(pCache->spriteFrameByName("Background.png"));
		m_pBackground->setPosition(ccp(size.width/2, size.height/2));
		this->addChild(m_pBackground, 0);
		
		/**	星星	*/
		CCSprite* pStar = CCSprite::create("Image/Star.png");
		CC_BREAK_IF(! pStar);
		pStar->setPosition(ccp(size.width/2, size.height/2));
        this->addChild(pStar, 1);
		
		CCActionInterval* actionFade = CCFadeOut::create(2.0f);
		CCActionInterval* actionFadeBack = actionFade->reverse();
		CCActionInterval* seq2 = (CCActionInterval*)CCSequence::create(actionFade, actionFadeBack, NULL);
		CCRepeatForever* pRepeatForever = CCRepeatForever::create(seq2);
		pStar->runAction(pRepeatForever);

		/**	标题	*/
		CCSprite* pTitle = CCSprite::create("Image/Title.png");
		CC_BREAK_IF(! pTitle);
		pTitle->setPosition(ccp(size.width/2, size.height - 70));
        this->addChild(pTitle, 2);

		/**	开始游戏	*/
		CCMenuItemImage *pMenuStart = CCMenuItemImage::create();       
		pMenuStart->initWithTarget(this, menu_selector(CGameMenu::menuStartCallback));      
		pMenuStart->setNormalSpriteFrame(pCache->spriteFrameByName("StartNormal.png"));  
		pMenuStart->setSelectedSpriteFrame(pCache->spriteFrameByName("StartSelected.png"));
		pMenuStart->setPosition(ccp(size.width/2, 110));
		
		/**	游戏配置	*/
		CCMenuItemImage *pMenuOption = CCMenuItemImage::create();       
		pMenuOption->initWithTarget(this, menu_selector(CGameMenu::menuOptionCallback));      
		pMenuOption->setNormalSpriteFrame(pCache->spriteFrameByName("OptionNormal.png"));  
		pMenuOption->setSelectedSpriteFrame(pCache->spriteFrameByName("OptionSelected.png"));
		pMenuOption->setPosition(ccp(size.width/2, 75));

		/**	游戏结束	*/
		CCMenuItemImage *pMenuExit = CCMenuItemImage::create();       
		pMenuExit->initWithTarget(this, menu_selector(CGameMenu::menuExitCallback));      
		pMenuExit->setNormalSpriteFrame(pCache->spriteFrameByName("ExitNormal.png"));  
		pMenuExit->setSelectedSpriteFrame(pCache->spriteFrameByName("ExitSelected.png"));
		pMenuExit->setPosition(ccp(size.width/2, 40));
		 
		/**	游戏菜单	*/
		CCMenu *pMenu = CCMenu::create(pMenuStart,pMenuOption, pMenuExit, NULL);
		pMenu->setPosition(ccp(0, 0));
		this->addChild(pMenu, 3);
		
		/**	陨石掉落特效	*/
		m_pMeteorolite = CCParticleSun::create();
		m_pMeteorolite->setTexture(CCTextureCache::sharedTextureCache()->addImage("Image/Fire.png"));
		this->addChild(m_pMeteorolite, 1);
		move(0);
		this->schedule(schedule_selector(CGameMenu::move), 5.0f);
		
		/**	飞机	*/
		CCSprite* pPlane = CCSprite::create("Image/PlaneIco.png");
		CC_BREAK_IF(! pPlane);
		pPlane->setPosition(ccp(size.width/2, size.height/2));
        this->addChild(pPlane, 2);
		
		bRet = true;
	}
	while(0);

    return bRet;
}
void CGameMenu::menuStartCallback(CCObject* pSender)
{
	CGameEvents::startFlipInterface(GAME_LEVEL);
}

void CGameMenu::menuOptionCallback(CCObject* pSender)
{
	
}


void CGameMenu::menuExitCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();
}

void CGameMenu::move(float dt)
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	int x = - 40;
	int y = rand()%((int)(size.width + size.height)/2) + size.height/2;
	m_pMeteorolite->setPosition(ccp(x, y));
	m_pMeteorolite->runAction(CCMoveBy::create(5, ccp(size.width + 40, -size.height - 40)));
}