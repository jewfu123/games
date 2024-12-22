#include "GameOver.h"
#include "GameEvents.h"

CCScene* CGameOver::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        CGameOver *layer = CGameOver::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

bool CGameOver::init()
{
    bool bRet = false;

    do{
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		
		/**	背景	*/
		CCSprite *pBackGround = CCSprite::create("Image/GameOver.png");
		CC_BREAK_IF(! pBackGround);
		pBackGround->setPosition(ccp(size.width/2, size.height/2));
		this->addChild(pBackGround, 0);

		/**	设置返回按钮	*/
		CCMenuItemImage *pBack = CCMenuItemImage::create();       
		pBack->initWithTarget(this, menu_selector(CGameOver::back));      
		pBack->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("Back.png"));  
		pBack->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("Back.png"));
		pBack->setPosition(ccp(size.width - 39, 21));

		/**	添加菜单	*/
		CCMenu *pMenu = CCMenu::create(pBack, NULL);
		pMenu->setPosition(ccp(0, 0));
		this->addChild(pMenu, 6);

		bRet = true;
	}while(0);

	return bRet;
}

void CGameOver::back(CCObject* pSender)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Music/Title.mp3", true);
	CGameEvents::startFlipInterface(GAME_MENU);
}