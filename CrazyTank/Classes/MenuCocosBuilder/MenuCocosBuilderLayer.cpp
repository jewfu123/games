#include "MenuCocosBuilderLayer.h"
#include "GameScene.h"
#include "AboutLayer.h"
#include "HelpLayer.h"
#include "../EditBoxLayer/EditBoxLayerLoader.h"
#include "MenuCocosBuilderLayerLoader.h"
#include "../ShopCocosBuilder/ShopCocosBuilderLayerLoader.h"
#include "../ShopCocosBuilder/cashShopCocosBuilderLayerLoader.h"
#include "../ShopCocosBuilder/BoxCocosBuilderLayer.h"
#include "../AchieveCocosBuilder/AchieveCocosBuilderLoader.h"
#include "../option/optionLoader.h"
#include "MenuLayer.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
#include <jni.h>
#include "platform/android/jni/JniHelper.h" 
#include <android/log.h> 
#endif 


USING_NS_CC;
USING_NS_CC_EXT;
using namespace CocosDenshion;

MenuCocosBuilderLayer::MenuCocosBuilderLayer()
: white1Sprite(NULL), 
  menuLayerShow(false),
  menuLayer(NULL),
  Button_act(NULL),
  act1(NULL),
  act2(NULL),
  act3(NULL),
  act4(NULL),
  act5(NULL),
  gold_box(NULL),
  life_box(NULL),
  button_voice(NULL),
  startBool(true),
  exitShow(false),
  layer1(NULL),
  layer2(NULL)
{}

MenuCocosBuilderLayer::~MenuCocosBuilderLayer()
{
    CC_SAFE_RELEASE(white1Sprite);
	CC_SAFE_RELEASE(menuLayer);
	CC_SAFE_RELEASE(Button_act);
	CC_SAFE_RELEASE(act1);
	CC_SAFE_RELEASE(act2);
	CC_SAFE_RELEASE(act3);
	CC_SAFE_RELEASE(act4);
}


/*CCNode *  MenuCocosBuilderLayer::openCBBI(const char * pCCBFileName,const char *actionName) 
{ 
    //创建一个自动释放的CCNode加载库 
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary(); 
    ccNodeLoaderLibrary->registerCCNodeLoader("MenuCocosBuilderLayer", MenuCocosBuilderLayerLoader::loader());//
        
    //创建一个自动释放的CCB读取类（CCBReader） 
    cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary); 
    ccbReader->autorelease(); 
        
    //读取ccbi文件获取根节点 
    CCNode * node = ccbReader->readNodeGraphFromFile(pCCBFileName, this); 
    
	  CCArray* allSeq = ccbReader->getAnimationManager()->getSequences(); 
	  CCLog("MenuCocosBuilderLayer============================") ;
	  for (int i=0; i<allSeq->count(); i++)
	  {     
		 //获取到每一个Seq 
		 CCBSequence* everySeq = (CCBSequence*)allSeq->objectAtIndex(i);
		    //获取每个序列动作周期、名字以及id 
		 // everySeq->getDuration(); 
		  CCLog("MenuCocosBuilderLayer===========%s",everySeq->getName()) ; 
		  CCLog("MenuCocosBuilderLayer===========%d",everySeq->getSequenceId()) ;
		  
	  } 

	ccbReader->getAnimationManager()->runAnimationsForSequenceNamed(actionName);

    return node; 
}*/


/*void MenuCocosBuilderLayer::openTest(const char * pCCBFileName, const char * pCCNodeName, CCNodeLoader * pCCNodeLoader) {
   
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();

   //ccNodeLoaderLibrary->registerCCNodeLoader("TestHeaderLayer", TestHeaderLayerLoader::loader());
    if(pCCNodeName != NULL && pCCNodeLoader != NULL) {
        ccNodeLoaderLibrary->registerCCNodeLoader(pCCNodeName, pCCNodeLoader);
    }
    cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
    ccbReader->autorelease();

    // Load the scene from the ccbi-file, setting this class as
    // the owner will cause lblTestTitle to be set by the CCBReader.
    // lblTestTitle is in the TestHeader.ccbi, which is referenced
    // from each of the test scenes.
    CCNode * node = ccbReader->readNodeGraphFromFile(pCCBFileName, this);

    CCScene * scene = CCScene::create();
    if(node != NULL) {
        scene->addChild(node);
    }

    ccColor3B transitionColor;
    transitionColor.r = 0;
    transitionColor.g = 0;
    transitionColor.b = 0;
    
    CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, scene, transitionColor));
}*/


