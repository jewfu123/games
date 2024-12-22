#include "shopTeach_2.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace CocosDenshion;
shopTeach_2::shopTeach_2(void):
	mAnimationManager(NULL),
		star_box(NULL),
		gold_box(NULL)
{
}


shopTeach_2::~shopTeach_2(void)
{
	CC_SAFE_RELEASE_NULL(mAnimationManager);
}
void shopTeach_2::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader) {
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

	//金钱
	sprintf(lable,"%d",gold);
	CCLabelAtlas* moneyNum=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
	moneyNum->setAnchorPoint(ccp(0.5,0.5));
	moneyNum->setPosition(gold_box->getContentSize().width/2+10,gold_box->getContentSize().height/2);
	gold_box->addChild(moneyNum,1,2);


	tanker *tank =tanker::spriteCreate(1,1,1,1);
	tank->setTankerAction();
	tank->setPosition(ccp(160,330));
	tank->setState(tanker::moveRight);
	tank->upDataNewBirth(0.1);
	//this->addChild(tank,1,100);
	this->addChild(tank,1,100);
	//this->addChild(tank,1,100);

	 seqIndex = 0;
	 this->setTouchEnabled(true); 

	 SimpleAudioEngine::sharedEngine()->preloadEffect("sound/15.ogg");//购买升级成功
	 SimpleAudioEngine::sharedEngine()->preloadEffect("sound/19.ogg");//装备成功
	 SimpleAudioEngine::sharedEngine()->preloadEffect("sound/10.ogg");//普通按钮
	 SimpleAudioEngine::sharedEngine()->preloadEffect("sound/21.ogg");//升级成功

}
SEL_MenuHandler shopTeach_2::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName) {
	CCLog("pSelectorName:%s",pSelectorName);

	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "goumai1", shopTeach_2::goumai1);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "zhuangbei1", shopTeach_2::zhuangbei1);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_body", shopTeach_2::button_body);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "goumai2", shopTeach_2::goumai2);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "zhuangbei2", shopTeach_2::zhuangbei2);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "back1", shopTeach_2::back1);
	return NULL;
}
SEL_CCControlHandler shopTeach_2::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName) {

	return NULL;
}
bool shopTeach_2::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode) {
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "life_box", CCSprite*, this->star_box);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "gold_box", CCSprite*, this->gold_box);
	return false;
}
bool shopTeach_2::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue){
	bool bRet = false;

	return bRet;
}

