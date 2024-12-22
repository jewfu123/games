#include "ShopCocosBuilderLayer.h"
#include "ShopCocosBuilderLayerLoader.h"
#include "cashShopCocosBuilderLayerLoader.h"
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
ShopCocosBuilderLayer::ShopCocosBuilderLayer(void)
	:button_weapon(NULL),
	button_body(NULL),
	button_item(NULL),
	star_box(NULL),
	gold_box(NULL),
	tank_box(NULL),
	move(false),
	isPay(false),
	SeqLength(0),
	no_moneyShow(false)
{

}


ShopCocosBuilderLayer::~ShopCocosBuilderLayer(void)
{
	CC_SAFE_RELEASE(button_weapon);
	CC_SAFE_RELEASE(button_body);
	CC_SAFE_RELEASE(button_item);
	CC_SAFE_RELEASE(star_box);
    CC_SAFE_RELEASE(gold_box);
	CC_SAFE_RELEASE(tank_box);
}

// void  ShopCocosBuilderLayer::onEnter (){
//	CCLog("onEnter");
//	
//	//setShowBar();
//}
void ShopCocosBuilderLayer::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader) {
	/*CCClippingNode *clipper = CCClippingNode::create(CCSprite::create("shop/attribute_substance.png"));//shop/attribute_substance.png
    clipper->setContentSize(CCSize(0,0));  
	clipper->setAnchorPoint(ccp(0, 0.5));
    this->addChild(clipper,1);  
    clipper->setPosition(ccp(130,205));

	CCSprite *content = CCSprite::create("shop/attribute_substance.png");
    content->setAnchorPoint(ccp(0, 0.5));
    content->setPosition( ccp(100, clipper->getContentSize().height / 2));//clipper->getContentSize().width / 2
	clipper->addChild(content);*/
	CCSize size = CCDirector::sharedDirector()->getWinSize();

	XMLParser *priceXmlParser = XMLParser::parseWithFile("shop/price.xml");
	
	char priceName[30];
	for(int i=0;i<12;i++){
		for(int j=0;j<4;j++){
			sprintf(priceName,"price_%d_%d",i,j);
			price[i][j] = (priceXmlParser->getString(priceName))->intValue();
		}
	}

	int point[4]={40,75,110,150};

	CCSprite *attribute_box1 = CCSprite::create("shop/attribute_box.png");
    attribute_box1->setAnchorPoint(ccp(0, 0.5));
    attribute_box1->setPosition(ccp(100,  this->getContentSize().height/ 2-point[0]));//clipper->getContentSize().width / 2
	this->addChild(attribute_box1,1);

	CCSprite *attribute_box2 = CCSprite::create("shop/attribute_box.png");
    attribute_box2->setAnchorPoint(ccp(0, 0.5));
    attribute_box2->setPosition(ccp(100,  this->getContentSize().height/ 2-point[1]));//clipper->getContentSize().width / 2
	this->addChild(attribute_box2,1);

	CCSprite *attribute_box3 = CCSprite::create("shop/attribute_box.png");
    attribute_box3->setAnchorPoint(ccp(0, 0.5));
    attribute_box3->setPosition(ccp(100,  this->getContentSize().height/ 2-point[2]));//clipper->getContentSize().width / 2
	this->addChild(attribute_box3,1);

	CCSprite *attribute_box4 = CCSprite::create("shop/attribute_box.png");
    attribute_box4->setAnchorPoint(ccp(0, 0.5));
    attribute_box4->setPosition(ccp(100,  this->getContentSize().height/ 2-point[3]));//clipper->getContentSize().width / 2
	this->addChild(attribute_box4,1);

	
	UserRecord *gradeUR = new UserRecord("tank.db","tank");

	turretId = gradeUR->readIntFromSQLite("turretId");//炮塔
	turretGrade = gradeUR->readIntFromSQLite("turretGrade");//炮塔等级

	chassisId = gradeUR->readIntFromSQLite("chassisId");//底盘
	chassisGrade = gradeUR->readIntFromSQLite("chassisGrade");//底盘等级
	
	
	CCLOG("11------turretId:%d",turretId);
	CCLOG("11------turretGrade:%d",turretGrade);
	CCLOG("11------chassisId:%d",chassisId);
	CCLOG("11------chassisGrade:%d",chassisGrade);

	gold = gradeUR->readIntFromSQLite("gold");
	star = gradeUR->readIntFromSQLite("star");

	int addAttackIndex = gradeUR->readIntFromSQLite("addAttackIndex");//
	int addDefenseIndex = gradeUR->readIntFromSQLite("addDefenseIndex");//
	int addCritIndex = gradeUR->readIntFromSQLite("addCritIndex");//
	int addMoveIndex = gradeUR->readIntFromSQLite("addMoveIndex");//
	gradeUR->closeDB();
	free(gradeUR);

	UserRecord *ur = new UserRecord("tank.db","shop");//商店物品
	int  _doubleAttack= ur->readIntFromSQLite("doubleAttack");
	int  _doubleDefense= ur->readIntFromSQLite("doubleDefense");

	ur->closeDB();
	free(ur);

	XMLParser *maxXmlParser = XMLParser::parseWithFile("tanker/tankerMAX.xml");

	max[0] = (maxXmlParser->getString("MaxAttack"))->floatValue();
	max[1]  = (maxXmlParser->getString("Maxdefense"))->floatValue();
	max[2]  = (maxXmlParser->getString("MaxmoveSpeed"))->floatValue();
	max[3]  = (maxXmlParser->getString("MaxbulletRange"))->floatValue();

	char layerName[30];
    sprintf(layerName,"tanker/chassis_%d_%d.xml", chassisId,chassisGrade);//坦克种类
	XMLParser *chassisXmlParser = XMLParser::parseWithFile(layerName);

	sprintf(layerName,"tanker/turret_%d_%d.xml", turretId,turretGrade);//坦克种类
	XMLParser *turretXmlParser = XMLParser::parseWithFile(layerName);


	float attack[4]={0,0.06f,0.12f,0.18f};
	float defense[4]={0,0.06f,0.12f,0.18f};
	float crit[4]={0,0.06f,0.12f,0.18f};
	float move[4]={0,0.1f,0.2f,0.3f};

	CCLOG("==------value[0]:%d",(turretXmlParser->getString("bulletRange"))->intValue());
	CCLOG("==------value[0]:%d",(turretXmlParser->getString("rateOfFire"))->intValue());
	float  value[4];
	value[0]= (int)((turretXmlParser->getString("bulletRange"))->intValue()*(attack[addAttackIndex]+1.0)*(_doubleAttack>0?2.0:1.0));//火力
	value[1]= (int)((chassisXmlParser->getString("defense"))->intValue()*((defense[addDefenseIndex]+1.0))*(_doubleDefense>0?2.0:1.0));//装甲
	value[2]= (int)((chassisXmlParser->getString("moveSpeed"))->intValue()*((move[addMoveIndex]+1.0)));//动力
	value[3]= max[3] - (turretXmlParser->getString("rateOfFire"))->floatValue();//射速

	CCLOG("==------value[0]:%f",value[0]);
	CCLOG("==------value[1]:%f",value[1]);
	CCLOG("==------value[2]:%f",value[2]);
	CCLOG("==------value[3]:%f",value[3]);

	//火力\装甲\动力\射程
   for(int i=0;i<4;i++){
	CCSprite * backSprite = CCSprite::create("shop/background_unknown.png");
	backSprite->setPosition(ccp(100, this->getContentSize().height/ 2-point[i]));
	backSprite->setAnchorPoint( ccp(0, 0.5) );
	this->addChild(backSprite,0);

	int kTagClipperNode = 100;

	CCClippingNode *clipper= CCClippingNode::create();
    //clipper->setTag( kTagClipperNode);
	CCLOG("------------------------:%f",(value[i]/max[i]));
    clipper->setContentSize( CCSizeMake((value[i]/max[i])*230, 200) );
    clipper->setAnchorPoint( ccp(0, 0.5) );
    clipper->setPosition( ccp(100, this->getContentSize().height/ 2-point[i]) );
    //clipper->runAction(CCRepeatForever::create(CCRotateBy::create(1, 45)));
    this->addChild(clipper,0,i);


	CCDrawNode *stencil = CCDrawNode::create();
    CCPoint rectangle[4];
    rectangle[0]= ccp(0, 0);
    rectangle[1]= ccp(clipper->getContentSize().width,0);
    rectangle[2]= ccp(clipper->getContentSize().width,clipper->getContentSize().height);
    rectangle[3]= ccp(0, clipper->getContentSize().height);
    //绘制一个矩形
    ccColor4F white = {1, 1, 1, 1};
    stencil->drawPolygon(rectangle,4, white, 1, white);
	//为设置裁减节点类设置“裁减模板”
    clipper->setStencil(stencil);
    //设置裁减节点类所放的内容
	int kTagContentNode = 200;
    CCSprite*content = CCSprite::create("shop/attribute_substance.png");
    content->setTag( kTagContentNode);
    content->setAnchorPoint( ccp(0, 0.5) );
    content->setPosition( ccp(0, clipper->getContentSize().height/ 2) );//clipper->getContentSize().width / 2
    clipper->addChild(content);
  }
   //默认
   button_weapon->selected();
    
   UserRecord *tankUR = new UserRecord("tank.db","tank");
   int  tankCont = tankUR->readIntFromSQLite("tankCont");
   tankUR->closeDB();
   free(tankUR);
   
   char lable[30];
    //生命恢复
   sprintf(lable,"%d",tankCont);
   CCLabelAtlas* lifeNum=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
   lifeNum->setAnchorPoint(ccp(0.5,0.5));
   lifeNum->setPosition(star_box->getContentSize().width/2-15,star_box->getContentSize().height/2);
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
	this->schedule(schedule_selector(ShopCocosBuilderLayer::showTime),1.0f);



	//金钱
	sprintf(lable,"%d",gold);
    CCLabelAtlas* moneyNum=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
	moneyNum->setAnchorPoint(ccp(0.5,0.5));
	moneyNum->setPosition(gold_box->getContentSize().width/2+10,gold_box->getContentSize().height/2);
	gold_box->addChild(moneyNum,1,2);

	WeapShoew();

	//---------------------------------------显示坦克--------------------------------------------
	tanker *tank =tanker::spriteCreate(chassisId,turretId,chassisGrade,turretGrade);
	tank->setTankerAction();
	tank->setPosition(ccp(130,100));
	tank->setState(tanker::moveRight);
	tank->upDataNewBirth(0.1);
	//this->addChild(tank,1,100);
	tank_box->addChild(tank,1,100);

    this->setTouchEnabled(true); 


	//-------------------------------购买成功--------------------------------
//	 CCSize size =CCDirector::sharedDirector()->getWinSize();
	CCNodeLoaderLibrary *burSuccessloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	buySuccessBreader= new cocos2d::extension::CCBReader(burSuccessloaderlibrary); 
	CCNode  *buySccessNode  = buySuccessBreader->readNodeGraphFromFile("shop/UI_box_goumaichenggong.ccbi",this);

	buySccessNode->setPosition(ccp(size.width/2,size.height/2));

	if(buySccessNode  != NULL) {
		this->addChild(buySccessNode,12,2013);
	}
	buySccessNode->setVisible(false);
	 //------------------------------金钱不足------------------------------

	 CCNodeLoaderLibrary *noMoneyloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	 noMoneyBreader= new cocos2d::extension::CCBReader(noMoneyloaderlibrary); 
	 CCNode  *noMoneyNode  = noMoneyBreader->readNodeGraphFromFile("shop/UI_box_jinqianbuzu.ccbi",this);

	 noMoneyNode->setPosition(ccp(size.width/2,size.height/2));

	 if(noMoneyNode  != NULL) {
		 this->addChild(noMoneyNode,12,2014);
	 }
	 noMoneyNode->setVisible(false);
	 //-------------------------------升级成功-----------------------------------

	 CCNodeLoaderLibrary *upEquiploaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	 upGradeBreader= new cocos2d::extension::CCBReader(upEquiploaderlibrary); 
	 CCNode  *upGradeNode  = upGradeBreader->readNodeGraphFromFile("shop/UI_box_shengjichenggong.ccbi",this);

	 upGradeNode->setPosition(ccp(size.width/2,size.height/2));

	 if(upGradeNode  != NULL) {
		  this->addChild(upGradeNode,12,2015);
	 }
	  upGradeNode->setVisible(false);
   //--------------------------------购买已满-----------------------------------
	  CCNodeLoaderLibrary *fullloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	  fullBreader= new cocos2d::extension::CCBReader(fullloaderlibrary); 
	  CCNode  *fullNode  = fullBreader->readNodeGraphFromFile("shop/UI_box_full.ccbi",this);

	  fullNode->setPosition(ccp(size.width/2,size.height/2));

	  if(fullNode  != NULL) {
		  this->addChild(fullNode,12,2016);
	  }
	  fullNode->setVisible(false);

	  CCMenuItemImage *leftItem = CCMenuItemImage::create(
		  "shop/botton.png",
		  "shop/botton.png",
		  this,
		  menu_selector(ShopCocosBuilderLayer::leftButton));
	  leftItem->setPosition(ccp(size.width/2-15, size.height/2+20));

	  CCMenuItemImage *rightItem = CCMenuItemImage::create(
		  "shop/botton.png",
		  "shop/botton.png",
		  this,
		  menu_selector(ShopCocosBuilderLayer::rightButton));
	  rightItem->setPosition(ccp(size.width/2+370, size.height/2+20));

	  CCMenu* pMenu = CCMenu::create(leftItem,rightItem, NULL);
	  pMenu->setPosition(CCPointZero);
	  this->addChild(pMenu,1,1008);
	SimpleAudioEngine::sharedEngine()->preloadEffect("sound/15.ogg");//购买升级成功
	SimpleAudioEngine::sharedEngine()->preloadEffect("sound/19.ogg");//装备成功
	SimpleAudioEngine::sharedEngine()->preloadEffect("sound/10.ogg");//普通按钮
	SimpleAudioEngine::sharedEngine()->preloadEffect("sound/21.ogg");//升级成功

}
void ShopCocosBuilderLayer::registerWithTouchDispatcher(void)
{
     CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}
