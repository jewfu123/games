#include "PBulletSprite2.h"
#include "GameResource.h"
#include "EnemySprite.h"
#include "GameScene.h"
#include <cmath>

PBulletSprite2 *PBulletSprite2::createBullet(CCPoint point, EnemySprite *_enemy){
	PBulletSprite2* bullet = new PBulletSprite2();
	if(bullet && bullet->initWithSpriteFrameName(s_bullet2)){
		bullet->autorelease();
		bullet->initBullet(point, _enemy);
		return bullet;
	} 
	CC_SAFE_DELETE(bullet);
	return NULL;
}
void PBulletSprite2::initBullet(CCPoint point, EnemySprite *_enemy){
	radius=500;
	isFirst=true;
	enemy=_enemy;
	speed=1;
	location=point;
	this->setPosition(point);
	//this->setAnchorPoint(ccp(0.5f, 0));


	scheduleUpdate();
}
void PBulletSprite2::removeOneself(){
	this->removeFromParentAndCleanup(true);
}
void PBulletSprite2::update(float dt){
	CCSize size=CCDirector::sharedDirector()->getWinSize();
	double speed=12;

	if(enemy && enemy->isRunning()){
		if(!enemy->boundingBox().intersectsRect(this->boundingBox())){
				double r=(enemy->getPositionX()-location.x)/320;
				if(r>1){
					r=1;
				}
			double deltax = enemy->getPositionX() - this->getPositionX();
			double deltay = enemy->getPositionY() - this->getPositionY(); 
			if( deltax == 0 ){
				if(enemy->getPositionY() >= this->getPositionY())        
					deltax = 0.0000001;
				else                                 
					deltax = -0.0000001;
			}
			if( deltay == 0 ){
				if( enemy->getPositionX() >= this->getPositionX() )             
					deltay = 0.0000001;
				else                                
					deltay = -0.0000001;
			}
			double angle=0;
			if( deltax>0 && deltay>0){
				angle = atan(fabs(deltay/deltax)); 
				//this->setRotation(this->getRotation()+angle*r);
			}else if( deltax<0 && deltay>0 ){
				angle = 3.1415926-atan(fabs(deltay/deltax));
				//this->setRotation(this->getRotation()-angle*r);
				
			}else if( deltax<0 && deltay<0 ){
				angle = 3.1415926+atan(fabs(deltay/deltax));
				//this->setRotation(this->getRotation()-angle*r);
			}else{
				angle = 2*3.1415926-atan(fabs(deltay/deltax));
				//this->setRotation(this->getRotation()+angle*r);
			}
			//this->setRotation(this->getRotation()+angle);

			CCPoint temp=ccpAdd(this->getPosition(), ccp(speed*cos(angle), speed*sin(angle)));
			angle = atan((this->getPositionX()- temp.x)/(this->getPositionY()-temp.y));

			this->setRotation(this->getRotation()+angle*2);
			this->setPosition(temp);
		}else{
			enemy->setLock(false);
			if(enemy->getIsBoss()){
				GameScene::sharedWorld()->killEnemy(enemy, 500);
			}else{
				GameScene::sharedWorld()->killEnemy(enemy, 1500);
			}
			
			this->removeFromParentAndCleanup(true);
		}
	}else{
		enemy=NULL;
		if(size.height>this->getPositionY() || this->getPositionY()>0){
			this->setRotation(0);
			if(this->isFlipY()){
				this->setPosition(ccpAdd(this->getPosition(), ccp(0, -speed)));
			}else{
				this->setPosition(ccpAdd(this->getPosition(), ccp(0, speed)));
			}
			CCArray * array = GameScene::sharedWorld()->getEnemys();
			for (unsigned int i =0; i<array->count(); i++) {
				EnemySprite* e = (EnemySprite*)array->objectAtIndex(i);
				if(e->boundingBox().intersectsRect(this->boundingBox())){
					e->setLock(false);
					if(enemy->getIsBoss()){
						GameScene::sharedWorld()->killEnemy(enemy, 500);
					}else{
						GameScene::sharedWorld()->killEnemy(enemy, 1500);
					}
					this->removeFromParentAndCleanup(true);
					break;
				}
			}
		}else{
			this->removeFromParentAndCleanup(true);
		}
	}
}
void PBulletSprite2::onExit(){
	unscheduleUpdate();
	CCSprite::onExit();
}