void shopTeach_2::setAnimationManager(cocos2d::extension::CCBAnimationManager *pAnimationManager){
	CC_SAFE_RELEASE_NULL(mAnimationManager);
	mAnimationManager = pAnimationManager;
	CC_SAFE_RETAIN(mAnimationManager);
}

 void shopTeach_2::registerWithTouchDispatcher(void){
	  CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
 }
 bool shopTeach_2::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
	 CCLog("------------------------------shopTeach_2");
	 if(seqIndex==0){
		 mAnimationManager->runAnimationsForSequenceNamed("3rd");
		 seqIndex=1;

		 /*UserRecord *gradeUR = new UserRecord("tank.db","tank");
		 int gold = gradeUR->readIntFromSQLite("gold");
		 gradeUR->saveInt("gold",gold+1000);
		 gradeUR->closeDB();
		 free(gradeUR);*/

		 addGold(1000);
	 }


	 return true;
 }
 void shopTeach_2::goumai1(CCObject * pSender){
	 if(sound){
		 SimpleAudioEngine::sharedEngine()->playEffect("sound/15.ogg",false);
	 }

	 CCSize size = CCDirector::sharedDirector()->getWinSize();
	 CCNodeLoaderLibrary *burSuccessloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	 cocos2d::extension::CCBReader *buySuccessBreader= new cocos2d::extension::CCBReader(burSuccessloaderlibrary); 
	 CCNode  *buySccessNode  = buySuccessBreader->readNodeGraphFromFile("shop/effect_level1.ccbi",this);

	 buySccessNode->setPosition(ccp(size.width/2+295,size.height/2+17));

	 if(buySccessNode  != NULL) {
		 this->addChild(buySccessNode,12,2013);
	 }
	 this->scheduleOnce(schedule_selector(shopTeach_2::removeChildEffect_level),2.0f );

	  mAnimationManager->runAnimationsForSequenceNamed("5th");
	  addGold(-300);
 }
 void shopTeach_2::removeChildEffect_level(float time){
	 if(this->getChildByTag(2013)){
		 this->removeChildByTag(2013,-1);
	 }
 }
 void shopTeach_2::zhuangbei1(CCObject * pSender){
	 if(sound){
		 SimpleAudioEngine::sharedEngine()->playEffect("sound/19.ogg",false);
	 }

	 this->removeChildByTag(100,-1);


	 tanker *tank =tanker::spriteCreate(1,2,1,1);
	 tank->setTankerAction();
	 tank->setPosition(ccp(160,330));
	 tank->setState(tanker::moveRight);
	 tank->upDataNewBirth(0.1);
	 //this->addChild(tank,1,100);
	 this->addChild(tank,1,100);
	 //this->addChild(tank,1,100);

	 mAnimationManager->runAnimationsForSequenceNamed("7th");
 }

 void shopTeach_2::button_body(CCObject * pSender){
	 if(sound){
		 SimpleAudioEngine::sharedEngine()->playEffect("sound/10.ogg",false);
	 }
	 mAnimationManager->runAnimationsForSequenceNamed("9th");
 }
 void shopTeach_2::goumai2(CCObject * pSender){
	 if(sound){
		 SimpleAudioEngine::sharedEngine()->playEffect("sound/15.ogg",false);
	 }
	 mAnimationManager->runAnimationsForSequenceNamed("11th");
	 addGold(-240);
 }
 void shopTeach_2::zhuangbei2(CCObject * pSender){
	 if(sound){
		 SimpleAudioEngine::sharedEngine()->playEffect("sound/19.ogg",false);
	 }
	 CCSize size = CCDirector::sharedDirector()->getWinSize();
	 CCNodeLoaderLibrary *burSuccessloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	 cocos2d::extension::CCBReader *buySuccessBreader= new cocos2d::extension::CCBReader(burSuccessloaderlibrary); 
	 CCNode  *buySccessNode  = buySuccessBreader->readNodeGraphFromFile("shop/effect_level1.ccbi",this);

	 buySccessNode->setPosition(ccp(size.width/2+295,size.height/2+17));

	 if(buySccessNode  != NULL) {
		 this->addChild(buySccessNode,12,2013);
	 }
	 this->scheduleOnce(schedule_selector(shopTeach_2::removeChildEffect_level),2.0f);


	 tanker *tank =tanker::spriteCreate(2,2,1,1);
	 tank->setTankerAction();
	 tank->setPosition(ccp(160,330));
	 tank->setState(tanker::moveRight);
	 tank->upDataNewBirth(0.1);
	 //this->addChild(tank,1,100);
	 this->addChild(tank,1,100);

	 mAnimationManager->runAnimationsForSequenceNamed("13th");
 }
 void shopTeach_2::back1(CCObject * pSender){
	// mAnimationManager->runAnimationsForSequenceNamed("13th");
	 if(sound){
		 SimpleAudioEngine::sharedEngine()->playEffect("sound/10.ogg",false);
	 }
	   SaveBooleanToXML("teachShop",true);
	   UserRecord *ur = new UserRecord("tank.db","tank");
	   ur->saveInt("turretId",2);
	   ur->saveInt("chassisId",2);//底盘

	   ur->closeDB();
	   free(ur);
	   char lable[30];
	   UserRecord *urShop = new UserRecord("tank.db","shop");
	   sprintf(lable,"shopbox%d",2);
	   urShop->saveInt(lable,1);
	   sprintf(lable,"shopbox%d",8);
	   urShop->saveInt(lable,1);
	   urShop->closeDB();
	   free(urShop);

	   mapIndex = 2;
	   CCScene *pScene = GameScene::scene();
	   CCDirector::sharedDirector()->replaceScene(pScene);
 }

 void shopTeach_2::addGold(int goldNum){
	 UserRecord *gradeUR = new UserRecord("tank.db","tank");
	 int gold = gradeUR->readIntFromSQLite("gold");

	 char lable[30];
	 sprintf(lable,"%d",goldNum+gold);
	 ((CCLabelAtlas*)(gold_box->getChildByTag(2)))->setString(lable);


	 gradeUR->saveInt("gold",goldNum+gold);
	 gradeUR->closeDB();
	 free(gradeUR);

 }