void ShopCocosBuilderLayer::leftButton(CCObject* pSender){
	if(move){
		return;
	}
	CCNode *node = (this->getChildByTag(500))->getChildByTag(1);
	if(index>1){
		move = true;

		CCActionInterval* move = CCMoveBy::create(0.5, ccp(175,0));
		CCActionInterval* move_ease_in = CCEaseElasticOut::create (move, 2.5);
		node->runAction(CCSequence::create(move_ease_in, CCCallFunc::create(this,callfunc_selector(ShopCocosBuilderLayer::callBackMoveAction)),NULL));
		index--;
	}

}
void ShopCocosBuilderLayer::rightButton(CCObject* pSender){
	if(move){
		return;
	}
	CCNode *node = (this->getChildByTag(500))->getChildByTag(1);

	if(index<SeqLength-1){
		move = true;

		CCActionInterval* move = CCMoveBy::create(0.5, ccp(-175,0));
		CCActionInterval* move_ease_in = CCEaseElasticOut::create (move, 2.5);
		node->runAction(CCSequence::create(move_ease_in, CCCallFunc::create(this,callfunc_selector(ShopCocosBuilderLayer::callBackMoveAction)),NULL));
		index++;
	}



}
void ShopCocosBuilderLayer::showTime(float  t){
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

void ShopCocosBuilderLayer::buySuccess(){

	 this->getChildByTag(2013)->setVisible(true);
	 buySuccessBreader->getAnimationManager()->runAnimationsForSequenceNamed("1st");
}


void ShopCocosBuilderLayer::noMoney(){
	yesPay(this);
	/*this->getChildByTag(2014)->setVisible(true);
	noMoneyBreader->getAnimationManager()->runAnimationsForSequenceNamed("1st");

	CCSize size =CCDirector::sharedDirector()->getWinSize();
	CCMenuItemImage *yesItem = CCMenuItemImage::create(
		"ccbResources/active/button_yes0.png",
		"ccbResources/active/button_yes1.png",
		this,
		menu_selector(ShopCocosBuilderLayer::yesPay));//yes
	yesItem->setPosition(ccp(-100, -65));

	CCMenuItemImage *noItem = CCMenuItemImage::create(
		"ccbResources/active/button_no0.png",
		"ccbResources/active/button_no1.png",
		this,
		menu_selector(ShopCocosBuilderLayer::no));
	noItem->setPosition(ccp(100, -65));

	CCMenu* pMenu = CCMenu::create(yesItem,noItem, NULL);
	pMenu->setPosition(CCPointZero);
	this->getChildByTag(2014)->addChild(pMenu,1,1);
	*/
}
void ShopCocosBuilderLayer::yes(CCObject* pSender){
	if(!no_moneyShow){
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
			menu_selector(ShopCocosBuilderLayer::yesPay));
		yesItem->setPosition(ccp(size.width/2-100, size.height/2-65));

		CCMenuItemImage *noItem = CCMenuItemImage::create(
			"ccbResources/active/button_no0.png",
			"ccbResources/active/button_no1.png",
			this,
			menu_selector(ShopCocosBuilderLayer::noPay));
		noItem->setPosition(ccp(size.width/2+100, size.height/2-65));

		CCMenu* pMenu = CCMenu::create(yesItem,noItem, NULL);
		pMenu->setPosition(CCPointZero);
		payNode->addChild(pMenu,1,1);

		no_moneyShow = true;
	}
}
void ShopCocosBuilderLayer::no(CCObject* pSender){

	this->getChildByTag(2014)->removeChildByTag(1,-1);
	this->getChildByTag(2014)->setVisible(false);

}
void ShopCocosBuilderLayer::yesPay(CCObject* pSender){

	CCLog("onGold1Clicked");
	playBottnEffect();
	//this->removeChildByTag(8510,-1);

	this->getChildByTag(2014)->removeChildByTag(1,-1);
	this->getChildByTag(2014)->setVisible(false);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
	billingPointIndex = 0;//计费点
	SaveBooleanToXML("payResultBool",false);
	SaveBooleanToXML("payGameOver",false);
	//payResultBool = false;;
	//payGameOver = false;
	this->schedule(schedule_selector(ShopCocosBuilderLayer::updateBillingPoint), 0.1f);

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
	no_moneyShow = false;

}
void ShopCocosBuilderLayer::addGold(int _gold){
	UserRecord *urgold = new UserRecord("tank.db","tank");
	int gold = urgold->readIntFromSQLite("gold");
	urgold->saveInt("gold",gold+_gold);
	urgold->closeDB();
	free(urgold);
	this->setShowGold(gold+_gold);

}
/*extern "C"  {
	void Java_quyou_game_tank_billing_success17()
	{

		SaveBooleanToXML("payGameOver",true);
		SaveBooleanToXML("payResultBool",true);
		CCLog("17************ssssssssssssss");
	}

	void Java_quyou_game_tank_billing_fail17()
	{
		SaveBooleanToXML("payGameOver",true);
		SaveBooleanToXML("payResultBool",false);
		CCLog("17************ffffffffffffff");
	}

}*/
void ShopCocosBuilderLayer::updateBillingPoint(float time){

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
	switch (billingPointIndex)
	{
	case 0:
		{
			if(LoadBooleanFromXML("payGameOver")){
				this->unschedule(schedule_selector(ShopCocosBuilderLayer::updateBillingPoint));
				if(LoadBooleanFromXML("payResultBool")){//付费成功
					addGold(400);
				}
			}
		}
		break;
	}
#endif
}

