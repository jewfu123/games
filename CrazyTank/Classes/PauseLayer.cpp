#include "PauseLayer.h"
#include "Tool.h"
#include "GameScene.h"

using namespace cocos2d;
using namespace CocosDenshion;


CCScene* PauseLayer::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
		scene = CCScene::create();
        CC_BREAK_IF(! scene);
        // 'layer' is an autorelease object
        PauseLayer *layer = PauseLayer::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool PauseLayer::init()
{
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

        CC_BREAK_IF(! CCLayer::init());

        //////////////////////////////////////////////////////////////////////////
        // add your codes below...
        //////////////////////////////////////////////////////////////////////////

        // 1. Add a menu item with "X" image, which is clicked to quit the program.

        // Create a "close" menu item with close icon, it's an auto release object.
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		CCLayerColor  *clc =  CCLayerColor::create(ccc4(0, 0, 0, 255));
	    this->addChild(clc,1,15);

		CCSprite* groundHelp = CCSprite::create("PauseLayer.png");
		groundHelp->setPosition(ccp(size.width/2,size.height/2));
		this->addChild(groundHelp, 1);

		this->setTouchEnabled(true);

	   bRet = true;
    } while (0);
	//this->scheduleUpdate();
	
    return bRet;
}
//单点触摸
void PauseLayer::registerWithTouchDispatcher(){
	CCDirector* pDirector = CCDirector::sharedDirector();  
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority + 1, true);
}


bool PauseLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
	CCLog("ccTouchBegan");
	CCDirector::sharedDirector()->popScene();

	if(sound){
		SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
		SimpleAudioEngine::sharedEngine()->resumeAllEffects();
	}
    CCDirector::sharedDirector()->startAnimation();
	if(sceneIndex==30000){//游戏中
		// paused = false;
		// gameTime.start();
		// emergeTime.start();
	}

	return true;
}

