#include "GCLayer.h"
#include "MenuLayer.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;
using namespace cocos2d::extension; 
USING_NS_CC;

GCLayer::GCLayer(void)
{
}


GCLayer::~GCLayer(void)
{
}

CCScene* GCLayer::scene(){
	CCScene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		GCLayer *layer = GCLayer::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}

void GCLayer::registerWithTouchDispatcher(void)
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
} 

bool GCLayer::init()
{
	bool bRet = false;
	do {
		CCSize wSize = CCDirector::sharedDirector()->getWinSize();  
		cocos2d::CCProgressTimer *progressTimer = CCProgressTimer::create(CCSprite::create("faguangquan1.png"));  // faguangquan1.png
		progressTimer->setType(kCCProgressTimerTypeRadial);  
		// 默认的情况下，环形渐变的方向是：顺时针   
		// 改变其渐变的方向 Makes the ridial CCW (逆时针)   
		progressTimer->setReverseProgress(true);  
		progressTimer->setPosition(wSize.width/2,wSize.height/2);  
		this->addChild(progressTimer,10000,10000);  

		//CCProgressTo *progressTo = CCProgressTo::create(2.0, 100);  
		//等价于：   
		//CCProgressFromTo *progressFromTo = CCProgressFromTo::create(2.0, 0, 100);   
		//progressTimer->runAction(CCRepeatForever::create(progressTo));  

		CCSprite *load = CCSprite::create("load.png");
		load->setPosition(ccp(wSize.width/2,wSize.height/2-100));
		this->addChild(load,10000,10001);



		this->scheduleUpdate();
		this->schedule(schedule_selector(GCLayer::LoadData),0.2f);
		loadingIndex = 0;

		SimpleAudioEngine::sharedEngine()->preloadEffect("sound/31.ogg");
		bRet = true;
	} while (0);
	return bRet;
}
void GCLayer::update(float time){
	if(this->getChildByTag(10000)){
		cocos2d::CCProgressTimer *progressTimer = (CCProgressTimer*)this->getChildByTag(10000);
		float percentage = progressTimer->getPercentage();  
		if (percentage < 100) {  
			percentage += 5;  
			progressTimer->setPercentage(percentage);  
		}  
	}
}
void GCLayer::LoadData(float time){

	//初始化
	if (!LoadBooleanFromXML("IS_EXISTED"))   
	{   

		if(loadingIndex==0){
			UserRecord *ur = new UserRecord("tank.db","shop");//商店物品等级
			char lable[30];
			for(int i=0;i<12;i++){
				sprintf(lable,"shopbox%d",i+1);
				ur->saveInt(lable,0);
			}


			ur->closeDB();
			free(ur);
			loadingIndex = 1;
			return;
		}

		if(loadingIndex==1){
			UserRecord *ur = new UserRecord("tank.db","shop");//商店物品等级
			char lable[30];

			ur->saveInt("shopbox1",1);//初始装备
			ur->saveInt("shopbox7",1);//初始装备

			ur->saveBool("greenTank",true);//是否购买
			ur->saveBool("blueTank",false);
			ur->saveBool("redTank",false);

			ur->saveInt("doubleAttack",0);//双倍攻击
			ur->saveInt("doubleDefense",0);//双倍防御
			ur->saveBool("unlockTank",false);//解锁坦克

			ur->closeDB();
			free(ur);
			loadingIndex = 2;
			return;
		}
		if(loadingIndex==2){
			UserRecord *gradeUR = new UserRecord("tank.db","tank");
			gradeUR->saveInt("turretId",1);//炮塔
			gradeUR->saveInt("turretGrade",1);//炮塔等级

			gradeUR->saveInt("chassisId",1);//底盘
			gradeUR->saveInt("chassisGrade",1);//底盘等级

			gradeUR->saveInt("bomb",3);//炸弹数量
			gradeUR->saveInt("landmine",5);//地雷数量
			gradeUR->saveInt("tankCont",3);//玩家坦克数量

			gradeUR->saveInt("gold",0);//金币
			gradeUR->saveInt("star",0);//星币


			gradeUR->saveInt("bombIndex",1);//炸弹等级
			gradeUR->saveInt("landmineIndex",1);//地雷等级

			gradeUR->saveInt("addLifeIndex",0);//增加生命等级

			gradeUR->saveInt("addAttackIndex",0);//攻击力强化等级
			gradeUR->saveInt("addDefenseIndex",0);//防御强化等级
			gradeUR->saveInt("addCritIndex",0);//暴击强化等级
			gradeUR->saveInt("addMoveIndex",0);//动力强化等级

			gradeUR->saveInt("colorTank",0);//坦克颜色，绿色
			gradeUR->saveInt("enemyNum",0);//击毁敌人坦克数

			gradeUR->saveInt("passCheckNum",1);//通过关卡数

			gradeUR->closeDB();
			free(gradeUR);
			loadingIndex = 3;
			return;
		}
		if(loadingIndex==3){
			char lable[30];
			UserRecord *gradeUR = new UserRecord("tank.db","tank");
			for(int i=0;i<43;i++){
				sprintf(lable,"passCheck%d",i);
				gradeUR->saveInt(lable,0);//关卡是否开启

				sprintf(lable,"passCheckIndex%d",i);
				gradeUR->saveInt(lable,0);////每一关卡等级
			}

			gradeUR->saveInt("passCheck1",1);//第一关开启
			//gradeUR->saveInt("passCheck9",1);//第一关开启

			//gradeUR->saveInt("passCheck10",0);//第一关开启
			//gradeUR->saveInt("passCheck20",0);//第一关开启
			//gradeUR->saveInt("passCheck30",0);//第一关开启
			//gradeUR->saveInt("passCheck40",0);//第一关开启

			gradeUR->closeDB();
			free(gradeUR);
			loadingIndex = 4;
			return;
		}
		if(loadingIndex==4){
			//完成任务数
			UserRecord *achieveUR = new UserRecord("tank.db","achieveRecord");
			for(int i=0;i<10;i++){
				char lable[30];
				sprintf(lable,"achieveRecord%d", i);
				achieveUR->saveInt(lable,0);

				sprintf(lable,"isRecordGold%d", i);//是否领取任务奖励金币
				achieveUR->saveInt(lable,0);
			}

			achieveUR->saveInt("achieveRecord5",1);
			achieveUR->saveInt("achieveRecord8",1);
			achieveUR->closeDB();
			free(achieveUR);
		}

		survival_time = 0;
		SaveBooleanToXML("survival",true);

		SaveIntegerToXML("leaveGame_time",millisecondNow());
		SaveIntegerToXML("regainLife_time",60*15*1000);

		SaveBooleanToXML("sound", true);   //声音开关
		SaveBooleanToXML("moveMark", true);//移动标志
		SaveBooleanToXML("moveControlHandle", false);//重力或者摇杆

		SaveBooleanToXML("IS_EXISTED", true);      
	}

	survival_time = LoadIntegerFromXML("survival_time");
	regainLife_time = LoadIntegerFromXML("regainLife_time");
	leaveGame_time = LoadIntegerFromXML("leaveGame_time");
	//SaveIntegerToXML("survival",false);

	UserRecord *tankUR = new UserRecord("tank.db","tank");
	int  tankCont = tankUR->readIntFromSQLite("tankCont");
	CCLog("tankCont:%d",tankCont);
	gameSceneMagazine = true;

	if(tankCont<5){
		int second = (millisecondNow()+(60*15*1000-regainLife_time)-leaveGame_time)/1000;
		CCLog("millisecondNow():%lu",millisecondNow());
		CCLog("(60*15*1000-regainLife_time):%d",(60*15*1000-regainLife_time));
		CCLog("leaveGame_time:%d",leaveGame_time);//18072034

		int num = second/(15*60);
		CCLog("num:%d",num);
		tankCont = (tankCont+num)>5?5:(tankCont+num);

		if(tankCont>=5){
			regainLifeTime.setMillis(0);
		}else{
			CCLog("second:%d",second);
			CCLog("second(15*60*1000):%d",(second%(15*60)));//763109
			regainLifeTime.setMillis((15*60-(second%(15*60)))*1000);
			regainLifeTime.start();
		}
	}else{
		regainLifeTime.setMillis(0);
	}
	//  regainLifeTime.setMillis(15*1000);
	//   regainLifeTime.start();
	if(tankCont<0){
		tankCont = 0;
	}
	tankUR->saveInt("tankCont",tankCont);
	tankUR->closeDB();
	free(tankUR);
	CCLog("tankCont:%d",tankCont);

	
	this->unschedule(schedule_selector(GCLayer::LoadData));
	this->unschedule(schedule_selector(GCLayer::update));
	this->removeChildByTag(10000,-1);
	this->removeChildByTag(10001,-1);

	/*CCNodeLoaderLibrary *logoloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	cocos2d::extension::CCBReader *logoBreader= new cocos2d::extension::CCBReader(logoloaderlibrary); 
	logoBreader->autorelease(); 
	CCNode  *logoNode  = logoBreader->readNodeGraphFromFile("logo.ccbi",this);
	logoNode->setPosition(ccp(0,0));
	this->addChild(logoNode,0,12300);
	logoBreader->getAnimationManager()->runAnimationsForSequenceNamed("1st");
	logoBreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(GCLayer::logoEnd)); 
	*/
	//logoEnd();
	goEnd();
}
void GCLayer::yesCallback(CCObject* pSender){
	this->setKeypadEnabled(true);  
	this->setTouchEnabled(true); 

	this->removeChildByTag(401,-1);
	this->removeChildByTag(400,-1);


	CCNodeLoaderLibrary* lib = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	//lib->registerCCNodeLoader("MenuCocosBuilderLayer", MenuCocosBuilderLayerLoader::loader());//MenuCocosBuilderLayer
	cocos2d::extension::CCBReader* gcReader = new cocos2d::extension::CCBReader(lib);
	gcReader->autorelease();
	CCNode * node = (CCNode*)gcReader->readNodeGraphFromFile("gc/jingtou1.ccbi", this);// MainMenu_pressmore.ccbi MainMenuScene.ccbi
	//reader->autorelease();
	if (NULL!=node) {
		this->addChild(node,1,321);
	}
	gcReader->getAnimationManager()->runAnimationsForSequenceNamed("1st");
	gcReader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(GCLayer::goEnd));

	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCMenuItemImage *skip = CCMenuItemImage::create(
		"gc/skip_1.png",
		"gc/skip_2.png",
		this,
		menu_selector(GCLayer::skipCallback));

	skip->setPosition(ccp(size.width-50,size.height-40));
	CCMenu* pMenu = CCMenu::create(skip,NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu,2);

	sound = LoadBooleanFromXML("sound");

	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("sound/animation.ogg");
	if(sound){
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sound/animation.ogg",true);
	}
	//goEnd();
}