void ShopCocosBuilderLayer::noPay(CCObject* pSender){
	no_moneyShow = false;
	this->removeChildByTag(8510,-1);

	
}
void ShopCocosBuilderLayer::upGrade(){
	this->getChildByTag(2015)->setVisible(true);
	upGradeBreader->getAnimationManager()->runAnimationsForSequenceNamed("1st");
}
void ShopCocosBuilderLayer::upFull(){
	this->getChildByTag(2016)->setVisible(true);
	fullBreader->getAnimationManager()->runAnimationsForSequenceNamed("1st");
}

void ShopCocosBuilderLayer::playBottnEffect(){
	if(sound){
		SimpleAudioEngine::sharedEngine()->playEffect("sound/10.ogg",false);
	}
}
void ShopCocosBuilderLayer::playBuyAndUpgradeEffect(){
	if(sound){
		SimpleAudioEngine::sharedEngine()->playEffect("sound/15.ogg",false);
	}
}
void ShopCocosBuilderLayer::playEquipEffect(){
	if(sound){
		SimpleAudioEngine::sharedEngine()->playEffect("sound/19.ogg",false);
	}
}
 void  ShopCocosBuilderLayer::playUpgradeEffect(){
	 if(sound){
		 SimpleAudioEngine::sharedEngine()->playEffect("sound/21.ogg",false);
	 }
 }
SEL_MenuHandler ShopCocosBuilderLayer::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
	CCLog("pSelectorName===%s",pSelectorName);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_back:", ShopCocosBuilderLayer::onBackClicked);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_cashshop", ShopCocosBuilderLayer::onButton_cashshopClicked);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_weapon:", ShopCocosBuilderLayer::onWeaponClicked);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_body:", ShopCocosBuilderLayer::onBodyClicked);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_item:", ShopCocosBuilderLayer::onItemClicked);
	return NULL;
}
SEL_CCControlHandler ShopCocosBuilderLayer::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){

	return NULL;
}

bool ShopCocosBuilderLayer::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
	CCLog("onAssignCCBMemberVariable===%s",pMemberVariableName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_weapon", CCMenuItemImage*, this->button_weapon);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_body", CCMenuItemImage*, this->button_body);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_item", CCMenuItemImage*, this->button_item);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "life_box", CCSprite*, this->star_box);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "gold_box", CCSprite*, this->gold_box);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "tank_box", CCSprite*, this->tank_box);
	
	return false;
}

