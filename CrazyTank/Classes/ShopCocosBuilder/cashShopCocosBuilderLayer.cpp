#include "cashShopCocosBuilderLayer.h"
#include "cashShopCocosBuilderLayerLoader.h"
#include "ShopCocosBuilderLayerLoader.h"
#include "../xml/XMLParser.h"
#include "BoxCocosBuilderLayerLoader.h"
#include "../UserRecord/UserRecord.h"
#include "Tanker.h"
#include "SimpleAudioEngine.h"
#include "MenuLayer.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
#include <jni.h>
#include "platform/android/jni/JniHelper.h" 
#include <android/log.h> 
#endif 

USING_NS_CC;
USING_NS_CC_EXT;
using namespace CocosDenshion;
cashShopCocosBuilderLayer::cashShopCocosBuilderLayer(void):
	star_box(NULL),
	gold_box(NULL),
	secondPay(false),
	page1(NULL),
	page2(NULL)
{

}


cashShopCocosBuilderLayer::~cashShopCocosBuilderLayer(void)
{

}
void cashShopCocosBuilderLayer::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader) {
	UserRecord *gradeUR = new UserRecord("tank.db","tank");
	int gold = gradeUR->readIntFromSQLite("gold");
	int  tankCont = gradeUR->readIntFromSQLite("tankCont");
	gradeUR->closeDB();
	free(gradeUR);
	char lable[30];


	 //生命恢复
	 sprintf(lable,"%d",tankCont);
	 CCLabelAtlas* lifeNum=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
	 lifeNum->setAnchorPoint(ccp(0.5,0.5));
	 lifeNum->setPosition(star_box->getContentSize().width/2-10,star_box->getContentSize().height/2);
	 star_box->addChild(lifeNum,1,1);


	 sprintf(lable,"%d%d",(regainLifeTime.getSurplusMillis()/(1000*60))/10,(regainLifeTime.getSurplusMillis()/(1000*60))%10);
	 //sprintf(lable,"%d",star);
	 CCLabelAtlas* starNum=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
	 starNum->setAnchorPoint(ccp(0.5,0.5));
	 starNum->setPosition(star_box->getContentSize().width/2+20,star_box->getContentSize().height/2);
	 star_box->addChild(starNum,1,2);

	 sprintf(lable,"%d%d",((regainLifeTime.getSurplusMillis()/1000)%60)/10,((regainLifeTime.getSurplusMillis()/1000)%60)%10);
	 CCLabelAtlas* lifeTimeS=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
	 lifeTimeS->setAnchorPoint(ccp(0.5,0.5));
	 lifeTimeS->setPosition(star_box->getContentSize().width/2+52,star_box->getContentSize().height/2);
	 star_box->addChild(lifeTimeS,1,3);

	 this->schedule(schedule_selector(cashShopCocosBuilderLayer::showTime),1.0f);
	//金钱
	sprintf(lable,"%d",gold);
	CCLabelAtlas* moneyNum=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
	moneyNum->setAnchorPoint(ccp(0.5,0.5));
	moneyNum->setPosition(gold_box->getContentSize().width/2+10,gold_box->getContentSize().height/2);
	gold_box->addChild(moneyNum,1,2);

	
	//-------------------------------购买成功--------------------------------
	CCSize size =CCDirector::sharedDirector()->getWinSize();
	CCNodeLoaderLibrary *burSuccessloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	buySuccessBreader= new cocos2d::extension::CCBReader(burSuccessloaderlibrary); 
	CCNode  *buySccessNode  = buySuccessBreader->readNodeGraphFromFile("shop/UI_box_goumaichenggong.ccbi",this);

	buySccessNode->setPosition(ccp(size.width/2,size.height/2));

	if(buySccessNode  != NULL) {
		this->addChild(buySccessNode,12,2013);
	}
	buySccessNode->setVisible(false);


	page1->selected();

	//payGameOver = false;
	//payResultBool = false;
	billingPointIndex = 0;

	SimpleAudioEngine::sharedEngine()->preloadEffect("sound/15.ogg");//购买升级成功
}
void cashShopCocosBuilderLayer::registerWithTouchDispatcher(void)
{
     CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}


void cashShopCocosBuilderLayer::playBottnEffect(){
	if(sound){
		SimpleAudioEngine::sharedEngine()->playEffect("sound/15.ogg",false);
	}
}
SEL_MenuHandler cashShopCocosBuilderLayer::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_back:", cashShopCocosBuilderLayer::onBackClicked);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_shop:", cashShopCocosBuilderLayer::onButtonShopClicked);

	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "page1", cashShopCocosBuilderLayer::onPage1Clicked);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "page2", cashShopCocosBuilderLayer::onPage2Clicked);

	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_gold0", cashShopCocosBuilderLayer::onGold0Clicked);//onShowPay0Clicked
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_gold1", cashShopCocosBuilderLayer::onGold1Clicked);//onShowPay1Clicked
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_gold2", cashShopCocosBuilderLayer::onGold2Clicked);//onShowPay2Clicked
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_gold3", cashShopCocosBuilderLayer::onGold3Clicked);//onShowPay3Clicked
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_gold4", cashShopCocosBuilderLayer::onGold4Clicked);//onShowPay4Clicked
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_gold5", cashShopCocosBuilderLayer::onGold5Clicked);//onShowPay5Clicked

	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_suit2", cashShopCocosBuilderLayer::onSuite2Clicked);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_suit3", cashShopCocosBuilderLayer::onSuite3Clicked);

	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "left", cashShopCocosBuilderLayer::leftOrRight);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "right", cashShopCocosBuilderLayer::leftOrRight);
	//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_gold6", cashShopCocosBuilderLayer::onShowPay6Clicked);

	return NULL;
}
SEL_CCControlHandler cashShopCocosBuilderLayer::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){

	return NULL;
}

