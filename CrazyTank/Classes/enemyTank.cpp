#include "enemyTank.h"

using namespace cocos2d::extension; 
using namespace cocos2d;

enemyTank::enemyTank(void):enemyKing(0),
	                       trackRadius(0),
						   defense(0),
						   lifeNum(0),
						   moveSpeed(0),
						   trackState(false),
						   trackPathStep(0),
						   penetrate(0),
						   critRate(0),
						   rateOfFire(0),
						   bulletStrength(0),
						   bulletRange(0),
						   bulletAttack(0),
						   bulletSpeed(0),
						   show(false)
{
}


enemyTank::~enemyTank(void)
{
	CC_SAFE_RELEASE_NULL(ccbreader);
	CC_SAFE_RELEASE_NULL(enemyFireBreader);
	CC_SAFE_RELEASE_NULL(enemyExplodeBreader);
	CC_SAFE_RELEASE_NULL(bomBreader);
	CC_SAFE_RELEASE_NULL( standBreader);
	CC_SAFE_RELEASE_NULL( appearBreader);
}
void enemyTank::myInit(int king)
{
	this->enemyKing = king;
	this->destroy = false;

	char layerName[30];
	sprintf(layerName,"enemy/enemy_%d.xml", enemyKing);
	XMLParser *pXmlParser = XMLParser::parseWithFile(layerName);

	defense = (pXmlParser->getString("defense"))->intValue();
    lifeNum = (pXmlParser->getString("lifeNum"))->intValue();
	moveSpeed = (pXmlParser->getString("moveSpeed"))->floatValue();

	trackRadius = (pXmlParser->getString("trackRadius"))->intValue();
	//CCLog("===trackRadius:%d",trackRadius);

	penetrate = (pXmlParser->getString("penetrate"))->intValue();
	critRate = (pXmlParser->getString("critRate"))->intValue();
	rateOfFire = (pXmlParser->getString("rateOfFire"))->floatValue();
	bulletStrength = (pXmlParser->getString("bulletStrength"))->intValue();
	bulletRange = (pXmlParser->getString("bulletRange"))->intValue();
	bulletAttack = (pXmlParser->getString("bulletAttack"))->intValue();
	bulletSpeed = (pXmlParser->getString("bulletSpeed"))->intValue();

	changeDirTime.setMillis(1500);
	changeDirTime.start();
	
	rateOfFireTime.setMillis(rateOfFire*1000);
	rateOfFireTime.start();

	recordLifeNum = lifeNum;

}

enemyTank* enemyTank::spriteCreate(int king){

	 enemyTank *pobSprite = new enemyTank();       
	 if (pobSprite && pobSprite->init())//备注1  initWithFile pobSprite->init()
	 {  
		 pobSprite->myInit(king);        
		 pobSprite->autorelease(); 
		 return pobSprite;     
	 }     
	 CC_SAFE_DELETE(pobSprite); 
	 return NULL;
}


 CCObject* enemyTank::copyWithZone(cocos2d::CCZone* pZone){
	CCZone *pNewZone = NULL;  
    enemyTank *pRet = NULL;
    if (pZone && pZone->m_pCopyObject)  
    {  
        pRet = (enemyTank*)(pZone->m_pCopyObject);  
    }  
   else  
    {  
        pRet = enemyTank::spriteCreate(2); 
		pRet->setTankerAction();
        pNewZone = new CCZone(pRet);  
    }  
   //copy member data   
    pRet->m_nTag = m_nTag;  
    CC_SAFE_DELETE(pNewZone);  
	pRet->autorelease(); 

    return pRet; 
}
/*enemyTank* enemyTank::CopyEnemyTank(){

	  enemyTank *pRet = NULL;

	 // pRet = (enemyTank*)(otherEnemyTank->m_pCopyObject);  
	  pRet = this;
	  return pRet; 
}*/

