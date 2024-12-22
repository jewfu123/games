#include "PBulletSprite1.h"
#include "GameResource.h"
#include "GameScene.h"
#include "PlaneSprite.h"
#include <cmath>

PBulletSprite1 *PBulletSprite1::createBullet(int _state, CCPoint point, float r){
	PBulletSprite1* bullet = new PBulletSprite1();
	const char *name=NULL;
	if(bullet && bullet->initWithSpriteFrameName(s_enemy_bullet_0)){
		bullet->autorelease();
		bullet->initBullet(_state, point, r);
		return bullet;
	}
	CC_SAFE_DELETE(bullet);
	return NULL;
}
void PBulletSprite1::initBullet(int _state, CCPoint point, float r){
	radius=500;
	speed=1;
	state=_state;
	this->setPosition(point);
	float x=std::sin(r)*radius;
	float y=std::cos(r)*radius;
	this->runAction(CCSequence::create(CCMoveTo::create(3, ccpAdd(point, ccp(x, y)))
		, CCCallFunc::create(this, callfunc_selector(PBulletSprite1::removeOneself)), NULL));
	scheduleUpdate();
}
void PBulletSprite1::removeOneself(){
	this->removeFromParentAndCleanup(true);
}
void PBulletSprite1::update(float dt){
	if(state!=0){
		for(int i=0; i<speed; i++){
			this->setPosition(ccpAdd(this->getPosition(), ccp(0,-3)));
		}
	}
	PlaneSprite *player=GameScene::sharedWorld()->getPlayer();
	if(player){
		float x=player->getPositionX();
		float y=player->getPositionY();
		float w=player->getContentSize().width/2;
		float h=player->getContentSize().height/2;
		if(player->getIsAlive()
			&& this->boundingBox().intersectsRect(CCRectMake(x-w/2, y-h/2, w, h))){
				GameScene::sharedWorld()->killPlayer();
				this->removeFromParentAndCleanup(true);
		}
	}
}
void PBulletSprite1::onExit(){
	unscheduleUpdate();
	CCSprite::onExit();
}