bool cashShopCocosBuilderLayer::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
	CCLog("onAssignCCBMemberVariable===%s",pMemberVariableName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "life_box", CCSprite*, this->star_box);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "gold_box", CCSprite*, this->gold_box);
	
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "page1:", CCMenuItemImage*, this->page1);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "page2:", CCMenuItemImage*, this->page2);
	return false;
}

bool cashShopCocosBuilderLayer::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue){
	   bool bRet = false;

	   return bRet;
}

void  cashShopCocosBuilderLayer::leftOrRight(CCObject * pSender){

	if(page2->isSelected()){
		CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
		CCBAnimationManager *manager = (CCBAnimationManager*)scene->getUserObject();
		manager->runAnimationsForSequenceNamed("2nd");
		page1->selected();
		page2->unselected();
	}else{
		CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
		CCBAnimationManager *manager = (CCBAnimationManager*)scene->getUserObject();
		manager->runAnimationsForSequenceNamed("3rds");

		page1->unselected();
		page2->selected();
	}

}

void cashShopCocosBuilderLayer::onPage1Clicked(CCObject * pSender){
	if(page2->isSelected()){
	  CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	  CCBAnimationManager *manager = (CCBAnimationManager*)scene->getUserObject();
	  manager->runAnimationsForSequenceNamed("2nd");
	  page1->selected();
	  page2->unselected();
	}else{
		 page1->selected();
	}
}

void cashShopCocosBuilderLayer::onPage2Clicked(CCObject * pSender){
	if(page1->isSelected()){
		CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
		CCBAnimationManager *manager = (CCBAnimationManager*)scene->getUserObject();
		manager->runAnimationsForSequenceNamed("3rds");

		page1->unselected();
		page2->selected();
	}else{
		page2->selected();
	}
}

void cashShopCocosBuilderLayer::showTime(float  t){
	if(star_box){
		char lable[50];
		CCLabelAtlas* lifeTimeM= (CCLabelAtlas*)(star_box->getChildByTag(2));
		sprintf(lable,"%d%d",(regainLifeTime.getSurplusMillis()/(1000*60))/10,(regainLifeTime.getSurplusMillis()/(1000*60))%10);
		lifeTimeM->setString(lable);

		CCLabelAtlas* lifeTimeS= (CCLabelAtlas*)(star_box->getChildByTag(3));
		sprintf(lable,"%d%d",((regainLifeTime.getSurplusMillis()/1000)%60)/10,((regainLifeTime.getSurplusMillis()/1000)%60)%10);
		lifeTimeS->setString(lable);

		if(regainLifeTime.getSurplusMillis()<=0){
			UserRecord *gradeUR = new UserRecord("tank.db","tank");
			int  tankCont = gradeUR->readIntFromSQLite("tankCont");

			tankCont++;

			if(tankCont<5){

				regainLifeTime.setMillis(15*60*1000);
				regainLifeTime.start();
			}else{
				regainLifeTime.setMillis(0);
			    
			}
			if(tankCont<=5){
				CCLabelAtlas* lifeTimeN= (CCLabelAtlas*)(star_box->getChildByTag(1));
				sprintf(lable,"%d",tankCont);
				lifeTimeN->setString(lable);

				gradeUR->saveInt("tankCont",tankCont);
			}

			gradeUR->closeDB();
			free(gradeUR);
		}
	}
}
void  cashShopCocosBuilderLayer::keyBackClicked(){
	onBackClicked(this);
}

