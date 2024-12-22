#include "enemyBullet.h"
#include "Tool.h"

using namespace cocos2d::extension; 
using namespace cocos2d;

enemyBullet::enemyBullet(void):bulletSpeed(0),
	bulletRange(0),
	bulletStrength(0),
	penetrate(false)
{
}


enemyBullet::~enemyBullet(void)
{
}

void    enemyBullet::myInit(enemyTank* _enemyTank){
	bulletSpeed =_enemyTank->getBulletSpeed();
	bulletAttack=_enemyTank->getBulletAttack();
	bulletRange = _enemyTank->getBulletRange();
	bulletStrength = _enemyTank->getBulletStrength();
	critRate = _enemyTank->getCritRate();

	king = _enemyTank->getKing();//坦克种类

	bulletState = END;
	this->destroy = false;
}

enemyBullet* enemyBullet::spriteCreate(enemyTank* _enemyTank){
	enemyBullet *pobSprite = new enemyBullet();       
	if (pobSprite && pobSprite->init())//备注1
	{        		 
		 pobSprite->myInit(_enemyTank);        
		 pobSprite->autorelease(); 
		 return pobSprite;     
	 }     
	 CC_SAFE_DELETE(pobSprite); 
	 return NULL;
}
void enemyBullet::setBulletAction(){
	  char pszFileName[30];
	  sprintf(pszFileName,"enemy/enemybullet_%d.ccbi",king);

	  CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	  ccbreader = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
	  
	 //读取一个ccbi的文件,生成一个ccnode实例
     CCNode  *animationsTest  = ccbreader->readNodeGraphFromFile(pszFileName,this);//tank11.ccbi
	 //设置坐标 
	 //animationsTest->setPosition(ccp(pobSprite->getPositionX()-5,pobSprite->getPositionY()+60)); 

	 if(animationsTest  != NULL){     
		 this->addChild(animationsTest,0,0);
	 }

	 //获取所有的动作序列 
	  CCArray* allSeq = ccbreader->getAnimationManager()->getSequences(); 
	  CCLog("============================enemyBullet") ;
	  for (int i=0; i<(allSeq->count()); i++)
	  {     
		 //获取到每一个Seq 
		 CCBSequence* everySeq = (CCBSequence*)allSeq->objectAtIndex(i);
		  //获取每个序列动作周期、名字以及id 
		 // everySeq->getDuration(); 
		  CCLog("===========%s",everySeq->getName()) ; 
		  CCLog("===========%d",everySeq->getSequenceId()) ;
		  
	  } 
	 //ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("flying");
}

void enemyBullet::setName(char *name){
	this->name = name;
}

const char* enemyBullet::getName(){
	return name;
}

void enemyBullet::setDestroy(bool destroy){
	this->destroy = destroy;
}

bool enemyBullet::getDestroy(){
	return destroy;
}

void enemyBullet::setLayerName(const char *layerName){
	this->layerName = layerName;
}

const char* enemyBullet::getLayerName(){
	return layerName;
}


void enemyBullet::setState(int state){
	bulletState = state;
	switch(bulletState){
	     case moveUp:
		    {
			   this->destroy = false;

			   if(king==1){//子弹1
				   ccbreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(enemyBullet::goEnd)); 
			   }
			   ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("up");
		    }
		    break;
		 case moveDown:
			 {
				 this->destroy = false;

				 if(king==1){//子弹1
					 ccbreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(enemyBullet::goEnd)); 
				 }
				 ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("down");
			 }
			 break;
		 case moveLeft:
			 {
				 this->destroy = false;

				 if(king==1){//子弹1
					 ccbreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(enemyBullet::goEnd)); 
				 }
				 ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("left");

			 }
			 break;
		 case moveRight:
			 {
				 this->destroy = false;

				 if(king==1){//子弹1
					 ccbreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(enemyBullet::goEnd)); 
				 }

				 ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("right");
			 }
			 break;
	   case EXPLODE:
		   {
			  //ccbreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(enemyBullet::goEnd)); 
		     ccbreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(enemyBullet::goEnd)); 
			 ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("burst");//burst
			 //this->destroy = true;
		   }
		   break;
	   case END:
		   {
			  
		   }
		   break;
	}
}
int  enemyBullet::getState(){

	return bulletState;
}

void enemyBullet::goEnd(){
  if(bulletState==EXPLODE){
	  setState(END);
	  this->destroy = true;
	 // this->setPosition(ccp(-50,-50));
  }
}
int  enemyBullet::getBulletSpeed(){
	return bulletSpeed;
}
int   enemyBullet::getBulletAttack(){
	//int attack = bulletAttack;//
	critRateBool = randomBool(critRate);
	CCLog("enemyBulletbulletAttack:%d",bulletAttack);

	int attack = (bulletAttack+(critRateBool?bulletAttack:0))*random(8,12)/10.0f;//几率内双倍伤害
   CCLog("enemyBulletattack:%d",attack);
	return attack;
}
int   enemyBullet::getBullStrenght()
{
	return bulletStrength;
}

bool  enemyBullet::getCritRateBool(){

	return critRateBool;

}