bool ShopCocosBuilderLayer::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue){
	   bool bRet = false;

	   return bRet;
}
void  ShopCocosBuilderLayer::keyBackClicked(){
	onBackClicked(this);
}
void ShopCocosBuilderLayer::onBackClicked(CCObject * pSender){
	
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
void ShopCocosBuilderLayer::onWeaponClicked(CCObject * pSender){

  WeapShoew();
  button_weapon->selected();
  button_body->unselected();
  button_item->unselected();

  playBottnEffect();
}
void ShopCocosBuilderLayer::onBodyClicked(CCObject * pSender){

	BodyShoew();
	button_weapon->unselected();
	button_body->selected();
	button_item->unselected();
	playBottnEffect();
}

void ShopCocosBuilderLayer::onItemClicked(CCObject * pSender){

	 ItemShow();
	 button_weapon->unselected();
	 button_body->unselected();
	 button_item->selected();

	 playBottnEffect();
}
void ShopCocosBuilderLayer::onButton_cashshopClicked(CCObject * pSender){

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
	//CCDirector::sharedDirector()->pushScene(scene);
	CCDirector::sharedDirector()->replaceScene(scene);

}
void ShopCocosBuilderLayer::showTanker(){
	CCLog("\\\\\\\\\\\\\\\\\\\\chassisId:%d",chassisId);
	CCLog("\\\\\\\\\\\\\\\\\\\\turretId:%d",turretId);
	tank_box->removeChildByTag(100);

	tanker *tank =tanker::spriteCreate(chassisId,turretId,chassisGrade,turretGrade);
	tank->setTankerAction();
	tank->setPosition(ccp(130,100));
	tank->setState(tanker::moveRight);
	tank->upDataNewBirth(0.1);
	//this->addChild(tank,1,100);
	tank_box->addChild(tank,1,100);



}

bool ShopCocosBuilderLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){

	  return true;

}

void ShopCocosBuilderLayer::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
	cocos2d::CCPoint  startPoint = pTouch->getLocationInView();
	startPoint =  CCDirector::sharedDirector()->convertToGL(startPoint);

	CCPoint endPoint = pTouch->getPreviousLocationInView();
	endPoint =  CCDirector::sharedDirector()->convertToGL(endPoint);
	
	//CCLog(" startPoint.x:%f  endPoint.x:%f  ",startPoint.x,endPoint.x);
	if(move){
		return;
	}
	CCNode *node = (this->getChildByTag(500))->getChildByTag(1);

	//向右
	if(endPoint.x-startPoint.x>10.0){
		if(index<SeqLength-1){
		//CCLog("LEFT=============--------------------===");
		  move = true;
		
		  CCActionInterval* move = CCMoveBy::create(0.5, ccp(-175,0));
		  CCActionInterval* move_ease_in = CCEaseElasticOut::create (move, 2.5);
		  node->runAction(CCSequence::create(move_ease_in, CCCallFunc::create(this,callfunc_selector(ShopCocosBuilderLayer::callBackMoveAction)),NULL));
		  index++;
		}else{
			node->setPositionX(node->getPositionX()+(startPoint.x-endPoint.x));

			CCActionInterval* move = CCMoveBy::create(0.5, ccp((endPoint.x-startPoint.x),0));
			CCActionInterval* move_ease_in = CCEaseElasticOut::create (move, 2.5);
			node->runAction(CCSequence::create(move_ease_in, CCCallFunc::create(this,callfunc_selector(ShopCocosBuilderLayer::callBackMoveAction)),NULL));
		}
	//向左
	}else if(startPoint.x-endPoint.x>10){
		if(index>1){
		//CCLog("RIGHT=============--------------------===");
		move = true;

		CCActionInterval* move = CCMoveBy::create(0.5, ccp(175,0));
		CCActionInterval* move_ease_in = CCEaseElasticOut::create (move, 2.5);
		node->runAction(CCSequence::create(move_ease_in, CCCallFunc::create(this,callfunc_selector(ShopCocosBuilderLayer::callBackMoveAction)),NULL));
		 index--;
		}else{
			node->setPositionX(node->getPositionX()+(startPoint.x-endPoint.x));

			CCActionInterval* move = CCMoveBy::create(0.5, ccp((endPoint.x-startPoint.x),0));
			CCActionInterval* move_ease_in = CCEaseElasticOut::create (move, 2.5);
			node->runAction(CCSequence::create(move_ease_in, CCCallFunc::create(this,callfunc_selector(ShopCocosBuilderLayer::callBackMoveAction)),NULL));

		}
	}
}

void ShopCocosBuilderLayer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){


}

void ShopCocosBuilderLayer::callBackMoveAction(){
	move = false;
}

void  ShopCocosBuilderLayer::setShowGold(int goldNum){
	char lable[30];
	sprintf(lable,"%d",goldNum);
	((CCLabelAtlas*)(gold_box->getChildByTag(2)))->setString(lable);

	UserRecord *gradeUR = new UserRecord("tank.db","tank");
	gradeUR->saveInt("gold",goldNum);
	gradeUR->closeDB();
	free(gradeUR);
}

