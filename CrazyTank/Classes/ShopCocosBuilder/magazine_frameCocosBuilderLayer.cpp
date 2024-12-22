#include "../xml/XMLParser.h"
#include "../UserRecord/UserRecord.h"
#include "Tanker.h"
#include "SimpleAudioEngine.h"
#include "MenuLayer.h"
#include "magazine_frameCocosBuilderLayer.h"
#include "GameScene.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
#include <jni.h>
#include "platform/android/jni/JniHelper.h" 
#include <android/log.h> 
#endif 

USING_NS_CC;
USING_NS_CC_EXT;
using namespace CocosDenshion;
magazine_frameCocosBuilderLayer::magazine_frameCocosBuilderLayer(void):
star_box(NULL),
gold_box(NULL),
mine_box(NULL),
nuke_box(NULL),
button_page1(NULL),
button_page2(NULL),
button_page3(NULL)
{

}


magazine_frameCocosBuilderLayer::~magazine_frameCocosBuilderLayer(void)
{

}
void magazine_frameCocosBuilderLayer::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader){
	UserRecord *gradeUR = new UserRecord("tank.db","tank");
	int gold = gradeUR->readIntFromSQLite("gold");
	int tankCont = gradeUR->readIntFromSQLite("tankCont");
	int landmine = gradeUR->readIntFromSQLite("landmine");//地雷
	int bomb = gradeUR->readIntFromSQLite("bomb");//炸弹
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

	this->schedule(schedule_selector(magazine_frameCocosBuilderLayer::showTime),1.0f);
	//金钱
	sprintf(lable,"%d",gold);
	CCLabelAtlas* moneyNum=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
	moneyNum->setAnchorPoint(ccp(0.5,0.5));
	moneyNum->setPosition(gold_box->getContentSize().width/2+10,gold_box->getContentSize().height/2);
	gold_box->addChild(moneyNum,1,2);

	//地雷
	sprintf(lable,"%d",landmine);
	CCLabelAtlas* landmineNum=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
	landmineNum->setAnchorPoint(ccp(0.5,0.5));
	landmineNum->setPosition(mine_box->getContentSize().width/2+10,mine_box->getContentSize().height/2);
	mine_box->addChild(landmineNum,1,2);

	//炸弹
	sprintf(lable,"%d",bomb);
	CCLabelAtlas* bombNum=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
	bombNum->setAnchorPoint(ccp(0.5,0.5));
	bombNum->setPosition(nuke_box->getContentSize().width/2+10,nuke_box->getContentSize().height/2);
	nuke_box->addChild(bombNum,1,2);

	pageIndex = 2;
	button_page3->selected();

	CCSize size = CCDirector::sharedDirector()->getWinSize();

	CCMenuItemImage *leftItem = CCMenuItemImage::create(
		"ccbResources/repeat/20140209/zuojiantou.png",
		"ccbResources/repeat/20140209/zuojiantou2.png",
		this,
		menu_selector(magazine_frameCocosBuilderLayer::onbutton_LiftClicked));
	leftItem->setPosition(ccp(-265,0));
	
	CCPoint pointL=ccp(leftItem->getPosition().x-3, leftItem->getPosition().y);
	CCPoint pointR=ccp(leftItem->getPosition().x+3, leftItem->getPosition().y);
	CCMoveTo* moveLeft= CCMoveTo::create(0.09f, pointL);
	CCMoveTo* moveRight= CCMoveTo::create(0.09f, pointR);

	CCFiniteTimeAction* action= CCSequence::create(moveLeft, moveRight,NULL);
	CCActionInterval* actionShake=CCRepeatForever::create((CCActionInterval*)action);

	leftItem->runAction(actionShake);

	CCMenuItemImage *rightItem = CCMenuItemImage::create(
		"ccbResources/repeat/20140209/youjiantou.png",
		"ccbResources/repeat/20140209/youjiantou2.png",
		this,
		menu_selector(magazine_frameCocosBuilderLayer::onbutton_RightClicked));
	rightItem->setPosition(ccp(265,0));

	CCPoint rpointL=ccp(rightItem->getPosition().x-3, rightItem->getPosition().y);
	CCPoint rpointR=ccp(rightItem->getPosition().x+3, rightItem->getPosition().y);
	CCMoveTo* rmoveLeft= CCMoveTo::create(0.09f, rpointL);
	CCMoveTo* rmoveRight= CCMoveTo::create(0.09f, rpointR);

	CCFiniteTimeAction* raction= CCSequence::create(rmoveRight,rmoveLeft,NULL);
	CCActionInterval* ractionShake=CCRepeatForever::create((CCActionInterval*)raction);

	rightItem->runAction(ractionShake);

	CCMenu* pMenu = CCMenu::create(leftItem,rightItem,NULL);
	this->addChild(pMenu,1,1);


	//-----------------大关卡说明-------------------------------------------------------
	sprintf(lable,"menu/text_shengcun%d.png",(mapIndex/10));
	CCSprite *name = CCSprite::create(lable);
	name->setPosition(ccp(65,size.height-28));
	this->addChild(name,2,100);

	sprintf(lable,"%d%d",0,mapIndex%10);
	CCLabelAtlas* mapNum=CCLabelAtlas::create(lable, "menu/num_magazine.png", 24, 26, '0');
	mapNum->setAnchorPoint(ccp(0.5,0.5));
	mapNum->setPosition(150,size.height-28);
	this->addChild(mapNum,1,2);
	//-------------------------------购买成功--------------------------------
	CCNodeLoaderLibrary *burSuccessloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	buySuccessBreader= new cocos2d::extension::CCBReader(burSuccessloaderlibrary); 
	CCNode  *buySccessNode  = buySuccessBreader->readNodeGraphFromFile("shop/UI_box_goumaichenggong.ccbi",this);

	buySccessNode->setPosition(ccp(size.width/2,size.height/2));

	if(buySccessNode  != NULL) {
		this->addChild(buySccessNode,12,2013);
	}
	buySccessNode->setVisible(false);

	SimpleAudioEngine::sharedEngine()->preloadEffect("sound/15.ogg");//购买升级成功
}
void magazine_frameCocosBuilderLayer::playBottnEffect(){
	if(sound){
		SimpleAudioEngine::sharedEngine()->playEffect("sound/15.ogg",false);
	}
}
SEL_MenuHandler magazine_frameCocosBuilderLayer::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_back", magazine_frameCocosBuilderLayer::onBackClicked);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_page1:", magazine_frameCocosBuilderLayer::onbutton_page1Clicked);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_page2:", magazine_frameCocosBuilderLayer::onbutton_page2Clicked);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_page3:", magazine_frameCocosBuilderLayer::onbutton_page3Clicked);

	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_buy1", magazine_frameCocosBuilderLayer::onbutton_buy1Clicked);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_buy2", magazine_frameCocosBuilderLayer::onbutton_buy2Clicked);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_buy3", magazine_frameCocosBuilderLayer::onbutton_buy3Clicked);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_buy4", magazine_frameCocosBuilderLayer::onbutton_buy4Clicked);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_buy5", magazine_frameCocosBuilderLayer::onbutton_buy5Clicked);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_buy6", magazine_frameCocosBuilderLayer::onbutton_buy6Clicked);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_start", magazine_frameCocosBuilderLayer::onbutton_startClicked);

	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_lingqu1", magazine_frameCocosBuilderLayer::button_lingqu1Clicked);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_lingqu2", magazine_frameCocosBuilderLayer::button_lingqu2Clicked);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_lingqu3", magazine_frameCocosBuilderLayer::button_lingqu3Clicked);



	return NULL;
}
SEL_CCControlHandler magazine_frameCocosBuilderLayer::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){

	return NULL;
}