void cashShopCocosBuilderLayer::onBackClicked(CCObject * pSender){

	if(gameScene){
		CCScene *scene = CCScene::create();
		MenuLayer *node = MenuLayer::create();
		scene->addChild(node);
		CCDirector::sharedDirector()->popScene();
	}else{
		CCScene *scene = CCScene::create();
		MenuLayer *node = MenuLayer::create();
		scene->addChild(node);
		CCDirector::sharedDirector()->replaceScene(scene);
	}
	 
}
extern "C"  {
	void Java_quyou_game_tank_billing_success0()
	{
		//CCScene* scene = CCDirector::sharedDirector()->getRunningScene();

		//payGameOver = true;
		//payResultBool = true;;
		SaveBooleanToXML("payGameOver",true);
		SaveBooleanToXML("payResultBool",true);
		CCLog("000************ssssssssssssss");
	}

	void Java_quyou_game_tank_billing_fail0()
	{
		//payGameOver = true;
		//payResultBool = false;;
		SaveBooleanToXML("payGameOver",true);
		SaveBooleanToXML("payResultBool",false);
		CCLog("000************ffffffffffffff");
	}

	void Java_quyou_game_tank_billing_success1()
	{

		SaveBooleanToXML("payGameOver",true);
		SaveBooleanToXML("payResultBool",true);
		CCLog("111************ssssssssssssss");
	}

	void Java_quyou_game_tank_billing_fail1()
	{
		SaveBooleanToXML("payGameOver",true);
		SaveBooleanToXML("payResultBool",false);
		CCLog("111************ffffffffffffff");
	}

	void Java_quyou_game_tank_billing_success2()
	{

		SaveBooleanToXML("payGameOver",true);
		SaveBooleanToXML("payResultBool",true);

		CCLog("22************ssssssssssssss");
	}
	void Java_quyou_game_tank_billing_fail2()//com.game.mouseMobile
	{
		SaveBooleanToXML("payGameOver",true);
		SaveBooleanToXML("payResultBool",false);
		CCLog("2222************ffffffffffffff");

	}

	void Java_quyou_game_tank_billing_success3()
	{

		SaveBooleanToXML("payGameOver",true);
		SaveBooleanToXML("payResultBool",true);

		CCLog("333************ssssssssssssss");
	}
	void Java_quyou_game_tank_billing_fail3()//com.game.mouseMobile
	{
		SaveBooleanToXML("payGameOver",true);
		SaveBooleanToXML("payResultBool",false);
		CCLog("3333************ffffffffffffff");

	}

	void Java_quyou_game_tank_billing_success5()
	{

		SaveBooleanToXML("payGameOver",true);
		SaveBooleanToXML("payResultBool",true);

		CCLog("55************ssssssssssssss");
	}
	void Java_quyou_game_tank_billing_fail5()//com.game.mouseMobile
	{
		SaveBooleanToXML("payGameOver",true);
		SaveBooleanToXML("payResultBool",false);
		CCLog("5555************ffffffffffffff");
	}
	void Java_quyou_game_tank_billing_success6()
	{

		SaveBooleanToXML("payGameOver",true);
		SaveBooleanToXML("payResultBool",true);

		CCLog("6666************ssssssssssssss");
	}
	void Java_quyou_game_tank_billing_fail6()//com.game.mouseMobile
	{
		SaveBooleanToXML("payGameOver",true);
		SaveBooleanToXML("payResultBool",false);
		CCLog("6666************ffffffffffffff");
	}
}
void cashShopCocosBuilderLayer::updateBillingPoint(float time){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
	switch (billingPointIndex)
	{
	case 14:
		{
			if(LoadBooleanFromXML("payGameOver")){
				this->unschedule(schedule_selector(cashShopCocosBuilderLayer::updateBillingPoint));
				if(LoadBooleanFromXML("payResultBool")){//付费成功
					UserRecord *gradeUR = new UserRecord("tank.db","tank");
					int turretId = gradeUR->readIntFromSQLite("turretId");//炮塔
					int turretGrade = gradeUR->readIntFromSQLite("turretGrade");//炮塔等级
					int chassisId = gradeUR->readIntFromSQLite("chassisId");//底盘
					int chassisGrade = gradeUR->readIntFromSQLite("chassisGrade");//底盘等级

					gradeUR->saveInt("turretId",5);
					gradeUR->saveInt("chassisId",5);

					gradeUR->closeDB();
					free(gradeUR);

					char lable[30];
					UserRecord *ur = new UserRecord("tank.db","shop");
					sprintf(lable,"shopbox%d",5);
					ur->saveInt(lable,1);//保存等级
					sprintf(lable,"shopbox%d",11);
					ur->saveInt(lable,1);//保存等级
					ur->closeDB();
					free(ur);

				}
			}
		}
		break;
	case 15:
		{
			if(LoadBooleanFromXML("payGameOver")){
				this->unschedule(schedule_selector(cashShopCocosBuilderLayer::updateBillingPoint));
				if(LoadBooleanFromXML("payResultBool")){//付费成功
					UserRecord *gradeUR = new UserRecord("tank.db","tank");
					int turretId = gradeUR->readIntFromSQLite("turretId");//炮塔
					int turretGrade = gradeUR->readIntFromSQLite("turretGrade");//炮塔等级
					int chassisId = gradeUR->readIntFromSQLite("chassisId");//底盘
					int chassisGrade = gradeUR->readIntFromSQLite("chassisGrade");//底盘等级

					gradeUR->saveInt("turretId",6);
					gradeUR->saveInt("chassisId",6);

					gradeUR->closeDB();
					free(gradeUR);

					char lable[30];
					UserRecord *ur = new UserRecord("tank.db","shop");
					sprintf(lable,"shopbox%d",6);
					ur->saveInt(lable,1);//保存等级
					sprintf(lable,"shopbox%d",12);
					ur->saveInt(lable,1);//保存等级
					ur->closeDB();
					free(ur);
				}
			}
		}
		break;
	case 9:
		{
				if(LoadBooleanFromXML("payGameOver")){
					this->unschedule(schedule_selector(cashShopCocosBuilderLayer::updateBillingPoint));
					if(LoadBooleanFromXML("payResultBool")){//付费成功
						UserRecord *gradeUR = new UserRecord("tank.db","tank");
						int turretId = gradeUR->readIntFromSQLite("turretId");//炮塔
						int turretGrade = gradeUR->readIntFromSQLite("turretGrade");//炮塔等级
						int chassisId = gradeUR->readIntFromSQLite("chassisId");//底盘
						int chassisGrade = gradeUR->readIntFromSQLite("chassisGrade");//底盘等级

						gradeUR->saveInt("turretId",4);
						gradeUR->saveInt("chassisId",4);

						gradeUR->closeDB();
						free(gradeUR);

						char lable[30];
						UserRecord *ur = new UserRecord("tank.db","shop");
						sprintf(lable,"shopbox%d",4);
						ur->saveInt(lable,1);//保存等级
						sprintf(lable,"shopbox%d",10);
						ur->saveInt(lable,1);//保存等级
						ur->closeDB();
						free(ur);
					}
				}
			}
			break;
		  case 0:
			{
			   if(LoadBooleanFromXML("payGameOver")){
				   this->unschedule(schedule_selector(cashShopCocosBuilderLayer::updateBillingPoint));
				   if(LoadBooleanFromXML("payResultBool")){//付费成功
					   addGold(400);
				   }
			   }
			}
			break;
		  case 1:{
			       if(LoadBooleanFromXML("payGameOver")){
				     this->unschedule(schedule_selector(cashShopCocosBuilderLayer::updateBillingPoint));
				    if(LoadBooleanFromXML("payResultBool")){//付费成功
					  addGold(840);//840
				    }
			       }
				 }
			  break;
		  case 2:{//addGold(3300);
			     if(LoadBooleanFromXML("payGameOver")){
				  this->unschedule(schedule_selector(cashShopCocosBuilderLayer::updateBillingPoint));
				   if(LoadBooleanFromXML("payResultBool")){//付费成功
					  addGold(3300);// 3300
				   }
			      }
				 }
			  break;
		  case 3:
			  {
				  if(LoadBooleanFromXML("payGameOver")){
					  this->unschedule(schedule_selector(cashShopCocosBuilderLayer::updateBillingPoint));
					  if(LoadBooleanFromXML("payResultBool")){//付费成功
						  UserRecord *gradeUR = new UserRecord("tank.db","tank");
						  int tankCont = gradeUR->readIntFromSQLite("tankCont");
						  if(tankCont<0){
							  tankCont = 0;
						  }
						  char lable[30];
						  sprintf(lable,"%d",tankCont+1);
						  ((CCLabelAtlas*)(star_box->getChildByTag(1)))->setString(lable);

						  gradeUR->saveInt("tankCont",tankCont+1);
						  gradeUR->closeDB();
						  free(gradeUR);
						  if(tankCont+1>=5){
							  regainLifeTime.setMillis(0);
						  }
					  }
				  }
			  }
			  break;
		  case 11:
			  {
				  if(LoadBooleanFromXML("payGameOver")){
					  this->unschedule(schedule_selector(cashShopCocosBuilderLayer::updateBillingPoint));
					  if(LoadBooleanFromXML("payResultBool")){//付费成功
						  UserRecord *gradeUR = new UserRecord("tank.db","tank");
						  int tankCont = gradeUR->readIntFromSQLite("tankCont");
						  if(tankCont<0){
							  tankCont = 0;
						  }
						  char lable[30];
						  sprintf(lable,"%d",tankCont+20);
						  ((CCLabelAtlas*)(star_box->getChildByTag(1)))->setString(lable);

						  gradeUR->saveInt("tankCont",tankCont+20);
						  gradeUR->closeDB();
						  free(gradeUR);
						  if(tankCont+20>=5){
							  regainLifeTime.setMillis(0);
						  }
					  }
				  }
			  }
			  break;

	    }

     #endif
}
void cashShopCocosBuilderLayer::onShowPay0Clicked(CCObject * pSender){
	char lable[30];
	UserRecord *urs = new UserRecord("tank.db","shop");
	sprintf(lable,"shopbox%d",4);
	int shopbox4= urs->readIntFromSQLite(lable);
	sprintf(lable,"shopbox%d",10);
	int shopbox10 = urs->readIntFromSQLite(lable);
	urs->closeDB();
	free(urs);

	if(shopbox4==0||shopbox10==0){

	if(!secondPay){
		CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		cocos2d::extension::CCBReader *ccbreaderPay = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
		CCNode  *payNode  = ccbreaderPay->readNodeGraphFromFile("pay/warning_pay.ccbi",this);
		ccbreaderPay->autorelease();

		this->addChild(payNode,100,8510);

		CCSize size =CCDirector::sharedDirector()->getWinSize();
		CCMenuItemImage *yesItem = CCMenuItemImage::create(
			"ccbResources/active/button_yes0.png",
			"ccbResources/active/button_yes1.png",
			this,
			menu_selector(cashShopCocosBuilderLayer::onGold0Clicked));
		yesItem->setPosition(ccp(size.width/2-100, size.height/2-65));

		CCMenuItemImage *noItem = CCMenuItemImage::create(
			"ccbResources/active/button_no0.png",
			"ccbResources/active/button_no1.png",
			this,
			menu_selector(cashShopCocosBuilderLayer::no));
		noItem->setPosition(ccp(size.width/2+100, size.height/2-65));

		CCMenu* pMenu = CCMenu::create(yesItem,noItem, NULL);
		pMenu->setPosition(CCPointZero);
		payNode->addChild(pMenu,1,1);

		secondPay = true;
	}
  }
}
void cashShopCocosBuilderLayer::onShowPay1Clicked(CCObject * pSender){
  if(!secondPay){
	CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	cocos2d::extension::CCBReader *ccbreaderPay = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
	CCNode  *payNode  = ccbreaderPay->readNodeGraphFromFile("pay/warning_pay.ccbi",this);
	ccbreaderPay->autorelease();

	this->addChild(payNode,100,8510);

	CCSize size =CCDirector::sharedDirector()->getWinSize();
	CCMenuItemImage *yesItem = CCMenuItemImage::create(
		"ccbResources/active/button_yes0.png",
		"ccbResources/active/button_yes1.png",
		this,
		menu_selector(cashShopCocosBuilderLayer::onGold1Clicked));
	yesItem->setPosition(ccp(size.width/2-100, size.height/2-65));

	CCMenuItemImage *noItem = CCMenuItemImage::create(
		"ccbResources/active/button_no0.png",
		"ccbResources/active/button_no1.png",
		this,
		menu_selector(cashShopCocosBuilderLayer::no));
	noItem->setPosition(ccp(size.width/2+100, size.height/2-65));

	CCMenu* pMenu = CCMenu::create(yesItem,noItem, NULL);
	pMenu->setPosition(CCPointZero);
	payNode->addChild(pMenu,1,1);

	secondPay = true;
  }
}
 void cashShopCocosBuilderLayer::onShowPay2Clicked(CCObject * pSender){
	 if(!secondPay){
		 CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		 cocos2d::extension::CCBReader *ccbreaderPay = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
		 CCNode  *payNode  = ccbreaderPay->readNodeGraphFromFile("pay/warning_pay.ccbi",this);
		 ccbreaderPay->autorelease();

		 this->addChild(payNode,100,8510);

		 CCSize size =CCDirector::sharedDirector()->getWinSize();
		 CCMenuItemImage *yesItem = CCMenuItemImage::create(
			 "ccbResources/active/button_yes0.png",
			 "ccbResources/active/button_yes1.png",
			 this,
			 menu_selector(cashShopCocosBuilderLayer::onGold2Clicked));
		 yesItem->setPosition(ccp(size.width/2-100, size.height/2-65));

		 CCMenuItemImage *noItem = CCMenuItemImage::create(
			 "ccbResources/active/button_no0.png",
			 "ccbResources/active/button_no1.png",
			 this,
			 menu_selector(cashShopCocosBuilderLayer::no));
		 noItem->setPosition(ccp(size.width/2+100, size.height/2-65));

		 CCMenu* pMenu = CCMenu::create(yesItem,noItem, NULL);
		 pMenu->setPosition(CCPointZero);
		 payNode->addChild(pMenu,1,1);

		 secondPay = true;
	 }
 }
 void cashShopCocosBuilderLayer::onShowPay3Clicked(CCObject * pSender){
	 if(!secondPay){
		 CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		 cocos2d::extension::CCBReader *ccbreaderPay = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
		 CCNode  *payNode  = ccbreaderPay->readNodeGraphFromFile("pay/warning_pay.ccbi",this);
		 ccbreaderPay->autorelease();

		 this->addChild(payNode,100,8510);

		 CCSize size =CCDirector::sharedDirector()->getWinSize();
		 CCMenuItemImage *yesItem = CCMenuItemImage::create(
			 "ccbResources/active/button_yes0.png",
			 "ccbResources/active/button_yes1.png",
			 this,
			 menu_selector(cashShopCocosBuilderLayer::onGold3Clicked));
		 yesItem->setPosition(ccp(size.width/2-100, size.height/2-65));

		 CCMenuItemImage *noItem = CCMenuItemImage::create(
			 "ccbResources/active/button_no0.png",
			 "ccbResources/active/button_no1.png",
			 this,
			 menu_selector(cashShopCocosBuilderLayer::no));
		 noItem->setPosition(ccp(size.width/2+100, size.height/2-65));

		 CCMenu* pMenu = CCMenu::create(yesItem,noItem, NULL);
		 pMenu->setPosition(CCPointZero);
		 payNode->addChild(pMenu,1,1);

		 secondPay = true;
	 }
 }
 void cashShopCocosBuilderLayer::onShowPay4Clicked(CCObject * pSender){
	 if(!secondPay){
		 CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		 cocos2d::extension::CCBReader *ccbreaderPay = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
		 CCNode  *payNode  = ccbreaderPay->readNodeGraphFromFile("pay/warning_pay.ccbi",this);
		 ccbreaderPay->autorelease();

		 this->addChild(payNode,100,8510);

		 CCSize size =CCDirector::sharedDirector()->getWinSize();
		 CCMenuItemImage *yesItem = CCMenuItemImage::create(
			 "ccbResources/active/button_yes0.png",
			 "ccbResources/active/button_yes1.png",
			 this,
			 menu_selector(cashShopCocosBuilderLayer::onGold4Clicked));
		 yesItem->setPosition(ccp(size.width/2-100, size.height/2-65));

		 CCMenuItemImage *noItem = CCMenuItemImage::create(
			 "ccbResources/active/button_no0.png",
			 "ccbResources/active/button_no1.png",
			 this,
			 menu_selector(cashShopCocosBuilderLayer::no));
		 noItem->setPosition(ccp(size.width/2+100, size.height/2-65));

		 CCMenu* pMenu = CCMenu::create(yesItem,noItem, NULL);
		 pMenu->setPosition(CCPointZero);
		 payNode->addChild(pMenu,1,1);

		 secondPay = true;
	 }
 }
 void cashShopCocosBuilderLayer::onShowPay5Clicked(CCObject * pSender){
	 if(!secondPay){
		 CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		 cocos2d::extension::CCBReader *ccbreaderPay = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
		 CCNode  *payNode  = ccbreaderPay->readNodeGraphFromFile("pay/warning_pay.ccbi",this);
		 ccbreaderPay->autorelease();

		 this->addChild(payNode,100,8510);

		 CCSize size =CCDirector::sharedDirector()->getWinSize();
		 CCMenuItemImage *yesItem = CCMenuItemImage::create(
			 "ccbResources/active/button_yes0.png",
			 "ccbResources/active/button_yes1.png",
			 this,
			 menu_selector(cashShopCocosBuilderLayer::onGold5Clicked));
		 yesItem->setPosition(ccp(size.width/2-100, size.height/2-65));

		 CCMenuItemImage *noItem = CCMenuItemImage::create(
			 "ccbResources/active/button_no0.png",
			 "ccbResources/active/button_no1.png",
			 this,
			 menu_selector(cashShopCocosBuilderLayer::no));
		 noItem->setPosition(ccp(size.width/2+100, size.height/2-65));

		 CCMenu* pMenu = CCMenu::create(yesItem,noItem, NULL);
		 pMenu->setPosition(CCPointZero);
		 payNode->addChild(pMenu,1,1);

		 secondPay = true;
	 }
 }
