#include "Tanker.h"

using namespace cocos2d::extension; 
using namespace cocos2d;


tanker::tanker():godBool(false),
	             newBirthGod(false),
	             loseBloodNumDirection(false)
{}

tanker::~tanker(void)
{
	 CC_SAFE_RELEASE_NULL(ccbreader);
	 CC_SAFE_RELEASE_NULL(firebreader);
	 CC_SAFE_RELEASE_NULL(lvupbreader);
	 CC_SAFE_RELEASE_NULL(godbreader);
	 CC_SAFE_RELEASE_NULL( tankExplodeBreader);
	 CC_SAFE_RELEASE_NULL( cureBreader);
	 CC_SAFE_RELEASE_NULL(moveSpeedBreader);
}

void tanker::myInit(int chassis,int turret,int chassisGrade,int turretGrade)//const char *pszFileName
{
	this->chassisKind = chassis;
	this->turretKing = turret;
	this->chassisGrade = chassisGrade;
	this->turretGrade = turretGrade;

	char layerName[30];

    sprintf(layerName,"tanker/chassis_%d_%d.xml", chassis,chassisGrade);
	XMLParser *pXmlParser = XMLParser::parseWithFile(layerName);

	defense = ((pXmlParser->getString("defense"))->intValue()+addDefenseIndex*4);//*(doubleDefense?2:1);
    lifeNum = (pXmlParser->getString("lifeNum"))->intValue()+addLifeIndex*50;
	moveSpeed = ((pXmlParser->getString("moveSpeed"))->floatValue()*(1+addMoveIndex*5.0/100.0));

	sprintf(layerName,"tanker/turret_%d_%d.xml", chassis,chassisGrade);//turret_1_0.xml
	XMLParser *turretXMLParser = XMLParser::parseWithFile(layerName);
	weight = (turretXMLParser->getString("weight"))->intValue();//重量


	//CCLog("floatValue():%f  moveSpeed:%f",(pXmlParser->getString("moveSpeed"))->floatValue(),moveSpeed);
	//CCLog("floatValue():%d  lifeNum:%d", (pXmlParser->getString("lifeNum"))->intValue(),lifeNum);
	//CCLog("floatValue():%d  defense:%d", (pXmlParser->getString("defense"))->intValue(),defense);

	moveSpeed = moveSpeed- weight;

	CCLog("moveSpeed:%f",moveSpeed);

	MAXLife = lifeNum;
	recordMoveSpeed = moveSpeed;
	this->destroy = false;
	
}

tanker* tanker::spriteCreate(int chassis,int turret,int chassisGrade,int turretGrade){
	 tanker *pobSprite = new tanker();       
	 if (pobSprite && pobSprite->init() )//备注1  initWithFile   pobSprite->init()
	 {        
		 
		 pobSprite->myInit(chassis,turret,chassisGrade,turretGrade);        
		 pobSprite->autorelease(); 
		 
		 return pobSprite;     
	 }     
	 CC_SAFE_DELETE(pobSprite); 

	 return NULL;
}