void MenuCocosBuilderLayer::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader) {
	act1->selected();

	if(LoadBooleanFromXML("sound")){
		button_voice->unselected();
	}else{
		button_voice->selected();
	}
	//act1->setPosition(ccp(0,0));

  //  CCRotateBy * ccRotateBy = CCRotateBy::create(20.0f, 360);
  //  CCRepeatForever * ccRepeatForever = CCRepeatForever::create(ccRotateBy);
   //this->menu->runAction(ccRepeatForever);

	  UserRecord *gradeUR = new UserRecord("tank.db","tank");
	  int  gold = gradeUR->readIntFromSQLite("gold");
	  int  tankCont = gradeUR->readIntFromSQLite("tankCont");
	  gradeUR->closeDB();
	  free(gradeUR);

	  char lable[30];
	  sprintf(lable,"%d",gold);
	  CCLabelAtlas* moneyNum=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
	  moneyNum->setAnchorPoint(ccp(0.5,0.5));
	  moneyNum->setPosition(gold_box->getContentSize().width/2+10,gold_box->getContentSize().height/2);
	  gold_box->addChild(moneyNum,1,2);


	  CCLog("tankCont:%d",tankCont);
	  sprintf(lable,"%d",tankCont);
	  CCLabelAtlas* lifeNum=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
	  lifeNum->setAnchorPoint(ccp(0.5,0.5));
	  lifeNum->setPosition(life_box->getContentSize().width/2-15,life_box->getContentSize().height/2);
	  life_box->addChild(lifeNum,1,1);

	  
	  CCLog("regainLifeTime.getSurplusMillis():%ld",regainLifeTime.getSurplusMillis());
	  sprintf(lable,"%d%d",(regainLifeTime.getSurplusMillis()/(1000*60))/10,(regainLifeTime.getSurplusMillis()/(1000*60))%10);
	  CCLabelAtlas* lifeTimeM=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
	  lifeTimeM->setAnchorPoint(ccp(0.5,0.5));
	  lifeTimeM->setPosition(life_box->getContentSize().width/2+20,life_box->getContentSize().height/2);
	  life_box->addChild(lifeTimeM,1,2);

	  sprintf(lable,"%d%d",((regainLifeTime.getSurplusMillis()/1000)%60)/10,((regainLifeTime.getSurplusMillis()/1000)%60)%10);
	  CCLabelAtlas* lifeTimeS=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
	  lifeTimeS->setAnchorPoint(ccp(0.5,0.5));
	  lifeTimeS->setPosition(life_box->getContentSize().width/2+52,life_box->getContentSize().height/2);
	  life_box->addChild(lifeTimeS,1,3);

	  if(LoadBooleanFromXML("activeNum")){
		  layer1->setVisible(false);
		  layer2->setVisible(false);
	  }
}


SEL_MenuHandler MenuCocosBuilderLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName) {
	//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onMenuClicked:", MenuCocosBuilderLayer::onMenuTestClicked);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onMenuClicked:", MenuCocosBuilderLayer::onMenuClicked);

	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_guanyu:", MenuCocosBuilderLayer::onAboutClicked);//关于
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_help:", MenuCocosBuilderLayer::onHelpClicked);//帮助
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_option:", MenuCocosBuilderLayer::onOptionClicked);//设置
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_achineve:", MenuCocosBuilderLayer::onAchineveClicked);//成就
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_start:", MenuCocosBuilderLayer::onStartClicked);//开始游戏
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_shop:", MenuCocosBuilderLayer::onShopClicked);//商店
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_back:", MenuCocosBuilderLayer::onBackClicked);//返回
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_moregame:", MenuCocosBuilderLayer::onMoreGameClicked);//更多游戏

	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "gift", MenuCocosBuilderLayer::onGiftClicked);//礼包

	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_quit", MenuCocosBuilderLayer::onQuitClicked);//退出

	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_act1:", MenuCocosBuilderLayer::onButton_act1Clicked);//button_act1
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_act2:", MenuCocosBuilderLayer::onButton_act2Clicked);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_act3:", MenuCocosBuilderLayer::onButton_act3Clicked);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_act4:", MenuCocosBuilderLayer::onButton_act4Clicked);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_act5:", MenuCocosBuilderLayer::onButton_act5Clicked);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_cashshop", MenuCocosBuilderLayer::onButton_cashshopClicked);
    return NULL;
}