/* void cashShopCocosBuilderLayer::onShowPay6Clicked(CCObject * pSender){
	 if(!secondPay){
		 CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		 cocos2d::extension::CCBReader *ccbreaderPay = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
		 CCNode  *payNode  = ccbreaderPay->readNodeGraphFromFile("pay/warning_pay.ccbi",this);
		 ccbreaderPay->autorelease();

		 this->addChild(payNode,100,8510);

		 CCSize size =CCDirector::sharedDirector()->getWinSize();
		 CCMenuItemImage *yesItem = CCMenuItemImage::create(
			 "ccbResources/active/button_yes0.png",
			 "ccbResources/active/button_yes1.png",
			 this,
			 menu_selector(cashShopCocosBuilderLayer::onGold6Clicked));
		 yesItem->setPosition(ccp(size.width/2-100, size.height/2-65));

		 CCMenuItemImage *noItem = CCMenuItemImage::create(
			 "ccbResources/active/button_no0.png",
			 "ccbResources/active/button_no1.png",
			 this,
			 menu_selector(cashShopCocosBuilderLayer::no));
		 noItem->setPosition(ccp(size.width/2+100, size.height/2-65));

		 CCMenu* pMenu = CCMenu::create(yesItem,noItem, NULL);
		 pMenu->setPosition(CCPointZero);
		 payNode->addChild(pMenu,1,1);

		 secondPay = true;
	 }
 }*/
 void cashShopCocosBuilderLayer::no(CCObject* pSender){
	 secondPay = false;
	 this->removeChildByTag(8510,-1);

 }

 void cashShopCocosBuilderLayer::onGold0Clicked(CCObject * pSender){
	 char lable[30];
	 UserRecord *urs = new UserRecord("tank.db","shop");
	 sprintf(lable,"shopbox%d",4);
	 int shopbox4= urs->readIntFromSQLite(lable);
	 sprintf(lable,"shopbox%d",10);
	 int shopbox10 = urs->readIntFromSQLite(lable);
	 urs->closeDB();
	 free(urs);
if(shopbox4==0||shopbox10==0){
	 CCLog("onGold0Clicked");
	 playBottnEffect();
	// this->removeChildByTag(8510,-1);
  #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
	 billingPointIndex = 9;//计费点
	 SaveBooleanToXML("payResultBool",false);
	 SaveBooleanToXML("payGameOver",false);
	 //payResultBool = false;;
	 //payGameOver = false;
	 this->schedule(schedule_selector(cashShopCocosBuilderLayer::updateBillingPoint), 0.1f);

	 JniMethodInfo minfo;
	 CCLog("============"); 
	 bool isHave = JniHelper::getStaticMethodInfo(minfo,"quyou/game/tank/billing","show_java_dialog", "(I)I");//"com/game/test/JniFruitHelper","show_java_dialog"
	 CCLog("------------"); 
	 jint _int=0;
	 if (isHave) {
		 CCLog("jni-java函数执行完毕"); 
		 _int = minfo.env->CallStaticIntMethod(minfo.classID, minfo.methodID,billingPointIndex);//
	 }else{
		 CCLog("jjni:此函数存在"); 
	 }
   #else
	 //addGold(1);
	 UserRecord *gradeUR = new UserRecord("tank.db","tank");
	 int turretId = gradeUR->readIntFromSQLite("turretId");//炮塔
	 int turretGrade = gradeUR->readIntFromSQLite("turretGrade");//炮塔等级
	 int chassisId = gradeUR->readIntFromSQLite("chassisId");//底盘
	 int chassisGrade = gradeUR->readIntFromSQLite("chassisGrade");//底盘等级

	 gradeUR->saveInt("turretId",4);
	 gradeUR->saveInt("chassisId",4);

	 gradeUR->closeDB();
	 free(gradeUR);

	 char lable[30];
	 UserRecord *ur = new UserRecord("tank.db","shop");
	 sprintf(lable,"shopbox%d",4);
	 ur->saveInt(lable,1);//保存等级
	 sprintf(lable,"shopbox%d",10);
	 ur->saveInt(lable,1);//保存等级
	 ur->closeDB();
	 free(ur);

   #endif
	 secondPay = false;
  }else{
	  CCSize size = CCDirector::sharedDirector()->getWinSize();
	  CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	  cocos2d::extension::CCBReader *ccbreaderPay = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
	  CCNode  *payNode  = ccbreaderPay->readNodeGraphFromFile("pay/UI_box_yijinggoumai.ccbi",this);
	  ccbreaderPay->autorelease();
	  payNode->setPosition(ccp(size.width/2,size.height/2));

	  this->addChild(payNode,100,85);
	  ccbreaderPay->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(cashShopCocosBuilderLayer::yijinggoumaiEnd)); 
  }
 }

  void cashShopCocosBuilderLayer::yijinggoumaiEnd(){
	  this->removeChildByTag(85,-1);

  }