void GCLayer::skipCallback(CCObject* pSender){
	goEnd(); 
}


void GCLayer::logoEnd(){

	CCSize size = CCDirector::sharedDirector()->getWinSize();
	explainbool = true;
	if(!explainbool){
		CCSprite* explain = CCSprite::create("explain.png");
		explain->setPosition(ccp(size.width/2, size.height/2));
		this->addChild(explain, 10,400);

		CCMenuItemImage *shi=CCMenuItemImage::create("explain2.png","explain1.png", this,
			menu_selector(GCLayer::noCallback));
		shi->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2-100, CCDirector::sharedDirector()->getWinSize().height/2-150));

		CCMenuItemImage *fou=CCMenuItemImage::create("explain4.png","explain3.png", this,
			menu_selector(GCLayer::yesCallback));
		fou->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2+100, CCDirector::sharedDirector()->getWinSize().height/2-150));

		CCMenu* explainMenu = CCMenu::create(shi,fou,NULL);
		explainMenu->setPosition(CCPointZero);
		this->addChild(explainMenu, 11,401);

		this->setKeypadEnabled(false);  
		this->setTouchEnabled(false); 
		explainbool = true;
	}else{

		CCNodeLoaderLibrary* lib = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		//lib->registerCCNodeLoader("MenuCocosBuilderLayer", MenuCocosBuilderLayerLoader::loader());//MenuCocosBuilderLayer
		cocos2d::extension::CCBReader* gcReader = new cocos2d::extension::CCBReader(lib);
		gcReader->autorelease();
		CCNode * node = (CCNode*)gcReader->readNodeGraphFromFile("gc/jingtou1.ccbi", this);// MainMenu_pressmore.ccbi MainMenuScene.ccbi
		//reader->autorelease();
		if (NULL!=node) {
			this->addChild(node,1,321);
		}
		gcReader->getAnimationManager()->runAnimationsForSequenceNamed("1st");
		gcReader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(GCLayer::goEnd));

		CCSize size = CCDirector::sharedDirector()->getWinSize();
		CCMenuItemImage *skip = CCMenuItemImage::create(
			"gc/skip_1.png",
			"gc/skip_2.png",
			this,
			menu_selector(GCLayer::skipCallback));

		skip->setPosition(ccp(size.width-50,size.height-40));
		CCMenu* pMenu = CCMenu::create(skip,NULL);
		pMenu->setPosition(CCPointZero);
		this->addChild(pMenu,2);

		sound = LoadBooleanFromXML("sound");

		SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("sound/animation.ogg");
		if(sound){
			SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sound/animation.ogg",true);
		}
	}


}

bool GCLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
	return true;
}

void GCLayer::noCallback(CCObject* pSender){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
	CCTextureCache::sharedTextureCache()->removeAllTextures();
	CCDirector::sharedDirector()->end();
}

void GCLayer::goEnd(){

	CCScene *scene = CCScene::create();
	MenuLayer *node = MenuLayer::create();
	scene->addChild(node);
	CCDirector::sharedDirector()->replaceScene(scene);
}
