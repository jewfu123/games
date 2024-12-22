#include "MapSelectCocosBuilderLayer.h"
#include "MapSelectCocosBuilderLoader.h"
#include "MenuCocosBuilderLayer.h"
#include "../ShopCocosBuilder/magazine_frameCocosBuilderLayerLoader.h"
#include "GameScene.h"
#include "../teach/shopTeach_1Loader.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
#include <jni.h>
#include "platform/android/jni/JniHelper.h" 
#include <android/log.h> 
#endif 

USING_NS_CC;
USING_NS_CC_EXT;
MapSelectCocosBuilderLayer::MapSelectCocosBuilderLayer(void):
EntranceClickedBool(false),
secondPay(false)
{

}


MapSelectCocosBuilderLayer::~MapSelectCocosBuilderLayer(void)
{

}
void MapSelectCocosBuilderLayer::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader) {

	
}

SEL_MenuHandler MapSelectCocosBuilderLayer::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
	//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "entrance:", MapSelectCocosBuilderLayer::onEntranceClicked);
	return NULL;
}
SEL_CCControlHandler MapSelectCocosBuilderLayer::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
	CCLOG("============onResolveCCBCCControlSelector:%s",pSelectorName);
	CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "entrance:", MapSelectCocosBuilderLayer::onCCControlButtonFunkyClicked);
	return NULL;
}

bool MapSelectCocosBuilderLayer::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
	CCLOG("============pMemberVariableName:%s",pMemberVariableName);

	
	return false;
}