SEL_CCControlHandler MenuCocosBuilderLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName) {
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "shop", MenuCocosBuilderLayer::onMenuTestClicked);
    //CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSpriteTestClicked", MenuCocosBuilderLayer::onSpriteTestClicked);
    //CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onButtonTestClicked", MenuCocosBuilderLayer::onButtonTestClicked);
    //CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAnimationsTestClicked", MenuCocosBuilderLayer::onAnimationsTestClicked);
    //CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onParticleSystemTestClicked", MenuCocosBuilderLayer::onParticleSystemTestClicked);
    //CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onScrollViewTestClicked", MenuCocosBuilderLayer::onScrollViewTestClicked);
    return NULL;
}

bool MenuCocosBuilderLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode) {

	CCLog("onAssignCCBMemberVariable:%s",pMemberVariableName);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ccMenu", CCLayer*, this->menuLayer);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "Button_act", CCMenu*, this->Button_act);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_act1", CCMenuItemImage*, this->act1);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_act2", CCMenuItemImage*, this->act2);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_act3", CCMenuItemImage*, this->act3);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_act4", CCMenuItemImage*, this->act4);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_act5", CCMenuItemImage*, this->act5);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "gold_box", CCSprite*, this->gold_box);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "life_box", CCSprite*, this->life_box);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_voice", CCMenuItemImage*, this->button_voice);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "1111", CCMenuItemImage*, this->layer1);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "2222", CCNode*, this->layer2);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_cashshop", CCMenuItemImage*, this->button_cashshop);
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "white1", CCSprite*, this->white1Sprite);
   // CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mTestTitleLabelTTF", CCLabelTTF *, this->mTestTitleLabelTTF);

    return false;
}

bool MenuCocosBuilderLayer::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue)
{
    bool bRet = false;
   /* if (pTarget == this)
    {
        if (0 == strcmp(pMemberVariableName, "mCustomPropertyInt"))
        {
            this->mCustomPropertyInt = pCCBValue->getIntValue();
            CCLog("mCustomPropertyInt = %d", mCustomPropertyInt);
            bRet = true;
        }
        else if ( 0 == strcmp(pMemberVariableName, "mCustomPropertyFloat"))
        {
            this->mCustomPropertyFloat = pCCBValue->getFloatValue();
            CCLog("mCustomPropertyFloat = %f", mCustomPropertyFloat);
            bRet = true;
        }
        else if ( 0  == strcmp(pMemberVariableName, "mCustomPropertyBoolean"))
        {
            this->mCustomPropertyBoolean = pCCBValue->getBoolValue();
            CCLog("mCustomPropertyBoolean = %d", mCustomPropertyBoolean);
            bRet = true;
        }
        else if ( 0  == strcmp(pMemberVariableName, "mCustomPropertyString"))
        {
            this->mCustomPropertyString = pCCBValue->getStringValue();
            CCLog("mCustomPropertyString = %s", mCustomPropertyString.c_str());
            bRet = true;
        }
        
    }*/

    return bRet;
}

void MenuCocosBuilderLayer::onMenuTestClicked(CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent) {
    //this->openTest("ccb/ccb/TestMenus.ccbi", "TestMenusLayer", MenuTestLayerLoader::loader());
	  CCLog("===================================onMenuTestClicked");
	
}

void MenuCocosBuilderLayer::onSpriteTestClicked(CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent) {
    //this->openTest("ccb/ccb/TestSprites.ccbi", "TestSpritesLayer", SpriteTestLayerLoader::loader());
}

