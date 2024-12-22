#include "BoxCocosBuilderLayer.h"
#include "../UserRecord/UserRecord.h"
#include"ShopCocosBuilderLayer.h"
#include "../xml/XMLParser.h"
#include "Tool.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
#include <jni.h>
#include "platform/android/jni/JniHelper.h" 
#include <android/log.h> 
#endif 

USING_NS_CC;
USING_NS_CC_EXT;

BoxCocosBuilderLayer::BoxCocosBuilderLayer(void):
mAnimationManager(NULL),
king(0),
id(0),
secondPay(false)
{
   
}


BoxCocosBuilderLayer::~BoxCocosBuilderLayer(void)
{
	CC_SAFE_RELEASE_NULL(mAnimationManager);
}
void BoxCocosBuilderLayer::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader) {
	buyClickBool = false;
}

SEL_MenuHandler BoxCocosBuilderLayer::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
	CCLog("BoxCocosBuilderLayer::onResolveCCBCCMenuItemSelector===%s",pSelectorName);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_buy", BoxCocosBuilderLayer::onbutton_buyClicked);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_equip", BoxCocosBuilderLayer::onbutton_equipClicked);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_upgrade", BoxCocosBuilderLayer::onbutton_upgradeClicked);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_buy_lock", BoxCocosBuilderLayer::onbutton_buyLcokClicked);

	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_buy1", BoxCocosBuilderLayer::onbutton_buyItemClicked);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_equip1", BoxCocosBuilderLayer::onbutton_equipItemClicked);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_upgrade1", BoxCocosBuilderLayer::onbutton_upgradeItemClicked);

	return NULL;
}

SEL_CCControlHandler BoxCocosBuilderLayer::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
	CCLog("BoxCocosBuilderLayer::onResolveCCBCCControlSelector===%s",pSelectorName);
	CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "shopbox00", BoxCocosBuilderLayer::onControlShopBox);
	return NULL;
}

bool BoxCocosBuilderLayer::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
	CCLog("BoxCocosBuilderLayer::onAssignCCBMemberVariable===%s",pMemberVariableName);

	return false;
}

bool BoxCocosBuilderLayer::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue){
	   bool bRet = false;

	   return bRet;
}

void BoxCocosBuilderLayer::setAnimationManager(CCBAnimationManager *pAnimationManager){
	CC_SAFE_RELEASE_NULL(mAnimationManager);
	mAnimationManager = pAnimationManager;
	CC_SAFE_RETAIN(mAnimationManager);
}
CCBAnimationManager* BoxCocosBuilderLayer::getAnimationManager(){
	return mAnimationManager;
}

