#include "EnemyLayer.h"
#include "EnemySprite.h"
#include "GameResource.h"
#include "GameScene.h"
#include "GiftSprite.h"

bool EnemyLayer::init(){
	if(!CCLayer::init()){
		return false;
	}
	level=1;
	bossCount=1;
	CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage(s_game_ui_img);
	batchNode = CCSpriteBatchNode::createWithTexture(texture, 8*8);
	addChild(batchNode);
	schedule(schedule_selector(EnemyLayer::autoCreateEnemy), 2);
	return true;
}
void EnemyLayer::autoCreateEnemy(float dt){
	int randomCount=CCRANDOM_0_1()*(4+(level-1)*2);
	for (int i =0; i<randomCount; i++) {
		int randomType=0;
		int random=CCRANDOM_0_1()*20;
		if(random>=0 && random<=14){
			randomType=0;
		}else if(random>=15 && random<=18){
			randomType=1;
		}else if(random>=19 && random<=20){
			randomType=2;
		}
		EnemySprite* enemy=EnemySprite::createEnemy(randomType, false);
		batchNode->addChild(enemy);
		GameScene::sharedWorld()->getEnemys()->addObject(enemy);
	}
}
void EnemyLayer::startCreateEnemy(){
	schedule(schedule_selector(EnemyLayer::autoCreateEnemy), 2);
}
void EnemyLayer::stopCreatEnemy(){
	unschedule(schedule_selector(EnemyLayer::autoCreateEnemy));
}
void EnemyLayer::removeEnemy(EnemySprite *_enemy){
	if(_enemy->getIsBoss()){
		if(_enemy->getType()==9){
			batchNode->addChild(GiftSprite::create(1, _enemy->getPosition()));
			batchNode->addChild(GiftSprite::create(0, ccpAdd(_enemy->getPosition(), ccp(0, 60))));
			batchNode->addChild(GiftSprite::create(0, ccpAdd(_enemy->getPosition(), ccp(60, 0))));
			batchNode->addChild(GiftSprite::create(0, ccpAdd(_enemy->getPosition(), ccp(0, -60))));
			batchNode->addChild(GiftSprite::create(0, ccpAdd(_enemy->getPosition(), ccp(-60, 0))));
			startCreateEnemy();
			batchNode->removeChild(_enemy, true);
		}else if(_enemy->getType()==10){
			batchNode->removeChild(_enemy, true);
			GameScene::sharedWorld()->showSucceed();
		}
		
	}else{
		int r=CCRANDOM_0_1()*50;
		if(r>=49){
			batchNode->addChild(GiftSprite::create(1, _enemy->getPosition()));
		}else if(r>=30){
			batchNode->addChild(GiftSprite::create(0, _enemy->getPosition()));
		}
		batchNode->removeChild(_enemy, true);
	}

}
void EnemyLayer::onExit(){
	unschedule(schedule_selector(EnemyLayer::autoCreateEnemy));
	CCLayer::onExit();
}
void EnemyLayer::goBoss(int type){
	EnemySprite* enemy=EnemySprite::createEnemy(9+type, true);
	batchNode->addChild(enemy);
	GameScene::sharedWorld()->getEnemys()->addObject(enemy);
}