void enemyTank::setTankerAction(){
	char pszFileName[30];
	//------------------------坦克-----------------------------------------
	CCLog("===enemyKing:%d",enemyKing);
	sprintf(pszFileName,"enemy/enemy%d.ccbi", enemyKing);
	CCLog("===enemyKing:%s",pszFileName);
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
	  CCArray* allSeq = ccbreader->getAnimationManager()->getSequences(); 
	  CCLog("enemyTank============================") ;
	  for (int i=0; i<allSeq->count(); i++)
	  {     
		 //获取到每一个Seq 
		 CCBSequence* everySeq = (CCBSequence*)allSeq->objectAtIndex(i);
		    //获取每个序列动作周期、名字以及id 
		 // everySeq->getDuration(); 
		  CCLog("enemyTank===========%s",everySeq->getName()) ; 
		  CCLog("enemyTank===========%d",everySeq->getSequenceId()) ;
		  
	  } 
	 setState(moveUp);

	 //-------------------------------开火------------------------------------
	 sprintf(pszFileName,"enemy/gunfire_enemy%d.ccbi",enemyKing);
	 CCNodeLoaderLibrary *firenodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	 enemyFireBreader = new cocos2d::extension::CCBReader(firenodeloaderlibrary); 

	 CCNode  *animationsGun  = enemyFireBreader->readNodeGraphFromFile(pszFileName,this);

	  if(animationsGun  != NULL) {     
		 this->addChild(animationsGun,1,1);
	  }
	   	 //获取所有的动作序列 
	  CCArray* allSeq1 = enemyFireBreader->getAnimationManager()->getSequences(); 
	  CCLog("============================enemyFireBreader") ;
	  for (int i=0; i<(allSeq1->count()); i++)
	  {     
		 //获取到每一个Seq 
		 CCBSequence* everySeq = (CCBSequence*)allSeq1->objectAtIndex(i);
		    //获取每个序列动作周期、名字以及id 
		 // everySeq->getDuration(); 
		  CCLog("===========%s",everySeq->getName()) ; 
		  CCLog("===========%d",everySeq->getSequenceId()) ;
		  
	  }
	  //---------------------------爆炸-----------------------------
	  CCNodeLoaderLibrary *explodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	  enemyExplodeBreader = new cocos2d::extension::CCBReader(explodeloaderlibrary); 

	  CCNode  *animationsExplode  = enemyExplodeBreader->readNodeGraphFromFile("enemy/effect_explode_tank.ccbi",this);

	  if(animationsExplode  != NULL) {     
		  this->addChild(animationsExplode,2,2);
	  }
	  //获取所有的动作序列 
	  CCArray* allSeqExplode = enemyExplodeBreader->getAnimationManager()->getSequences(); 
	  CCLog("============================enemyExplodeBreader") ;
	  for (int i=0; i<(allSeqExplode->count()); i++)
	  {     
		  //获取到每一个Seq 
		  CCBSequence* everySeq = (CCBSequence*)allSeqExplode->objectAtIndex(i);
		  //获取每个序列动作周期、名字以及id 
		  // everySeq->getDuration(); 
		  CCLog("===========%s",everySeq->getName()) ; 
		  CCLog("===========%d",everySeq->getSequenceId()) ;
	  }

	  animationsExplode->setVisible(false);
	  //---------------------------被轰炸-----------------------------

	  CCNodeLoaderLibrary *bomloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	  bomBreader = new cocos2d::extension::CCBReader(bomloaderlibrary); 

	  CCNode  *bomExplode  = bomBreader->readNodeGraphFromFile("enemy/bom_all.ccbi",this);

	  if(bomExplode  != NULL) {     
		  this->addChild(bomExplode,2,3);
	  }
	  //获取所有的动作序列 
	  CCArray* allBomSeqExplode = bomBreader->getAnimationManager()->getSequences(); 
	  CCLog("============================enemyExplodeBreader");
	  for (int i=0; i<(allBomSeqExplode->count()); i++)
	  {     
		  //获取到每一个Seq 
		  CCBSequence* everySeq = (CCBSequence*)allBomSeqExplode->objectAtIndex(i);
		  //获取每个序列动作周期、名字以及id 
		  // everySeq->getDuration(); 
		  CCLog("===========%s",everySeq->getName()) ; 
		  CCLog("===========%d",everySeq->getSequenceId()) ;
	  }
	 // bomBreader->getAnimationManager()->runAnimationsForSequenceNamed("1st");
	 bomExplode->setVisible(false);
	 //----------------------------------站立不动--------------------------
	 CCNodeLoaderLibrary *standloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	 standBreader = new cocos2d::extension::CCBReader(standloaderlibrary); 

	 CCNode  *standExplode  = standBreader->readNodeGraphFromFile("enemy/particle_lock.ccbi",this);

	 if(standExplode  != NULL) {     
		 this->addChild(standExplode,2,4);
	 }
	 standExplode->setVisible(false);
	 //----------------------------------出现----------------------appearBreader
	 CCNodeLoaderLibrary *appearloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	 appearBreader = new cocos2d::extension::CCBReader(appearloaderlibrary); 

	 CCNode  *appeatExplode  = appearBreader->readNodeGraphFromFile("enemy/effect_appear.ccbi",this);

	 if(appeatExplode  != NULL) {
		 this->addChild(appeatExplode,2,5);
	 }
	 appearBreader->getAnimationManager()->runAnimationsForSequenceNamed("1st");

	 CCLayer *bloodLayer = CCLayer::create();
	 this->addChild(bloodLayer,2,6);

	 //----------------------------------血条------------------------------
	if(enemyKing>=11){
	 CCSprite * lifeBox = CCSprite::create("enemy/lifeBox.png");
	 lifeBox->setPosition(ccp(-40, enemyKing>10?60:40));
	 lifeBox->setAnchorPoint( ccp(0, 0.5) );
	 this->addChild(lifeBox,1,7);

	 int kTagClipperNode = 100;

	 CCClippingNode *clipper= CCClippingNode::create();
	 clipper->setContentSize( CCSizeMake(82, 200) );
	 clipper->setAnchorPoint( ccp(0, 0.5));
	 clipper->setPosition(ccp(-40,enemyKing>10?60:40));

	 this->addChild(clipper,0,8);

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
	 //
	}
	 /*CCLabelAtlas* loseBloodNum=CCLabelAtlas::create("0", "UI/num.png", 18, 18, '0');
	 loseBloodNum->setAnchorPoint(ccp(0.5,0.5));
	 this->addChild(loseBloodNum,1,1021);
	 loseBloodNum->setVisible(false);



	 CCLabelAtlas* loseBloodCritNum=CCLabelAtlas::create("0", "critNum.png", 19, 19, '0');
	 loseBloodCritNum->setAnchorPoint(ccp(0.5,0.5));
	 this->addChild(loseBloodCritNum,1,1022);
	 loseBloodCritNum->setVisible(false);*/
}