bool MapSelectCocosBuilderLayer::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue){
	   bool bRet = false;
	   CCLOG("============pMemberVariableName:%s",pMemberVariableName);
	   return bRet;
}

 void MapSelectCocosBuilderLayer::onCCControlButtonFunkyClicked(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent){
	  CCLOG("============onCCControlButtonFunkyClicked");

	  if(((MenuLayer*)(this->getParent()))->teach){
		  return ;
	  }
	  int actIndex = ((MenuLayer*)(this->getParent()))->actIndex;
	  int selsecMap = ((MenuLayer*)(this->getParent()))->selsecMap;


	  if((this->getTag()-200)-selsecMap==1){
		  ((MenuLayer*)(this->getParent()))->moveMenu(true);
	  }else if((this->getTag()-200)-selsecMap==-1){
		  ((MenuLayer*)(this->getParent()))->moveMenu(false);
	  }else{

		  if(((MenuLayer*)(this->getParent()))->passCheck[selsecMap+actIndex*10]!=1){
			  return ;
		  }else{
			  mapIndex = selsecMap+actIndex*10;
		  }

		  CCLog("mapIndex:%f",mapIndex);
		  if(mapIndex==2&&!LoadBooleanFromXML("teachShop")){
			  ((MenuLayer*)(this->getParent()))->teach = true;
			  shopTeachShow();
			  //SaveBooleanToXML("teachShop");
			  return;
		  }

		  //if(mapIndex==6){//计费点  mapIndex==11
			  if(!LoadBooleanFromXML("pay0")){
				  if(!(((MenuLayer*)(this->getParent()))->getChildByTag(8510))){
					  /*CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
					  cocos2d::extension::CCBReader *ccbreaderPay = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
					  CCNode  *payNode  = ccbreaderPay->readNodeGraphFromFile("pay/JHJM.ccbi",this);
					  ccbreaderPay->autorelease();

					  ((MenuLayer*)(this->getParent()))->addChild(payNode,100,8510);
					  ccbreaderPay->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(MapSelectCocosBuilderLayer::showPayEnd)); */
					  ysePay(this);
				  }
				  return;
			  }
		 // }
		  
		  //坦克数量少于1
		  UserRecord *gradeUR = new UserRecord("tank.db","tank");
		  int  tankCont = gradeUR->readIntFromSQLite("tankCont");
		  gradeUR->closeDB();
		  free(gradeUR);

		  if(tankCont<=0){//坦克数量
			  /*CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
			  cocos2d::extension::CCBReader *ccbreaderPay = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
			  CCNode  *payNode  = ccbreaderPay->readNodeGraphFromFile("pay/warning_lifebuy.ccbi",this);
			  ccbreaderPay->autorelease();

			  ((MenuLayer*)(this->getParent()))->addChild(payNode,100,8510);
			  ccbreaderPay->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(MapSelectCocosBuilderLayer::showPayTank)); 
			  */
			  yseTankPay(this);
			  return;
		  }
		  if(mapIndex>=40){
			  if(!LoadBooleanFromXML("survival"))//生存之旅
			  {
				  /*CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
				  cocos2d::extension::CCBReader *ccbreaderPay = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
				  CCNode  *payNode  = ccbreaderPay->readNodeGraphFromFile("pay/warning_survival.ccbi",this);
				  ccbreaderPay->autorelease();

				  ((MenuLayer*)(this->getParent()))->addChild(payNode,100,8510);
				  ccbreaderPay->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(MapSelectCocosBuilderLayer::showPaySurvival)); 
				  */
				  return;
			  }
		  }
		  

		  if(!EntranceClickedBool){
			 // EntranceClickedBool = true;
			  ((MenuLayer*)(this->getParent()))->playEffect(22);

			  //创建一个自动释放的CCNode加载库 
			  CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary(); 
			  ccNodeLoaderLibrary->registerCCNodeLoader("magazine_frame", magazine_frameCocosBuilderLayerLoader::loader());//

			  //创建一个自动释放的CCB读取类（CCBReader） 
			  cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary); 
			  ccbReader->autorelease(); 

			  //读取ccbi文件获取根节点 
			  CCNode * node = ccbReader->readNodeGraphFromFile("shop/magazine_frame.ccbi", this); 

			  CCArray* allSeq = ccbReader->getAnimationManager()->getSequences(); 
			  CCLog("magazine_frameCocosBuilderLayer============================");

			  for (int i=0; i<allSeq->count(); i++)
			  {     
				  //获取到每一个Seq 
				  CCBSequence* everySeq = (CCBSequence*)allSeq->objectAtIndex(i);
				  //获取每个序列动作周期、名字以及id 
				  //everySeq->getDuration(); 
				  CCLog("magazine_frameCocosBuilderLayer===========%s",everySeq->getName()) ; 
				  CCLog("magazine_frameCocosBuilderLayer===========%d",everySeq->getSequenceId()) ;
			  } 

			  CCScene * scene = CCScene::create();
			  if(node != NULL) {
				  scene->addChild(node);
			  }
			  scene->setUserObject(ccbReader->getAnimationManager());

			//  CCDirector::sharedDirector()->replaceScene(scene);
			  CCDirector::sharedDirector()->pushScene(scene);


			/*  CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
			  cocos2d::extension::CCBReader *ccbreader = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
			  CCNode  *loading  = ccbreader->readNodeGraphFromFile("scene_loading.ccbi",this);
			  ccbreader->autorelease();
			  loading->setAnchorPoint(ccp(0.5,0.5));
			  loading->setPosition(ccp(this->getParent()->getContentSize().width/2,this->getParent()->getContentSize().height/2));

			  this->getParent()->addChild(loading,1000,300);

			  CCArray* allSeq = ccbreader->getAnimationManager()->getSequences(); 
			  CCLog("============================MapSelectCocosBuilderLayer") ;
			  for (int i=0; i<allSeq->count(); i++)
			  {     
				  //获取到每一个Seq 
				  CCBSequence* everySeq = (CCBSequence*)allSeq->objectAtIndex(i);
				  //获取每个序列动作周期、名字以及id 
				  // everySeq->getDuration(); 
				  CCLog("===========%s",everySeq->getName()) ; 
				  CCLog("===========%d",everySeq->getSequenceId()) ;

			  } 
			  ccbreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(MapSelectCocosBuilderLayer::end)); */
		  }
	  }
 }
  void MapSelectCocosBuilderLayer::showPayTank(){

	  CCSize size =CCDirector::sharedDirector()->getWinSize();

	  CCNode * node =((MenuLayer*)(this->getParent()))->getChildByTag(8510);


	  CCMenuItemImage *yesItem = CCMenuItemImage::create(
		  "ccbResources/active/button_yes0.png",
		  "ccbResources/active/button_yes1.png",
		  this,
		  menu_selector(MapSelectCocosBuilderLayer::yseTankPay));//onShowPayTankSecondClicked
	  yesItem->setPosition(ccp(size.width/2-120, size.height/2-70));

	  CCMenuItemImage *noItem = CCMenuItemImage::create(
		  "ccbResources/active/button_no0.png",
		  "ccbResources/active/button_no1.png",
		  this,
		  menu_selector(MapSelectCocosBuilderLayer::noTankPay));
	  noItem->setPosition(ccp(size.width/2+120, size.height/2-70));

	  CCMenu* pMenu = CCMenu::create(yesItem,noItem, NULL);
	  pMenu->setPosition(CCPointZero);
	  node->addChild(pMenu,1,1);
  }

 void MapSelectCocosBuilderLayer::onShowPayTankSecondClicked(CCObject * pSender){
	 if(!secondPay){
		 CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		 cocos2d::extension::CCBReader *ccbreaderPay = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
		 CCNode  *payNode  = ccbreaderPay->readNodeGraphFromFile("pay/warning_pay.ccbi",this);
		 ccbreaderPay->autorelease();


		 ((MenuLayer*)(this->getParent()))->addChild(payNode,100,7510);

		 CCSize size =CCDirector::sharedDirector()->getWinSize();
		 CCMenuItemImage *yesItem = CCMenuItemImage::create(
			 "ccbResources/active/button_yes0.png",
			 "ccbResources/active/button_yes1.png",
			 this,
			 menu_selector(MapSelectCocosBuilderLayer::yseTankPay));
		 yesItem->setPosition(ccp(size.width/2-100, size.height/2-65));

		 CCMenuItemImage *noItem = CCMenuItemImage::create(
			 "ccbResources/active/button_no0.png",
			 "ccbResources/active/button_no1.png",
			 this,
			 menu_selector(MapSelectCocosBuilderLayer::no));
		 noItem->setPosition(ccp(size.width/2+100, size.height/2-65));

		 CCMenu* pMenu = CCMenu::create(yesItem,noItem, NULL);
		 pMenu->setPosition(CCPointZero);
		 payNode->addChild(pMenu,1,1);

		 secondPay = true;
	 }



  }

  void MapSelectCocosBuilderLayer::yseTankPay(CCObject* pSender){
	// ((MenuLayer*)(this->getParent()))->removeChildByTag(8510,-1);
	 //((MenuLayer*)(this->getParent()))->removeChildByTag(7510,-1);

     #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
	  billingPointIndex = 4;//计费点
	  SaveBooleanToXML("payResultBool",false);
	  SaveBooleanToXML("payGameOver",false);
	  //payResultBool = false;;
	  //payGameOver = false;
	  this->schedule(schedule_selector(MapSelectCocosBuilderLayer::updateBillingPoint), 0.1f);

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
	  int  tankCont = gradeUR->readIntFromSQLite("tankCont");
	  gradeUR->saveInt("tankCont",1);
	  gradeUR->closeDB();
	  free(gradeUR);
	  MenuLayer* node = (MenuLayer*)(this->getParent());
	  CCLabelAtlas* lifeTimeN= (CCLabelAtlas*)((((MenuCocosBuilderLayer*)node->getChildByTag(321))->life_box)->getChildByTag(1));
	  char lable[50];
	  sprintf(lable,"%d",1);
	  lifeTimeN->setString(lable);
#endif
  }

  void MapSelectCocosBuilderLayer::noTankPay(CCObject* pSender){
	  ((MenuLayer*)(this->getParent()))->removeChildByTag(8510,-1);

  }

  void MapSelectCocosBuilderLayer:: showPaySurvival(){

	  CCSize size =CCDirector::sharedDirector()->getWinSize();

	  CCNode * node =((MenuLayer*)(this->getParent()))->getChildByTag(8510);


	  CCMenuItemImage *yesItem = CCMenuItemImage::create(
		  "ccbResources/active/button_yes0.png",
		  "ccbResources/active/button_yes1.png",
		  this,
		  menu_selector(MapSelectCocosBuilderLayer::yseSurvivalPay));
	  yesItem->setPosition(ccp(size.width/2-120, size.height/2-70));

	  CCMenuItemImage *noItem = CCMenuItemImage::create(
		  "ccbResources/active/button_no0.png",
		  "ccbResources/active/button_no1.png",
		  this,
		  menu_selector(MapSelectCocosBuilderLayer::noSurvivalPay));
	  noItem->setPosition(ccp(size.width/2+120, size.height/2-70));

	  CCMenu* pMenu = CCMenu::create(yesItem,noItem, NULL);
	  pMenu->setPosition(CCPointZero);
	  node->addChild(pMenu,1,1);

  }
 void MapSelectCocosBuilderLayer::yseSurvivalPay(CCObject* pSender){
	  ((MenuLayer*)(this->getParent()))->removeChildByTag(8510,-1);

   #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
	  billingPointIndex = 4;//计费点
	  SaveBooleanToXML("payResultBool",false);
	  SaveBooleanToXML("payGameOver",false);
	  //payResultBool = false;;
	  //payGameOver = false;
	  this->schedule(schedule_selector(MapSelectCocosBuilderLayer::updateBillingPoint), 0.1f);

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
	
	  SaveBooleanToXML("survival",false);
	  if(!EntranceClickedBool){
		  EntranceClickedBool = true;
		  ((MenuLayer*)(this->getParent()))->playEffect(22);

		  CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		  cocos2d::extension::CCBReader *ccbreader = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
		  CCNode  *loading  = ccbreader->readNodeGraphFromFile("scene_loading.ccbi",this);
		  ccbreader->autorelease();
		  loading->setAnchorPoint(ccp(0.5,0.5));
		  loading->setPosition(ccp(this->getParent()->getContentSize().width/2,this->getParent()->getContentSize().height/2));

		  this->getParent()->addChild(loading,1000,300);

		  CCArray* allSeq = ccbreader->getAnimationManager()->getSequences(); 
		  CCLog("============================MapSelectCocosBuilderLayer") ;
		  for (int i=0; i<allSeq->count(); i++)
		  {     
			  //获取到每一个Seq 
			  CCBSequence* everySeq = (CCBSequence*)allSeq->objectAtIndex(i);
			  //获取每个序列动作周期、名字以及id 
			  // everySeq->getDuration(); 
			  CCLog("===========%s",everySeq->getName()) ; 
			  CCLog("===========%d",everySeq->getSequenceId()) ;

		  } 
		  ccbreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(MapSelectCocosBuilderLayer::end)); 


	  }

