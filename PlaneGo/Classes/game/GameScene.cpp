#include "GameScene.h"
#include "MapLayer.h"
#include "EnemyLayer.h"
#include "PlayerLayer.h"
#include "PlaneSprite.h"
#include "EnemySprite.h"
#include "GameMenuLayer.h"
#include "GameSettingLayer.h"
#include "GameDialogLayer.h"
#include "StringUtil.h"
#include "GameResource.h"
#include "AudioUtil.h"
#include "KeyId.h"
#include "../plane/PlaneScene.h"
#include "../select/SelectScene.h"
#include "JniUtil.h"


static GameScene *sh;

GameScene *GameScene::sharedWorld(){
	if(sh!=NULL){
		return sh;
	}
	return NULL;
}

GameScene *GameScene::create(int _num, int _type, int _level){
	GameScene *pRet =new GameScene();
	if (pRet && pRet->init(_num, _type, _level)){  
		pRet->autorelease(); 
		return pRet;
	} else { 
		delete pRet; 
		pRet = NULL; 
		return NULL; 
	} 
}
bool GameScene::init(int _num, int _type, int _level){
	if(!CCScene::init()){
		return false;
	}

	sh=this;
	num=_num;
	type=_type;
	level=_level;
	life=3;
	enemys=CCArray::create();
	CC_SAFE_RETAIN(enemys);
    mapLayer=MapLayer::create();
	this->addChild(mapLayer);
	enemyLayer=EnemyLayer::create();
	this->addChild(enemyLayer);
	playerLayer=PlayerLayer::create();
	this->addChild(playerLayer);
	menuLayer=GameMenuLayer::create();
	this->addChild(menuLayer, 10);
	return true;
}
bool GameScene::killEnemy(EnemySprite *_enemy, int value){
	if(_enemy->attackEnemy(value)){

		CCParticleSystemQuad * particle = CCParticleSystemQuad::create(s_particle_boom);
		particle->setPosition(_enemy->getPosition());
		particle->setAutoRemoveOnFinish(true);
		this->addChild(particle);


		AudioUtil::soundBomb(0);
		//menuLayer->setCoinValue(menuLayer->getCoinValue()+_enemy->getCoin());
		menuLayer->setScoreValue(menuLayer->getScoreValue()+_enemy->getScore());
		enemys->removeObject(_enemy);
		enemyLayer->removeEnemy(_enemy);
		return true;
	}
	return false;
}
void GameScene::addCoin(int coin){
	AudioUtil::soundGift();
	menuLayer->setCoinValue(menuLayer->getCoinValue()+coin);
}
void GameScene::addBomb(){
	AudioUtil::soundGift();
	menuLayer->addBomb(1);
}
void GameScene::showSetting(){
	this->addChild(GameSettingLayer::create(), 11);
}
void GameScene::showSucceed(){
	JniUtil::sendStringJNI("ad_popup");
	this->removeChild(enemyLayer, true);
	int n=CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_NUM);
	if(n<num+1){
		CCUserDefault::sharedUserDefault()->setIntegerForKey(KEY_NUM, num+1);
	}
	GameDialogLayer *dialogLayer=GameDialogLayer::create(1
		, menuLayer->getCoinValue(), menuLayer->getScoreValue());
	this->addChild(dialogLayer, 11);
}
void GameScene::showFail(){
	JniUtil::sendStringJNI("ad_popup");
	GameDialogLayer *dialogLayer=GameDialogLayer::create(0
		, menuLayer->getCoinValue(), menuLayer->getScoreValue());
	this->addChild(dialogLayer, 11);
}

void GameScene::goBoss(int type){
	enemyLayer->stopCreatEnemy();
	enemyLayer->goBoss(type);
}
void GameScene::gameBack(){
	CCDirector::sharedDirector()->replaceScene(PlaneScene::create(num));
}
void GameScene::gamePause(){
	getPlayer()->pauseSchedulerAndActions();
	menuLayer->pauseSchedulerAndActions();
}
void GameScene::gameResume(){
	getPlayer()->resumeSchedulerAndActions();
	menuLayer->resumeSchedulerAndActions();
}


void GameScene::toSelect(){
	CCDirector::sharedDirector()->replaceScene(SelectScene::create());
}
void GameScene::gameAgain(){
	CCDirector::sharedDirector()->replaceScene(GameScene::create(num, type, 1));
}
void GameScene::gameNext(){
	CCDirector::sharedDirector()->replaceScene(GameScene::create(num+1, type, 1));
}
PlaneSprite *GameScene::getPlayer(){
	return playerLayer->getPlayer();
}
void GameScene::killPlayer(){
	if(life>0){
		AudioUtil::soundBomb(0);
		life--;
		menuLayer->reduceLife();
		playerLayer->again();
	}else{
		life=0;
		playerLayer->again();
		showFail();
	}
}
void GameScene::reduceCoin(int _coin){
	menuLayer->updateCoin(menuLayer->getCoinValue()-_coin);
}
void GameScene::bomb(){
	AudioUtil::soundBomb(1);
	playerLayer->removerAllBullet();
	while(enemys->count()>0){
		EnemySprite *enemy=(EnemySprite *)enemys->objectAtIndex(0);
		bool isBoss=enemy->getIsBoss();
		if(isBoss){
			enemy->setLock(false);
			killEnemy(enemy, 2000);
			break;
		}else{
			killEnemy(enemy, 2000);
		}
	}
	float n=40;
	CCMoveBy* left1 = CCMoveBy::create(0.005f,ccp(n,0));
	CCMoveBy* right1 = CCMoveBy::create(0.005f,ccp(-n,0));
	CCMoveBy* top1 = CCMoveBy::create(0.005f,ccp(0,n));
	CCMoveBy* rom1 = CCMoveBy::create(0.005f,ccp(0,-n));
	CCFiniteTimeAction* action3 = CCSequence::create(left1,right1,top1,rom1,left1->reverse(),right1->reverse(),top1->reverse(),rom1->reverse(),NULL);
	CCRepeat *action = CCRepeat::create(action3,10);
	this->runAction(CCSequence::create(action, CCCallFunc::create(this, callfunc_selector(GameScene::resumePoint)), NULL));
}
void GameScene::upLevel(){
	level++;
	if(level>5){
		level=5;
	}else{
		AudioUtil::soundUpLevel();
		enemyLayer->setLevel(level);
		JniUtil::sendStringJNI(
			CCString::createWithFormat(StringUtil::getString("game_up_level"), level)->getCString());
		playerLayer->updateLevel();
	}
}
GameScene::~GameScene(){
	CC_SAFE_RELEASE(enemys);
}
void GameScene::resumePoint(){
	this->setPosition(ccp(0, 0));
}