void  ShopCocosBuilderLayer::setShowBar(){
	
	CCLOG("22------turretId:%d",turretId);
	CCLOG("22------turretGrade:%d",turretGrade);
	CCLOG("22------chassisId:%d",chassisId);
	CCLOG("22------chassisGrade:%d",chassisGrade);

	UserRecord *gradeUR = new UserRecord("tank.db","tank");

	int addAttackIndex = gradeUR->readIntFromSQLite("addAttackIndex");//
	int addDefenseIndex = gradeUR->readIntFromSQLite("addDefenseIndex");//
	int addCritIndex = gradeUR->readIntFromSQLite("addCritIndex");//
	int addMoveIndex = gradeUR->readIntFromSQLite("addMoveIndex");//

	 gradeUR->closeDB();
	 free(gradeUR);


	 UserRecord *ur = new UserRecord("tank.db","shop");//商店物品
	 int  _doubleAttack= ur->readIntFromSQLite("doubleAttack");
	 int  _doubleDefense= ur->readIntFromSQLite("doubleDefense");

	 ur->closeDB();
	 free(ur);

	char layerName[30];
	sprintf(layerName,"tanker/chassis_%d_%d.xml", chassisId,chassisGrade);//
	XMLParser *chassisXmlParser = XMLParser::parseWithFile(layerName);

	sprintf(layerName,"tanker/turret_%d_%d.xml", turretId,turretGrade);//
	XMLParser *turretXmlParser = XMLParser::parseWithFile(layerName);

	float attack[4]={0,0.06f,0.12f,0.18f};
	float defense[4]={0,0.06f,0.12f,0.18f};
	float crit[4]={0,0.06f,0.12f,0.18f};
	float move[4]={0,0.1f,0.2f,0.3f};

	
	float  value[4];
	value[0]= (int)((turretXmlParser->getString("bulletRange"))->intValue()*(attack[addAttackIndex]+1.0)*(_doubleAttack>0?2.0:1.0));//火力
	value[1]= (int)((chassisXmlParser->getString("defense"))->intValue()*((defense[addDefenseIndex]+1.0))*(_doubleDefense>0?2.0:1.0));//装甲
	value[2]= (int)((chassisXmlParser->getString("moveSpeed"))->intValue()*((move[addMoveIndex]+1.0)));//动力
	value[3]= max[3] - (turretXmlParser->getString("rateOfFire"))->floatValue();//射速

	CCLOG("==------value[0]:%f",value[0]);
	CCLOG("==------value[1]:%f",value[1]);
	CCLOG("==------value[2]:%f",value[2]);
	CCLOG("==------value[3]:%f",value[3]);
	int point[4]={40,75,110,150};

	for(int i=0;i<4;i++){
		CCClippingNode *clipper=((CCClippingNode*)this->getChildByTag(i));//->setContentSize( CCSizeMake(200, 200)
		clipper->setContentSize(CCSizeMake((value[i]/max[i])*230, 200));

		CCDrawNode *stencil = CCDrawNode::create();
		CCPoint rectangle[4];
		rectangle[0]= ccp(0, 0);
		rectangle[1]= ccp(clipper->getContentSize().width,0);
		rectangle[2]= ccp(clipper->getContentSize().width,clipper->getContentSize().height);
		rectangle[3]= ccp(0, clipper->getContentSize().height);
		ccColor4F white = {1, 1, 1, 1};
		stencil->drawPolygon(rectangle,4, white, 1, white);
		//为设置裁减节点类设置“裁减模板”
		clipper->setStencil(stencil);
	}
}
void ShopCocosBuilderLayer::WeapShoew(){
	CCLayer  *shopLayer = CCLayer::create();
	char lable[30];
	
	if(this->getChildByTag(500)){
		this->removeChildByTag(500,-1);
	}

	UserRecord *ur = new UserRecord("tank.db","tank");
	int turretId = ur->readIntFromSQLite("turretId");
	int turretGrade= ur->readIntFromSQLite("turretGrade");
	ur->closeDB();
	free(ur);

   for(int n=0;n<6;n++){
	 CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	 ccnodeloaderlibrary->registerCCNodeLoader("shopbox", BoxCocosBuilderLayerLoader::loader());
	 cocos2d::extension::CCBReader *ccbreader = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
	 ccbreader->autorelease();
	 sprintf(lable,"shop/shopbox%d.ccbi",n+1);
	 //读取一个ccbi的文件,生成一个ccnode实例
	 CCNode  *animationsTest  = ccbreader->readNodeGraphFromFile(lable,this);//

	 ((BoxCocosBuilderLayer*)animationsTest)->setAnimationManager(ccbreader->getAnimationManager());
	 ((BoxCocosBuilderLayer*)animationsTest)->setKingANDId(0,n+1);
	 //设置坐标 
	 //animationsTest->setPosition(ccp(520,255)); 
	 animationsTest->setAnchorPoint(ccp(0,0.5));
	 animationsTest->setPosition(ccp(115+n*175,shopLayer->getContentSize().height/2)); 
	 if(animationsTest!= NULL){
		 //this->addChild(animationsTest,1,0);
		 shopLayer->addChild(animationsTest,1,n);
	 }

	 //获取所有的动作序列 
	  CCArray* allSeq = ccbreader->getAnimationManager()->getSequences(); 
	  CCLog("shopbox============================") ;
	  for (int i=0; i<allSeq->count(); i++)
	  {     
		 //获取到每一个Seq 
		 CCBSequence* everySeq = (CCBSequence*)allSeq->objectAtIndex(i);
		    //获取每个序列动作周期、名字以及id 
		 // everySeq->getDuration(); 
		  CCLog("shopbox===========%s",everySeq->getName()) ; 
		  CCLog("shopbox===========%d",everySeq->getSequenceId()) ;
		  
	  } 

	   UserRecord *ur = new UserRecord("tank.db","shop");
	   sprintf(lable,"shopbox%d",n+1);
	   int grade = ur->readIntFromSQLite(lable);//等级

	   ur->closeDB();
	   free(ur);

		if(grade==0){
			if(n>4){
				char lable[50];
				sprintf(lable,"shopBoxLock%d",n);
				if(LoadBooleanFromXML(lable)){
					ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("pre-buy");
				}else{
					ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("locked");
				}
			}else{
				ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("pre-buy");
			}
			
		}else if(grade==1){
			if(turretId==n+1){
				ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("level1_e");//
			}else{
				ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("level1");
			}
		}else if(grade==2){
			if(turretId==n+1){
				ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("level2_e");
			}else{
				ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("level2");
			}
		}else if(grade==3){
           if(turretId==n+1){
			   ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("level3_e");
		   }else{
			    ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("level3");
		   }
		}

		if(grade!=3){//显示购买升级需要的金钱
		  sprintf(lable,"%d",price[n][grade]);
		  CCLabelAtlas* moneyNum=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
		  moneyNum->setAnchorPoint(ccp(0.5,0.5));
		  moneyNum->setPosition(-20,58);
		  animationsTest->addChild(moneyNum,10,2081);
		}
	}
  
   	CCClippingNode *clipper= CCClippingNode::create();
    clipper->setTag(500);
    clipper->setContentSize(CCSizeMake(350, 480) );
    clipper->setAnchorPoint( ccp(0, 0.5) );
    clipper->setPosition( ccp(400, this->getContentSize().height/ 2) );
    //clipper->runAction(CCRepeatForever::create(CCRotateBy::create(1, 45)));
   


	CCDrawNode *stencil = CCDrawNode::create();
    CCPoint rectangle[4];
    rectangle[0]= ccp(0, 0);
    rectangle[1]= ccp(clipper->getContentSize().width,0);
    rectangle[2]= ccp(clipper->getContentSize().width,clipper->getContentSize().height);
    rectangle[3]= ccp(0, clipper->getContentSize().height);
    //绘制一个矩形
    ccColor4F white = {1, 1, 1, 1};
    stencil->drawPolygon(rectangle,4, white, 1, white);
	//为设置裁减节点类设置“裁减模板”
    clipper->setStencil(stencil);

	this->addChild(clipper,0,500);

    //设置裁减节点类所放的内容
	shopLayer->setAnchorPoint(ccp(0,0));
	shopLayer->setPosition(ccp(0,clipper->getContentSize().height/2+15));
	clipper->addChild(shopLayer,0,1);

	index = 1;
	SeqLength = 6;

	//----------------------------外发光--------------------------------------
	CCNodeLoaderLibrary *shop_backgroundlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	cocos2d::extension::CCBReader* shop_backgroundBreader= new cocos2d::extension::CCBReader(shop_backgroundlibrary);
	shop_backgroundBreader->autorelease();
	CCNode  *shop_backgroundNode  = shop_backgroundBreader->readNodeGraphFromFile("shop/shopbox_light.ccbi",this);
	shop_backgroundNode->setAnchorPoint(ccp(0,0.5));
	shop_backgroundNode->setPosition(ccp(115,shopLayer->getContentSize().height/2-240));

	if(shop_backgroundNode  != NULL) {
		shopLayer->addChild(shop_backgroundNode,12,2018);
	}
	shop_backgroundNode->setVisible(false);
}

