#include "HelpLayer.h"


USING_NS_CC;
USING_NS_CC_EXT;

CCScene* HelpLayer::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
		scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        HelpLayer *layer = HelpLayer::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

bool HelpLayer::init()
{
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////
	  CCSize size = CCDirector::sharedDirector()->getWinSize();
		CC_BREAK_IF(! CCLayer::init());
	
		CCSpriteFrameCache *UI_mission=CCSpriteFrameCache::sharedSpriteFrameCache();
		UI_mission->addSpriteFramesWithFile("ccbResources/UI_mission.plist");

		/*CCSprite *back = CCSprite::create("ccbResources/back.png");
		back->setPosition(ccp(size.width/2,size.height/2));
		this->addChild(back);*/
		CCNodeLoaderLibrary *backloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		cocos2d::extension::CCBReader *backBreader= new cocos2d::extension::CCBReader(backloaderlibrary); 
		backBreader->autorelease(); 
		CCNode  *backNode  = backBreader->readNodeGraphFromFile("shop/scene_shop_background.ccbi",this);
		backNode->setPosition(ccp(0,0));
		this->addChild(backNode);

		CCMenuItemSprite *backMenu= CCMenuItemSprite::create(CCSprite::createWithSpriteFrame(UI_mission->spriteFrameByName("button_back.png")),CCSprite::createWithSpriteFrame(UI_mission->spriteFrameByName("button_back_1.png"))
			,this,menu_selector(HelpLayer::menuCloseCallback));
		backMenu->setPosition(ccp(82,30));

		CCMenu* menu = CCMenu::create(backMenu,NULL);
       menu->setPosition(CCPointZero);
       this->addChild(menu, 101,201);

	   //����һ���Զ��ͷŵ�CCNode���ؿ� 
	   CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary(); 


	   //����һ���Զ��ͷŵ�CCB��ȡ�ࣨCCBReader�� 
	   cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary); 
	   ccbReader->autorelease(); 

	   //��ȡccbi�ļ���ȡ���ڵ� 
	   CCNode * node = ccbReader->readNodeGraphFromFile("help.ccbi", this); 
	   this->addChild(node, 1,202);

		bRet = true;
    } while (0);
	 return bRet;
}
//���أ��˳�
void  HelpLayer::keyBackClicked(){
	menuCloseCallback(this);
}
 void HelpLayer::menuCloseCallback(CCObject* pSender){
	 CCDirector::sharedDirector()->popScene();
}