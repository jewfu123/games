#include "GiftSprite.h"
#include "GameResource.h"
#include "PlaneSprite.h"
#include "GameScene.h"

GiftSprite *GiftSprite::create(int _type, CCPoint point){
	GiftSprite* pRet = new GiftSprite();
	_type%=3;
	if(pRet && pRet->initWithSpriteFrameName(CCString::createWithFormat(s_gift_d, _type)->getCString())){
		pRet->autorelease();
		pRet->init(_type, point);
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	
	return NULL;
}

void GiftSprite::init(int _type, CCPoint point){
	type=_type;
	this->setPosition(point);
	this->scheduleUpdate();
}
void GiftSprite::update(float time){
	if(GameScene::sharedWorld()->getPlayer()){
		if(this->boundingBox().intersectsRect(GameScene::sharedWorld()->getPlayer()->boundingBox())){
			if(type==0){
				GameScene::sharedWorld()->addCoin(10);
			}else if(type==1){
				GameScene::sharedWorld()->addBomb();
			}
			this->removeFromParentAndCleanup(true);
		}else{
			float speed=5;
			float playerX=GameScene::sharedWorld()->getPlayer()->getPositionX();
			float playerY=GameScene::sharedWorld()->getPlayer()->getPositionY();
			if(ccpDistance(this->getPosition(), ccp(playerX, playerY))<200){
				speed=20;
			}
			double deltax = playerX - this->getPositionX();
			double deltay = playerY - this->getPositionY(); 
			if( deltax == 0 ){
				if(playerY >= this->getPositionY())        
					deltax = 0.0000001;
				else                                 
					deltax = -0.0000001;
			}
			if( deltay == 0 ){
				if( playerX >= this->getPositionX() )             
					deltay = 0.0000001;
				else                                
					deltay = -0.0000001;
			}
			double angle=0;
			if( deltax>0 && deltay>0){
				angle = atan(fabs(deltay/deltax)); 
			}else if( deltax<0 && deltay>0 ){
				angle = 3.1415926-atan(fabs(deltay/deltax));
			}else if( deltax<0 && deltay<0 ){
				angle = 3.1415926+atan(fabs(deltay/deltax));
			}else{
				angle = 2*3.1415926-atan(fabs(deltay/deltax));
			}
			//this->setRotation(this->getRotation()+angle*r);
			this->setPosition(ccpAdd(this->getPosition(), ccp(speed*cos(angle), speed*sin(angle))));
		}
	}
}