void cashShopCocosBuilderLayer::onGold1Clicked(CCObject * pSender){
	CCLog("onGold1Clicked");
	playBottnEffect();
	//this->removeChildByTag(8510,-1);

 #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
	billingPointIndex = 0;//计费点
	SaveBooleanToXML("payResultBool",false);
	SaveBooleanToXML("payGameOver",false);
	//payResultBool = false;;
	//payGameOver = false;
	this->schedule(schedule_selector(cashShopCocosBuilderLayer::updateBillingPoint), 0.1f);

	JniMethodInfo minfo;
	CCLog("============"); 
	bool isHave = JniHelper::getStaticMethodInfo(minfo,"quyou/game/tank/billing","show_java_dialog", "(I)I");//"com/game/test/JniFruitHelper","show_java_dialog"
	CCLog("------------"); 
	jint _int=0;
	if (isHave) {
		CCLog("jni-java函数执行完毕"); 
		_int = minfo.env->CallStaticIntMethod(minfo.classID, minfo.methodID,billingPointIndex);//
	}else{
		CCLog("jjni:此函数存在"); 
	}
  #else
	addGold(400);
  #endif
  secondPay = false;
}
void cashShopCocosBuilderLayer::onGold2Clicked(CCObject * pSender){
	playBottnEffect();
	//this->removeChildByTag(8510,-1);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
	billingPointIndex = 1;//计费点
	SaveBooleanToXML("payResultBool",false);
	SaveBooleanToXML("payGameOver",false);
	//payResultBool = false;;
	//payGameOver = false;
	this->schedule(schedule_selector(cashShopCocosBuilderLayer::updateBillingPoint), 0.1f);

	JniMethodInfo minfo;
	CCLog("============"); 
	bool isHave = JniHelper::getStaticMethodInfo(minfo,"quyou/game/tank/billing","show_java_dialog", "(I)I");//"com/game/test/JniFruitHelper","show_java_dialog"
	CCLog("------------"); 
	jint _int=0;
	if (isHave) {
		CCLog("jni-java函数执行完毕"); 
		_int = minfo.env->CallStaticIntMethod(minfo.classID, minfo.methodID,billingPointIndex);//
	}else{
		CCLog("jjni:此函数存在"); 
	}
#else
	addGold(840);
#endif
	 secondPay = false;

}
void cashShopCocosBuilderLayer::onGold3Clicked(CCObject * pSender){
//	this->removeChildByTag(8510,-1);
	playBottnEffect();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
	billingPointIndex = 2;//计费点
	SaveBooleanToXML("payResultBool",false);
	SaveBooleanToXML("payGameOver",false);
	//payResultBool = false;;
	//payGameOver = false;
	this->schedule(schedule_selector(cashShopCocosBuilderLayer::updateBillingPoint), 0.1f);

	JniMethodInfo minfo;
	CCLog("============"); 
	bool isHave = JniHelper::getStaticMethodInfo(minfo,"quyou/game/tank/billing","show_java_dialog", "(I)I");//"com/game/test/JniFruitHelper","show_java_dialog"
	CCLog("------------"); 
	jint _int=0;
	if (isHave) {
		CCLog("jni-java函数执行完毕"); 
		_int = minfo.env->CallStaticIntMethod(minfo.classID, minfo.methodID,billingPointIndex);//
	}else{
		CCLog("jjni:此函数存在"); 
	}
#else
	addGold(3300);
#endif
	 secondPay = false;

}