#endif
  }
 void MapSelectCocosBuilderLayer:: noSurvivalPay(CCObject* pSender){
	  ((MenuLayer*)(this->getParent()))->removeChildByTag(8510,-1);
 }
 extern "C"  {
	 void Java_quyou_game_tank_billing_success4()
	 {
		 //CCScene* scene = CCDirector::sharedDirector()->getRunningScene();

		 //payGameOver = true;
		 //payResultBool = true;;
		//  SaveBooleanToXML("pay0",true);
		 SaveBooleanToXML("payGameOver",true);
		 SaveBooleanToXML("payResultBool",true);
		 CCLog("444************ssssssssssssss");
	 }

	 void Java_quyou_game_tank_billing_fail4()
	 {
		 //payGameOver = true;
		 //payResultBool = false;;
		 SaveBooleanToXML("payGameOver",true);
		 SaveBooleanToXML("payResultBool",false);
		 CCLog("444************ffffffffffffff");
	 }
	 void Java_quyou_game_tank_billing_success9()
	 {
		 //CCScene* scene = CCDirector::sharedDirector()->getRunningScene();

		 //payGameOver = true;
		 //payResultBool = true;;
		 SaveBooleanToXML("payGameOver",true);
		 SaveBooleanToXML("payResultBool",true);
		 CCLog("999************ssssssssssssss");
	 }

	 void Java_quyou_game_tank_billing_fail9()
	 {
		 //payGameOver = true;
		 //payResultBool = false;;
		 SaveBooleanToXML("payGameOver",true);
		 SaveBooleanToXML("payResultBool",false);
		 CCLog("999************ffffffffffffff");
	 }
 }
 void MapSelectCocosBuilderLayer::updateBillingPoint(float time){
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	 switch (billingPointIndex)
	 {
	 case 16:{
		 if(LoadBooleanFromXML("payGameOver")){
			 this->unschedule(schedule_selector(MapSelectCocosBuilderLayer::updateBillingPoint));
			 if(LoadBooleanFromXML("payResultBool")){//付费成功
				 SaveBooleanToXML("pay0",true);
				 UserRecord *gradeUR = new UserRecord("tank.db","tank");
				 int gold = gradeUR->readIntFromSQLite("gold");
				 int landmine = gradeUR->readIntFromSQLite("landmine");//地雷
				 int bomb = gradeUR->readIntFromSQLite("bomb");//炸弹


				 int goldNum = 666;
				 char lable[30];
				 sprintf(lable,"%d",goldNum+gold);
				 MenuLayer* node = (MenuLayer*)(this->getParent());
				 CCLabelAtlas* lifeTimeN= (CCLabelAtlas*)((((MenuCocosBuilderLayer*)node->getChildByTag(321))->gold_box)->getChildByTag(2));
				 lifeTimeN->setString(lable);

				 gradeUR->saveInt("gold",goldNum+gold);
				 gradeUR->saveInt("landmine",landmine+10);
				 gradeUR->saveInt("bomb",bomb+5);

				 gradeUR->closeDB();
				 free(gradeUR);

			    }else{//付费失败
					CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary(); 
					ccNodeLoaderLibrary->registerCCNodeLoader("magazine_frame", magazine_frameCocosBuilderLayerLoader::loader());//

					//创建一个自动释放的CCB读取类（CCBReader） 
					cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary); 
					ccbReader->autorelease(); 

					//读取ccbi文件获取根节点 
					CCNode * node = ccbReader->readNodeGraphFromFile("shop/magazine_frame.ccbi", this); 

					CCArray* allSeq = ccbReader->getAnimationManager()->getSequences(); 
					CCLog("updateBillingPoint  payResultBool ============================");

					for (int i=0; i<allSeq->count(); i++)
					{     
						//获取到每一个Seq 
						CCBSequence* everySeq = (CCBSequence*)allSeq->objectAtIndex(i);
						//获取每个序列动作周期、名字以及id 
						//everySeq->getDuration(); 
						CCLog("magazine_frameCocosBuilderLayer===========%s",everySeq->getName()) ; 
						CCLog("magazine_frameCocosBuilderLayer===========%d",everySeq->getSequenceId()) ;
					} 

					CCScene * scene = CCScene::create();
					if(node != NULL) {
						scene->addChild(node);
					}
					scene->setUserObject(ccbReader->getAnimationManager());

					//  CCDirector::sharedDirector()->replaceScene(scene);
					CCDirector::sharedDirector()->pushScene(scene);

			    }
		      }

			 }
			 break;
	  case 14:
		 {
			 if(LoadBooleanFromXML("payGameOver")){
				 this->unschedule(schedule_selector(MapSelectCocosBuilderLayer::updateBillingPoint));
				 if(LoadBooleanFromXML("payResultBool")){//付费成功

					
					 if(!EntranceClickedBool){
						 EntranceClickedBool = true;
						 ((MenuLayer*)(this->getParent()))->playEffect(22);

						 CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
						 cocos2d::extension::CCBReader *ccbreader = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
						 CCNode  *loading  = ccbreader->readNodeGraphFromFile("scene_loading.ccbi",this);
						 ccbreader->autorelease();
						 loading->setAnchorPoint(ccp(0.5,0.5));
						 loading->setPosition(ccp(this->getParent()->getContentSize().width/2,this->getParent()->getContentSize().height/2));

						 this->getParent()->addChild(loading,1000,300);

						 CCArray* allSeq = ccbreader->getAnimationManager()->getSequences(); 
						 CCLog("============================MapSelectCocosBuilderLayer") ;
						 for (int i=0; i<allSeq->count(); i++)
						 {     
							 //获取到每一个Seq 
							 CCBSequence* everySeq = (CCBSequence*)allSeq->objectAtIndex(i);
							 //获取每个序列动作周期、名字以及id 
							 // everySeq->getDuration(); 
							 CCLog("===========%s",everySeq->getName()) ; 
							 CCLog("===========%d",everySeq->getSequenceId()) ;

						 } 
						 ccbreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(MapSelectCocosBuilderLayer::end)); 


					 }


				 }
			 }
		 }
		 break;
	  case 4:
		  {
			  if(LoadBooleanFromXML("payGameOver")){
				  this->unschedule(schedule_selector(MapSelectCocosBuilderLayer::updateBillingPoint));
				  if(LoadBooleanFromXML("payResultBool")){//付费成功
					  UserRecord *gradeUR = new UserRecord("tank.db","tank");
					  int  tankCont = gradeUR->readIntFromSQLite("tankCont");
					  gradeUR->saveInt("tankCont",1);
					  gradeUR->closeDB();
					  free(gradeUR);
					  MenuLayer* node = (MenuLayer*)(this->getParent());
					  CCLabelAtlas* lifeTimeN= (CCLabelAtlas*)((((MenuCocosBuilderLayer*)node->getChildByTag(321))->life_box)->getChildByTag(1));
					  char lable[50];
					  sprintf(lable,"%d",1);
					  lifeTimeN->setString(lable);
				  
				  }
			  }

		  }
		  break;
	 }
	#endif
 }

 void MapSelectCocosBuilderLayer::showPayEnd(){

	 CCSize size =CCDirector::sharedDirector()->getWinSize();

	 CCNode * node =((MenuLayer*)(this->getParent()))->getChildByTag(8510);

	 CCMenuItemImage *yesItem = CCMenuItemImage::create(
		 "ccbResources/JHJM/jlan.png",
		 "ccbResources/JHJM/jlan.png",
		 this,
		 menu_selector(MapSelectCocosBuilderLayer::ysePay));//onShowPayEndSecondClicked
	 yesItem->setPosition(ccp(size.width/2, size.height/2-200));//ccp(size.width/2-80, size.height/2-75)

	 CCMenuItemImage *noItem = CCMenuItemImage::create(
		 "UI/guanbi1.png",
		 "UI/guanbi2.png",
		 this,
		 menu_selector(MapSelectCocosBuilderLayer::noPay));
	 noItem->setPosition(ccp(size.width/2+380, size.height/2+210));

	 CCMenu* pMenu = CCMenu::create(yesItem,noItem, NULL);//
	 pMenu->setPosition(CCPointZero);
	 node->addChild(pMenu,1,1);

 }

 void MapSelectCocosBuilderLayer::onShowPayEndSecondClicked(CCObject * pSender){
	 if(!secondPay){
		 CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		 cocos2d::extension::CCBReader *ccbreaderPay = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
		 CCNode  *payNode  = ccbreaderPay->readNodeGraphFromFile("pay/warning_pay.ccbi",this);
		 ccbreaderPay->autorelease();

		
		 ((MenuLayer*)(this->getParent()))->addChild(payNode,100,7510);

		 CCSize size =CCDirector::sharedDirector()->getWinSize();
		 CCMenuItemImage *yesItem = CCMenuItemImage::create(
			 "ccbResources/active/button_yes0.png",
			 "ccbResources/active/button_yes1.png",
			 this,
			 menu_selector(MapSelectCocosBuilderLayer::ysePay));
		 yesItem->setPosition(ccp(size.width/2-100, size.height/2-65));

		 CCMenuItemImage *noItem = CCMenuItemImage::create(
			 "ccbResources/active/button_no0.png",
			 "ccbResources/active/button_no1.png",
			 this,
			 menu_selector(MapSelectCocosBuilderLayer::no));
		 noItem->setPosition(ccp(size.width/2+100, size.height/2-65));

		 CCMenu* pMenu = CCMenu::create(yesItem,noItem, NULL);
		 pMenu->setPosition(CCPointZero);
		 payNode->addChild(pMenu,1,1);

		 secondPay = true;
	 }
 }

 void MapSelectCocosBuilderLayer::no(CCObject* pSender){
	  secondPay = false;
	 ((MenuLayer*)(this->getParent()))->removeChildByTag(7510,-1);
 }

 void MapSelectCocosBuilderLayer::ysePay(CCObject* pSender){
	secondPay = false;
	//((MenuLayer*)(this->getParent()))->removeChildByTag(8510,-1);
	//((MenuLayer*)(this->getParent()))->removeChildByTag(7510,-1);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
	 billingPointIndex = 16;//计费点
	 SaveBooleanToXML("payResultBool",false);
	 SaveBooleanToXML("payGameOver",false);
	 //payResultBool = false;;
	 //payGameOver = false;
	 this->schedule(schedule_selector(MapSelectCocosBuilderLayer::updateBillingPoint), 0.1f);

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
	 SaveBooleanToXML("pay0",true);
	 UserRecord *gradeUR = new UserRecord("tank.db","tank");
	 int gold = gradeUR->readIntFromSQLite("gold");
	 int landmine = gradeUR->readIntFromSQLite("landmine");//地雷
	 int bomb = gradeUR->readIntFromSQLite("bomb");//炸弹

	 int goldNum = 666;
	 char lable[30];
	 sprintf(lable,"%d",goldNum+gold);
	 MenuLayer* node = (MenuLayer*)(this->getParent());
	 CCLabelAtlas* lifeTimeN= (CCLabelAtlas*)((((MenuCocosBuilderLayer*)node->getChildByTag(321))->gold_box)->getChildByTag(2));
	 lifeTimeN->setString(lable);

	 gradeUR->saveInt("gold",goldNum+gold);
	 gradeUR->saveInt("landmine",landmine+10);
	 gradeUR->saveInt("bomb",bomb+5);

	 gradeUR->closeDB();
	 free(gradeUR);
   #endif
 }
 void MapSelectCocosBuilderLayer::noPay(CCObject* pSender){
	 ((MenuLayer*)(this->getParent()))->removeChildByTag(8510,-1);

 }
 