void MenuCocosBuilderLayer::onButtonTestClicked(CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent) {
    //this->openTest("ccb/ccb/TestButtons.ccbi", "TestButtonsLayer", ButtonTestLayerLoader::loader());
}

void MenuCocosBuilderLayer::onMenuClicked(CCObject * pSender) {
    //this->openTest("ccb/ccb/TestMenus.ccbi", "TestMenusLayer", MenuTestLayerLoader::loader());
	if(exitShow)
	{
		return ;
	}
	if(menuLayerShow){
	  CCActionInterval* move = CCMoveBy::create(0.8f, ccp(0,-270));//-225 270
	  CCActionInterval* move_ease_in = CCEaseElasticOut::create(move, 2.5);
	  menuLayer->runAction(move_ease_in);
	  menuLayerShow = false;
	  ((MenuLayer*)(this->getParent()))->playEffect(25);
	}else{
	  CCActionInterval* move = CCMoveBy::create(0.8f, ccp(0,270));//225 270
	  CCActionInterval* move_ease_in = CCEaseElasticOut::create(move, 2.5);
	  menuLayer->runAction(move_ease_in);
	  menuLayerShow = true;
	  ((MenuLayer*)(this->getParent()))->playEffect(26);
	}
}

void MenuCocosBuilderLayer::onGiftClicked(CCObject * pSender){
	CCSize size = CCDirector::sharedDirector()->getWinSize();

	CCNodeLoaderLibrary* lib = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	lib->registerCCNodeLoader("nbgift", EditBoxLayerLoader::loader());//MenuCocosBuilderLayer
	cocos2d::extension::CCBReader *reader = new cocos2d::extension::CCBReader(lib);
	CCNode * node = (CCNode*)reader->readNodeGraphFromFile("menu/scene_nbgift.ccbi", this);// MainMenu_pressmore.ccbi MainMenuScene.ccbi
	reader->autorelease();

	reader->getAnimationManager()->runAnimationsForSequenceNamed("1st");


	CCScene *pScene = CCScene::create();
	if (NULL!=node) {
		pScene->addChild(node,1,321);
	}
	//CCDirector::sharedDirector()->pushScene(pScene);
	CCDirector::sharedDirector()->replaceScene(pScene);
	((MenuLayer*)(this->getParent()))->playBottnEffect();
}

void MenuCocosBuilderLayer::onAboutClicked(CCObject * pSender){
	CCScene *pScene = AboutLayer::scene();
	CCDirector::sharedDirector()->pushScene(pScene);
	onMenuClicked(this) ;
	((MenuLayer*)(this->getParent()))->playBottnEffect();
}
void MenuCocosBuilderLayer::onHelpClicked(CCObject * pSender){
	CCScene *pScene = HelpLayer::scene();
	CCDirector::sharedDirector()->pushScene(pScene);
	onMenuClicked(this) ;

	((MenuLayer*)(this->getParent()))->playBottnEffect();
}