void cashShopCocosBuilderLayer::onGold4Clicked(CCObject * pSender){
	//this->removeChildByTag(8510,-1);
	playBottnEffect();
  #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
	billingPointIndex = 3;//计费点
	SaveBooleanToXML("payResultBool",false);
	SaveBooleanToXML("payGameOver",false);
	//payResultBool = false;;
	//payGameOver = false;
	this->schedule(schedule_selector(cashShopCocosBuilderLayer::updateBillingPoint), 0.1f);

	JniMethodInfo minfo;
	CCLog("============"); 
	bool isHave = JniHelper::getStaticMethodInfo(minfo,"quyou/game/tank/billing","show_java_dialog", "(I)I");//"com/game/test/JniFruitHelper","show_java_dialog"
	CCLog("------------"); 
	jint _int=0;
	if (isHave) {
		CCLog("jni-java函数执行完毕"); 
		_int = minfo.env->CallStaticIntMethod(minfo.classID, minfo.methodID,billingPointIndex);//
	}else{
		CCLog("jjni:此函数存在"); 
	}
  #else
	UserRecord *gradeUR = new UserRecord("tank.db","tank");
	int tankCont = gradeUR->readIntFromSQLite("tankCont");
	if(tankCont<0){
		tankCont = 0;
	}
	char lable[30];
	sprintf(lable,"%d",tankCont+1);
	((CCLabelAtlas*)(star_box->getChildByTag(1)))->setString(lable);

	gradeUR->saveInt("tankCont",tankCont+1);
	gradeUR->closeDB();
	free(gradeUR);
	if(tankCont+1>=5){
		regainLifeTime.setMillis(0);
	}
  #endif
  secondPay = false;
}
void cashShopCocosBuilderLayer::onGold5Clicked(CCObject * pSender){
	playBottnEffect();
	//this->removeChildByTag(8510,-1);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
	billingPointIndex = 11;//计费点
	SaveBooleanToXML("payResultBool",false);
	SaveBooleanToXML("payGameOver",false);
	//payResultBool = false;;
	//payGameOver = false;
	this->schedule(schedule_selector(cashShopCocosBuilderLayer::updateBillingPoint), 0.1f);

	JniMethodInfo minfo;
	CCLog("============"); 
	bool isHave = JniHelper::getStaticMethodInfo(minfo,"quyou/game/tank/billing","show_java_dialog", "(I)I");//"com/game/test/JniFruitHelper","show_java_dialog"
	CCLog("------------"); 
	jint _int=0;
	if (isHave) {
		CCLog("jni-java函数执行完毕"); 
		_int = minfo.env->CallStaticIntMethod(minfo.classID, minfo.methodID,billingPointIndex);//
	}else{
		CCLog("jjni:此函数存在"); 
	}
#else
	UserRecord *gradeUR = new UserRecord("tank.db","tank");
	int tankCont = gradeUR->readIntFromSQLite("tankCont");
	if(tankCont<0){
		tankCont = 0;
	}
	char lable[30];
	sprintf(lable,"%d",tankCont+20);
	((CCLabelAtlas*)(star_box->getChildByTag(1)))->setString(lable);

	gradeUR->saveInt("tankCont",tankCont+20);
	gradeUR->closeDB();
	free(gradeUR);
	if(tankCont+20>=5){
		regainLifeTime.setMillis(0);
	}
#endif
	secondPay = false;
}