void ShopCocosBuilderLayer::BodyShoew(){
	CCLayer  *shopLayer = CCLayer::create();
	char lable[30];
	
	if(this->getChildByTag(500)){
		this->removeChildByTag(500,-1);
	}

  	UserRecord *ur = new UserRecord("tank.db","tank");
	int chassisId = ur->readIntFromSQLite("chassisId");
	int chassisGrade= ur->readIntFromSQLite("chassisGrade");
	ur->closeDB();
	free(ur);

   for(int n=0;n<6;n++){
	 CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	 ccnodeloaderlibrary->registerCCNodeLoader("shopbox", BoxCocosBuilderLayerLoader::loader());
	 cocos2d::extension::CCBReader *ccbreader = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
	 ccbreader->autorelease();
	 sprintf(lable,"shop/shopbox%d.ccbi",n+7);
	 //读取一个ccbi的文件,生成一个ccnode实例
	 CCNode  *animationsTest  = ccbreader->readNodeGraphFromFile(lable,this);//

	 ((BoxCocosBuilderLayer*)animationsTest)->setAnimationManager(ccbreader->getAnimationManager());
	 ((BoxCocosBuilderLayer*)animationsTest)->setKingANDId(1,n+7);
	 //设置坐标 
	 //animationsTest->setPosition(ccp(520,255)); 
	 animationsTest->setAnchorPoint(ccp(0,0.5));
	 animationsTest->setPosition(ccp(115+n*175,shopLayer->getContentSize().height/2)); 
	 if(animationsTest  != NULL){     
		 //this->addChild(animationsTest,1,0);
		 shopLayer->addChild(animationsTest,1,n);
	 }

	 //获取所有的动作序列 
	  CCArray* allSeq = ccbreader->getAnimationManager()->getSequences(); 
	  CCLog("shopbox============================") ;
	  for (int i=0; i<allSeq->count(); i++)
	  {     
		 //获取到每一个Seq 
		 CCBSequence* everySeq = (CCBSequence*)allSeq->objectAtIndex(i);
		    //获取每个序列动作周期、名字以及id 
		 // everySeq->getDuration(); 
		  CCLog("shopbox===========%s",everySeq->getName()) ; 
		  CCLog("shopbox===========%d",everySeq->getSequenceId()) ;
		  
	  } 

	   UserRecord *ur = new UserRecord("tank.db","shop");
	   sprintf(lable,"shopbox%d",n+7);
	   int grade = ur->readIntFromSQLite(lable);//等级

	   ur->closeDB();
	   free(ur);


		if(grade==0){
			if(n>4){
				char lable[50];
				sprintf(lable,"shopBoxLock%d",n+6);
				if(LoadBooleanFromXML(lable)){
					ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("pre-buy");
				}else{

					ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("locked");
				}
			}else{
				ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("pre-buy");
			}
		}else if(grade==1){
			if(chassisId==n+1){
				ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("level1_e");
			}else{
				ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("level1");
			}
		}else if(grade==2){
			if(chassisId==n+1){
				ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("level2_e");
			}else{
				ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("level2");
			}
		}else if(grade==3){
			if(chassisId==n+1){
				ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("level3_e");
			}else{
				ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("level3");
			}
		}
		if(grade!=3){//显示购买升级需要的金钱
			sprintf(lable,"%d",price[6+n][grade]);
			CCLabelAtlas* moneyNum=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
			moneyNum->setAnchorPoint(ccp(0.5,0.5));
			moneyNum->setPosition(-20,55);
			animationsTest->addChild(moneyNum,10,2081);
		}
	}
  
   	CCClippingNode *clipper= CCClippingNode::create();
    clipper->setTag(500);
    clipper->setContentSize(CCSizeMake(350, 480) );
    clipper->setAnchorPoint( ccp(0, 0.5) );
    clipper->setPosition( ccp(400, this->getContentSize().height/ 2) );
    //clipper->runAction(CCRepeatForever::create(CCRotateBy::create(1, 45)));
   


	CCDrawNode *stencil = CCDrawNode::create();
    CCPoint rectangle[4];
    rectangle[0]= ccp(0, 0);
    rectangle[1]= ccp(clipper->getContentSize().width,0);
    rectangle[2]= ccp(clipper->getContentSize().width,clipper->getContentSize().height);
    rectangle[3]= ccp(0, clipper->getContentSize().height);
    //绘制一个矩形
    ccColor4F white = {1, 1, 1, 1};
    stencil->drawPolygon(rectangle,4, white, 1, white);
	//为设置裁减节点类设置“裁减模板”
    clipper->setStencil(stencil);

	this->addChild(clipper,0,500);

    //设置裁减节点类所放的内容
	shopLayer->setAnchorPoint(ccp(0,0));
	shopLayer->setPosition(ccp(0,clipper->getContentSize().height/2+15));
	clipper->addChild(shopLayer,0,1);

	index = 1;
	SeqLength = 6;

	//----------------------------外发光--------------------------------------
	CCNodeLoaderLibrary *shop_backgroundlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	cocos2d::extension::CCBReader* shop_backgroundBreader= new cocos2d::extension::CCBReader(shop_backgroundlibrary);
	shop_backgroundBreader->autorelease();
	CCNode  *shop_backgroundNode  = shop_backgroundBreader->readNodeGraphFromFile("shop/shopbox_light.ccbi",this);
	shop_backgroundNode->setAnchorPoint(ccp(0,0.5));
	shop_backgroundNode->setPosition(ccp(115,shopLayer->getContentSize().height/2-240));

	if(shop_backgroundNode  != NULL) {
		shopLayer->addChild(shop_backgroundNode,12,2018);
	}
	shop_backgroundNode->setVisible(false);
}

void ShopCocosBuilderLayer::showGradeMoney(int index,int grade){
  if(grade<3){
	CCLabelAtlas* atlas = (CCLabelAtlas*)(((this->getChildByTag(500))->getChildByTag(1))->getChildByTag(index>5?(index-6):index))->getChildByTag(2081);
	char lable[30];
	sprintf(lable,"%d",price[index][grade]);
	atlas->setString(lable);
  }else{
	  CCLabelAtlas* atlas = (CCLabelAtlas*)(((this->getChildByTag(500))->getChildByTag(1))->getChildByTag(index>5?(index-6):index))->getChildByTag(2081);
	  atlas->setVisible(false);
  }
}

