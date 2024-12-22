#include "AboutLayer.h"
#include "cocos2d.h"
#include "cocos-ext.h"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d;


CCScene* AboutLayer::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
		scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        AboutLayer *layer = AboutLayer::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

bool AboutLayer::init()
{
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////
	   CCSize size = CCDirector::sharedDirector()->getWinSize();
		CC_BREAK_IF(! CCLayer::init());
	

		CCNodeLoaderLibrary *backloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		cocos2d::extension::CCBReader *backBreader= new cocos2d::extension::CCBReader(backloaderlibrary); 
		backBreader->autorelease(); 
		CCNode  *backNode  = backBreader->readNodeGraphFromFile("shop/scene_shop_background.ccbi",this);
		backNode->setPosition(ccp(0,0));
		this->addChild(backNode,0,12300);

		CCSpriteFrameCache *UI_mission=CCSpriteFrameCache::sharedSpriteFrameCache();
		UI_mission->addSpriteFramesWithFile("ccbResources/UI_mission.plist");

		CCSprite *back = CCSprite::create("about.png");
		back->setPosition(ccp(size.width/2,size.height/2+100));
		this->addChild(back);



		CCMenuItemSprite *backMenu= CCMenuItemSprite::create(CCSprite::createWithSpriteFrame(UI_mission->spriteFrameByName("button_back.png")),CCSprite::createWithSpriteFrame(UI_mission->spriteFrameByName("button_back_1.png"))
			,this,menu_selector(AboutLayer::menuCloseCallback));
		backMenu->setPosition(ccp(82,30));

		CCMenu* menu = CCMenu::create(backMenu,NULL);
       menu->setPosition(CCPointZero);
       this->addChild(menu, 101,201);

		bRet = true;
    } while (0);
	 return bRet;
}
//·µ»Ø£¬ÍË³ö
void  AboutLayer::keyBackClicked(){
	menuCloseCallback(this);
}
 void AboutLayer::menuCloseCallback(CCObject* pSender){


	 CCDirector::sharedDirector()->popScene();
}