bool magazine_frameCocosBuilderLayer::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
	CCLog("onAssignCCBMemberVariable===%s",pMemberVariableName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "life_box", CCSprite*, this->star_box);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "gold_box", CCSprite*, this->gold_box);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mine_box", CCSprite*, this->mine_box);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "nuke_box", CCSprite*, this->nuke_box);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_page1", CCMenuItemImage*, this->button_page1);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_page2", CCMenuItemImage*, this->button_page2);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_page3", CCMenuItemImage*, this->button_page3);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_page1:", CCMenuItemImage*, this->button_lingqu1);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_page2:", CCMenuItemImage*, this->button_lingqu2);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_page3:", CCMenuItemImage*, this->button_lingqu3);

	return false;
}

bool magazine_frameCocosBuilderLayer::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue){
	bool bRet = false;

	return bRet;
}

extern "C"  {
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

}
void magazine_frameCocosBuilderLayer::onbutton_LiftClicked(CCObject * pSender){
	int index = (pageIndex-1+3)%3;
	
	if(index==0){
		onbutton_page1Clicked(this);
	}else if(index==1){
		onbutton_page2Clicked(this);
	}else if(index==2){
		onbutton_page3Clicked(this);
	}
}
void magazine_frameCocosBuilderLayer::onbutton_RightClicked(CCObject * pSender){
	int index = (pageIndex+1)%3;

	if(index==0){
		onbutton_page1Clicked(this);
	}else if(index==1){
		onbutton_page2Clicked(this);
	}else if(index==2){
		onbutton_page3Clicked(this);
	}
}