void enemyTank::beAttack(int attack,bool critRateBool){
	lifeNum-=(attack-defense);

	CCLog("------------------attack:%d",attack);
	CCLog("------------------defense:%d",defense);

	if(lifeNum<=0){
	   lifeNum = 0;
	   setState(explode);//爆炸

	   if(this->getChildByTag(7)){
		    this->getChildByTag(7)->setVisible(false);
	   }

	   if(this->getChildByTag(8)){
		    this->getChildByTag(8)->setVisible(false);
	   }
	}else{
	 if(enemyKing>=11){
		CCClippingNode *clipper=((CCClippingNode*)this->getChildByTag(8));//->setContentSize( CCSizeMake(200, 200)
		clipper->setContentSize(CCSizeMake((1.0*lifeNum/recordLifeNum)*82, 200));
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
	}

	  if(critRateBool)
	  {
		 // CCLabelAtlas* loseBloodNum=((CCLabelAtlas*)this->getChildByTag(1022));
		  //loseBloodNum->setVisible(true);
		  CCLabelAtlas* loseBloodCritNum=CCLabelAtlas::create("0", "critNum.png", 19, 19, '0');
		  loseBloodCritNum->setAnchorPoint(ccp(0.5,0.5));
		 
		   this->getChildByTag(6)->addChild(loseBloodCritNum,1);

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
		  bezier.controlPoint_1 = ccp(sx, sy); //起始点  
		  bezier.controlPoint_2 = ccp(sx+(loseBloodNumDirection?pos_1x:-pos_1x), sy+pos_1y); //控制点  
		  bezier.endPosition = ccp(sx+(loseBloodNumDirection?pos_2x:-pos_2x), sy-pos_2y); //结束位置  

		  CCActionInterval*  actionBezier = CCBezierBy::create(loseBloodNumTime, bezier);

		  loseBloodCritNum->runAction(CCSequence::create(actionBezier,CCCallFuncN::create(this,callfuncN_selector(enemyTank::loseBloodCritNumEnd)),NULL));
		  loseBloodNumDirection =!loseBloodNumDirection;
	  }else{
	  //  CCLabelAtlas* loseBloodNum=((CCLabelAtlas*)this->getChildByTag(1021));
	    //loseBloodNum->setVisible(true);
		  CCLabelAtlas* loseBloodNum=CCLabelAtlas::create("0", "UI/num.png", 18, 18, '0');
		  loseBloodNum->setAnchorPoint(ccp(0.5,0.5));
		   this->getChildByTag(6)->addChild(loseBloodNum,1);

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
	     loseBloodNum->runAction(CCSequence::create(actionBezier,CCCallFuncN::create(this,callfuncN_selector(enemyTank::loseBloodCritNumEnd)),NULL));
	     loseBloodNumDirection =!loseBloodNumDirection;
	  }
	//}
	  CCLog("------------------////////////////////");
}

void enemyTank::loseBloodCritNumEnd(CCNode* sender){
	 this->getChildByTag(6)->removeChild(sender,-1);
}
void enemyTank::loseBloodNumEnd(CCNode* sender){
	 this->getChildByTag(6)->removeChild(sender,-1);
}

void enemyTank::setShowStand(bool  isStand){
	if(!this->getChildByTag(4)){
		CCLog("jjjjjjjjjjjjjjjjjjjjjjjjjjj");
		return;
	}
	if(isStand){
		//if(this->getChildByTag(4)){
			(this->getChildByTag(4))->setVisible(true);
			standBreader->getAnimationManager()->runAnimationsForSequenceNamed("1st");
		//}
		
	}else{
		//if(this->getChildByTag(4)){
			(this->getChildByTag(4))->setVisible(false);
		//}
	}
}


bool enemyTank::getShow4(){
	if(this->getChildByTag(4)){
		return this->getChildByTag(4)->isVisible();
	}else{
		return false;
	}
	
}

 void enemyTank::setShow(bool  show){
	 this->show = show;
	 if(show){
		 rateOfFireTime.reset();
		 rateOfFireTime.start();

	     this->getChildByTag(6)->removeAllChildren();
		 this->getChildByTag(6)->setVisible(true);
		 this->getChildByTag(0)->setVisible(true);
		 tankState = moveDown;
		 lifeNum = recordLifeNum;//回复生命值
		 appearBreader->getAnimationManager()->runAnimationsForSequenceNamed("1st");
		 if((this->getChildByTag(4))->isVisible()){
			 (this->getChildByTag(4))->setVisible(false);
		 }
		
		 if(this->getChildByTag(7)){
			 this->getChildByTag(7)->setVisible(true);
		 }
		 
		 if(this->getChildByTag(8)){
			  this->getChildByTag(8)->setVisible(true);

			  CCClippingNode *clipper=((CCClippingNode*)this->getChildByTag(8));//->setContentSize( CCSizeMake(200, 200)
			  clipper->setContentSize(CCSizeMake((1.0*lifeNum/recordLifeNum)*70, 200));


			  CCLog("------------------lifeNum:%d",lifeNum);
			  CCLog("------------------clipper:%f",(1.0*lifeNum/recordLifeNum)*70);
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
 }

 bool enemyTank::getShow(){
	 return show;
 }

bool  enemyTank::isTrackRadius(cocos2d::CCPoint enemyTilePot,cocos2d::CCPoint playerTilePot){
	if(trackRadius>0){
		if(abs(enemyTilePot.x-playerTilePot.x)<=trackRadius&&abs(enemyTilePot.y-playerTilePot.y)<=trackRadius){
			if(!trackState){
			   setTrackState(true);
			   return  true;
			}
		}else{
			trackState = false;
		}
	}
	return  false;
}
int  enemyTank::getKing(){
	return enemyKing;
}
int   enemyTank::getPenetrate(){
	return penetrate;
}
int   enemyTank::getCritRate(){
	return critRate;
}
int   enemyTank::getBulletSpeed(){
	return bulletSpeed;
}
int   enemyTank::getBulletStrength(){
	return bulletStrength;
}
int   enemyTank::getBulletAttack(){
	return bulletAttack;
}
int   enemyTank::getBulletRange(){
	return bulletRange;
}
/*void  enemyTank::setMapTag(int tag){
	mapChildTag = tag;
}
int   enemyTank::getMapTag(){
	return mapChildTag;
}*/
bool  enemyTank::getTrackState(){

	return trackState;

}

void  enemyTank::setTrackState(bool trackBool){
	trackState = trackBool;
}

void enemyTank::setTrackPath(cocos2d::CCArray  *path){
	trackPath = path;
	trackPathStep = 0;
}
CCArray* enemyTank::getTrackPath(){
	return trackPath;
}

void enemyTank::setTrackPathStep(int step){
	if(step<trackPath->count()){//
		trackPathStep = step;
	}
}
int enemyTank::getTrackPathStep(){
	if(trackPathStep<trackPath->count()){
		return trackPathStep ;
	}else{
		return -1;
	}
	
}

void enemyTank::setName(char *name){
	this->name = name;
}

const char* enemyTank::getName(){
	return name;
}

void enemyTank::setDestroy(bool destroy){
	this->destroy = destroy;
}

bool enemyTank::getDestroy(){
	return destroy;
}

void enemyTank::setLayerName(const char *layerName){
	this->layerName = layerName;
}

const char* enemyTank::getLayerName(){
	return layerName;
}
int enemyTank::getLifeNum(){
	return  lifeNum;
}

float enemyTank::getSpeed(){
	return moveSpeed;
}

int enemyTank::getTankState(){
	return tankState;
}


bool  enemyTank::isChangeDir(){
	// CCLog("changeDirTime.getSurplusMillis():%ld",changeDirTime.getSurplusMillis());
	if(changeDirTime.getSurplusMillis()<=0){

	}
	return changeDirTime.getSurplusMillis()<=0?true:false;
}

int enemyTank::getState(){
	return tankState;
}
void enemyTank::setState(int state){
	if(tankState==state||tankState==explode){
		return;
	}
	tankState = state; 

	//changeDirTime.setMillis(1500);
	changeDirTime.reset();
    changeDirTime.start();
   // CCLog("==changeDirTime.getSurplusMillis():%ld",changeDirTime.getSurplusMillis());

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
	case explode:
		{
			this->getChildByTag(2)->setVisible(true);
			enemyExplodeBreader->getAnimationManager()->runAnimationsForSequenceNamed("burst");
			this->schedule(schedule_selector(enemyTank::hideTank),0.08f);

			 enemyExplodeBreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(enemyTank::goEnd)); 
			 setTrackState(false);
		}
		break;
	case BOMBING:
		{
			this->getChildByTag(3)->setVisible(true);
			bomBreader->getAnimationManager()->runAnimationsForSequenceNamed("1st");
			bomBreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(enemyTank::bomIsAttack)); 

		}
		break;
	case BOMATTACK:
		{
		  bomBreader->getAnimationManager()->runAnimationsForSequenceNamed("2nd");
		  bomBreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(enemyTank::bomIsAttack2nd));
		}
		break;
	case BEATTACK:
		 {
			
		 }
		 break;
	 }
}


