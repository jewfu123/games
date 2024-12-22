#include "PlayerLayer.h"
#include "GameResource.h"
#include "PBulletSprite0.h"
#include "PBulletSprite1.h"
#include "PBulletSprite2.h"
#include "PBulletSprite3.h"
#include "GameScene.h"
#include "EnemySprite.h"
#include "PlaneSprite.h"


enum{
      bullet2Id0, bullet2Id1, bullet2Id2
};


bool PlayerLayer::init(){
	if(!CCLayer::init()){
		return false;
	}
	player=NULL;
	int type=GameScene::sharedWorld()->getType();
	int level=GameScene::sharedWorld()->getLevel();
	ddTime=3;
	CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage(s_game_ui_img);
	batchNode = CCSpriteBatchNode::createWithTexture(texture, 8*8);
	addChild(batchNode);

	return true;
}
void PlayerLayer::autoCreateBullet(float dt){
	int type=GameScene::sharedWorld()->getType();
	int level=GameScene::sharedWorld()->getLevel();
	if(type==0){
		switch(level){
		case 1:
			batchNode->addChild(PBulletSprite0::createBullet(1, player->getPosition(),0, 1));
			break;
		case 2:
			batchNode->addChild(PBulletSprite0::createBullet(1, ccpAdd(player->getPosition(), ccp(-20, -10)),0, 0.75f));
			batchNode->addChild(PBulletSprite0::createBullet(1, player->getPosition(),0, 1));
			batchNode->addChild(PBulletSprite0::createBullet(1, ccpAdd(player->getPosition(), ccp(20, -10)),0, 0.75f));
			break;
		case 3:
			batchNode->addChild(PBulletSprite0::createBullet(1, ccpAdd(player->getPosition(), ccp(-40, -20)),0, 0.5f));
			batchNode->addChild(PBulletSprite0::createBullet(1, ccpAdd(player->getPosition(), ccp(-20, -10)),0, 0.75f));
			batchNode->addChild(PBulletSprite0::createBullet(1, player->getPosition(),0, 1));
			batchNode->addChild(PBulletSprite0::createBullet(1, ccpAdd(player->getPosition(), ccp(20, -10)),0, 0.75f));
			batchNode->addChild(PBulletSprite0::createBullet(1, ccpAdd(player->getPosition(), ccp(40, -20)),0, 0.5f));
			break;
		case 4:
			batchNode->addChild(PBulletSprite0::createBullet(1, ccpAdd(player->getPosition(), ccp(-60, -30)),0, 0.4f));
			batchNode->addChild(PBulletSprite0::createBullet(1, ccpAdd(player->getPosition(), ccp(-40, -20)),0, 0.6f));
			batchNode->addChild(PBulletSprite0::createBullet(1, ccpAdd(player->getPosition(), ccp(-20, -10)),0, 0.8f));
			batchNode->addChild(PBulletSprite0::createBullet(1, player->getPosition(),0, 1));
			batchNode->addChild(PBulletSprite0::createBullet(1, ccpAdd(player->getPosition(), ccp(20, -10)),0, 0.8f));
			batchNode->addChild(PBulletSprite0::createBullet(1, ccpAdd(player->getPosition(), ccp(40, -20)),0, 0.6f));
			batchNode->addChild(PBulletSprite0::createBullet(1, ccpAdd(player->getPosition(), ccp(60, -30)),0, 0.4f));
			break;
		case 5:
			for(int i=0; i<3; i++){
				float r=i*10-10;
				if(i==1){
					batchNode->addChild(PBulletSprite0::createBullet(1, ccpAdd(player->getPosition(), ccp(-40, -20)),r, 0.5f));
					batchNode->addChild(PBulletSprite0::createBullet(1, ccpAdd(player->getPosition(), ccp(40, -20)),r, 0.5f));
				}
				batchNode->addChild(PBulletSprite0::createBullet(1, ccpAdd(player->getPosition(), ccp(-20, -10)),r, 0.75f));
				batchNode->addChild(PBulletSprite0::createBullet(1, player->getPosition(),r, 1));
				batchNode->addChild(PBulletSprite0::createBullet(1, ccpAdd(player->getPosition(), ccp(20, -10)),r, 0.75f));
			}
			break;
		}
	}else if(type==1){
		if(level==5){
			batchNode->addChild(PBulletSprite0::createBullet(1, player->getPosition(),-20, 0.5f));
			batchNode->addChild(PBulletSprite0::createBullet(1, player->getPosition(),-10, 0.5f));
			batchNode->addChild(PBulletSprite0::createBullet(1, player->getPosition(),10, 0.5f));
			batchNode->addChild(PBulletSprite0::createBullet(1, player->getPosition(), 20, 0.5f));
		}
	}else if(type==2){
		int count=1+(level-1)*2;
		for(int i=0; i<count; i++){
			batchNode->addChild(PBulletSprite0::createBullet(2, player->getPosition(), 25*(count)/2-i*25-25*0.5f, 1));
		}
	}else if(type==3){
		int count=(level-1)*2+1;
		for(int i=0; i<count; i++){
			batchNode->addChild(PBulletSprite0::createBullet(0, player->getPosition(), 5*count/2-i*5-5*0.5f, 1));
		}
	}else if(type==4){
		if(level==5){
			batchNode->addChild(PBulletSprite0::createBullet(0, player->getPosition(),-20, 1));
			batchNode->addChild(PBulletSprite0::createBullet(0, player->getPosition(),-10, 1));
			batchNode->addChild(PBulletSprite0::createBullet(0, player->getPosition(),10, 1));
			batchNode->addChild(PBulletSprite0::createBullet(0, player->getPosition(), 20, 1));
		}
	}else if(type==5){
		CCSize size=CCDirector::sharedDirector()->getWinSize();
		for(int i=0; i<level+1; i++){
			if(GameScene::sharedWorld()&&GameScene::sharedWorld()->isRunning()){
				CCArray *array=GameScene::sharedWorld()->getEnemys();
				if(array && array->count()>0){
					if(ddTime<=0){
						CCObject *object=NULL;
						CCARRAY_FOREACH(array, object){
							EnemySprite *enemy=(EnemySprite *)object;
							//if(!enemy->getLock() && enemy->getPositionY()>player->getPositionY()+50 && enemy->getPositionY()<size.height){
							float eY = enemy->getPositionY();
							if(!enemy->getLock() && eY<size.height && ccpDistance(enemy->getPosition(), player->getPosition())>200){
								enemy->setLock(true);
								ddTime=3;
								PBulletSprite2 *bullet = PBulletSprite2::createBullet(player->getPosition(), enemy);
								if(eY< player->getPositionY()){
									bullet->setFlipY(true);
								}
								batchNode->addChild(bullet);
								//array->removeObject(enemy);
								break;
							}
						}
					}else{
						ddTime--;
					}
				}
			}
			
		}
	}
}
void PlayerLayer::removeBullet(PBulletSprite0 *_bullet){
	batchNode->removeChild(_bullet, true);
}
void PlayerLayer::onEnter(){
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, true);
	CCLayer::onEnter();
}
void PlayerLayer::onEnterTransitionDidFinish(){
	int type=GameScene::sharedWorld()->getType();
	int level=GameScene::sharedWorld()->getLevel();

	CCSize size=CCDirector::sharedDirector()->getWinSize();
	CCPoint oPoint=CCDirector::sharedDirector()->getVisibleOrigin();
	player=PlaneSprite::createPlane(type, level);
	player->setPosition(ccp(size.width*0.5f, -player->getContentSize().height));

	this->addChild(player, 1);
	player->runAction(CCSequence::create(
		CCMoveTo::create(0.8f, ccp(size.width*0.5f, oPoint.y+player->getContentSize().height*0.5f+120))
		, CCCallFuncN::create(this, callfuncN_selector(PlayerLayer::setShoot))
		, NULL));
	CCLayer::onEnterTransitionDidFinish();
}
void PlayerLayer::again(){
	CCArray *array=GameScene::sharedWorld()->getEnemys();
	CCObject *object=NULL;
	CCARRAY_FOREACH(array, object){
		EnemySprite *enemy=(EnemySprite *)object;
		enemy->setLock(false);
	}
	CCParticleSystemQuad * particle = CCParticleSystemQuad::create(s_particle_boom);
	particle->setPosition(player->getPosition());
	particle->setAutoRemoveOnFinish(true);
	GameScene::sharedWorld()->addChild(particle);
	player=NULL;

	unschedule(schedule_selector(PlayerLayer::autoCreateBullet));
	this->removeAllChildrenWithCleanup(true);
	ddTime=3;
	CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage(s_game_ui_img);
	batchNode = CCSpriteBatchNode::createWithTexture(texture, 8*8);
	addChild(batchNode);

	int type=GameScene::sharedWorld()->getType();
	int level=GameScene::sharedWorld()->getLevel();

	CCSize size=CCDirector::sharedDirector()->getWinSize();
	CCPoint oPoint=CCDirector::sharedDirector()->getVisibleOrigin();
	player=PlaneSprite::createPlane(type, level);
	player->setPosition(ccp(size.width*0.5f, -player->getContentSize().height));

	this->addChild(player,1);
	player->runAction(CCSequence::create(
		CCMoveTo::create(0.8f, ccp(size.width*0.5f, oPoint.y+player->getContentSize().height*0.5f+120))
		, CCCallFuncN::create(this, callfuncN_selector(PlayerLayer::setShoot))
		, NULL));
}

