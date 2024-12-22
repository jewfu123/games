#include "PlaneSprite.h"
#include "GameResource.h"

PlaneSprite* PlaneSprite::createPlane(int _type, int _level){
	PlaneSprite* plane = new PlaneSprite();
	_type%=6;
	if(plane && plane->initWithSpriteFrameName(CCString::createWithFormat(s_player_d, _type)->getCString())){
		plane->autorelease();
		plane->planeInit(_type, _level);
		return plane;
	}
	CC_SAFE_DELETE(plane);
	return NULL;
}

void PlaneSprite::planeInit(int _type, int _level){
	type=_type;
	level=_level;
	isAlive=false;

	this->runAction(CCSequence::create(CCBlink::create(3, 30)
		, CCCallFunc::create(this, callfunc_selector(PlaneSprite::setAlive)), NULL));
	if(_type==4){
		CCSprite *bottomAnim=CCSprite::createWithSpriteFrameName(CCString::createWithFormat(s_player_bottom_d, 0)->getCString());
		bottomAnim->setPosition(ccp(this->getContentSize().width*0.5f, -18));
		this->addChild(bottomAnim);
		CCArray* animFrames = CCArray::createWithCapacity(3);
		for(int i = 0; i < 3; i++) {
			CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
				CCString::createWithFormat(s_player_bottom_d, i)->getCString());
			animFrames->addObject(frame);
		} 
		CCAnimation *animation = CCAnimation::createWithSpriteFrames(animFrames, 0.1f);
		animation->setLoops(-1);
		bottomAnim->runAction(CCAnimate::create(animation));
		
	}else{
		for(int i=0; i<2; i++){
			CCSprite *bottomAnim=CCSprite::createWithSpriteFrameName(CCString::createWithFormat(s_player_bottom_d, 0)->getCString());
			bottomAnim->setPosition(ccp(this->getContentSize().width*0.5f-12+i*24, -18));
			this->addChild(bottomAnim);
			CCArray* animFrames = CCArray::createWithCapacity(3);
			for(int i = 0; i < 3; i++) {
				CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(
					CCString::createWithFormat(s_player_bottom_d, i)->getCString());
				animFrames->addObject(frame);
			} 
			CCAnimation *animation = CCAnimation::createWithSpriteFrames(animFrames, 0.1f);
			animation->setLoops(-1);
			bottomAnim->runAction(CCAnimate::create(animation));
		}
	}
	
}
void PlaneSprite::setAlive(){
	isAlive=true;
}