void magazine_frameCocosBuilderLayer::onbutton_page1Clicked(CCObject * pSender){
	if(pageIndex==1){
		CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
		CCBAnimationManager *manager = (CCBAnimationManager*)scene->getUserObject();
		manager->runAnimationsForSequenceNamed("page2_page1");
	}else if(pageIndex==2){
		CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
		CCBAnimationManager *manager = (CCBAnimationManager*)scene->getUserObject();
		manager->runAnimationsForSequenceNamed("page3_page1");
	}
	pageIndex = 0;
	button_page1->selected();
	button_page2->unselected();
	button_page3->unselected();
}
void magazine_frameCocosBuilderLayer::onbutton_page2Clicked(CCObject * pSender){
	if(pageIndex==0){
		CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
		CCBAnimationManager *manager = (CCBAnimationManager*)scene->getUserObject();
		manager->runAnimationsForSequenceNamed("page1_page2");
	}else if(pageIndex==2){
		CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
		CCBAnimationManager *manager = (CCBAnimationManager*)scene->getUserObject();
		manager->runAnimationsForSequenceNamed("page3_page2");

	}
	pageIndex = 1;
	button_page2->selected();
	button_page1->unselected();
	button_page3->unselected();
	
}
void magazine_frameCocosBuilderLayer::onbutton_page3Clicked(CCObject * pSender){

	if(pageIndex==0){
		CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
		CCBAnimationManager *manager = (CCBAnimationManager*)scene->getUserObject();
		manager->runAnimationsForSequenceNamed("page1_page3");
	}else if(pageIndex==1){
		CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
		CCBAnimationManager *manager = (CCBAnimationManager*)scene->getUserObject();
		manager->runAnimationsForSequenceNamed("page2_page3");

	}


	pageIndex = 2;
	button_page3->selected();
	button_page2->unselected();
	button_page1->unselected();

}

void magazine_frameCocosBuilderLayer::showTime(float  t){
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
void  magazine_frameCocosBuilderLayer::keyBackClicked(){
	onBackClicked(this);
}
void magazine_frameCocosBuilderLayer::onBackClicked(CCObject * pSender){
	CCScene *scene = CCScene::create();
	MenuLayer *node = MenuLayer::create();
	scene->addChild(node);
	
	if(gameSceneMagazine){
		CCDirector::sharedDirector()->popScene();
	}else{
		CCDirector::sharedDirector()->replaceScene(scene);
		gameSceneMagazine = true;
	}
	
}

void magazine_frameCocosBuilderLayer::button_lingqu1Clicked(CCObject * pSender){
	char lable[30];
	UserRecord *urs = new UserRecord("tank.db","shop");
	sprintf(lable,"shopbox%d",4);
	int shopbox4= urs->readIntFromSQLite(lable);
	sprintf(lable,"shopbox%d",10);
	int shopbox10 = urs->readIntFromSQLite(lable);
	urs->closeDB();
	free(urs);
	if(shopbox4==0||shopbox10==0){
		CCLog("button_lingqu1Clicked");
		playBottnEffect();
		// this->removeChildByTag(8510,-1);
      #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
		billingPointIndex = 9;//计费点
		SaveBooleanToXML("payResultBool",false);
		SaveBooleanToXML("payGameOver",false);
		//payResultBool = false;;
		//payGameOver = false;
		this->schedule(schedule_selector(magazine_frameCocosBuilderLayer::updateBillingPoint), 0.1f);

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
	}else{
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		cocos2d::extension::CCBReader *ccbreaderPay = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
		CCNode  *payNode  = ccbreaderPay->readNodeGraphFromFile("pay/UI_box_yijinggoumai.ccbi",this);
		ccbreaderPay->autorelease();
		payNode->setPosition(ccp(size.width/2,size.height/2));

		this->addChild(payNode,100,85);
		ccbreaderPay->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(magazine_frameCocosBuilderLayer::yijinggoumaiEnd)); 
	}

}