void MenuCocosBuilderLayer::onOptionClicked(CCObject * pSender){
	//onMenuClicked(this) ;
	if(LoadBooleanFromXML("sound")){
		SaveBooleanToXML("sound",false);
		button_voice->selected();
		sound = LoadBooleanFromXML("sound");

		if(SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()){
		   SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
		}
		
	}else{
		SaveBooleanToXML("sound",true);
		
		button_voice->unselected();
		sound = LoadBooleanFromXML("sound");

		if(sound){
			if(!SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()){
				SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sound/2.ogg",true);
			}
		}
	}
	


	/*CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary(); 
	ccNodeLoaderLibrary->registerCCNodeLoader("scene_option_menu", optionLoader::loader());//
	//ccNodeLoaderLibrary->registerCCNodeLoader("scene_achievement", optionLoader::loader());//

	//创建一个自动释放的CCB读取类（CCBReader） 
	cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary); 
	ccbReader->autorelease(); 
	//读取ccbi文件获取根节点 
	CCNode * node = ccbReader->readNodeGraphFromFile("scene_option_menu.ccbi", this); //scene_option.ccbi

	CCArray* allSeq = ccbReader->getAnimationManager()->getSequences(); 
	CCLog("MenuCocosBuilderLayer============================") ;
	for (int i=0; i<allSeq->count(); i++)
	{     
		//获取到每一个Seq 
		CCBSequence* everySeq = (CCBSequence*)allSeq->objectAtIndex(i);
		//获取每个序列动作周期、名字以及id 
		// everySeq->getDuration(); 
		CCLog("MenuCocosBuilderLayer===========%s",everySeq->getName()) ; 
		CCLog("MenuCocosBuilderLayer===========%d",everySeq->getSequenceId()) ;

	} 
	CCScene * scene = CCScene::create();

	CCSize size = CCDirector::sharedDirector()->getWinSize();
	//CCSprite *back = CCSprite::create("ccbResources/back.png");
	//back->setPosition(ccp(size.width/2,size.height/2));
	//scene->addChild(back);

	if(node != NULL) {
		scene->addChild(node);
		//this->addChild(node);
	}
	//CCLog("hhhhhhhhhhhhhhhhhhhhhh");
	//((MenuLayer*)this->getParent())->setTouchEnabled(false);

	CCDirector::sharedDirector()->pushScene(scene);*/
	//Button_act->setTouchEnabled(false);
	//act1->setEnabled(false);
	//act2->setEnabled(false);
	//act3->setEnabled(false);
	//act4->setEnabled(false);

	((MenuLayer*)(this->getParent()))->playBottnEffect();

}
void MenuCocosBuilderLayer::onAchineveClicked(CCObject * pSender){
	if(exitShow)
	{
		return ;
	}
	 onMenuClicked(this) ;
	 	  //创建一个自动释放的CCNode加载库 
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary(); 
    ccNodeLoaderLibrary->registerCCNodeLoader("scene_achievement", AchieveCocosBuilderLoader::loader());//
        
    //创建一个自动释放的CCB读取类（CCBReader） 
    cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary); 
    ccbReader->autorelease(); 
        
    //读取ccbi文件获取根节点 
    CCNode * node = ccbReader->readNodeGraphFromFile("achieve/scene_achievement.ccbi", this); 
    
	 CCArray* allSeq = ccbReader->getAnimationManager()->getSequences(); 
	 CCLog("MenuCocosBuilderLayer============================") ;
	  for (int i=0; i<allSeq->count(); i++)
	  {     
		 //获取到每一个Seq 
		 CCBSequence* everySeq = (CCBSequence*)allSeq->objectAtIndex(i);
		    //获取每个序列动作周期、名字以及id 
		 // everySeq->getDuration(); 
		  CCLog("MenuCocosBuilderLayer===========%s",everySeq->getName()) ; 
		  CCLog("MenuCocosBuilderLayer===========%d",everySeq->getSequenceId()) ;
		  
	  } 

    CCScene * scene = CCScene::create();

	CCSize size = CCDirector::sharedDirector()->getWinSize();
	//CCSprite *back = CCSprite::create("ccbResources/back.png");
	//back->setPosition(ccp(size.width/2,size.height/2));
	//scene->addChild(back);
	CCNodeLoaderLibrary *backloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	cocos2d::extension::CCBReader *backBreader= new cocos2d::extension::CCBReader(backloaderlibrary); 
	backBreader->autorelease(); 
	CCNode  *backNode  = backBreader->readNodeGraphFromFile("shop/scene_shop_background.ccbi",this);
	backNode->setPosition(ccp(0,0));
	scene->addChild(backNode,0,12300);


    if(node != NULL) {
        scene->addChild(node);
    }
   CCDirector::sharedDirector()->pushScene(scene);

   ((MenuLayer*)(this->getParent()))->playBottnEffect();
}

