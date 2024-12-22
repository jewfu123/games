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
		// Ĭ�ϵ�����£����ν���ķ����ǣ�˳ʱ��   
		// �ı��佥��ķ��� Makes the ridial CCW (��ʱ��)   
		progressTimer->setReverseProgress(true);  
		progressTimer->setPosition(wSize.width/2,wSize.height/2);  
		this->addChild(progressTimer,10000,10000);  

		//CCProgressTo *progressTo = CCProgressTo::create(2.0, 100);  
		//�ȼ��ڣ�   
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

	//��ʼ��
	if (!LoadBooleanFromXML("IS_EXISTED"))   
	{   

		if(loadingIndex==0){
			UserRecord *ur = new UserRecord("tank.db","shop");//�̵���Ʒ�ȼ�
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
			UserRecord *ur = new UserRecord("tank.db","shop");//�̵���Ʒ�ȼ�
			char lable[30];

			ur->saveInt("shopbox1",1);//��ʼװ��
			ur->saveInt("shopbox7",1);//��ʼװ��

			ur->saveBool("greenTank",true);//�Ƿ���
			ur->saveBool("blueTank",false);
			ur->saveBool("redTank",false);

			ur->saveInt("doubleAttack",0);//˫������
			ur->saveInt("doubleDefense",0);//˫������
			ur->saveBool("unlockTank",false);//����̹��

			ur->closeDB();
			free(ur);
			loadingIndex = 2;
			return;
		}
		if(loadingIndex==2){
			UserRecord *gradeUR = new UserRecord("tank.db","tank");
			gradeUR->saveInt("turretId",1);//����
			gradeUR->saveInt("turretGrade",1);//�����ȼ�

			gradeUR->saveInt("chassisId",1);//����
			gradeUR->saveInt("chassisGrade",1);//���̵ȼ�

			gradeUR->saveInt("bomb",3);//ը������
			gradeUR->saveInt("landmine",5);//��������
			gradeUR->saveInt("tankCont",3);//���̹������

			gradeUR->saveInt("gold",0);//���
			gradeUR->saveInt("star",0);//�Ǳ�


			gradeUR->saveInt("bombIndex",1);//ը���ȼ�
			gradeUR->saveInt("landmineIndex",1);//���׵ȼ�

			gradeUR->saveInt("addLifeIndex",0);//���������ȼ�

			gradeUR->saveInt("addAttackIndex",0);//������ǿ���ȼ�
			gradeUR->saveInt("addDefenseIndex",0);//����ǿ���ȼ�
			gradeUR->saveInt("addCritIndex",0);//����ǿ���ȼ�
			gradeUR->saveInt("addMoveIndex",0);//����ǿ���ȼ�

			gradeUR->saveInt("colorTank",0);//̹����ɫ����ɫ
			gradeUR->saveInt("enemyNum",0);//���ٵ���̹����

			gradeUR->saveInt("passCheckNum",1);//ͨ���ؿ���

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
				gradeUR->saveInt(lable,0);//�ؿ��Ƿ���

				sprintf(lable,"passCheckIndex%d",i);
				gradeUR->saveInt(lable,0);////ÿһ�ؿ��ȼ�
			}

			gradeUR->saveInt("passCheck1",1);//��һ�ؿ���
			//gradeUR->saveInt("passCheck9",1);//��һ�ؿ���

			//gradeUR->saveInt("passCheck10",0);//��һ�ؿ���
			//gradeUR->saveInt("passCheck20",0);//��һ�ؿ���
			//gradeUR->saveInt("passCheck30",0);//��һ�ؿ���
			//gradeUR->saveInt("passCheck40",0);//��һ�ؿ���

			gradeUR->closeDB();
			free(gradeUR);
			loadingIndex = 4;
			return;
		}
		if(loadingIndex==4){
			//���������
			UserRecord *achieveUR = new UserRecord("tank.db","achieveRecord");
			for(int i=0;i<10;i++){
				char lable[30];
				sprintf(lable,"achieveRecord%d", i);
				achieveUR->saveInt(lable,0);

				sprintf(lable,"isRecordGold%d", i);//�Ƿ���ȡ���������
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

		SaveBooleanToXML("sound", true);   //��������
		SaveBooleanToXML("moveMark", true);//�ƶ���־
		SaveBooleanToXML("moveControlHandle", false);//��������ҡ��

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
