#include "Bullet.h"
#include "Tool.h"

using namespace cocos2d::extension; 
using namespace cocos2d;


//int Bullet::bulletAttack=0;
int Bullet::weight=0;
Bullet::Bullet(void):bulletSpeed(0),
	                 bulletRange(0),
					 bulletStrength(0),
					 rateOfFire(0),
					 critRate(0),
					 bulletAttack(0),
					 penetrate(false),
					 critRateBool(false)
{
	
}


Bullet::~Bullet(void)
{

}

void Bullet::myInit(int turret,int turretGrade){
	this->turretKing = turret;
	this->turretGrade = turretGrade;

    char layerName[30];

    sprintf(layerName,"tanker/turret_%d_%d.xml", turret,turretGrade);
    XMLParser *  pXmlParser = XMLParser::parseWithFile(layerName);

	bulletSpeed = (pXmlParser->getString("bulletSpeed"))->intValue();
	bulletAttack = (int)(((pXmlParser->getString("bulletAttack"))->intValue()*(1.0+addAttackIndex*10/100.0))*(doubleAttack>0?2:1));

	bulletRange = (pXmlParser->getString("bulletRange"))->intValue();
	bulletStrength = (pXmlParser->getString("bulletStrength"))->intValue();
	rateOfFire = (pXmlParser->getString("rateOfFire"))->floatValue();
	critRate = (pXmlParser->getString("critRate"))->intValue()+addCritIndex*4;
	penetrate = (pXmlParser->getString("penetrate"))->boolValue();

	weight = (pXmlParser->getString("weight"))->intValue();

	CCLog("===bulletAttack:%d",bulletAttack);
	bulletState = END;
	this->destroy = false;
	
};

Bullet* Bullet::spriteCreate(int turret,int turretGrade){

	Bullet *pobSprite = new Bullet();       
	if (pobSprite && pobSprite->init())//备注1
	{        
		 
		 pobSprite->myInit(turret,turretGrade);        
		 pobSprite->autorelease(); 
		 return pobSprite;     
	 }     
	 CC_SAFE_DELETE(pobSprite); 
	 return NULL;
};
void Bullet::setBulletAction(){
	  char pszFileName[30];
	  sprintf(pszFileName,"tanker/bullet_%d.ccbi",turretKing);

	  CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	  ccbreader = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
	  
	 //读取一个ccbi的文件,生成一个ccnode实例
	 CCNode  *animationsTest  = ccbreader->readNodeGraphFromFile(pszFileName,this);//tank11.ccbi
	 //设置坐标 
	 //animationsTest->setPosition(ccp(pobSprite->getPositionX()-5,pobSprite->getPositionY()+60)); 

	 if(animationsTest  != NULL) {     
		 this->addChild(animationsTest,0,0);
	 }

	 //获取所有的动作序列 
	  CCArray* allSeq = ccbreader->getAnimationManager()->getSequences(); 
	  CCLog("============================Bullet") ;
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


void Bullet::setName(char *name){
	this->name = name;
}

const char* Bullet::getName(){
	return name;
}

void Bullet::setDestroy(bool destroy){
	this->destroy = destroy;
}

bool Bullet::getDestroy(){
	return destroy;
}

void Bullet::setLayerName(const char *layerName){
	this->layerName = layerName;
}

const char* Bullet::getLayerName(){
	return layerName;
}
int   Bullet::getBullStrenght()
{
   return bulletStrength;
}
void Bullet::setBulletAttackByLvUp(int lv){
//	bulletAttack=(bulletAttack+lv*30);
}

int   Bullet::getBulletAttack(){
	
    critRateBool = randomBool(critRate);

	CCLog("BulletbulletAttack:%d",bulletAttack);

	if(critTime.getSurplusMillis()>0){
       critRateBool = true;
	}

	int attack = (int)(bulletAttack+(critRateBool?bulletAttack:0))*random(8,12)/10;//几率内双倍伤害
	CCLog("Bulletattack:%d",attack);

	return attack;
}

bool  Bullet::getCritRateBool(){

	return critRateBool;

}

void Bullet::setUpCrit(){
	critTime.setMillis(15*1000);
	critTime.start();
}

int   Bullet::getWeight(){
	return weight;
};
float Bullet::getRateOfFire(){
	return rateOfFire;
}

void Bullet::setState(int state){
	bulletState = state;
	switch(bulletState){
	   case moveUp:
		   {
			   this->destroy = false;
			   ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("up");
			   if(turretKing==5){
				   ccbreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(Bullet::go5End)); 
			   }
		   }
		   break;
	   case moveDown:
		   {
			   this->destroy = false;
			   ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("down");
			   if(turretKing==5){
				   ccbreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(Bullet::go5End)); 
			   }
		   }
		   break;
	   case moveLeft:
		   {
			   this->destroy = false;
			   ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("left");//
			   if(turretKing==5){
				   ccbreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(Bullet::go5End)); 
			   }
		   }
		   break;
	   case moveRight:
		   {
			   
			   this->destroy = false;
			   ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("right");
			   if(turretKing==5){
				   ccbreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(Bullet::go5End)); 
			   }
		   }
		   break;
	   case EXPLODE:
		   {
			    ccbreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(Bullet::goEnd)); 
			    ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("burst");
				this->destroy = true;
		   }
		   break;
	   case END:
		   {
			 
             
		   }
		   break;
	}
}
int  Bullet::getState(){

	return bulletState;
}
void Bullet::goEnd(){
  if(bulletState==EXPLODE){
	  setState(END);
	  this->destroy = true;
	  this->setPosition(ccp(0,0));
	  this->setVisible(false);
  }
}
void Bullet::go5End(){
	setState(EXPLODE);
}
int  Bullet::getBulletSpeed(){
	return bulletSpeed;
}