void magazine_frameCocosBuilderLayer::button_lingqu2Clicked(CCObject * pSender){
	char lable[30];
	UserRecord *urs = new UserRecord("tank.db","shop");
	sprintf(lable,"shopbox%d",5);
	int shopbox5= urs->readIntFromSQLite(lable);
	sprintf(lable,"shopbox%d",11);
	int shopbox11 = urs->readIntFromSQLite(lable);
	urs->closeDB();
	free(urs);
	if(shopbox5==0||shopbox11==0){
		CCLog("button_lingqu1Clicked");
		playBottnEffect();
		// this->removeChildByTag(8510,-1);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
		billingPointIndex = 14;//计费点
		SaveBooleanToXML("payResultBool",false);
		SaveBooleanToXML("payGameOver",false);
		//payResultBool = false;;
		//payGameOver = false;
		this->schedule(schedule_selector(magazine_frameCocosBuilderLayer::updateBillingPoint), 0.1f);

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
	}else{
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		cocos2d::extension::CCBReader *ccbreaderPay = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
		CCNode  *payNode  = ccbreaderPay->readNodeGraphFromFile("pay/UI_box_yijinggoumai.ccbi",this);
		ccbreaderPay->autorelease();
		payNode->setPosition(ccp(size.width/2,size.height/2));

		this->addChild(payNode,100,85);
		ccbreaderPay->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(magazine_frameCocosBuilderLayer::yijinggoumaiEnd)); 
	}
}
void magazine_frameCocosBuilderLayer::button_lingqu3Clicked(CCObject * pSender){
	char lable[30];
	UserRecord *urs = new UserRecord("tank.db","shop");
	sprintf(lable,"shopbox%d",6);
	int shopbox6= urs->readIntFromSQLite(lable);
	sprintf(lable,"shopbox%d",12);
	int shopbox12 = urs->readIntFromSQLite(lable);
	urs->closeDB();
	free(urs);
	if(shopbox6==0||shopbox12==0){
		CCLog("button_lingqu1Clicked");
		playBottnEffect();
		// this->removeChildByTag(8510,-1);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
		billingPointIndex = 15;//计费点
		SaveBooleanToXML("payResultBool",false);
		SaveBooleanToXML("payGameOver",false);
		//payResultBool = false;;
		//payGameOver = false;
		this->schedule(schedule_selector(magazine_frameCocosBuilderLayer::updateBillingPoint), 0.1f);

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
	}else{
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		cocos2d::extension::CCBReader *ccbreaderPay = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
		CCNode  *payNode  = ccbreaderPay->readNodeGraphFromFile("pay/UI_box_yijinggoumai.ccbi",this);
		ccbreaderPay->autorelease();
		payNode->setPosition(ccp(size.width/2,size.height/2));

		this->addChild(payNode,100,85);
		ccbreaderPay->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(magazine_frameCocosBuilderLayer::yijinggoumaiEnd)); 
	}

}

void magazine_frameCocosBuilderLayer::yijinggoumaiEnd(){
	this->removeChildByTag(85,-1);

}