void cashShopCocosBuilderLayer::onSuite2Clicked(CCObject * pSender){
	char lable[30];
	UserRecord *urs = new UserRecord("tank.db","shop");
	sprintf(lable,"shopbox%d",5);
	int shopbox5= urs->readIntFromSQLite(lable);
	sprintf(lable,"shopbox%d",11);
	int shopbox11 = urs->readIntFromSQLite(lable);
	urs->closeDB();
	free(urs);
	if(shopbox5==0||shopbox11==0){
		playBottnEffect();
		// this->removeChildByTag(8510,-1);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
		billingPointIndex = 14;//计费点
		SaveBooleanToXML("payResultBool",false);
		SaveBooleanToXML("payGameOver",false);
		//payResultBool = false;;
		//payGameOver = false;
		this->schedule(schedule_selector(cashShopCocosBuilderLayer::updateBillingPoint), 0.1f);

		JniMethodInfo minfo;
		CCLog("============"); 
		bool isHave = JniHelper::getStaticMethodInfo(minfo,"quyou/game/tank/billing","show_java_dialog", "(I)I");//"com/game/test/JniFruitHelper","show_java_dialog"
		CCLog("------------"); 
		jint _int=0;
		if (isHave) {
			CCLog("jni-java函数执行完毕"); 
			_int = minfo.env->CallStaticIntMethod(minfo.classID, minfo.methodID,billingPointIndex);//
		}else{
			CCLog("jjni:此函数存在"); 
		}
#else
		//addGold(1);
		UserRecord *gradeUR = new UserRecord("tank.db","tank");
		int turretId = gradeUR->readIntFromSQLite("turretId");//炮塔
		int turretGrade = gradeUR->readIntFromSQLite("turretGrade");//炮塔等级
		int chassisId = gradeUR->readIntFromSQLite("chassisId");//底盘
		int chassisGrade = gradeUR->readIntFromSQLite("chassisGrade");//底盘等级

		gradeUR->saveInt("turretId",5);
		gradeUR->saveInt("chassisId",5);

		gradeUR->closeDB();
		free(gradeUR);

		char lable[30];
		UserRecord *ur = new UserRecord("tank.db","shop");
		sprintf(lable,"shopbox%d",5);
		ur->saveInt(lable,1);//保存等级
		sprintf(lable,"shopbox%d",11);
		ur->saveInt(lable,1);//保存等级
		ur->closeDB();
		free(ur);

#endif
	secondPay = false;
	}else{
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		cocos2d::extension::CCBReader *ccbreaderPay = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
		CCNode  *payNode  = ccbreaderPay->readNodeGraphFromFile("pay/UI_box_yijinggoumai.ccbi",this);
		ccbreaderPay->autorelease();
		payNode->setPosition(ccp(size.width/2,size.height/2));

		this->addChild(payNode,100,85);
		ccbreaderPay->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(cashShopCocosBuilderLayer::yijinggoumaiEnd)); 
	}


}

