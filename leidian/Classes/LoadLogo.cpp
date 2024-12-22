#include "LoadLogo.h"
#include "GameEvents.h"

CCScene* CLoadLogo::scene()
{
	CCScene * scene = NULL;
	do 
	{
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		CLoadLogo *layer = CLoadLogo::create();
		CC_BREAK_IF(! layer);

		scene->addChild(layer);
	} while (0);

	return scene;
}

bool CLoadLogo::init()
{
	do{
		CC_BREAK_IF(! CCLayer::init());
		CCSize size = CCDirector::sharedDirector()->getWinSize();

		CCSprite* pSprite = CCSprite::create("Image/LoadLogo.png");
		CC_BREAK_IF(! pSprite);

		pSprite->setPosition(ccp(size.width/2, size.height/2));
		this->addChild(pSprite, 0);

		this->scheduleOnce(schedule_selector(CLoadLogo::makeTransition), 1.0f);
	}
	while(0);

	return true;
}

void CLoadLogo::makeTransition(float dt)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Music/Title.mp3", true);
	CGameEvents::startFlipInterface(GAME_MENU);
}