void magazine_frameCocosBuilderLayer::onbutton_buy1Clicked(CCObject * pSender){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
	billingPointIndex = 12;//计费点
	SaveBooleanToXML("payResultBool",false);
	SaveBooleanToXML("payGameOver",false);
	//payResultBool = false;;
	//payGameOver = false;
	this->schedule(schedule_selector(magazine_frameCocosBuilderLayer::updateBillingPoint), 0.1f);

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
	UserRecord *ur = new UserRecord("tank.db","tank");
	int bomb = ur->readIntFromSQLite("bomb");
	bomb+=2;
	ur->saveInt("bomb",bomb);
	ur->closeDB();
	free(ur);

	char tm[50];
	sprintf(tm, "%d",bomb); 
	CCLabelAtlas* _landmineNum =(CCLabelAtlas*) (nuke_box->getChildByTag(2));
	_landmineNum->setString(tm);

	this->getChildByTag(2013)->setVisible(true);
	buySuccessBreader->getAnimationManager()->runAnimationsForSequenceNamed("1st");
	playBottnEffect();
#endif
}
void magazine_frameCocosBuilderLayer::onbutton_buy2Clicked(CCObject * pSender){

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
	billingPointIndex = 13;//计费点
	SaveBooleanToXML("payResultBool",false);
	SaveBooleanToXML("payGameOver",false);
	//payResultBool = false;;
	//payGameOver = false;
	this->schedule(schedule_selector(magazine_frameCocosBuilderLayer::updateBillingPoint), 0.1f);

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

	UserRecord *ur = new UserRecord("tank.db","tank");
	int landmine = ur->readIntFromSQLite("landmine");
	landmine+=4;
	ur->saveInt("landmine",landmine);
	ur->closeDB();
	free(ur);

	char tm[50];
	sprintf(tm, "%d",landmine); 
	CCLabelAtlas* _landmineNum =(CCLabelAtlas*) (mine_box->getChildByTag(2));
	_landmineNum->setString(tm);

	this->getChildByTag(2013)->setVisible(true);
	buySuccessBreader->getAnimationManager()->runAnimationsForSequenceNamed("1st");
	playBottnEffect();
#endif
}

void magazine_frameCocosBuilderLayer::onbutton_buy3Clicked(CCObject * pSender){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
	billingPointIndex = 11;//计费点
	SaveBooleanToXML("payResultBool",false);
	SaveBooleanToXML("payGameOver",false);
	//payResultBool = false;;
	//payGameOver = false;
	this->schedule(schedule_selector(magazine_frameCocosBuilderLayer::updateBillingPoint), 0.1f);

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
	this->getChildByTag(2013)->setVisible(true);
	buySuccessBreader->getAnimationManager()->runAnimationsForSequenceNamed("1st");
	playBottnEffect();
#endif
}
void magazine_frameCocosBuilderLayer::onbutton_buy4Clicked(CCObject * pSender){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
	billingPointIndex = 0;//计费点
	SaveBooleanToXML("payResultBool",false);
	SaveBooleanToXML("payGameOver",false);
	//payResultBool = false;;
	//payGameOver = false;
	this->schedule(schedule_selector(magazine_frameCocosBuilderLayer::updateBillingPoint), 0.1f);

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
}
void magazine_frameCocosBuilderLayer::onbutton_buy5Clicked(CCObject * pSender){
  #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
	billingPointIndex = 1;//计费点
	SaveBooleanToXML("payResultBool",false);
	SaveBooleanToXML("payGameOver",false);
	//payResultBool = false;;
	//payGameOver = false;
	this->schedule(schedule_selector(magazine_frameCocosBuilderLayer::updateBillingPoint), 0.1f);

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
}
void magazine_frameCocosBuilderLayer::onbutton_buy6Clicked(CCObject * pSender){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
	billingPointIndex = 2;//计费点
	SaveBooleanToXML("payResultBool",false);
	SaveBooleanToXML("payGameOver",false);
	//payResultBool = false;;
	//payGameOver = false;
	this->schedule(schedule_selector(magazine_frameCocosBuilderLayer::updateBillingPoint), 0.1f);

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
	addGold(4888);
#endif
}