void MapSelectCocosBuilderLayer::onEntranceClicked(CCObject * pSender){
	 CCLog("-=-=-=MapSelectCocosBuilderLayer::onEntranceClicked :%d",this->getTag());


   int actIndex = ((MenuLayer*)(this->getParent()))->actIndex;
   int selsecMap = ((MenuLayer*)(this->getParent()))->selsecMap;


   if((this->getTag()-200)-selsecMap==1){
	   ((MenuLayer*)(this->getParent()))->moveMenu(true);
   }else if((this->getTag()-200)-selsecMap==-1){
	   ((MenuLayer*)(this->getParent()))->moveMenu(false);
   }else{

   if(((MenuLayer*)(this->getParent()))->passCheck[selsecMap+actIndex*10]!=1){
	   return ;
   }else{
	   mapIndex = selsecMap+actIndex*10;
   }

   if(!EntranceClickedBool){
	 EntranceClickedBool = true;
	  ((MenuLayer*)(this->getParent()))->playEffect(22);

	 CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	 cocos2d::extension::CCBReader *ccbreader = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
	 CCNode  *loading  = ccbreader->readNodeGraphFromFile("scene_loading.ccbi",this);
	 ccbreader->autorelease();
	 loading->setAnchorPoint(ccp(0.5,0.5));
	 loading->setPosition(ccp(this->getParent()->getContentSize().width/2,this->getParent()->getContentSize().height/2));
	
	  this->getParent()->addChild(loading,1000,300);

	  CCArray* allSeq = ccbreader->getAnimationManager()->getSequences(); 
	  CCLog("============================MapSelectCocosBuilderLayer") ;
	  for (int i=0; i<allSeq->count(); i++)
	  {     
		 //获取到每一个Seq 
		 CCBSequence* everySeq = (CCBSequence*)allSeq->objectAtIndex(i);
		    //获取每个序列动作周期、名字以及id 
		 // everySeq->getDuration(); 
		  CCLog("===========%s",everySeq->getName()) ; 
		  CCLog("===========%d",everySeq->getSequenceId()) ;
		  
	  } 
	  ccbreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(MapSelectCocosBuilderLayer::end)); 
	}
   }
}