void MenuCocosBuilderLayer::onStartClicked(CCObject * pSender){
	
	if(exitShow)
	{
		return ;
	}
	if(menuLayerShow){
		 onMenuClicked(this) ;
	}
	

	((MenuLayer*)(this->getParent()))->reader->getAnimationManager()->runAnimationsForSequenceNamed("start");
	//(this->getParent())->removeChildByTag(10,-1);//父节点
	this->schedule(schedule_selector(MenuCocosBuilderLayer::showSelectMap),1.735);//1.733f
	((MenuLayer*)(this->getParent()))-> playEffect(20);
	 startBool = false;
	
}

void  MenuCocosBuilderLayer::keyBackClicked(){

	onBackClicked(this);
}
void MenuCocosBuilderLayer::onBackClicked(CCObject * pSender){
	if(exitShow)
	{
		return ;
	}
	if(((MenuLayer*)(this->getParent()))->teach){
		return ;
	}

	if(!startBool){
		return;
	}
	((MenuLayer*)(this->getParent()))->reader->getAnimationManager()->runAnimationsForSequenceNamed("default");
	((MenuLayer*)(this->getParent()))->backMenu();

	((MenuLayer*)(this->getParent()))->playBottnEffect();
	((MenuLayer*)(this->getParent()))->selectMapBool = false;

	if(((MenuLayer*)(this->getParent()))->getChildByTag(900)){
		((MenuLayer*)(this->getParent()))->removeChildByTag(900);
	}
	((MenuLayer*)(this->getParent()))->actIndex = 0;
}

void MenuCocosBuilderLayer::onMoreGameClicked(CCObject * pSender){

	#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
		JniMethodInfo minfo;
		CCLog("============"); 
		bool isHave = JniHelper::getStaticMethodInfo(minfo,"quyou/game/tank/billing","moreGame", "()V");//"com/game/test/JniFruitHelper","show_java_dialog"
		CCLog("------------"); 
		jint _int=0;
		if (isHave) {
			CCLog("jni-java函数执行完毕"); 
			_int = minfo.env->CallStaticIntMethod(minfo.classID, minfo.methodID);//
		}else{
			CCLog("jjni:此函数存在"); 
		}
    #endif
}

void MenuCocosBuilderLayer::onShopClicked(CCObject * pSender){
	if(exitShow)
	{
		return ;
	}
	if(menuLayerShow){
		onMenuClicked(this) ;
	}

	if(!startBool){
		return;
	}

	//创建一个自动释放的CCNode加载库 
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary(); 
    ccNodeLoaderLibrary->registerCCNodeLoader("CCLayer_shop", ShopCocosBuilderLayerLoader::loader());//
        
    //创建一个自动释放的CCB读取类（CCBReader） 
    cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary); 
    ccbReader->autorelease(); 
        
    //读取ccbi文件获取根节点 
    CCNode * node = ccbReader->readNodeGraphFromFile("shop/scene_shop.ccbi", this); 
    
	 CCArray* allSeq = ccbReader->getAnimationManager()->getSequences(); 
	 CCLog("MenuCocosBuilderLayer============================");

	  for (int i=0; i<allSeq->count(); i++)
	  {     
		 //获取到每一个Seq 
		 CCBSequence* everySeq = (CCBSequence*)allSeq->objectAtIndex(i);
		  //获取每个序列动作周期、名字以及id 
		 //everySeq->getDuration(); 
		  CCLog("MenuCocosBuilderLayer===========%s",everySeq->getName()) ; 
		  CCLog("MenuCocosBuilderLayer===========%d",everySeq->getSequenceId()) ;
	  } 

    CCScene * scene = CCScene::create();

	CCNodeLoaderLibrary *backloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	cocos2d::extension::CCBReader *backBreader= new cocos2d::extension::CCBReader(backloaderlibrary); 
	backBreader->autorelease(); 
	CCNode  *backNode  = backBreader->readNodeGraphFromFile("shop/scene_shop_background.ccbi",this);
	backNode->setPosition(ccp(0,0));
	scene->addChild(backNode,0,12300);
	//CCSize size = CCDirector::sharedDirector()->getWinSize();
	//CCSprite *back = CCSprite::create("ccbResources/back.png");
	//back->setPosition(ccp(size.width/2,size.height/2));
	//scene->addChild(back);

    if(node != NULL) {
        scene->addChild(node);
    }
	 CCDirector::sharedDirector()->pushScene(scene);
	((MenuLayer*)(this->getParent()))-> playEffect(24);
}