void magazine_frameCocosBuilderLayer::updateBillingPoint(float time){
  #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
	switch (billingPointIndex)
	{
	   case 9:
		{
			if(LoadBooleanFromXML("payGameOver")){
				this->unschedule(schedule_selector(magazine_frameCocosBuilderLayer::updateBillingPoint));
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
	   case 14:
		   {
			   if(LoadBooleanFromXML("payGameOver")){
				   this->unschedule(schedule_selector(magazine_frameCocosBuilderLayer::updateBillingPoint));
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
				   this->unschedule(schedule_selector(magazine_frameCocosBuilderLayer::updateBillingPoint));
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
	  case 12:
		{
			if(LoadBooleanFromXML("payGameOver")){
				this->unschedule(schedule_selector(magazine_frameCocosBuilderLayer::updateBillingPoint));
				if(LoadBooleanFromXML("payResultBool")){//付费成功
					UserRecord *ur = new UserRecord("tank.db","tank");
					int bomb = ur->readIntFromSQLite("bomb");
					bomb+=2;
					ur->saveInt("bomb",bomb);
					ur->closeDB();
					free(ur);

					char tm[50];
					sprintf(tm, "%d",bomb); 
					CCLabelAtlas* _landmineNum =(CCLabelAtlas*) (nuke_box->getChildByTag(2));
					_landmineNum->setString(tm);

					this->getChildByTag(2013)->setVisible(true);
					buySuccessBreader->getAnimationManager()->runAnimationsForSequenceNamed("1st");
					playBottnEffect();
				}
			}
		}
		break;
	case 13:
		{
			if(LoadBooleanFromXML("payGameOver")){
				this->unschedule(schedule_selector(magazine_frameCocosBuilderLayer::updateBillingPoint));
				if(LoadBooleanFromXML("payResultBool")){//付费成功
					UserRecord *ur = new UserRecord("tank.db","tank");
					int landmine = ur->readIntFromSQLite("landmine");
					landmine+=4;
					ur->saveInt("landmine",landmine);
					ur->closeDB();
					free(ur);

					char tm[50];
					sprintf(tm, "%d",landmine); 
					CCLabelAtlas* _landmineNum =(CCLabelAtlas*) (mine_box->getChildByTag(2));
					_landmineNum->setString(tm);

					this->getChildByTag(2013)->setVisible(true);
					buySuccessBreader->getAnimationManager()->runAnimationsForSequenceNamed("1st");
					playBottnEffect();
				}
			}
			
		}
		break;
	case 0:
		{
			if(LoadBooleanFromXML("payGameOver")){
				this->unschedule(schedule_selector(magazine_frameCocosBuilderLayer::updateBillingPoint));
				if(LoadBooleanFromXML("payResultBool")){//付费成功
					addGold(400);
				}
			}
		}
		break;
	case 1:
		{
			if(LoadBooleanFromXML("payGameOver")){
				this->unschedule(schedule_selector(magazine_frameCocosBuilderLayer::updateBillingPoint));
				if(LoadBooleanFromXML("payResultBool")){//付费成功
					addGold(840);//840
				}
			}
		}
		break;
	case 2:
		{
			if(LoadBooleanFromXML("payGameOver")){
				this->unschedule(schedule_selector(magazine_frameCocosBuilderLayer::updateBillingPoint));
				if(LoadBooleanFromXML("payResultBool")){//付费成功
					addGold(3300);// 3300
				}
			}
		}
		break;
	case 11:
		{
			if(LoadBooleanFromXML("payGameOver")){
				this->unschedule(schedule_selector(magazine_frameCocosBuilderLayer::updateBillingPoint));
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
					this->getChildByTag(2013)->setVisible(true);
					buySuccessBreader->getAnimationManager()->runAnimationsForSequenceNamed("1st");
					playBottnEffect();
				}
			}
		}
		break;
	}
#endif
}

void magazine_frameCocosBuilderLayer::onbutton_startClicked(CCObject * pSender){
	    gameSceneMagazine = true;
		CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		cocos2d::extension::CCBReader *ccbreader = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
		CCNode  *loading  = ccbreader->readNodeGraphFromFile("scene_loading.ccbi",this);
		ccbreader->autorelease();
		loading->setAnchorPoint(ccp(0.5,0.5));
		loading->setPosition(ccp(0,0));

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
			CCLog("===========%d",everySeq->getSequenceId());
		} 
	    ccbreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(magazine_frameCocosBuilderLayer::end)); 
}

void magazine_frameCocosBuilderLayer::addGold(int goldNum){
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
	playBottnEffect();
}

void magazine_frameCocosBuilderLayer::end(){
	
	CCScene *pScene = GameScene::scene();
	CCDirector::sharedDirector()->replaceScene(pScene);
	if(mapIndex>=40){
		survival_time = millisecondNow()+30*60*1000;
		survivalModeTime.setMillis(30*60*1000);
		survivalModeTime.start();
	}
}