void BoxCocosBuilderLayer::onControlShopBox(CCObject * pSender,cocos2d::extension::CCControlEvent pCCControlEvent){
	if(king==0){
		CCLog("BoxCocosBuilderLayer::onControlShopBox===");
		CCNode* node=((((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->getChildByTag(500))->getChildByTag(1))->getChildByTag(2018);
		node->setPosition(ccp(115+(id-1)*175,node->getPositionY()));
		 node->setVisible(true);

		char lable[30];
		UserRecord *gradeUR = new UserRecord("tank.db","shop");
		sprintf(lable,"shopbox%d",id);
		int grade = gradeUR->readIntFromSQLite(lable);//等级
		gradeUR->closeDB();
		free(gradeUR);

		ShopCocosBuilderLayer*  shopLayer = ((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent());
		shopLayer->turretId = id;
		shopLayer->turretGrade = grade;

		shopLayer->setShowBar();
		shopLayer->showTanker();
	}else if(king==1){
		CCLog("BoxCocosBuilderLayer::onControlShopBox===");
		CCNode* node=((((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->getChildByTag(500))->getChildByTag(1))->getChildByTag(2018);
		node->setPosition(ccp(115+(id-6-1)*175,node->getPositionY()));
		 node->setVisible(true);

		char lable[30];
		UserRecord *gradeUR = new UserRecord("tank.db","shop");
		sprintf(lable,"shopbox%d",id);
		int grade = gradeUR->readIntFromSQLite(lable);//等级
		gradeUR->closeDB();
		free(gradeUR);
		//更改
		ShopCocosBuilderLayer*  shopLayer = ((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent());
		shopLayer->chassisId = id-6;
		shopLayer->chassisGrade = grade;

		shopLayer->setShowBar();
		shopLayer->showTanker();
	}else{
		CCLog("BoxCocosBuilderLayer::onControlShopBox===");
		CCLog("id:%d",id);
		CCNode* node=((((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->getChildByTag(500))->getChildByTag(1))->getChildByTag(2018);
		node->setPosition(ccp(115+(id-13)*175,node->getPositionY()));
		node->setVisible(true);
	}
}
void BoxCocosBuilderLayer::onbutton_buyLcokClicked(CCObject * pSender){
	bool ispay = ((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->isPay;

	if(!ispay){
	  /*CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	  cocos2d::extension::CCBReader *ccbreader = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
	  ccbreader->autorelease();
	  char lable[50];
	  sprintf(lable,"shop/warning_unlock%d.ccbi",id);//id>7?(id-6):
	 //读取一个ccbi的文件,生成一个ccnode实例
	  CCNode  *animationsTest  = ccbreader->readNodeGraphFromFile(lable,this);//

	  if(animationsTest!= NULL){
		((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->addChild(animationsTest,10,8790);
	  }
	  ccbreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(BoxCocosBuilderLayer::showPayEnd)); 
	  */
		ysePay(this);
	  //((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->isPay = true;
	}
}

void BoxCocosBuilderLayer::showPayEnd(){
	CCSize size =CCDirector::sharedDirector()->getWinSize();

	CCNode * node =((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->getChildByTag(8790);

	CCMenuItemImage *yesItem = CCMenuItemImage::create(
		"ccbResources/active/button_yes0.png",
		"ccbResources/active/button_yes1.png",
		this,
		menu_selector(BoxCocosBuilderLayer::ysePay));//onShowPaySecondClicked
	yesItem->setPosition(ccp(size.width/2-90, size.height/2-65));

	CCMenuItemImage *noItem = CCMenuItemImage::create(
		"ccbResources/active/button_no0.png",
		"ccbResources/active/button_no1.png",
		this,
		menu_selector(BoxCocosBuilderLayer::noPay));
	noItem->setPosition(ccp(size.width/2+90, size.height/2-65));

	CCMenu* pMenu = CCMenu::create(yesItem,noItem, NULL);
	pMenu->setPosition(CCPointZero);
	node->addChild(pMenu,1,1);


 }
void BoxCocosBuilderLayer::onShowPaySecondClicked(CCObject * pSender){
	if(!secondPay){
		CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		cocos2d::extension::CCBReader *ccbreaderPay = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
		CCNode  *payNode  = ccbreaderPay->readNodeGraphFromFile("pay/warning_pay.ccbi",this);
		ccbreaderPay->autorelease();

		CCNode * node =((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->getChildByTag(8790);
		node->addChild(payNode,100,8510);

		CCSize size =CCDirector::sharedDirector()->getWinSize();
		CCMenuItemImage *yesItem = CCMenuItemImage::create(
			"ccbResources/active/button_yes0.png",
			"ccbResources/active/button_yes1.png",
			this,
			menu_selector(BoxCocosBuilderLayer::ysePay));
		yesItem->setPosition(ccp(size.width/2-100, size.height/2-65));

		CCMenuItemImage *noItem = CCMenuItemImage::create(
			"ccbResources/active/button_no0.png",
			"ccbResources/active/button_no1.png",
			this,
			menu_selector(BoxCocosBuilderLayer::no));
		noItem->setPosition(ccp(size.width/2+100, size.height/2-65));

		CCMenu* pMenu = CCMenu::create(yesItem,noItem, NULL);
		pMenu->setPosition(CCPointZero);
		payNode->addChild(pMenu,1,1);

		secondPay = true;
	}
}
void BoxCocosBuilderLayer::no(CCObject* pSender){
	secondPay = false;
	CCNode * node =((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->getChildByTag(8790);
	node->removeChildByTag(8510,-1);

}
 void BoxCocosBuilderLayer::ysePay(CCObject* pSender){
   secondPay = false;
   ((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->removeChildByTag(8790);
   ((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->isPay = false;


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
   if(id>7){
	   billingPointIndex = 8;//计费点
   }else{
	   billingPointIndex = 7;//计费点
   }
   
   SaveBooleanToXML("payResultBool",false);
   SaveBooleanToXML("payGameOver",false);
   //payResultBool = false;;
   //payGameOver = false;
   this->schedule(schedule_selector(BoxCocosBuilderLayer::updateBillingPoint), 0.1f);

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
   ((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->setShowPre_buy(id>7?(id-6):id);
   char lable[50];
   sprintf(lable,"shopBoxLock%d",id-1);
   SaveBooleanToXML(lable,true);
#endif


  
}
 void BoxCocosBuilderLayer::noPay(CCObject* pSender){

  ((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->removeChildByTag(8790);
   ((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->isPay = false;
 }

 extern "C"  {
	/* void Java_quyou_game_tank_billing_success9()
	 {
		 //CCScene* scene = CCDirector::sharedDirector()->getRunningScene();

		 //payGameOver = true;
		 //payResultBool = true;;
		 SaveBooleanToXML("payGameOver",true);
		 SaveBooleanToXML("payResultBool",true);
		 CCLog("99999************ssssssssssssss");
	 }

	 void Java_quyou_game_tank_billing_fail9()
	 {
		 //payGameOver = true;
		 //payResultBool = false;;
		 SaveBooleanToXML("payGameOver",true);
		 SaveBooleanToXML("payResultBool",false);
		 CCLog("9999************ffffffffffffff");
	 }

	 void Java_quyou_game_tank_billing_success10()
	 {
		 //CCScene* scene = CCDirector::sharedDirector()->getRunningScene();

		 //payGameOver = true;
		 //payResultBool = true;;
		 SaveBooleanToXML("payGameOver",true);
		 SaveBooleanToXML("payResultBool",true);
		 CCLog("1000************ssssssssssssss");
	 }

	 void Java_quyou_game_tank_billing_fail10()
	 {
		 //payGameOver = true;
		 //payResultBool = false;;
		 SaveBooleanToXML("payGameOver",true);
		 SaveBooleanToXML("payResultBool",false);
		 CCLog("1000************ffffffffffffff");
	 }

	 void Java_quyou_game_tank_billing_success11()
	 {

		 SaveBooleanToXML("payGameOver",true);
		 SaveBooleanToXML("payResultBool",true);
		 CCLog("111************ssssssssssssss");
	 }

	 void Java_quyou_game_tank_billing_fail11()
	 {
		 SaveBooleanToXML("payGameOver",true);
		 SaveBooleanToXML("payResultBool",false);
		 CCLog("111************ffffffffffffff");
	 }

	 void Java_quyou_game_tank_billing_success12()
	 {

		 SaveBooleanToXML("payGameOver",true);
		 SaveBooleanToXML("payResultBool",true);
		 CCLog("12************ssssssssssssss");
	 }

	 void Java_quyou_game_tank_billing_fail12()
	 {
		 SaveBooleanToXML("payGameOver",true);
		 SaveBooleanToXML("payResultBool",false);
		 CCLog("12************ffffffffffffff");
	 }
	 void Java_quyou_game_tank_billing_success13()
	 {

		 SaveBooleanToXML("payGameOver",true);
		 SaveBooleanToXML("payResultBool",true);
		 CCLog("13************ssssssssssssss");
	 }

	 void Java_quyou_game_tank_billing_fail13()
	 {
		 SaveBooleanToXML("payGameOver",true);
		 SaveBooleanToXML("payResultBool",false);
		 CCLog("13************ffffffffffffff");
	 }
	 void Java_quyou_game_tank_billing_success14()
	 {

		 SaveBooleanToXML("payGameOver",true);
		 SaveBooleanToXML("payResultBool",true);
		 CCLog("14************ssssssssssssss");
	 }

	 void Java_quyou_game_tank_billing_fail14()
	 {
		 SaveBooleanToXML("payGameOver",true);
		 SaveBooleanToXML("payResultBool",false);
		 CCLog("14************ffffffffffffff");
	 }
	 void Java_quyou_game_tank_billing_success15()
	 {

		 SaveBooleanToXML("payGameOver",true);
		 SaveBooleanToXML("payResultBool",true);
		 CCLog("15************ssssssssssssss");
	 }

	 void Java_quyou_game_tank_billing_fail15()
	 {
		 SaveBooleanToXML("payGameOver",true);
		 SaveBooleanToXML("payResultBool",false);
		 CCLog("15************ffffffffffffff");
	 }
	 void Java_quyou_game_tank_billing_success16()
	 {

		 SaveBooleanToXML("payGameOver",true);
		 SaveBooleanToXML("payResultBool",true);
		 CCLog("16************ssssssssssssss");
	 }

	 void Java_quyou_game_tank_billing_fail16()
	 {
		 SaveBooleanToXML("payGameOver",true);
		 SaveBooleanToXML("payResultBool",false);
		 CCLog("16************ffffffffffffff");
	 }*/
  void Java_quyou_game_tank_billing_success7()
  {
	//CCScene* scene = CCDirector::sharedDirector()->getRunningScene();

	//payGameOver = true;
	//payResultBool = true;;
	SaveBooleanToXML("payGameOver",true);
	SaveBooleanToXML("payResultBool",true);
	CCLog("7777777************ssssssssssssss");
 }

  void Java_quyou_game_tank_billing_fail7()
  {
	//payGameOver = true;
	//payResultBool = false;;
	SaveBooleanToXML("payGameOver",true);
	SaveBooleanToXML("payResultBool",false);
	CCLog("777777************ffffffffffffff");
  }

  void Java_quyou_game_tank_billing_success8()
  {
	//CCScene* scene = CCDirector::sharedDirector()->getRunningScene();

	//payGameOver = true;
	//payResultBool = true;;
	SaveBooleanToXML("payGameOver",true);
	SaveBooleanToXML("payResultBool",true);
	CCLog("88888************ssssssssssssss");
 }

  void Java_quyou_game_tank_billing_fail8()
  {
	//payGameOver = true;
	//payResultBool = false;;
	SaveBooleanToXML("payGameOver",true);
	SaveBooleanToXML("payResultBool",false);
	CCLog("88888************ffffffffffffff");
   }

 }

  void BoxCocosBuilderLayer::updateBillingPoint(float time){
	  #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
	   switch (billingPointIndex){
		  // case 9:
		  // case 10:
          // case 11:
		   case 7:
				   
			   {
				   if(LoadBooleanFromXML("payGameOver")){
					   this->unschedule(schedule_selector(BoxCocosBuilderLayer::updateBillingPoint));
					   if(LoadBooleanFromXML("payResultBool")){//付费成功
						   ((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->setShowPre_buy(id>7?(id-6):id);
						   char lable[50];
						   sprintf(lable,"shopBoxLock%d",id-1);
						   SaveBooleanToXML(lable,true);
					   }
				   }
			   }
			   break;
			// case 13:
		    // case 14:
			// case 15:
			 case 8:
			   {
				   if(LoadBooleanFromXML("payGameOver")){
					   this->unschedule(schedule_selector(BoxCocosBuilderLayer::updateBillingPoint));
					   if(LoadBooleanFromXML("payResultBool")){//付费成功
						   ((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->setShowPre_buy(id>7?(id-6):id);
						   char lable[50];
						   sprintf(lable,"shopBoxLock%d",id-1);
						   SaveBooleanToXML(lable,true);
					   }
				   }
			   }
			   break;
	   }
	  #endif
  }


void BoxCocosBuilderLayer::onbutton_buyClicked(CCObject * pSender){
  
	UserRecord *urTank = new UserRecord("tank.db","tank");
	int gold = urTank->readIntFromSQLite("gold");//金钱
	urTank->closeDB();
	free(urTank);

 if(gold>=((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->price[id-1][0]){
	if(!buyClickBool){
	  CCLog("==================1111111");
	  buyClickBool = true;
	 CCSize size =CCDirector::sharedDirector()->getWinSize();
	 CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	 cocos2d::extension::CCBReader *ccbreader = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
	 ccbreader->autorelease();
	 //读取一个ccbi的文件,生成一个ccnode实例
	 CCNode  *animationsTest  = ccbreader->readNodeGraphFromFile("shop/effect_level1.ccbi",this);//tank11.ccbi
	 //设置坐标
	 animationsTest->setPosition(ccp(0,0));
	 ccbreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(BoxCocosBuilderLayer::callBackButton_Buy)); 
	 if(animationsTest!= NULL) {
		 this->addChild(animationsTest,1,2986);
	 }
	 
	 
	  ((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->playBuyAndUpgradeEffect();
	}
	
  }else{
	  ((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->noMoney();
  }


}

void BoxCocosBuilderLayer::callBackButton_Buy(){
  
	CCLog("==================22222222");
	buyClickBool = false;

	UserRecord *urgold = new UserRecord("tank.db","tank");
	int gold = urgold->readIntFromSQLite("gold");
	urgold->saveInt("gold",gold-((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->price[id-1][0]);
	urgold->closeDB();
	free(urgold);

	ShopCocosBuilderLayer*  shopLayer = ((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent());
	shopLayer->setShowGold(gold-((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->price[id-1][0]);

	mAnimationManager->runAnimationsForSequenceNamed("level1");
	((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->buySuccess();

	char lable[30];
	UserRecord *ur = new UserRecord("tank.db","shop");
	sprintf(lable,"shopbox%d",id);
	//int grade = ur->readIntFromSQLite(lable);//等级
	ur->saveInt(lable,1);//保存等级
	ur->closeDB();
	free(ur);


	((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->showGradeMoney(id-1,1);
	//------------------ 6,9-----------------------
	UserRecord *achieveUR = new UserRecord("tank.db","achieveRecord");
	char taskname[30];

	sprintf(taskname,"achieveRecord%d", 5);
	int achieveRecord5 = achieveUR->readIntFromSQLite(taskname);

	CCLOG("achieveRecord5:%d",achieveRecord5);

	if(achieveRecord5+1<=6){
		achieveUR->saveInt(taskname,achieveRecord5+1);
		if(achieveRecord5+1==6){
			showAchieve(6);
		}
	}

	sprintf(taskname,"achieveRecord%d", 8);
	int achieveRecord8 = achieveUR->readIntFromSQLite(taskname);

	CCLOG("achieveRecord8:%d",achieveRecord8);

	if(achieveRecord8+1<=12){
		achieveUR->saveInt(taskname,achieveRecord8+1);
		if(achieveRecord8+1==12){
			showAchieve(9);
		}
	}
	achieveUR->closeDB();
	free(achieveUR);
	 
	this->scheduleOnce(schedule_selector(BoxCocosBuilderLayer::removeChildEffect_level),2.0f );
}
void BoxCocosBuilderLayer::removeChildEffect_level(float time){
	buyClickBool = false;
	if(this->getChildByTag(2986)){
		this->removeChildByTag(2986,-1);
	}
}

void BoxCocosBuilderLayer::showAchieve(int index){
	CCSize size =CCDirector::sharedDirector()->getWinSize();
	char pszFileName[30];

  switch(index){
	case 6:
		{
			sprintf(pszFileName,"achieve/acv_com%d.ccbi", index);
			CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
			cocos2d::extension::CCBReader *ccbreader = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
			ccbreader->autorelease();
			//读取一个ccbi的文件,生成一个ccnode实例
			CCNode  *animationsTest  = ccbreader->readNodeGraphFromFile(pszFileName,this);//tank11.ccbi
			//设置坐标 
			animationsTest->setPosition(ccp(size.width/2,size.height/2)); 

			//ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("GoUp");
			ccbreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(BoxCocosBuilderLayer::showAchieveEnd)); 

			if(animationsTest!= NULL) {     
				((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->addChild(animationsTest,1,5000);
			}
		}
		break;
	case 8:
		{
			UserRecord *achieveUR = new UserRecord("tank.db","achieveRecord");
			char taskname[30];

			sprintf(taskname,"achieveRecord%d", 7);
			int achieveRecord7 = achieveUR->readIntFromSQLite(taskname);

			

		  if(achieveRecord7==0){
			sprintf(pszFileName,"achieve/acv_com%d.ccbi", index);
			CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
			cocos2d::extension::CCBReader *ccbreader = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
			ccbreader->autorelease();
			//读取一个ccbi的文件,生成一个ccnode实例
			CCNode  *animationsTest  = ccbreader->readNodeGraphFromFile(pszFileName,this);//tank11.ccbi
			//设置坐标 
			animationsTest->setPosition(ccp(size.width/2,size.height/2)); 

			//ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("GoUp");
			ccbreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(BoxCocosBuilderLayer::showAchieveEnd)); 

			if(animationsTest!= NULL) {     
				((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->addChild(animationsTest,1,5000);
			}

			achieveUR->saveInt(taskname,1);
		  }
		  achieveUR->closeDB();
		  free(achieveUR);
		}
		break;
	case 9:
		{
			sprintf(pszFileName,"achieve/acv_com%d.ccbi", index);
			CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
			cocos2d::extension::CCBReader *ccbreader = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
			ccbreader->autorelease();
			//读取一个ccbi的文件,生成一个ccnode实例
			CCNode  *animationsTest  = ccbreader->readNodeGraphFromFile(pszFileName,this);//tank11.ccbi
			//设置坐标 
			animationsTest->setPosition(ccp(size.width/2,size.height/2)); 

			//ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("GoUp");
			ccbreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(BoxCocosBuilderLayer::showAchieveEnd)); 

			if(animationsTest!= NULL) {
				((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->addChild(animationsTest,1,5000);
			}
		}
		break;
	}
	
}
void BoxCocosBuilderLayer::showAchieveEnd(){
	if(((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->getChildByTag(5000)){
		((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->removeChildByTag(5000,-1);
	}
}
void BoxCocosBuilderLayer::onbutton_equipClicked(CCObject * pSender){
	char lable[30];
	((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->playEquipEffect();
	
	if(king==0){
	   UserRecord *gradeUR = new UserRecord("tank.db","shop");
	   sprintf(lable,"shopbox%d",id);
	   int grade = gradeUR->readIntFromSQLite(lable);//等级
	   gradeUR->closeDB();
	   free(gradeUR);

	   UserRecord *ur = new UserRecord("tank.db","tank");
	   int turretId = ur->readIntFromSQLite("turretId");
	   int turretGrade= ur->readIntFromSQLite("turretGrade");

	   ur->saveInt("turretId",id);//
	   ur->saveInt("turretGrade",grade);//等级
	   ur->closeDB();
	   free(ur);

	  ((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->setChangeAnimation(turretId,turretGrade);

	   sprintf(lable,"level%d_e",grade);
	   mAnimationManager->runAnimationsForSequenceNamed(lable);


	   //更改
	   ShopCocosBuilderLayer*  shopLayer = ((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent());
	   shopLayer->turretId = id;
	   shopLayer->turretGrade = grade;

	   shopLayer->setShowBar();

	   shopLayer->showTanker();


	   CCLog("BoxCocosBuilderLayer::onControlShopBox===");
	   CCNode* node=((((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->getChildByTag(500))->getChildByTag(1))->getChildByTag(2018);
	   node->setPosition(ccp(115+(id-1)*175,node->getPositionY()));
	   node->setVisible(true);


	}else if(king==1){
	   UserRecord *gradeUR = new UserRecord("tank.db","shop");
	   sprintf(lable,"shopbox%d",id);
	   int grade = gradeUR->readIntFromSQLite(lable);//等级
	   gradeUR->closeDB();
	   free(gradeUR);

	   UserRecord *ur = new UserRecord("tank.db","tank");
	   int chassisId = ur->readIntFromSQLite("chassisId");
	   int chassisGrade= ur->readIntFromSQLite("chassisGrade");

	   ur->saveInt("chassisId",id-6);//
	   ur->saveInt("chassisGrade",grade);//等级
	   ur->closeDB();
	   free(ur);

	   ((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->setChangeAnimation(chassisId,chassisGrade);

	    sprintf(lable,"level%d_e",grade);
	    mAnimationManager->runAnimationsForSequenceNamed(lable);

		//更改
		ShopCocosBuilderLayer*  shopLayer = ((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent());
		shopLayer->chassisId = id-6;
		shopLayer->chassisGrade = grade;

		shopLayer->setShowBar();
		shopLayer->showTanker();

		CCLog("BoxCocosBuilderLayer::onControlShopBox===");
		CCNode* node=((((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->getChildByTag(500))->getChildByTag(1))->getChildByTag(2018);
		node->setPosition(ccp(115+(id-6-1)*175,node->getPositionY()));
		 node->setVisible(true);
	}
}

void BoxCocosBuilderLayer::onbutton_upgradeClicked(CCObject * pSender){

	char lable[30];
	UserRecord *ur = new UserRecord("tank.db","shop");
	sprintf(lable,"shopbox%d",id);
	int grade = ur->readIntFromSQLite(lable);//等级
	ur->closeDB();
	free(ur);

	UserRecord *urTank = new UserRecord("tank.db","tank");
	int gold = urTank->readIntFromSQLite("gold");//金钱
	urTank->closeDB();
	free(urTank);

	//id-1,grade+1
  if(gold>=((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->price[id-1][grade]){
	 if(!buyClickBool){
	  UserRecord *urgold = new UserRecord("tank.db","tank");
	  urgold->saveInt("gold",gold-((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->price[id-1][grade]);
	  urgold->closeDB();
	  free(urgold);
	  ShopCocosBuilderLayer*  shopLayer = ((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent());
	  shopLayer->setShowGold(gold-((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->price[id-1][grade]);


	((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->playBuyAndUpgradeEffect();
	((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->playUpgradeEffect();

	
		CCSize size =CCDirector::sharedDirector()->getWinSize();
		CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		cocos2d::extension::CCBReader *ccbreader = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
		ccbreader->autorelease();
		//读取一个ccbi的文件,生成一个ccnode实例
		
		char lable[30];
		sprintf(lable,"shop/effect_level%d.ccbi",grade+1);
		CCNode  *animationsTest  = ccbreader->readNodeGraphFromFile(lable,this);//tank11.ccbi
		//设置坐标
		animationsTest->setPosition(ccp(0,0));
		ccbreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(BoxCocosBuilderLayer::callBackButton_Upgrade)); 
		if(animationsTest!= NULL) {
			this->addChild(animationsTest,1,2986);
		}
		//更改
		//ShopCocosBuilderLayer*  shopLayer = ((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent());
		if(king==0){
			shopLayer->turretId = id;
			shopLayer->turretGrade = grade+1;
		}else if(king==1){
			shopLayer->chassisId = id-6;
			shopLayer->chassisGrade = grade+1;
		}

		shopLayer->setShowBar();

		shopLayer->showTanker();
		buyClickBool = true;
	}
  }else{
	  ((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->noMoney();
  }
}

void  BoxCocosBuilderLayer::callBackButton_Upgrade(){
	//buyClickBool = false;
	/*if(this->getChildByTag(2986)){
		this->removeChildByTag(2986,-1);
	}*/
	this->scheduleOnce(schedule_selector(BoxCocosBuilderLayer::removeChildEffect_level),2.0f );

	((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->upGrade();

	int  _id=0;
	int  _grad = 0;

	int index = 0;

	if(king==0){
		UserRecord *ur = new UserRecord("tank.db","tank");
		_id = ur->readIntFromSQLite("turretId");
		_grad= ur->readIntFromSQLite("turretGrade");

		index = id;
		if(_id==index){
			ur->saveInt("turretGrade",_grad+1);
		}
		ur->closeDB();
		free(ur);

	}else if(king==1){
		UserRecord *ur = new UserRecord("tank.db","tank");
		_id = ur->readIntFromSQLite("chassisId");
		_grad= ur->readIntFromSQLite("chassisGrade");

		index = id-6;
		if(_id==index){
			ur->saveInt("chassisGrade",_grad+1);
		}
		ur->closeDB();
		free(ur);
	}

	CCLOG("--------------------------_id:%d",_id);
	CCLOG("--------------------------_grad:%d",_grad);
	CCLOG("--------------------------index:%d",index);
	char lable[30];
	UserRecord *ur = new UserRecord("tank.db","shop");
	sprintf(lable,"shopbox%d",id);
	int grade = ur->readIntFromSQLite(lable);//等级
	if(grade<3){
		ur->saveInt(lable,grade+1);//保存等级
		if(_id==index){//已经装备
			sprintf(lable,"level%d_e",grade+1);
		}else{
			sprintf(lable,"level%d",grade+1);
		}
		mAnimationManager->runAnimationsForSequenceNamed(lable);

		((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->showGradeMoney(id-1,grade+1);
	}
	ur->closeDB();
	free(ur);

	this->scheduleOnce(schedule_selector(BoxCocosBuilderLayer::removeChildEffect_level),2.0f );
}

void  BoxCocosBuilderLayer::onbutton_buyItemClicked(CCObject * pSender){
	
	if(id==13){//购买地雷
		UserRecord *ur = new UserRecord("tank.db","tank");
		int gold = ur->readIntFromSQLite("gold");
		int landmine = ur->readIntFromSQLite("landmine");

		XMLParser *pXmlParser = XMLParser::parseWithFile("shop/landmine_price.xml");
		CCString  *price = pXmlParser->getString("landmine");//地雷价格
		/*if(landmine>=5){
			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->upFull();
			ur->closeDB();
			free(ur);
			return ;
		}*/

		if(gold>=price->intValue()){//金钱足够
			int landmineNum = ur->readIntFromSQLite("landmine");//地雷数量
			ur->saveInt("landmine",landmineNum+1);

			ur->closeDB();
			free(ur);

			char lable[30];
			sprintf(lable,"%d",landmineNum+1);
			((CCLabelAtlas*)(this->getChildByTag(1)))->setString(lable);

			ShopCocosBuilderLayer*  shopLayer = ((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent());
			shopLayer->setShowGold(gold-price->intValue());

			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->playBuyAndUpgradeEffect();
			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->buySuccess();
		}else{
			ur->closeDB();
			free(ur);

			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->playBuyAndUpgradeEffect();
			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->noMoney();
		}

		


	}else if (id==14){//炸弹
		UserRecord *ur = new UserRecord("tank.db","tank");
		
		int gold = ur->readIntFromSQLite("gold");
		int bomb = ur->readIntFromSQLite("bomb");

		XMLParser *pXmlParser = XMLParser::parseWithFile("shop/bomb_price.xml");
		CCString  *price = pXmlParser->getString("bomb");//

		/*if(bomb>=3){
			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->upFull();
			ur->closeDB();
			free(ur);
			return ;
		}*/
		if(gold>=price->intValue()){//金钱足够

			int bombNum = ur->readIntFromSQLite("bomb");//炸弹数量
			ur->saveInt("bomb",bombNum+1);

			ur->closeDB();
			free(ur);

			char lable[30];
			sprintf(lable,"%d",bombNum+1);

			((CCLabelAtlas*)(this->getChildByTag(1)))->setString(lable);

			ShopCocosBuilderLayer*  shopLayer = ((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent());
			shopLayer->setShowGold(gold-price->intValue());

			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->playBuyAndUpgradeEffect();
			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->buySuccess();
		}else{
			ur->closeDB();
			free(ur);

			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->playBuyAndUpgradeEffect();
			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->noMoney();
		}

	}/*else if(id==15){

		UserRecord *ur = new UserRecord("tank.db","tank");
		

		int gold = ur->readIntFromSQLite("gold");
		XMLParser *pXmlParser = XMLParser::parseWithFile("shop/addLifeIndex_price.xml");
		CCString  *price = pXmlParser->getString("addLifeIndex0");//

		if(gold>=price->intValue()){//金钱足够
			int addLifeIndex = ur->readIntFromSQLite("addLifeIndex");//
			ur->saveInt("addLifeIndex",addLifeIndex+1);
			ur->closeDB();
			free(ur);

			char lable[30];
			sprintf(lable,"level%d",addLifeIndex+1);
			mAnimationManager->runAnimationsForSequenceNamed(lable);

			XMLParser *pXmlParser = XMLParser::parseWithFile("shop/addLifeIndex_price.xml");
			CCString  *price = pXmlParser->getString("addLifeIndex0");//

			ShopCocosBuilderLayer*  shopLayer = ((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent());
			shopLayer->setShowGold(gold-price->intValue());

			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->playBuyAndUpgradeEffect();
			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->buySuccess();
		}else{
			ur->closeDB();
			free(ur);

			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->playBuyAndUpgradeEffect();
			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->noMoney();
		}
		
	}*/else if (id==15){
		UserRecord *ur = new UserRecord("tank.db","tank");
		

		int gold = ur->readIntFromSQLite("gold");
		XMLParser *pXmlParser = XMLParser::parseWithFile("shop/addAttackIndex_price.xml");
		CCString  *price = pXmlParser->getString("addAttackIndex0");//
		if(gold>=price->intValue()){//金钱足够
			int addAttackIndex = ur->readIntFromSQLite("addAttackIndex");//
			ur->saveInt("addAttackIndex",addAttackIndex+1);
			ur->closeDB();
			free(ur);

			char lable[30];

			sprintf(lable,"level%d",addAttackIndex+1);
			mAnimationManager->runAnimationsForSequenceNamed(lable);

			ShopCocosBuilderLayer*  shopLayer = ((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent());
			shopLayer->setShowGold(gold-price->intValue());

			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->playBuyAndUpgradeEffect();
			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->buySuccess();
		}else{
			ur->closeDB();
			free(ur);

			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->playBuyAndUpgradeEffect();
			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->noMoney();
		}

	}else if (id==16){//Defense
		UserRecord *ur = new UserRecord("tank.db","tank");
		

		int gold = ur->readIntFromSQLite("gold");

		XMLParser *pXmlParser = XMLParser::parseWithFile("shop/addDefenseIndex_price.xml");
		CCString  *price = pXmlParser->getString("addDefenseIndex0");//

		if(gold>=price->intValue()){//金钱足够
			int addDefenseIndex = ur->readIntFromSQLite("addDefenseIndex");//
			ur->saveInt("addDefenseIndex",addDefenseIndex+1);
			ur->closeDB();
			free(ur);

			char lable[30];

			sprintf(lable,"level%d",addDefenseIndex+1);
			mAnimationManager->runAnimationsForSequenceNamed(lable);

			ShopCocosBuilderLayer*  shopLayer = ((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent());
			shopLayer->setShowGold(gold-price->intValue());

			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->playBuyAndUpgradeEffect();
			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->buySuccess();
		}else{
			ur->closeDB();
			free(ur);

			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->playBuyAndUpgradeEffect();
			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->noMoney();
		}

	}else if(id==17){//暴击
		UserRecord *ur = new UserRecord("tank.db","tank");
		int gold = ur->readIntFromSQLite("gold");
		
		XMLParser *pXmlParser = XMLParser::parseWithFile("shop/addCritIndex_price.xml");
		CCString  *price = pXmlParser->getString("addCritIndex0");//

		if(gold>=price->intValue()){//金钱足够
			int addCritIndex = ur->readIntFromSQLite("addCritIndex");//
			ur->saveInt("addCritIndex",addCritIndex+1);
			ur->closeDB();
			free(ur);

			char lable[30];

			sprintf(lable,"level%d",addCritIndex+1);
			mAnimationManager->runAnimationsForSequenceNamed(lable);

			ShopCocosBuilderLayer*  shopLayer = ((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent());
			shopLayer->setShowGold(gold-price->intValue());

			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->playBuyAndUpgradeEffect();
			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->buySuccess();
		}else{
			ur->closeDB();
			free(ur);

			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->playBuyAndUpgradeEffect();
			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->noMoney();
		}
	}else if(id==18){//动力
		UserRecord *ur = new UserRecord("tank.db","tank");
		int gold = ur->readIntFromSQLite("gold");


		XMLParser *pXmlParser = XMLParser::parseWithFile("shop/addMoveIndex_price.xml");
		CCString  *price = pXmlParser->getString("addMoveIndex0");//

		if(gold>=price->intValue()){//金钱足够
			int addMoveIndex = ur->readIntFromSQLite("addMoveIndex");//
			ur->saveInt("addMoveIndex",addMoveIndex+1);
			ur->closeDB();
			free(ur);

			char lable[30];
			sprintf(lable,"level%d",addMoveIndex+1);
			mAnimationManager->runAnimationsForSequenceNamed(lable);

			ShopCocosBuilderLayer*  shopLayer = ((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent());
			shopLayer->setShowGold(gold-price->intValue());

			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->playBuyAndUpgradeEffect();
			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->buySuccess();
		}else{
			ur->closeDB();
			free(ur);

			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->playBuyAndUpgradeEffect();
			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->noMoney();
		}
		
	}/*else if(id==20){//绿颜色
		
		 UserRecord *gradeUR = new UserRecord("tank.db","tank");
		 int gold = gradeUR->readIntFromSQLite("gold");
		 gradeUR->closeDB();
		 free(gradeUR);

		 XMLParser *pXmlParser = XMLParser::parseWithFile("shop/colorTank_price.xml");
		 CCString  *price = pXmlParser->getString("greenTank");//

		if(gold>=price->intValue()){//金钱足够
			UserRecord *ur = new UserRecord("tank.db","shop");//商店物品等级
			ur->saveBool("greenTank",true);
			ur->closeDB();
			free(ur);

			mAnimationManager->runAnimationsForSequenceNamed("level1");

			ShopCocosBuilderLayer*  shopLayer = ((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent());
			shopLayer->setShowGold(gold-price->intValue());

			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->playBuyAndUpgradeEffect();
			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->buySuccess();
		}else{
			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->playBuyAndUpgradeEffect();
			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->noMoney();
		}

	}else if(id==21){//蓝颜色
		
		UserRecord *gradeUR = new UserRecord("tank.db","tank");
		int gold = gradeUR->readIntFromSQLite("gold");
		gradeUR->closeDB();
		free(gradeUR);

		XMLParser *pXmlParser = XMLParser::parseWithFile("shop/colorTank_price.xml");
		CCString  *price = pXmlParser->getString("buleTank");//

		if(gold>=price->intValue()){//金钱足够
			UserRecord *ur = new UserRecord("tank.db","shop");//商店物品等级
			ur->saveBool("blueTank",true);//
			ur->closeDB();
			free(ur);

			mAnimationManager->runAnimationsForSequenceNamed("level1");

			ShopCocosBuilderLayer*  shopLayer = ((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent());
			shopLayer->setShowGold(gold-price->intValue());
			showAchieve(8);

			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->playBuyAndUpgradeEffect();
			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->buySuccess();
		}else{
			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->playBuyAndUpgradeEffect();
			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->noMoney();
		}

		

	}else if(id==22){//红颜色
		
		UserRecord *gradeUR = new UserRecord("tank.db","tank");
		int gold = gradeUR->readIntFromSQLite("gold");
		gradeUR->closeDB();
		free(gradeUR);

		XMLParser *pXmlParser = XMLParser::parseWithFile("shop/colorTank_price.xml");
		CCString  *price = pXmlParser->getString("redTank");//

		if(gold>=price->intValue()){//金钱足够
			UserRecord *ur = new UserRecord("tank.db","shop");//商店物品等级
			ur->saveBool("redTank",true);
			ur->closeDB();
			free(ur);
			mAnimationManager->runAnimationsForSequenceNamed("level1");

			ShopCocosBuilderLayer*  shopLayer = ((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent());
			shopLayer->setShowGold(gold-price->intValue());

			showAchieve(8);
			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->playBuyAndUpgradeEffect();
			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->buySuccess();
		}else{
			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->playBuyAndUpgradeEffect();
			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->noMoney();
		}

		
	}*/else if(id==19){
		UserRecord *gradeUR = new UserRecord("tank.db","tank");
		int gold = gradeUR->readIntFromSQLite("gold");
		gradeUR->closeDB();
		free(gradeUR);

		XMLParser *priceXmlParser = XMLParser::parseWithFile("shop/double_price.xml");//双倍
		int price = (priceXmlParser->getString("doubleAttack"))->intValue();

		if(gold>=price){//金钱足够
			UserRecord *ur = new UserRecord("tank.db","shop");//商店物品
			ur->saveInt("doubleAttack",5);
			ur->closeDB();
			free(ur);

			mAnimationManager->runAnimationsForSequenceNamed("level1");

			char lable[30];
			sprintf(lable,"%d",5);
			CCLabelAtlas* moneyNum=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
			moneyNum->setAnchorPoint(ccp(0.5,0.5));
			moneyNum->setPosition(20,90);
			this->addChild(moneyNum,10,2081);


			ShopCocosBuilderLayer*  shopLayer = ((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent());
			shopLayer->setShowBar();
			shopLayer->setShowGold(gold-price);

			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->playBuyAndUpgradeEffect();
			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->buySuccess();


		}else{
			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->playBuyAndUpgradeEffect();
			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->noMoney();
		}

	}else if(id==20){

		UserRecord *gradeUR = new UserRecord("tank.db","tank");
		int gold = gradeUR->readIntFromSQLite("gold");
		gradeUR->closeDB();
		free(gradeUR);

		XMLParser *priceXmlParser = XMLParser::parseWithFile("shop/double_price.xml");//双倍
		int price = (priceXmlParser->getString("doubleDefense"))->intValue();

		if(gold>=price){//金钱足够

		UserRecord *ur = new UserRecord("tank.db","shop");//商店物品
		ur->saveInt("doubleDefense",5);
		ur->closeDB();
		free(ur);

		char lable[30];
		sprintf(lable,"%d",5);
		CCLabelAtlas* moneyNum=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
		moneyNum->setAnchorPoint(ccp(0.5,0.5));
		moneyNum->setPosition(20,90);
		this->addChild(moneyNum,10,2081);

		mAnimationManager->runAnimationsForSequenceNamed("level1");

		ShopCocosBuilderLayer*  shopLayer = ((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent());
		shopLayer->setShowBar();
		shopLayer->setShowGold(gold-price);

		((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->playBuyAndUpgradeEffect();
		((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->buySuccess();
		}else{
			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->playBuyAndUpgradeEffect();
			((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->noMoney();
		}


	}else if(id==21){
		UserRecord *gradeUR = new UserRecord("tank.db","tank");
		int gold = gradeUR->readIntFromSQLite("gold");
		gradeUR->closeDB();
		free(gradeUR);

		XMLParser *priceXmlParser = XMLParser::parseWithFile("shop/double_price.xml");//双倍
		int price = (priceXmlParser->getString("lockTank"))->intValue();

	 if(gold>=price){//金钱足够

		UserRecord *ur = new UserRecord("tank.db","shop");//商店物品
		ur->saveBool("unlockTank",true);
		ur->closeDB();
		free(ur);

		mAnimationManager->runAnimationsForSequenceNamed("level1");

		ShopCocosBuilderLayer*  shopLayer = ((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent());
		shopLayer->setShowGold(gold-price);

		((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->playBuyAndUpgradeEffect();
		((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->buySuccess();

	  }else{
		((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->playBuyAndUpgradeEffect();
		((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->noMoney();

	  }
	}
}

void BoxCocosBuilderLayer::onbutton_equipItemClicked(CCObject * pSender){
	/*((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->playBuyAndUpgradeEffect();

	if(id==20){
		UserRecord *gradeUR = new UserRecord("tank.db","tank");
		gradeUR->saveInt("colorTank",0);

		gradeUR->closeDB();
		free(gradeUR);

		mAnimationManager->runAnimationsForSequenceNamed("level1_e");

		((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->setChangeAnimation(9,1);
		((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->setChangeAnimation(10,1);

	}else if(id==21){
		UserRecord *gradeUR = new UserRecord("tank.db","tank");
		gradeUR->saveInt("colorTank",1);

		gradeUR->closeDB();
		free(gradeUR);

		mAnimationManager->runAnimationsForSequenceNamed("level1_e");

		((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->setChangeAnimation(8,1);
		((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->setChangeAnimation(10,1);
	}else if(id==22){
		UserRecord *gradeUR = new UserRecord("tank.db","tank");
		gradeUR->saveInt("colorTank",2);

		gradeUR->closeDB();
		free(gradeUR);

		mAnimationManager->runAnimationsForSequenceNamed("level1_e");

		((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->setChangeAnimation(9,1);
		((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->setChangeAnimation(8,1);
	}*/


}
void BoxCocosBuilderLayer::onbutton_upgradeItemClicked(CCObject * pSender){
	((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->playBuyAndUpgradeEffect();
	((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent())->upGrade();

	 if(id==13){//升级地雷
		UserRecord *ur = new UserRecord("tank.db","tank");
		int landmineIndex = ur->readIntFromSQLite("landmineIndex");//地雷等级
		ur->saveInt("landmineIndex",landmineIndex+1);
		ur->closeDB();
		free(ur);

		char lable[30];
		sprintf(lable,"level%d",landmineIndex+1);
		mAnimationManager->runAnimationsForSequenceNamed(lable);
	}/*else if(id==15){
		UserRecord *ur = new UserRecord("tank.db","tank");
		int addLifeIndex = ur->readIntFromSQLite("addLifeIndex");
		ur->saveInt("addLifeIndex",addLifeIndex+1);

		int gold = ur->readIntFromSQLite("gold");
		ur->closeDB();
		free(ur);

		char lable[30];

		sprintf(lable,"level%d",addLifeIndex+1);
		mAnimationManager->runAnimationsForSequenceNamed(lable);

		XMLParser *pXmlParser = XMLParser::parseWithFile("shop/addLifeIndex_price.xml");
		sprintf(lable,"addLifeIndex%d",addLifeIndex);
		CCString  *price = pXmlParser->getString(lable);//地雷价格

		ShopCocosBuilderLayer*  shopLayer = ((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent());
		shopLayer->setShowGold(gold-price->intValue());
	}*/else if (id==15){
		UserRecord *ur = new UserRecord("tank.db","tank");
		int addAttackIndex = ur->readIntFromSQLite("addAttackIndex");//
		ur->saveInt("addAttackIndex",addAttackIndex+1);

		int gold = ur->readIntFromSQLite("gold");
		ur->closeDB();
		free(ur);

		char lable[30];

		sprintf(lable,"level%d",addAttackIndex+1);
		mAnimationManager->runAnimationsForSequenceNamed(lable);


		XMLParser *pXmlParser = XMLParser::parseWithFile("shop/addAttackIndex_price.xml");

		sprintf(lable,"addAttackIndex%d",addAttackIndex);
		CCString  *price = pXmlParser->getString(lable);//

		ShopCocosBuilderLayer*  shopLayer = ((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent());
		shopLayer->setShowGold(gold-price->intValue());

		shopLayer->setShowBar();
	}else if (id==16){
		UserRecord *ur = new UserRecord("tank.db","tank");
		int addDefenseIndex = ur->readIntFromSQLite("addDefenseIndex");//
		ur->saveInt("addDefenseIndex",addDefenseIndex+1);

		int gold = ur->readIntFromSQLite("gold");
		ur->closeDB();
		free(ur);

		char lable[30];

		sprintf(lable,"level%d",addDefenseIndex+1);
		mAnimationManager->runAnimationsForSequenceNamed(lable);


		XMLParser *pXmlParser = XMLParser::parseWithFile("shop/addDefenseIndex_price.xml");

		sprintf(lable,"addDefenseIndex%d",addDefenseIndex);
		CCString  *price = pXmlParser->getString(lable);//

		ShopCocosBuilderLayer*  shopLayer = ((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent());
		shopLayer->setShowGold(gold-price->intValue());

		shopLayer->setShowBar();
	}else if(id==17){
		UserRecord *ur = new UserRecord("tank.db","tank");
		int addCritIndex = ur->readIntFromSQLite("addCritIndex");//
		ur->saveInt("addCritIndex",addCritIndex+1);

		int gold = ur->readIntFromSQLite("gold");
		ur->closeDB();
		free(ur);

		char lable[30];

		sprintf(lable,"level%d",addCritIndex+1);
		mAnimationManager->runAnimationsForSequenceNamed(lable);

		
		XMLParser *pXmlParser = XMLParser::parseWithFile("shop/addCritIndex_price.xml");
		sprintf(lable,"addCritIndex%d",addCritIndex);
		CCString  *price = pXmlParser->getString(lable);//

		ShopCocosBuilderLayer*  shopLayer = ((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent());
		shopLayer->setShowGold(gold-price->intValue());

		shopLayer->setShowBar();
	}else if(id==18){
		UserRecord *ur = new UserRecord("tank.db","tank");
		int addMoveIndex = ur->readIntFromSQLite("addMoveIndex");//
		ur->saveInt("addMoveIndex",addMoveIndex+1);

		int gold = ur->readIntFromSQLite("gold");
		ur->closeDB();
		free(ur);

		char lable[30];

		sprintf(lable,"level%d",addMoveIndex+1);
		mAnimationManager->runAnimationsForSequenceNamed(lable);


		XMLParser *pXmlParser = XMLParser::parseWithFile("shop/addMoveIndex_price.xml");
		sprintf(lable,"addMoveIndex%d",addMoveIndex);
		CCString  *price = pXmlParser->getString(lable);//

		ShopCocosBuilderLayer*  shopLayer = ((ShopCocosBuilderLayer*)((this->getParent())->getParent())->getParent());
		shopLayer->setShowGold(gold-price->intValue());

		shopLayer->setShowBar();
	}


}

void  BoxCocosBuilderLayer::setKingANDId(int k,int i){
	king = k;
	id = i;
}