void cashShopCocosBuilderLayer::onSuite3Clicked(CCObject * pSender){
	char lable[30];
	UserRecord *urs = new UserRecord("tank.db","shop");
	sprintf(lable,"shopbox%d",6);
	int shopbox6= urs->readIntFromSQLite(lable);
	sprintf(lable,"shopbox%d",12);
	int shopbox12 = urs->readIntFromSQLite(lable);
	urs->closeDB();
	free(urs);
	if(shopbox6==0||shopbox12==0){
		playBottnEffect();
		// this->removeChildByTag(8510,-1);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
		billingPointIndex = 15;//计费点
		SaveBooleanToXML("payResultBool",false);
		SaveBooleanToXML("payGameOver",false);
		//payResultBool = false;;
		//payGameOver = false;
		this->schedule(schedule_selector(cashShopCocosBuilderLayer::updateBillingPoint), 0.1f);

		JniMethodInfo minfo;
		CCLog("============"); 
		bool isHave = JniHelper::getStaticMethodInfo(minfo,"quyou/game/tank/billing","show_java_dialog", "(I)I");//"com/game/test/JniFruitHelper","show_java_dialog"
		CCLog("------------"); 
		jint _int=0;
		if (isHave) {
			CCLog("jni-java函数执行完毕"); 
			_int = minfo.env->CallStaticIntMethod(minfo.classID, minfo.methodID,billingPointIndex);//
		}else{
			CCLog("jjni:此函数存在"); 
		}
#else
		//addGold(1);
		UserRecord *gradeUR = new UserRecord("tank.db","tank");
		int turretId = gradeUR->readIntFromSQLite("turretId");//炮塔
		int turretGrade = gradeUR->readIntFromSQLite("turretGrade");//炮塔等级
		int chassisId = gradeUR->readIntFromSQLite("chassisId");//底盘
		int chassisGrade = gradeUR->readIntFromSQLite("chassisGrade");//底盘等级

		gradeUR->saveInt("turretId",6);
		gradeUR->saveInt("chassisId",6);

		gradeUR->closeDB();
		free(gradeUR);

		char lable[30];
		UserRecord *ur = new UserRecord("tank.db","shop");
		sprintf(lable,"shopbox%d",6);
		ur->saveInt(lable,1);//保存等级
		sprintf(lable,"shopbox%d",12);
		ur->saveInt(lable,1);//保存等级
		ur->closeDB();
		free(ur);

#endif
		secondPay = false;
	}else{
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		cocos2d::extension::CCBReader *ccbreaderPay = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
		CCNode  *payNode  = ccbreaderPay->readNodeGraphFromFile("pay/UI_box_yijinggoumai.ccbi",this);
		ccbreaderPay->autorelease();
		payNode->setPosition(ccp(size.width/2,size.height/2));

		this->addChild(payNode,100,85);
		ccbreaderPay->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(cashShopCocosBuilderLayer::yijinggoumaiEnd)); 
	}


}

void cashShopCocosBuilderLayer::addGold(int goldNum){
	UserRecord *gradeUR = new UserRecord("tank.db","tank");
	int gold = gradeUR->readIntFromSQLite("gold");

	char lable[30];
	sprintf(lable,"%d",goldNum+gold);
	((CCLabelAtlas*)(gold_box->getChildByTag(2)))->setString(lable);

	
	gradeUR->saveInt("gold",goldNum+gold);
	gradeUR->closeDB();
	free(gradeUR);

	this->getChildByTag(2013)->setVisible(true);
	buySuccessBreader->getAnimationManager()->runAnimationsForSequenceNamed("1st");
}


void cashShopCocosBuilderLayer::onButtonShopClicked(CCObject * pSender){
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

	if(node != NULL) {
		scene->addChild(node);
	}
	//CCDirector::sharedDirector()->pushScene(scene);
	CCDirector::sharedDirector()->replaceScene(scene);
}