void MenuCocosBuilderLayer::onButton_cashshopClicked(CCObject * pSender){
	if(exitShow)
	{
		return ;
	}
	if(menuLayerShow){
		onMenuClicked(this) ;
	}

	if(!startBool){
		return;
	}

	//创建一个自动释放的CCNode加载库 
	CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary(); 
	ccNodeLoaderLibrary->registerCCNodeLoader("CCLayer_shop2", cashShopCocosBuilderLayerLoader::loader());//

	//创建一个自动释放的CCB读取类（CCBReader） 
	cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary); 
	ccbReader->autorelease(); 

	//读取ccbi文件获取根节点 
	CCNode * node = ccbReader->readNodeGraphFromFile("shop/scene_shop2.ccbi", this); 

	CCArray* allSeq = ccbReader->getAnimationManager()->getSequences(); 
	CCLog("MenuCocosBuilderLayer============================");

	for (int i=0; i<allSeq->count(); i++)
	{     
		//获取到每一个Seq 
		CCBSequence* everySeq = (CCBSequence*)allSeq->objectAtIndex(i);
		//获取每个序列动作周期、名字以及id 
		//everySeq->getDuration(); 
		CCLog("MenuCocosBuilderLayer===========%s",everySeq->getName()) ; 
		CCLog("MenuCocosBuilderLayer===========%d",everySeq->getSequenceId()) ;
	}

	CCScene * scene = CCScene::create();
	scene->setUserObject(ccbReader->getAnimationManager());

	CCNodeLoaderLibrary *backloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	cocos2d::extension::CCBReader *backBreader= new cocos2d::extension::CCBReader(backloaderlibrary); 
	backBreader->autorelease(); 
	CCNode  *backNode  = backBreader->readNodeGraphFromFile("shop/scene_shop_background.ccbi",this);
	backNode->setPosition(ccp(0,0));
	scene->addChild(backNode,0,12300);
	//CCSize size = CCDirector::sharedDirector()->getWinSize();
	//CCSprite *back = CCSprite::create("ccbResources/back.png");
	//back->setPosition(ccp(size.width/2,size.height/2));
	//scene->addChild(back);

	if(node != NULL) {
		scene->addChild(node);
	}
	CCDirector::sharedDirector()->pushScene(scene);
	//CCDirector::sharedDirector()->replaceScene(scene);
	((MenuLayer*)(this->getParent()))-> playEffect(24);

}
void MenuCocosBuilderLayer::showSelectMap(float dt){
	int selsecIndex = ((MenuLayer*)(this->getParent()))->passCheckNum/10;
	if(selsecIndex==0){
	     onButton_act1Clicked(this);
	}else if(selsecIndex==1){

		onButton_act2Clicked(this);
	}else if(selsecIndex==2){
		onButton_act3Clicked(this);
	}else if(selsecIndex==3){
		onButton_act4Clicked(this);
	}else if(selsecIndex==4){
		onButton_act5Clicked(this);
	}
	
	this->unschedule(schedule_selector(MenuCocosBuilderLayer::showSelectMap));

	startBool = true;
	((MenuLayer*)(this->getParent()))->selectMapBool = true;
}

void MenuCocosBuilderLayer::onButton_act1Clicked(CCObject * pSender){
	act2->unselected();
	act3->unselected();
	act4->unselected();
	act5->unselected();

	act1->selected();

	((MenuLayer*)(this->getParent()))->firstSelectMap(0);
	((MenuLayer*)(this->getParent()))->playBottnEffect();
}
void MenuCocosBuilderLayer::onButton_act2Clicked(CCObject * pSender){
	act1->unselected();
	act3->unselected();
	act4->unselected();
	act5->unselected();

	act2->selected();

	((MenuLayer*)(this->getParent()))->firstSelectMap(1);
	((MenuLayer*)(this->getParent()))->playBottnEffect();
}
void MenuCocosBuilderLayer::onButton_act3Clicked(CCObject * pSender){
	act1->unselected();
	act2->unselected();
	act4->unselected();
	act5->unselected();

	act3->selected();

	((MenuLayer*)(this->getParent()))->firstSelectMap(2);
	((MenuLayer*)(this->getParent()))->playBottnEffect();
}
void MenuCocosBuilderLayer::onButton_act4Clicked(CCObject * pSender){
	act1->unselected();
	act2->unselected();
	act3->unselected();
	act5->unselected();

	act4->selected();

	((MenuLayer*)(this->getParent()))->firstSelectMap(3);
	((MenuLayer*)(this->getParent()))->playBottnEffect();
}