void tanker::setTankerAction(){
	//{	
	char pszFileName[30];
	//------------------------坦克-----------------------------------------
	CCLog("\\\\\\\\\\\\\\\\\\\\chassisKind:%d",chassisKind);
	CCLog("\\\\\\\\\\\\\\\\\\\\turretKing:%d",turretKing);
	sprintf(pszFileName,"tanker/tank%d%d.ccbi", chassisKind,turretKing);
	 // 创建一个自动释放的 node 加载库
	  CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	  ccbreader = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
	 //读取一个ccbi的文件,生成一个ccnode实例
	 CCNode  *animationsTest  = ccbreader->readNodeGraphFromFile(pszFileName,this);//tank11.ccbi
	 //设置坐标 
	 //animationsTest->setPosition(ccp(pobSprite->getPositionX()-5,pobSprite->getPositionY()+60)); 

	 if(animationsTest  != NULL) {     
		 this->addChild(animationsTest,1,0);
	 }

	 //获取所有的动作序列 
	/*  CCArray* allSeq = ccbreader->getAnimationManager()->getSequences(); 
	  CCLog("============================") ;
	  for (int i=0; i<allSeq->count(); i++)
	  {     
		 //获取到每一个Seq 
		 CCBSequence* everySeq = (CCBSequence*)allSeq->objectAtIndex(i);
		    //获取每个序列动作周期、名字以及id 
		 // everySeq->getDuration(); 
		  CCLog("===========%s",everySeq->getName()) ; 
		  CCLog("===========%d",everySeq->getSequenceId()) ;
		  
	  } */
	 setState(holdUp);
	 //-------------------------------开火------------------------------------
	 CCLog("chassisKind----------:%d",turretKing);
	 sprintf(pszFileName,"tanker/gunfire%d1.ccbi", turretKing);//,turretKing
	 CCNodeLoaderLibrary *firenodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	 firebreader = new cocos2d::extension::CCBReader(firenodeloaderlibrary); 

	 CCNode  *animationsGun  = firebreader->readNodeGraphFromFile(pszFileName,this);

	 if(animationsGun  != NULL) {     
		 this->addChild(animationsGun,1,1);
	 }
	 animationsGun->setVisible(false);
	   	 //获取所有的动作序列 
	/*  CCArray* allSeq1 = firebreader->getAnimationManager()->getSequences(); 
	  CCLog("============================") ;
	  for (int i=0; i<(allSeq1->count()); i++)
	  {     
		 //获取到每一个Seq 
		 CCBSequence* everySeq = (CCBSequence*)allSeq1->objectAtIndex(i);
		    //获取每个序列动作周期、名字以及id 
		 // everySeq->getDuration(); 
		  CCLog("===========%s",everySeq->getName()) ; 
		  CCLog("===========%d",everySeq->getSequenceId()) ;
		  
	  } */
	 //------------------------升级-----------------------------------------
	 CCNodeLoaderLibrary *lvupnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	 lvupbreader = new cocos2d::extension::CCBReader(lvupnodeloaderlibrary); 

	 CCNode  *animationsLvUp  = lvupbreader->readNodeGraphFromFile("effect_lvup.ccbi",this);//effect_lvup.ccbi

	   if(animationsLvUp  != NULL) {     
		 this->addChild(animationsLvUp,0,2);
	 }
	   	 //获取所有的动作序列 
	/*CCArray* allSeqLvUp = lvupbreader->getAnimationManager()->getSequences(); 
	  CCLog("============================") ;
	  for (int i=0; i<(allSeqLvUp->count()); i++)
	  {     
		 //获取到每一个Seq 
		 CCBSequence* everySeq = (CCBSequence*)allSeqLvUp->objectAtIndex(i);
		    //获取每个序列动作周期、名字以及id 
		 // everySeq->getDuration(); 
		  CCLog("===========%s",everySeq->getName()) ; 
		  CCLog("===========%d",everySeq->getSequenceId()) ;
		  
	  } */
	  setLvUp(turretGrade-1);
	  animationsLvUp->setVisible(false);
	 //------------------------无敌-----------------------------------------
	  CCNodeLoaderLibrary *godnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	  godbreader = new cocos2d::extension::CCBReader(godnodeloaderlibrary); 

	 CCNode  *animationsGod  = godbreader->readNodeGraphFromFile("tanker/particle_mode_god.ccbi",this);//

	 if(animationsLvUp  != NULL) {     
		 this->addChild(animationsGod,1,3);
	 }
	 animationsGod->setVisible(false);

	//获取所有的动作序列 
	/*CCArray* allSeqGod = godbreader->getAnimationManager()->getSequences(); 
	  CCLog("============================") ;
	  for (int i=0; i<(allSeqGod->count()); i++)
	  {     
		 //获取到每一个Seq 
		 CCBSequence* everySeq = (CCBSequence*)allSeqGod->objectAtIndex(i);
		    //获取每个序列动作周期、名字以及id 
		 // everySeq->getDuration(); 
		  CCLog("===========%s",everySeq->getName()) ; 
		  CCLog("===========%d",everySeq->getSequenceId()) ;
		  
	  }*/ 
	 // setGod();
	 //播放一个动作:根据动作名称进行播放 
   // godbreader->getAnimationManager()->runAnimationsForSequenceNamed("effect_explode"); //Flying HoldUp
	 // ccbreader->getAnimationManager()->runAnimationsForSequenceIdTweenDuration(0, 0.3f);

	// }
	  //---------------------------爆炸-----------------------------
	  CCNodeLoaderLibrary *explodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	  tankExplodeBreader = new cocos2d::extension::CCBReader(explodeloaderlibrary); 

	  CCNode  *animationsExplode  = tankExplodeBreader->readNodeGraphFromFile("enemy/effect_explode_tank.ccbi",this);

	  if(animationsExplode  != NULL) {     
		  this->addChild(animationsExplode,2,4);
	  }
	  //获取所有的动作序列 
	  /*CCArray* allSeqExplode = enemyExplodeBreader->getAnimationManager()->getSequences(); 
	  CCLog("============================enemyExplodeBreader") ;
	  for (int i=0; i<(allSeqExplode->count()); i++)
	  {     
		  //获取到每一个Seq 
		  CCBSequence* everySeq = (CCBSequence*)allSeqExplode->objectAtIndex(i);
		  //获取每个序列动作周期、名字以及id 
		  // everySeq->getDuration(); 
		  CCLog("===========%s",everySeq->getName()) ; 
		  CCLog("===========%d",everySeq->getSequenceId()) ;
	  }*/

	  animationsExplode->setVisible(false);

	  //-------------------------------加满血------------------------------------------
	  CCNodeLoaderLibrary *cureloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	  cureBreader = new cocos2d::extension::CCBReader(cureloaderlibrary); 

	  CCNode  *animationsCure  = cureBreader->readNodeGraphFromFile("tanker/particle_health.ccbi",this);

	  if(animationsCure  != NULL) {     
		 this->addChild(animationsCure,2,5);
	  }
	  animationsCure->setVisible(false);
	//  cureBreader->getAnimationManager()->runAnimationsForSequenceNamed("1st"); 
	  //------------------------------------------提速--------------------------
	  CCNodeLoaderLibrary *moveSpeedlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	  moveSpeedBreader = new cocos2d::extension::CCBReader(moveSpeedlibrary); 

	  CCNode  *animationsMoveSpeedTime  = moveSpeedBreader->readNodeGraphFromFile("tanker/particle_speedup.ccbi",this);

	  if(animationsMoveSpeedTime  != NULL) {     
		  this->addChild(animationsMoveSpeedTime,2,6);
	  }
	  animationsMoveSpeedTime->setVisible(false);
	 
	  //--------------------------------------------暴击----------------------------------------------------------
	  CCNodeLoaderLibrary *criticallibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	  criticalBreader = new cocos2d::extension::CCBReader(criticallibrary); 

	  CCNode  *animationsCritical  = criticalBreader->readNodeGraphFromFile("tanker/particle_critical.ccbi",this);

	  if(animationsCritical  != NULL) {     
		  this->addChild(animationsCritical,2,7);
	  }
	  animationsCritical->setVisible(false);


	  CCLayer *bloodLayer = CCLayer::create();
	  this->addChild(bloodLayer,2,8);

	  setNewBirth();
	  //----------------------------------血条------------------------------
	  CCSprite * lifeBox = CCSprite::create("enemy/lifeBox.png");
	  lifeBox->setPosition(ccp(-40, 50));
	  lifeBox->setAnchorPoint( ccp(0, 0.5) );
	  this->addChild(lifeBox,1,9);

	  CCSprite * di = CCSprite::create("enemy/di.png");
	  di->setPosition(ccp(-40, 50));
	  di->setAnchorPoint( ccp(0, 0.5) );
	  this->addChild(di,0,11);

	  int kTagClipperNode = 100;

	  CCClippingNode *clipper= CCClippingNode::create();
	  clipper->setContentSize( CCSizeMake(82, 200) );
	  clipper->setAnchorPoint( ccp(0, 0.5));
	  clipper->setPosition(ccp(-40,50));

	  this->addChild(clipper,0,10);

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
	  CCSprite*content = CCSprite::create("enemy/lifeBack.png");
	  content->setTag( kTagContentNode);
	  content->setAnchorPoint( ccp(0, 0.5) );
	  content->setPosition( ccp(0, clipper->getContentSize().height/2) );//clipper->getContentSize().width / 2
	  clipper->addChild(content);

	  /*CCLabelAtlas* loseBloodNum=CCLabelAtlas::create("0", "UI/num.png", 18, 18, '0');
	  loseBloodNum->setAnchorPoint(ccp(0.5,0.5));
	  this->addChild(loseBloodNum,1,1021);
	  loseBloodNum->setVisible(false);



	  CCLabelAtlas* loseBloodCritNum=CCLabelAtlas::create("0", "critNum.png", 19, 19, '0');
	  loseBloodCritNum->setAnchorPoint(ccp(0.5,0.5));
	  this->addChild(loseBloodCritNum,1,1022);
	  loseBloodCritNum->setVisible(false);*/

}
void tanker::beAttack(int attack,bool critRateBool){
	if(!godBool){
	lifeNum-=(attack-defense);

	//tankState = BEATTACK;
	if(lifeNum<=0){
		lifeNum = 0;
		setState(explode);//爆炸
		this->getChildByTag(9)->setVisible(false);
		this->getChildByTag(10)->setVisible(false);
		this->getChildByTag(11)->setVisible(false);
	}else{
		CCClippingNode *clipper=((CCClippingNode*)this->getChildByTag(10));//->setContentSize( CCSizeMake(200, 200)
		clipper->setContentSize(CCSizeMake((1.0*lifeNum/MAXLife)*82, 200));
		//CCLog("------------------lifeNum:%d",lifeNum);
		//CCLog("------------------clipper:%f",(1.0*lifeNum/recordLifeNum)*70);
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
	//else{
		//CCActionInterval*  actionBlink = CCBlink::create(0.2, 5);
		// CCActionInterval*  actionTint = CCTintBy::create(0.55, 0,0,0);
		// CCActionInterval*  action3 = CCFadeTo::create(2, 10);
		//(this->getChildByTag(0))->runAction(actionBlink);
		if(critRateBool)
		{
			CCLabelAtlas* loseBloodCritNum=CCLabelAtlas::create("0", "critNum.png", 19, 19, '0');
			loseBloodCritNum->setAnchorPoint(ccp(0.5,0.5));

			this->getChildByTag(8)->addChild(loseBloodCritNum,1);

			char tm[50];
			CCLog("------------------beAttack:%d",(attack-defense));
			sprintf(tm, "%d",(attack-defense)<0?1:(attack-defense)); 

			loseBloodCritNum->setString(tm);
			float sx= (this->getChildByTag(0))->getPositionX();
			float sy= (this->getChildByTag(0))->getPositionY();

			int pos_1x = random(loseBloodNumPos_1x_start,loseBloodNumPos_1x_end);
			int pos_1y = random(loseBloodNumPos_1y_start,loseBloodNumPos_1y_end);

			int pos_2x = random(loseBloodNumPos_2x_start,loseBloodNumPos_2x_end);
			int pos_2y = random(loseBloodNumPos_2y_start,loseBloodNumPos_2y_end);

			ccBezierConfig bezier; 
			bezier.controlPoint_1 = ccp(sx, sy); // 起始点  
			bezier.controlPoint_2 = ccp(sx+(loseBloodNumDirection?pos_1x:-pos_1x), sy+pos_1y); //控制点  
			bezier.endPosition = ccp(sx+(loseBloodNumDirection?pos_2x:-pos_2x), sy-pos_2y); // 结束位置  

			CCActionInterval*  actionBezier = CCBezierBy::create(loseBloodNumTime, bezier);

			loseBloodCritNum->runAction(CCSequence::create(actionBezier,CCCallFuncN::create(this,callfuncN_selector(tanker::loseBloodCritNumEnd)),NULL));
			loseBloodNumDirection =!loseBloodNumDirection;
		}else{
			//CCLabelAtlas* loseBloodNum=((CCLabelAtlas*)this->getChildByTag(1021));
			//loseBloodNum->setVisible(true);
			CCLabelAtlas* loseBloodNum=CCLabelAtlas::create("0", "UI/num.png", 18, 18, '0');
			loseBloodNum->setAnchorPoint(ccp(0.5,0.5));
			this->getChildByTag(8)->addChild(loseBloodNum,1);

			char tm[50];
			CCLog("------------------beAttack:%d",(attack-defense));
			sprintf(tm, "%d",(attack-defense)<0?1:(attack-defense)); 

			loseBloodNum->setString(tm);
			float sx= (this->getChildByTag(0))->getPositionX();
			float sy= (this->getChildByTag(0))->getPositionY();

			int pos_1x = random(loseBloodNumPos_1x_start,loseBloodNumPos_1x_end);
			int pos_1y = random(loseBloodNumPos_1y_start,loseBloodNumPos_1y_end);

			int pos_2x = random(loseBloodNumPos_2x_start,loseBloodNumPos_2x_end);
			int pos_2y = random(loseBloodNumPos_2y_start,loseBloodNumPos_2y_end);

			ccBezierConfig bezier; 
			bezier.controlPoint_1 = ccp(sx, sy); // 起始点  
			bezier.controlPoint_2 = ccp(sx+(loseBloodNumDirection?pos_1x:-pos_1x), sy+pos_1y); //控制点  
			bezier.endPosition = ccp(sx+(loseBloodNumDirection?pos_2x:-pos_2x), sy-pos_2y); // 结束位置  

			CCActionInterval*  actionBezier = CCBezierBy::create(loseBloodNumTime, bezier);

			loseBloodNum->runAction(CCSequence::create(actionBezier,CCCallFuncN::create(this,callfuncN_selector(tanker::loseBloodCritNumEnd)),NULL));
		    loseBloodNumDirection =!loseBloodNumDirection;
		}
	//}
  }
}
void tanker::loseBloodCritNumEnd(CCNode* sender){
	this->getChildByTag(8)->removeChild(sender,-1);
	
}
void tanker::loseBloodNumEnd(CCNode* sender){
	this->getChildByTag(8)->removeChild(sender,-1);

}

void tanker::setName(char *name){
	this->name = name;
}

const char* tanker::getName(){
	return name;
}

void tanker::setDestroy(bool destroy){
	this->destroy = destroy;
}

bool tanker::getDestroy(){
	return destroy;
}

void tanker::setLayerName(const char *layerName){
	this->layerName = layerName;
}

const char* tanker::getLayerName(){
	return layerName;
}

void tanker::openFire(){
	(this->getChildByTag(1))->setVisible(true);

	switch(tankState){
	  case moveUp:
	  case holdUp:
		  {
			 
			 firebreader->getAnimationManager()->runAnimationsForSequenceNamed("GunfireUp"); 
		  }
          break;
	  case moveDown:
	  case holdDown:
		  {
			  firebreader->getAnimationManager()->runAnimationsForSequenceNamed("GunfireDown"); 
		  }
		  break;
	  case moveLeft:
	  case holdLeft:
		  {
             firebreader->getAnimationManager()->runAnimationsForSequenceNamed("GunfireLeft");
		  }
		  break;
	  case moveRight:
	  case holdRight:
		  {
			  firebreader->getAnimationManager()->runAnimationsForSequenceNamed("GunfireRight");
		  }
		  break;
	}
}
void tanker::setLvUp(int lv){
	char pszFileName[30];
	//sprintf(pszFileName,"lv%d", lv);
	//lvupbreader->getAnimationManager()->runAnimationsForSequenceNamed(pszFileName);

	moveSpeed=moveSpeed+lv*20;//提升移动速度
	recordMoveSpeed = moveSpeed;

	Bullet::setBulletAttackByLvUp(lv);//提升子弹攻击力
}

 void tanker::setMoveSpeed(){
	// moveSpeedTime.setMillis(15*1000);
	// moveSpeedTime.start();
	// CCLOG("moveSpeed:%f",moveSpeed);
	// CCLOG("recordMoveSpeed:%f",recordMoveSpeed);

	 if(moveSpeed>recordMoveSpeed){
		 return;
	 }else{
	    moveSpeed = recordMoveSpeed*1.5f;
	   schedule(schedule_selector(tanker::upDataMoveSpeed),15.1f);

	   moveSpeedBreader->getAnimationManager()->runAnimationsForSequenceNamed("1st");
	   this->getChildByTag(6)->setVisible(true);
	 }
 }

  void tanker::upDataMoveSpeed(float dt){
	  //if(moveSpeedTime.getSurplusMillis()<=0){
	   this->getChildByTag(6)->setVisible(false);
	   moveSpeed = recordMoveSpeed;
	   unschedule(schedule_selector(tanker::upDataMoveSpeed));
	  //}
  }


void tanker::setCritical(){
	this->getChildByTag(7)->setVisible(true);
	schedule(schedule_selector(tanker::upDataCritical),15.1f);

	if(moveSpeed>recordMoveSpeed){
		return;
	}else{
		moveSpeed = recordMoveSpeed*1.3f;
	}
}
void tanker::upDataCritical(float dt){
	this->getChildByTag(7)->setVisible(false);
	unschedule(schedule_selector(tanker::upDataCritical));
	 moveSpeed = recordMoveSpeed;
}

bool tanker::isGod(){
	return godBool||newBirthGod;
}

void tanker::setNewBirth(){
    //--------------------------------------------------出生时，无敌------------------------------------------
	CCNodeLoaderLibrary *newBirthlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	cocos2d::extension::CCBReader  *newBirthBreader = new cocos2d::extension::CCBReader(newBirthlibrary); 

	CCNode  *animationsnewBirth  = newBirthBreader->readNodeGraphFromFile("tanker/effect_herobirth.ccbi",this);

	if(animationsnewBirth != NULL) {     
		this->addChild(animationsnewBirth,3,1029);
	}
	newBirthBreader->getAnimationManager()->runAnimationsForSequenceNamed("1st"); 

    newBirthGod = true;//设置成无敌状态
	schedule(schedule_selector(tanker::upDataNewBirth),3.0f);

}

void tanker::upDataNewBirth(float dt){
	this->removeChildByTag(1029);
	newBirthGod = false;
	unschedule(schedule_selector(tanker::upDataNewBirth));
}

void tanker::setPayGod(){
	godbreader->getAnimationManager()->runAnimationsForSequenceNamed("1st");

	godBool = true;//设置成无敌状态
	this->getChildByTag(3)->setVisible(true);
}
void tanker::setGod(){
	if(!godBool){
	  godbreader->getAnimationManager()->runAnimationsForSequenceNamed("1st");
	  //godTime.setMillis(15*1000);
	 // godTime.start();
	  godBool = true;//设置成无敌状态
	  this->getChildByTag(3)->setVisible(true);
	  schedule(schedule_selector(tanker::upDataGod),15.1f);
	}
}

void tanker::upDataGod(float dt){
	//if(godTime.getSurplusMillis()<=0){
		godBool = false;
		this->getChildByTag(3)->setVisible(false);
		unschedule(schedule_selector(tanker::upDataGod));
	//}
}
void tanker::addFullLift(){
	lifeNum = MAXLife;

	this->getChildByTag(8)->removeAllChildren();

	(this->getChildByTag(5))->setVisible(true);
	cureBreader->getAnimationManager()->runAnimationsForSequenceNamed("1st"); 
	this->schedule(schedule_selector(tanker::upDataTankCure),1.0f);


	CCClippingNode *clipper=((CCClippingNode*)this->getChildByTag(10));//->setContentSize( CCSizeMake(200, 200)
	clipper->setContentSize(CCSizeMake((1.0*lifeNum/MAXLife)*82, 200));
	//CCLog("------------------lifeNum:%d",lifeNum);
	//CCLog("------------------clipper:%f",(1.0*lifeNum/recordLifeNum)*70);
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

void tanker::upDataTankCure(float time){
	(this->getChildByTag(5))->setVisible(false);
	this->unschedule(schedule_selector(tanker::upDataTankCure));
}


int tanker::getMAXLift(){

	return MAXLife;
}
int tanker::getLife(){

	return lifeNum;
}
float  tanker::getMoveSpeed(){

	float speed;
	speed = moveSpeed-Bullet::getWeight();

	return speed;
};

void tanker::setState(int state){
	if(tankState==state){
		return;
	}
	tankState = state; 

  switch(tankState){
	case moveUp:
		{
		     ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("GoUp");
		}
		break;
	case moveDown:
		{
			 ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("GoDown");
		}
		break;
	case moveLeft:
		{
			
            ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("GoLeft");

		}
		break;
	case moveRight:
		{
		  ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("GoRight");
		}
		break;
	case holdUp:
		{
			 ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("HoldUp");//
			//ccbreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(tanker::goLeft)); 
		}
		break;
	case holdDown:
		{
			 ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("HoldDown");
		}
		break;
	case holdLeft:
		{
			ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("HoldLeft");//HoldLeft
		}
		break;
	case holdRight:
		{
			ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("HoldRight");
		}
		break;
	case explode:
		{
			this->getChildByTag(0)->setVisible(false);
			this->getChildByTag(4)->setVisible(true);
			tankExplodeBreader->getAnimationManager()->runAnimationsForSequenceNamed("burst");
			tankExplodeBreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(tanker::goEnd));
		}
       break;
    }
  }

void tanker::goEnd(){
	CCLog("tanker------------------------------show  goEnd");
	this->getChildByTag(3)->setVisible(false);
	setDestroy(true);
}
int tanker::getState(){
	CCLog("tanker-----------------tankState:%d",tankState);
	return tankState;
}

void tanker::goLeft(){
	//ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("HoldUp");
	//CCLog("*********************");
}