#include "MapLayer.h"
#include "GameResource.h"
#include "GameScene.h"


enum{
	idImg0, idImg1
};

bool MapLayer::init(){
	if(!CCLayer::init()){
		return false;
	}
	int num=GameScene::sharedWorld()->getNum();
	CCSize size=CCDirector::sharedDirector()->getWinSize();

	//speed=2;
	CCSprite * turnImg =CCSprite::create(CCString::createWithFormat(s_map_d, num%20)->getCString());
	turnImg->setPosition(ccp(turnImg->getContentSize().width*0.5,turnImg->getContentSize().height*0.5));
	this->addChild(turnImg,0,idImg0);

	CCSprite * turnImg2 =CCSprite::create(CCString::createWithFormat(s_map_d, num%20)->getCString());
	turnImg2->setPosition(ccp(turnImg2->getContentSize().width*0.5,turnImg2->getContentSize().height*1.5));
	this->addChild(turnImg2,0,idImg1);

	this->scheduleUpdate();
	return true;
}
void MapLayer::update(float dt){
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCSprite* sp1 = (CCSprite*)this->getChildByTag(idImg0);

	//for(int i=0; i<speed; i++){
		if(sp1->getPositionY()<=-sp1->getContentSize().height*0.5){
			sp1->setPosition(ccp(sp1->getContentSize().width*0.5,sp1->getContentSize().height*1.5f-1));
		}else{
			sp1->setPosition(ccpAdd(sp1->getPosition(), ccp(0,-1)));
		}

		CCSprite* sp2 = (CCSprite*)this->getChildByTag(idImg1);
		if(sp2->getPositionY()<=-sp2->getContentSize().height*0.5){
			sp2->setPosition(ccp(sp2->getContentSize().width*0.5,sp2->getContentSize().height*1.5f-1));
		}else{
			sp2->setPosition(ccpAdd(sp2->getPosition(), ccp(0,-1)));
		}
	//}
}
void MapLayer::onExit(){
	this->unscheduleUpdate();
	CCLayer::onExit();
}