void MenuCocosBuilderLayer::onButton_act5Clicked(CCObject * pSender){
	act1->unselected();
	act2->unselected();
	act3->unselected();
	act4->unselected();

	act5->selected();

	((MenuLayer*)(this->getParent()))->firstSelectMap(4);
	((MenuLayer*)(this->getParent()))->playBottnEffect();
}
void MenuCocosBuilderLayer::onQuitClicked(CCObject * pSender){

	if(exitShow)
	{
	  return ;
	}
	if(!startBool){
		return;
	}

	exitShow = true;

	CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	cocos2d::extension::CCBReader *ccbreader = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
	ccbreader->autorelease();
	//读取一个ccbi的文件,生成一个ccnode实例
	CCNode  *animationsTest  = ccbreader->readNodeGraphFromFile("UI/warning_quit.ccbi",this);//

	if(animationsTest!= NULL){
		this->addChild(animationsTest,10,8790);
	}
	ccbreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(MenuCocosBuilderLayer::showBackMenu)); 
}

void MenuCocosBuilderLayer::showBackMenu(){
	CCSize size =CCDirector::sharedDirector()->getWinSize();

	CCNode * node =this->getChildByTag(8790);


	CCMenuItemImage *yesItem = CCMenuItemImage::create(
		"ccbResources/active/button_yes0.png",
		"ccbResources/active/button_yes1.png",
		this,
		menu_selector(MenuCocosBuilderLayer::yse));
	yesItem->setPosition(ccp(size.width/2-90, size.height/2-30));

	CCMenuItemImage *noItem = CCMenuItemImage::create(
		"ccbResources/active/button_no0.png",
		"ccbResources/active/button_no1.png",
		this,
		menu_selector(MenuCocosBuilderLayer::no));
	noItem->setPosition(ccp(size.width/2+90, size.height/2-30));

	CCMenu* pMenu = CCMenu::create(yesItem,noItem, NULL);
	pMenu->setPosition(CCPointZero);
	node->addChild(pMenu,1,1);

}

void MenuCocosBuilderLayer::yse(CCObject* pSender){
	exitShow = false;
	 this->removeChildByTag(8790);
	 CCLog("millisecondNow:%d",(int)regainLifeTime.getSurplusMillis());
	SaveIntegerToXML("survival_time",(int)(survivalModeTime.getSurplusMillis()+millisecondNow()));
	//SaveIntegerToXML("survival_time",millisecondNow());
	 CCLog("leaveGame_time:%ld",(int)millisecondNow());
	SaveIntegerToXML("leaveGame_time",(int)millisecondNow());
	SaveIntegerToXML("regainLife_time",(int)regainLifeTime.getSurplusMillis());

   #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
  #endif
	CCTextureCache::sharedTextureCache()->removeAllTextures();
	CCDirector::sharedDirector()->end();

}
void MenuCocosBuilderLayer::no(CCObject* pSender){
	exitShow = false;
	 this->removeChildByTag(8790);
}

void MenuCocosBuilderLayer::onAnimationsTestClicked(CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent) {


}

void MenuCocosBuilderLayer::onParticleSystemTestClicked(CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent) {
    //this->openTest("ccb/ccb/TestParticleSystems.ccbi", "TestParticleSystemsLayer", ParticleSystemTestLayerLoader::loader());
}

void MenuCocosBuilderLayer::onScrollViewTestClicked(CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent)
{
   // this->openTest("ccb/ccb/TestScrollViews.ccbi", "TestScrollViewsLayer", ScrollViewTestLayerLoader::loader());
}
