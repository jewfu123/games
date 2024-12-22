#include "PBulletSprite0.h"
#include "GameResource.h"
#include "GameScene.h"
#include "EnemySprite.h"
#include "PBulletSprite1.h"
#include <cmath>


PBulletSprite0* PBulletSprite0::createBullet(int _state, CCPoint _position, float _rotation, float _scale){
	PBulletSprite0* bullet = new PBulletSprite0();
	const char *name;
	if(_state==2){
		name=s_bullet1;
	}else{
		name=CCString::createWithFormat(s_bullet0_d, _state)->getCString();
	}
	if(bullet && bullet->initWithSpriteFrameName(name)){
		bullet->autorelease();
		bullet->bulletInit(_state, _position, _rotation, _scale);
		return bullet;
	}
	CC_SAFE_DELETE(bullet);
	return NULL;
}
void PBulletSprite0::bulletInit(int _state,CCPoint _position, float _roation, float _scale){
	state=_state;
	speed = 10;
	
	roation=_roation*3.14f/180;
	this->setRotation(roation);
	scale=_scale;
	/*if(state!=2){
		this->setScale(0.5f+scale);
	}*/
	this->setPosition(_position);
	this->scheduleUpdate();
}
void PBulletSprite0::update(float time){
	CCSize size=CCDirector::sharedDirector()->getWinSize();
	if(this->getPositionY()>size.height+this->getContentSize().height*0.5f
		|| this->getPositionX()>size.width || this->getPositionX()<0){
		this->removeFromParentAndCleanup(true);
	}else{
		this->setPosition(ccpAdd(this->getPosition(), ccp(speed*sin(roation), speed*cos(roation))));
		if(state==2){
			this->setScale(this->getScale()+0.04f);
		}
		CCArray * array = GameScene::sharedWorld()->getEnemys();
		for (unsigned int i =0; i<array->count(); i++) {
			EnemySprite* enemy = (EnemySprite*)array->objectAtIndex(i);
			if(enemy->boundingBox().intersectsRect(this->boundingBox())){
				if(state!=2){
					GameScene::sharedWorld()->killEnemy(enemy, 100*scale);
				}else{
					GameScene::sharedWorld()->killEnemy(enemy, 200);
				}
				this->removeFromParentAndCleanup(true);
				break;
			}
		}
	}
}
void PBulletSprite0::onExit(){
	this->unscheduleUpdate();
	CCSprite::onExit();
}