void ShopCocosBuilderLayer::ItemShow(){
	CCLayer  *shopLayer = CCLayer::create();
	char lable[30];

	if(this->getChildByTag(500)){
		this->removeChildByTag(500,-1);
	}

	for(int n=0;n<8;n++){
		CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		ccnodeloaderlibrary->registerCCNodeLoader("shopbox", BoxCocosBuilderLayerLoader::loader());
		cocos2d::extension::CCBReader *ccbreader = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
		ccbreader->autorelease();
		sprintf(lable,"shop/sshopbox_item%d.ccbi",n+1);
		CCLog("lable:%s",lable);
		//读取一个ccbi的文件,生成一个ccnode实例
		CCNode  *animationsTest  = ccbreader->readNodeGraphFromFile(lable,this);//

		((BoxCocosBuilderLayer*)animationsTest)->setAnimationManager(ccbreader->getAnimationManager());
		((BoxCocosBuilderLayer*)animationsTest)->setKingANDId(2,n+13);
		//设置坐标 
		//animationsTest->setPosition(ccp(520,255)); 
		animationsTest->setAnchorPoint(ccp(0,0.5));
		animationsTest->setPosition(ccp(115+n*175,shopLayer->getContentSize().height/2)); 
		if(animationsTest  != NULL){
			//this->addChild(animationsTest,1,0);
			shopLayer->addChild(animationsTest,1,n);
		}

		//获取所有的动作序列 
		CCArray* allSeq = ccbreader->getAnimationManager()->getSequences(); 
		//CCLog("ItemShow============================") ;
		for (int i=0; i<allSeq->count(); i++)
		{     
			//获取到每一个Seq 
			CCBSequence* everySeq = (CCBSequence*)allSeq->objectAtIndex(i);
			//获取每个序列动作周期、名字以及id 
			// everySeq->getDuration(); 
			//CCLog("ItemShow===========%s",everySeq->getName()) ; 
			//CCLog("ItemShow===========%d",everySeq->getSequenceId()) ;

		} 

		if(n==0){//地雷
			UserRecord *ur = new UserRecord("tank.db","tank");
			int bombNum = ur->readIntFromSQLite("landmine");//地雷数量
			int landmineIndex = ur->readIntFromSQLite("landmineIndex");//地雷等级

			ur->closeDB();
			free(ur);

			char lable[30];
			sprintf(lable,"level%d",landmineIndex);
			ccbreader->getAnimationManager()->runAnimationsForSequenceNamed(lable);

			sprintf(lable,"%d",bombNum);
			CCLabelAtlas* Num=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
			Num->setAnchorPoint(ccp(0.5,0.5));
			Num->setPosition(ccp(28,90));
			animationsTest->addChild(Num,1,1);

		  if(landmineIndex<3){
			XMLParser *priceXmlParser = XMLParser::parseWithFile("shop/landmine_price.xml");//地雷升级价格
			sprintf(lable,"landmineIndex%d",landmineIndex);
			int price = (priceXmlParser->getString(lable))->intValue();

			sprintf(lable,"%d",price);
			CCLabelAtlas* moneyNum=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
			moneyNum->setAnchorPoint(ccp(0.5,0.5));
			moneyNum->setPosition(-20,58);
			animationsTest->addChild(moneyNum,10,2081);
			}
		}else if(n==1){

			UserRecord *ur = new UserRecord("tank.db","tank");
			int bombNum = ur->readIntFromSQLite("bomb");//炸弹数量
			int bombIndex = ur->readIntFromSQLite("bombIndex");//炸弹等级

			ur->closeDB();
			free(ur);

			char lable[30];
			//sprintf(lable,"level%d",bombIndex);
			ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("pre-buy");


			sprintf(lable,"%d",bombNum);
			CCLabelAtlas* Num=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
			Num->setAnchorPoint(ccp(0.5,0.5));
			Num->setPosition(ccp(28,90));
			animationsTest->addChild(Num,1,1);

			XMLParser *priceXmlParser = XMLParser::parseWithFile("shop/bomb_price.xml");//炸弹价格
			int price = (priceXmlParser->getString("bomb"))->intValue();

			sprintf(lable,"%d",price);
			CCLabelAtlas* moneyNum=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
			moneyNum->setAnchorPoint(ccp(0.5,0.5));
			moneyNum->setPosition(-20,58);
			animationsTest->addChild(moneyNum,10,2081);

		}
		/*else if(n==2){//生命增加值

			UserRecord *ur = new UserRecord("tank.db","tank");
			int addLifeIndex = ur->readIntFromSQLite("addLifeIndex");//

			ur->closeDB();
			free(ur);

			if(addLifeIndex==0){
				ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("pre-buy");
			}else{
			  char lable[30];
			  sprintf(lable,"level%d",addLifeIndex);
			  ccbreader->getAnimationManager()->runAnimationsForSequenceNamed(lable);
			}

			if(addLifeIndex<3){
				XMLParser *priceXmlParser = XMLParser::parseWithFile("shop/addLifeIndex_price.xml");//地雷升级价格
				sprintf(lable,"addLifeIndex%d",addLifeIndex);
				int price = (priceXmlParser->getString(lable))->intValue();

				sprintf(lable,"%d",price);
				CCLabelAtlas* moneyNum=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
				moneyNum->setAnchorPoint(ccp(0.5,0.5));
				moneyNum->setPosition(-20,58);
				animationsTest->addChild(moneyNum,10,2081);
			}
	   }*/
		else if (n==2){//攻击力

		   UserRecord *ur = new UserRecord("tank.db","tank");
		   int addAttackIndex = ur->readIntFromSQLite("addAttackIndex");//

		   ur->closeDB();
		   free(ur);

		   if(addAttackIndex==0){
			   ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("pre-buy");
		   }else{
			   char lable[30];
			   sprintf(lable,"level%d",addAttackIndex);
			   ccbreader->getAnimationManager()->runAnimationsForSequenceNamed(lable);
		   }

		   if(addAttackIndex<3){
			   XMLParser *priceXmlParser = XMLParser::parseWithFile("shop/addAttackIndex_price.xml");//地雷升级价格
			   sprintf(lable,"addAttackIndex%d",addAttackIndex);
			   int price = (priceXmlParser->getString(lable))->intValue();

			   sprintf(lable,"%d",price);
			   CCLabelAtlas* moneyNum=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
			   moneyNum->setAnchorPoint(ccp(0.5,0.5));
			   moneyNum->setPosition(-20,58);
			   animationsTest->addChild(moneyNum,10,2081);
		   }
	   }else if(n==3){
		   UserRecord *ur = new UserRecord("tank.db","tank");
		   int addDefenseIndex = ur->readIntFromSQLite("addDefenseIndex");//

		   ur->closeDB();
		   free(ur);

		   if(addDefenseIndex==0){
			   ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("pre-buy");
		   }else{
			   char lable[30];
			   sprintf(lable,"level%d",addDefenseIndex);
			   ccbreader->getAnimationManager()->runAnimationsForSequenceNamed(lable);
		   }

		   if(addDefenseIndex<3){
			   XMLParser *priceXmlParser = XMLParser::parseWithFile("shop/addDefenseIndex_price.xml");//地雷升级价格
			   sprintf(lable,"addDefenseIndex%d",addDefenseIndex);
			   int price = (priceXmlParser->getString(lable))->intValue();

			   sprintf(lable,"%d",price);
			   CCLabelAtlas* moneyNum=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
			   moneyNum->setAnchorPoint(ccp(0.5,0.5));
			   moneyNum->setPosition(-20,58);
			   animationsTest->addChild(moneyNum,10,2081);
		   }
	   }else if(n==4){//暴击
		   UserRecord *ur = new UserRecord("tank.db","tank");
		   int addCritIndex = ur->readIntFromSQLite("addCritIndex");//

		   ur->closeDB();
		   free(ur);

		   if(addCritIndex==0){
			   ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("pre-buy");
		   }else{
			   char lable[30];
			   sprintf(lable,"level%d",addCritIndex);
			   ccbreader->getAnimationManager()->runAnimationsForSequenceNamed(lable);
		   }

		   if(addCritIndex<3){
			   XMLParser *priceXmlParser = XMLParser::parseWithFile("shop/addCritIndex_price.xml");//地雷升级价格
			   sprintf(lable,"addCritIndex%d",addCritIndex);
			   int price = (priceXmlParser->getString(lable))->intValue();

			   sprintf(lable,"%d",price);
			   CCLabelAtlas* moneyNum=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
			   moneyNum->setAnchorPoint(ccp(0.5,0.5));
			   moneyNum->setPosition(-20,58);
			   animationsTest->addChild(moneyNum,10,2081);
		   }

	   }else if (n==5){//动力强化
		   UserRecord *ur = new UserRecord("tank.db","tank");
		   int addMoveIndex = ur->readIntFromSQLite("addMoveIndex");//

		   ur->closeDB();
		   free(ur);

		   if(addMoveIndex==0){
			   ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("pre-buy");
		   }else{
			   char lable[30];
			   sprintf(lable,"level%d",addMoveIndex);
			   ccbreader->getAnimationManager()->runAnimationsForSequenceNamed(lable);
		   }

		   if(addMoveIndex<3){
			   XMLParser *priceXmlParser = XMLParser::parseWithFile("shop/addMoveIndex_price.xml");//地雷升级价格
			   sprintf(lable,"addMoveIndex%d",addMoveIndex);
			   int price = (priceXmlParser->getString(lable))->intValue();

			   sprintf(lable,"%d",price);
			   CCLabelAtlas* moneyNum=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
			   moneyNum->setAnchorPoint(ccp(0.5,0.5));
			   moneyNum->setPosition(-20,58);
			   animationsTest->addChild(moneyNum,10,2081);
		   }
	   }/*else if(n==7){
		    UserRecord *gradeUR = new UserRecord("tank.db","tank");
			int colorTank = gradeUR->readIntFromSQLite("colorTank");

			gradeUR->closeDB();
			free(gradeUR);

            UserRecord *ur = new UserRecord("tank.db","shop");//商店物品等级

			if(ur->readBoolFromSQLite("greenTank")){
				if(colorTank==0){
					ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("level1_e");//装备
				}else{
					ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("level1");//装备
				}
				
			}else{
			    ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("pre-buy");
			}

			ur->closeDB();
			free(ur);


	   }else if(n==8){
		   UserRecord *gradeUR = new UserRecord("tank.db","tank");
		   int colorTank = gradeUR->readIntFromSQLite("colorTank");
		   CCLog("===================colorTank:%d",colorTank);
		   gradeUR->closeDB();
		   free(gradeUR);

		   UserRecord *ur = new UserRecord("tank.db","shop");//商店物品等级

		   if(ur->readBoolFromSQLite("blueTank")){
			   if(colorTank==1){
				   ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("level1_e");//装备
			   }else{
				   ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("level1");//装备
			   }

		   }else{
			   ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("pre-buy");

			   XMLParser *priceXmlParser = XMLParser::parseWithFile("shop/colorTank_price.xml");//蓝色价格
			 //  sprintf(lable,"addMoveIndex%d",addMoveIndex);
			   int price = (priceXmlParser->getString("buleTank"))->intValue();

			   sprintf(lable,"%d",price);
			   CCLabelAtlas* moneyNum=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
			   moneyNum->setAnchorPoint(ccp(0.5,0.5));
			   moneyNum->setPosition(-20,58);
			   animationsTest->addChild(moneyNum,10,2081);
		   }

		   ur->closeDB();
		   free(ur);
	   }else if(n==9){
		   UserRecord *gradeUR = new UserRecord("tank.db","tank");
		   int colorTank = gradeUR->readIntFromSQLite("colorTank");

		   gradeUR->closeDB();
		   free(gradeUR);

		   UserRecord *ur = new UserRecord("tank.db","shop");//商店物品等级

		   if(ur->readBoolFromSQLite("redTank")){
			   if(colorTank==2){
				   ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("level1_e");//装备
			   }else{
				   ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("level1");//装备
			   }

		   }else{
			   ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("pre-buy");
			   XMLParser *priceXmlParser = XMLParser::parseWithFile("shop/colorTank_price.xml");//红色价格
			   //  sprintf(lable,"addMoveIndex%d",addMoveIndex);
			   int price = (priceXmlParser->getString("redTank"))->intValue();

			   sprintf(lable,"%d",price);
			   CCLabelAtlas* moneyNum=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
			   moneyNum->setAnchorPoint(ccp(0.5,0.5));
			   moneyNum->setPosition(-20,58);
			   animationsTest->addChild(moneyNum,10,2081);
		   }

		   ur->closeDB();
		   free(ur);
	   }*/
	   else if(n==6){
		    UserRecord *ur = new UserRecord("tank.db","shop");//商店物品等级

			if(ur->readIntFromSQLite("doubleAttack")>0){

				 ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("level1");

				 sprintf(lable,"%d",ur->readIntFromSQLite("doubleAttack"));
				 CCLabelAtlas* moneyNum=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
				 moneyNum->setAnchorPoint(ccp(0.5,0.5));
				 moneyNum->setPosition(20,90);
				 animationsTest->addChild(moneyNum,10,2081);

			}else{
				 ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("pre-buy");

				 XMLParser *priceXmlParser = XMLParser::parseWithFile("shop/double_price.xml");//双倍
				 //  sprintf(lable,"addMoveIndex%d",addMoveIndex);
				 int price = (priceXmlParser->getString("doubleAttack"))->intValue();

				 sprintf(lable,"%d",price);
				 CCLabelAtlas* moneyNum=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
				 moneyNum->setAnchorPoint(ccp(0.5,0.5));
				 moneyNum->setPosition(-20,58);
				 animationsTest->addChild(moneyNum,10,2081);
			}
			ur->closeDB();
			free(ur);
	   }else if(n==7){
		   UserRecord *ur = new UserRecord("tank.db","shop");//商店物品等级

		   if(ur->readIntFromSQLite("doubleDefense")>0){

			   ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("level1");

			   sprintf(lable,"%d",ur->readIntFromSQLite("doubleDefense"));
			   CCLabelAtlas* moneyNum=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
			   moneyNum->setAnchorPoint(ccp(0.5,0.5));
			   moneyNum->setPosition(20,90);
			   animationsTest->addChild(moneyNum,10,2081);
		   }else{
			   ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("pre-buy");

			   XMLParser *priceXmlParser = XMLParser::parseWithFile("shop/double_price.xml");//双倍
			   //  sprintf(lable,"addMoveIndex%d",addMoveIndex);
			   int price = (priceXmlParser->getString("doubleDefense"))->intValue();

			   sprintf(lable,"%d",price);
			   CCLabelAtlas* moneyNum=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
			   moneyNum->setAnchorPoint(ccp(0.5,0.5));
			   moneyNum->setPosition(-20,58);
			   animationsTest->addChild(moneyNum,10,2081);
		   }
		   ur->closeDB();
		   free(ur);
	   }
	  /* else if(n==9){
		   UserRecord *ur = new UserRecord("tank.db","shop");//商店物品等级

		   if(ur->readBoolFromSQLite("unlockTank")){
			   ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("level1");
		   }else{
			   ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("pre-buy");

			   XMLParser *priceXmlParser = XMLParser::parseWithFile("shop/double_price.xml");//双倍
			   //  sprintf(lable,"addMoveIndex%d",addMoveIndex);
			   int price = (priceXmlParser->getString("lockTank"))->intValue();

			   sprintf(lable,"%d",price);
			   CCLabelAtlas* moneyNum=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
			   moneyNum->setAnchorPoint(ccp(0.5,0.5));
			   moneyNum->setPosition(-20,58);
			   animationsTest->addChild(moneyNum,10,2081);
		   }
		   ur->closeDB();
		   free(ur);
	   }*/

	}

	CCClippingNode *clipper= CCClippingNode::create();
	clipper->setTag(500);
	clipper->setContentSize(CCSizeMake(350, 480) );
	clipper->setAnchorPoint( ccp(0, 0.5) );
	clipper->setPosition( ccp(400, this->getContentSize().height/ 2) );
	//clipper->runAction(CCRepeatForever::create(CCRotateBy::create(1, 45)));

	CCDrawNode *stencil = CCDrawNode::create();
	CCPoint rectangle[4];
	rectangle[0]= ccp(0, 0);
	rectangle[1]= ccp(clipper->getContentSize().width,0);
	rectangle[2]= ccp(clipper->getContentSize().width,clipper->getContentSize().height);
	rectangle[3]= ccp(0, clipper->getContentSize().height);
	//绘制一个矩形
	ccColor4F white = {1, 1, 1, 1};
	stencil->drawPolygon(rectangle,4, white, 1, white);
	//为设置裁减节点类设置“裁减模板”
	clipper->setStencil(stencil);

	this->addChild(clipper,0,500);

	//设置裁减节点类所放的内容
	shopLayer->setAnchorPoint(ccp(0,0));
	shopLayer->setPosition(ccp(0,clipper->getContentSize().height/2+15));
	clipper->addChild(shopLayer,0,1);

	index = 1;
	SeqLength = 8;

	CCNodeLoaderLibrary *shop_backgroundlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	cocos2d::extension::CCBReader* shop_backgroundBreader= new cocos2d::extension::CCBReader(shop_backgroundlibrary);
	shop_backgroundBreader->autorelease();
	CCNode  *shop_backgroundNode  = shop_backgroundBreader->readNodeGraphFromFile("shop/shopbox_light.ccbi",this);
	shop_backgroundNode->setAnchorPoint(ccp(0,0.5));
	shop_backgroundNode->setPosition(ccp(115,shopLayer->getContentSize().height/2-240));

	if(shop_backgroundNode  != NULL) {
		shopLayer->addChild(shop_backgroundNode,12,2018);
	}
	shop_backgroundNode->setVisible(false);
}

void ShopCocosBuilderLayer::setChangeAnimation(int dex,int g){
	CCLog("--setChangeAnimation--dex:%d",dex);
	CCLog("--setChangeAnimation--  g:%d",g);

	cocos2d::extension::CCBAnimationManager *  BAmanager=((BoxCocosBuilderLayer*)((this->getChildByTag(500))->getChildByTag(1))->getChildByTag(dex-1))->getAnimationManager();

	char lable[30];

	sprintf(lable,"level%d",g);
	CCLog("setChangeAnimation:%s",lable);
	BAmanager->runAnimationsForSequenceNamed(lable);
}

 void ShopCocosBuilderLayer::setShowPre_buy(int dex){
	 CCLog("--setChangeAnimation--dex:%d",dex);

	 cocos2d::extension::CCBAnimationManager *  BAmanager=((BoxCocosBuilderLayer*)((this->getChildByTag(500))->getChildByTag(1))->getChildByTag(dex-1))->getAnimationManager();
	 BAmanager->runAnimationsForSequenceNamed("pre-buy");
 }