void PlayerLayer::onExit(){
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	unscheduleUpdate();
	unschedule(schedule_selector(PlayerLayer::autoCreateBullet));
	CCLayer::onExit();
}

bool PlayerLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
	return true;
}
void PlayerLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
	PBulletSprite3 *bullet=(PBulletSprite3 *)this->getChildByTag(bullet2Id0);
	PBulletSprite3 *bullet1=(PBulletSprite3 *)this->getChildByTag(bullet2Id1);
	PBulletSprite3 *bullet2=(PBulletSprite3 *)this->getChildByTag(bullet2Id2);
	CCPoint point=ccpAdd(player->getPosition(), pTouch->getDelta());
	CCSize size=CCDirector::sharedDirector()->getWinSize();
	CCPoint oPoint=CCDirector::sharedDirector()->getVisibleOrigin();

	if(point.x>oPoint.x+player->getContentSize().width*0.5f
		&& point.x<size.width-oPoint.x-player->getContentSize().width*0.5f
		&& point.y>oPoint.y+player->getContentSize().height*0.5f
		&& point.y<size.height-oPoint.y-player->getContentSize().height*0.5f){
		player->setPosition(point);
		if(bullet){
			bullet->setPosition(ccpAdd(point, ccp(0, player->getContentSize().height*0.5f)));
		}
		if(bullet1){
			bullet1->setPosition(ccpAdd(point, ccp(-50,  player->getContentSize().height*0.5f-50)));
		}
		if(bullet2){
			bullet2->setPosition(ccpAdd(point, ccp(50,  player->getContentSize().height*0.5f-50)));
		}
	}
}
void PlayerLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
}
void PlayerLayer::setShoot(CCNode *node){
	int type=GameScene::sharedWorld()->getType();
	int level=GameScene::sharedWorld()->getLevel();

	PlaneSprite *player=(PlaneSprite *)node;
	schedule(schedule_selector(PlayerLayer::autoCreateBullet), 0.2f);
	if(type==1 || type==4){
		int state=0;
		if(type==4){
			state=1;
		}
		switch(level){
		case 1:
			this->addChild(PBulletSprite3::createBullet(state, ccpAdd(player->getPosition(), ccp(0, player->getContentSize().height*0.5f)), 1), 0, bullet2Id0);
			break;
		case 2:
			this->addChild(PBulletSprite3::createBullet(state, ccpAdd(player->getPosition(), ccp(-50, player->getContentSize().height*0.5f-50)), 1), 0, bullet2Id1);
			this->addChild(PBulletSprite3::createBullet(state, ccpAdd(player->getPosition(), ccp(50, player->getContentSize().height*0.5f-50)), 1), 0, bullet2Id2);
			break;
		case 3:
			this->addChild(PBulletSprite3::createBullet(state, ccpAdd(player->getPosition(), ccp(-50, player->getContentSize().height*0.5f-50)), 1), 0, bullet2Id1);
			this->addChild(PBulletSprite3::createBullet(state, ccpAdd(player->getPosition(), ccp(0, player->getContentSize().height*0.5f)), 5), 0, bullet2Id0);
			this->addChild(PBulletSprite3::createBullet(state, ccpAdd(player->getPosition(), ccp(50, player->getContentSize().height*0.5f-50)), 1), 0, bullet2Id2);
			break;
		case 4:
			this->addChild(PBulletSprite3::createBullet(state, ccpAdd(player->getPosition(), ccp(-50, player->getContentSize().height*0.5f-50)), 6), 0, bullet2Id1);
			this->addChild(PBulletSprite3::createBullet(state, ccpAdd(player->getPosition(), ccp(50, player->getContentSize().height*0.5f-50)), 6), 0, bullet2Id2);
			break;
		case 5:
			this->addChild(PBulletSprite3::createBullet(state, ccpAdd(player->getPosition(), ccp(0, player->getContentSize().height*0.5f)), 10), 0, bullet2Id0);
			break;
		}
	}
}
void PlayerLayer::updateLevel(){
	player->setIsAlive(false);
	player->runAction(CCSequence::create(CCScaleTo::create(0.5f, 4.0f), CCScaleTo::create(0.5f, 1.0f)
		, CCCallFunc::create(this, callfunc_selector(PlayerLayer::setPlayerAlive)), NULL));
	int type=GameScene::sharedWorld()->getType();
	int level=GameScene::sharedWorld()->getLevel();
	if(type==1 || type==4){
		this->removeChildByTag(bullet2Id0);
		this->removeChildByTag(bullet2Id1);
		this->removeChildByTag(bullet2Id2);
		int state=0;
		if(type==4){
			state=1;
		}
		switch(level){
		case 1:
			this->addChild(PBulletSprite3::createBullet(state, ccpAdd(player->getPosition(), ccp(0, player->getContentSize().height*0.5f)), 1), 0, bullet2Id0);
			break;
		case 2:
			this->addChild(PBulletSprite3::createBullet(state, ccpAdd(player->getPosition(), ccp(-50, player->getContentSize().height*0.5f-50)), 1), 0, bullet2Id1);
			this->addChild(PBulletSprite3::createBullet(state, ccpAdd(player->getPosition(), ccp(50, player->getContentSize().height*0.5f-50)), 1), 0, bullet2Id2);
			break;
		case 3:
			this->addChild(PBulletSprite3::createBullet(state, ccpAdd(player->getPosition(), ccp(-50, player->getContentSize().height*0.5f-50)), 1), 0, bullet2Id1);
			this->addChild(PBulletSprite3::createBullet(state, ccpAdd(player->getPosition(), ccp(0, player->getContentSize().height*0.5f)), 5), 0, bullet2Id0);
			this->addChild(PBulletSprite3::createBullet(state, ccpAdd(player->getPosition(), ccp(50, player->getContentSize().height*0.5f-50)), 1), 0, bullet2Id2);
			break;
		case 4:
			this->addChild(PBulletSprite3::createBullet(state, ccpAdd(player->getPosition(), ccp(-50, player->getContentSize().height*0.5f-50)), 6), 0, bullet2Id1);
			this->addChild(PBulletSprite3::createBullet(state, ccpAdd(player->getPosition(), ccp(50, player->getContentSize().height*0.5f-50)), 6), 0, bullet2Id2);
			break;
		case 5:
			this->addChild(PBulletSprite3::createBullet(state, ccpAdd(player->getPosition(), ccp(0, player->getContentSize().height*0.5f)), 10), 0, bullet2Id0);
			break;
		}
	}
}
void PlayerLayer::setPlayerAlive(){
	player->setIsAlive(true);
}
PlaneSprite *PlayerLayer::getPlayer(){
	return player;
}
void PlayerLayer::removerAllBullet(){
	int type=GameScene::sharedWorld()->getType();
	if(type==5){
		batchNode->removeAllChildrenWithCleanup(true);
	}
}
