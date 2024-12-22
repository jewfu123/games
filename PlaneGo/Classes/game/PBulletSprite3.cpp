#include "PBulletSprite3.h"
#include "GameScene.h"
#include "GameResource.h"
#include "EnemySprite.h"

enum{
	topId
};

PBulletSprite3 *PBulletSprite3::createBullet(int _state, CCPoint point, int _level){
	PBulletSprite3* bullet = new PBulletSprite3();
	if(bullet && bullet->initWithSpriteFrameName(CCString::createWithFormat(s_bullet3_d, _state)->getCString())){
		bullet->autorelease();
		bullet->bulletInit(_state, point, _level);
		return bullet;
	}
	CC_SAFE_DELETE(bullet);
	return NULL;
}
void PBulletSprite3::bulletInit(int _state, CCPoint point, int _level){
	state=_state;
	level=_level;
	this->setAnchorPoint(ccp(0.5f, 0));
	this->setPosition(point);
	scheduleUpdate();
	this->setPreferredSize(CCSizeMake(20+4*level, 960));

	const char *name;
	if(_state==0){
		name=s_read_top_d;
	}else{
		name=s_blue_top_d;
	}
	CCSprite *topAnim=CCSprite::createWithSpriteFrameName(CCString::createWithFormat(name, 0)->getCString());
	topAnim->setScale(0.9f+level*0.1f);
	topAnim->setPosition(ccp(this->getContentSize().width*0.5f, this->getPreferredSize().height));
	this->addChild(topAnim, 0, topId);
	CCArray* animFrames = CCArray::createWithCapacity(3);
	for(int i = 0; i < 3; i++) {
		CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
			CCString::createWithFormat(name, i)->getCString());
		animFrames->addObject(frame);
	} 
	CCAnimation *animation = CCAnimation::createWithSpriteFrames(animFrames, 0.08f);
	animation->setLoops(-1);
	topAnim->runAction(CCAnimate::create(animation));


	if(_state==0){
		name=s_read_bottom_d;
	}else{
		name=s_blue_bottom_d;
	}
	CCSprite *bottomAnim=CCSprite::createWithSpriteFrameName(CCString::createWithFormat(name, 0)->getCString());
	bottomAnim->setScale(0.9f+level*0.1f);
	bottomAnim->setPosition(ccp(this->getContentSize().width*0.5f, 0));
	this->addChild(bottomAnim);
	animFrames = CCArray::createWithCapacity(3);
	for(int i = 0; i < 3; i++) {
		CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
			CCString::createWithFormat(name, i)->getCString());
		animFrames->addObject(frame);
	} 
	animation = CCAnimation::createWithSpriteFrames(animFrames, 0.08f);
	animation->setLoops(-1);
	bottomAnim->runAction(CCAnimate::create(animation));

}
void PBulletSprite3::update(float time){
	CCArray * array = GameScene::sharedWorld()->getEnemys();
	EnemySprite *tempEnemy=NULL;
	float temp=1000;
	for (unsigned int i =0; i<array->count(); i++) {
		EnemySprite* enemy = (EnemySprite*)array->objectAtIndex(i);
		if(enemy->boundingBox().intersectsRect(this->boundingBox())){
			float d=ccpDistance(this->getPosition(), enemy->getPosition());
			if(temp>d){
				temp=d;
				tempEnemy=enemy;
			}
		}
	}
	if(tempEnemy){
		this->setPreferredSize(CCSizeMake(20+4*level, ccpDistance(this->getPosition(), tempEnemy->getPosition())));
		CCSprite *topAnim=(CCSprite *)this->getChildByTag(topId);
		topAnim->setPosition(ccp((20+4*level)*0.5f, ccpDistance(this->getPosition(), tempEnemy->getPosition())));
		if(state==1){
			tempEnemy->setStop(90);
			GameScene::sharedWorld()->killEnemy(tempEnemy, 12+2*level);
		}else{
			GameScene::sharedWorld()->killEnemy(tempEnemy, 14+2*level);
		}
		
	}else{
		if(this->getPreferredSize().height!=960){
			this->setPreferredSize(CCSizeMake(20+4*level, 960));
			CCSprite *topAnim=(CCSprite *)this->getChildByTag(topId);
			topAnim->setPosition(ccp((20+4*level)*0.5f, 960));
		}
	}
}
void PBulletSprite3::onExit(){
	unscheduleUpdate();
	CCScale9Sprite::onExit();
}