void enemyTank::bomIsAttack(){
	setState(BOMATTACK);
	setTrackState(false);
	
	//tankState = BEATTACK;
}

void enemyTank::bomIsAttack2nd(){
	//beAttack(1000);
	lifeNum-=(600-defense);
	beAttack(600-defense,true);
	if(lifeNum<=0){
		this->schedule(schedule_selector(enemyTank::hideTank),0.01f);
		rateOfFireTime.reset();
		lifeNum = 0;
		goEnd();
	}
}

void enemyTank::hideTank(float dt){
	this->getChildByTag(0)->setVisible(false);
	this->unschedule(schedule_selector(enemyTank::hideTank));

	if((this->getChildByTag(4))->isVisible()){
		(this->getChildByTag(4))->setVisible(false);
	}
}

void enemyTank::goEnd(){
	 CCLog("------------------------------show  goEnd");
	 this->getChildByTag(3)->setVisible(false);
	 this->getChildByTag(6)->setVisible(false);
	 this->getChildByTag(2)->setVisible(false);
	 this->getChildByTag(5)->setVisible(false);
	 if((this->getChildByTag(4))->isVisible()){
		 (this->getChildByTag(4))->setVisible(false);
	 }

	 setDestroy(true);
	 setShow(false);
}

bool enemyTank::openFire(){
  if(rateOfFireTime.getSurplusMillis()<=0&&tankState!=BOMATTACK){
	switch(tankState){
	  case moveUp:
		  {			 
			 enemyFireBreader->getAnimationManager()->runAnimationsForSequenceNamed("GunfireUp"); 
		  }
          break;
	  case moveDown:
		  {
			  enemyFireBreader->getAnimationManager()->runAnimationsForSequenceNamed("GunfireDown"); 
		  }
		  break;
	  case moveLeft:
		  {
             enemyFireBreader->getAnimationManager()->runAnimationsForSequenceNamed("GunfireLeft");
		  }
		  break;
	  case moveRight:
		  {
			  enemyFireBreader->getAnimationManager()->runAnimationsForSequenceNamed("GunfireRight");
		  }
		  break;
	}
	rateOfFireTime.reset();
	rateOfFireTime.start();

	 return  true;
  }else{
	 return  false;
  }
}

 void enemyTank::setBulletArrayIndex(int  index){
	 this->bulletArrayIndex = index;
 }

 int  enemyTank::getBulletArrayIndex(){

	 return  bulletArrayIndex;
 }