void MapSelectCocosBuilderLayer::end(){

	CCScene *pScene = GameScene::scene();
    CCDirector::sharedDirector()->replaceScene(pScene);
	if(mapIndex>=40){
		
		survival_time = millisecondNow()+30*60*1000;
		survivalModeTime.setMillis(30*60*1000);
		survivalModeTime.start();
	}
}

void MapSelectCocosBuilderLayer::shopTeachShow(){

	//创建一个自动释放的CCNode加载库 
	CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary(); 
	ccNodeLoaderLibrary->registerCCNodeLoader("instruction_201", shopTeach_1Loader::loader());//

	//创建一个自动释放的CCB读取类（CCBReader） 
	cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary); 
	ccbReader->autorelease(); 

	//读取ccbi文件获取根节点 
	CCNode * node = ccbReader->readNodeGraphFromFile("teach/instruction_201.ccbi", this); 

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

    this->getParent()->addChild(node,1000,3000);

	((CCLayer*)this->getParent())->setTouchEnabled(false);
	((MenuCocosBuilderLayer*)((CCLayer*)this->getParent())->getChildByTag(321))->setTouchEnabled(false);
	((MenuCocosBuilderLayer*)((CCLayer*)this->getParent())->getChildByTag(321))->Button_act->setTouchEnabled(false);
	/*CCScene * scene = CCScene::create();
	CCNodeLoaderLibrary *backloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	cocos2d::extension::CCBReader *backBreader= new cocos2d::extension::CCBReader(backloaderlibrary); 
	backBreader->autorelease(); 
	CCNode  *backNode  = backBreader->readNodeGraphFromFile("shop/scene_shop_background.ccbi",this);
	backNode->setPosition(ccp(0,0));
	scene->addChild(backNode,0,12300);
	
	if(node != NULL) {
		scene->addChild(node);
	}
	CCDirector::sharedDirector()->pushScene(scene);*/
}