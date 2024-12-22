#include "EnemySprite.h"
#include "GameResource.h"
#include "GameScene.h"
#include "PlaneSprite.h"
#include "PBulletSprite1.h"

EnemySprite* EnemySprite::createEnemy(int _type, bool _isBoss){
	EnemySprite* enemy = new EnemySprite();
	if(enemy && enemy->initWithSpriteFrameName(CCString::createWithFormat(s_enemy_d, _type)->getCString())){
		enemy->autorelease();
		enemy->enemyInit(_type, _isBoss);
		return enemy;
	}
	CC_SAFE_DELETE(enemy);
	return NULL;
}

void EnemySprite::enemyInit(int _type, bool _isBoss){
	lock=false;
	isStop=false;
	type=_type;
	bossX=-3;
	isBoss=_isBoss;
	int level=GameScene::sharedWorld()->getLevel();
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCPoint oPoint=CCDirector::sharedDirector()->getVisibleOrigin();
	if(_type==0){
		score=200;
		coin=10;
		speed=3;
		maxHp=100+(level-1)*30;
	}else if(_type==1){
		score=600;
		coin=20;
		speed=2;
		maxHp=300+(level-1)*30;
	}else if(_type ==2){
		score=1500;
		coin=40;
		speed=1;
		maxHp=1500+(level-1)*30;
	}else if(_type==9){
		score=5000;
		coin=40;
		speed=3;
		maxHp=30000;
	}else if(_type ==10){
		score=20000;
		coin=40;
		speed=3;
		maxHp=50000;
	}
	if(isBoss){
		this->setPosition(ccp(size.width*0.5f
			,size.height-oPoint.y+this->getContentSize().height*0.5f-82));
	}else{
		this->setPosition(ccp(oPoint.x+this->getContentSize().width*0.5f+CCRANDOM_0_1()*(size.width-this->getContentSize().width-2*oPoint.x)
			, size.height-oPoint.y+this->getContentSize().height*0.5f-82));
	}
	if(type==2){
		this->schedule(schedule_selector(EnemySprite::autoCreateBullet), 2);
	}else if(type==9 || type==10){
		this->schedule(schedule_selector(EnemySprite::autoCreateBullet), 1);
	}
	this->scheduleUpdate();
}

void EnemySprite::update(float time){
	PlaneSprite *player=GameScene::sharedWorld()->getPlayer();
	if(player){
		float x=player->getPositionX();
		float y=player->getPositionY();
		float w=player->getContentSize().width*3/4;
		float h=player->getContentSize().height*3/4;
		if(player->getIsAlive()
			&& this->boundingBox().intersectsRect(CCRectMake(x-w/2, y-h/2, w, h))){
				GameScene::sharedWorld()->killPlayer();
				if(type<9){
					GameScene::sharedWorld()->getEnemys()->removeObject(this);
					this->removeFromParentAndCleanup(true);
				}
		}
	}
	if(!isStop){
		if(!isBoss){
			this->setPosition(ccpAdd(this->getPosition(), ccp(0,-3*speed)));
			if(this->getPositionY()<-this->getContentSize().height){
				GameScene::sharedWorld()->getEnemys()->removeObject(this);
				this->removeFromParentAndCleanup(true);
			}
		}else{
			CCSize size=CCDirector::sharedDirector()->getWinSize();
			CCPoint oPoint=CCDirector::sharedDirector()->getVisibleOrigin();
			if(this->getPositionY()>=size.height-oPoint.y-this->getContentSize().height*0.5f-82){
				this->setPosition(ccpAdd(this->getPosition(), ccp(0,-5)));
			}else{
				if(this->getPositionX()>=size.width+this->getContentSize().width*0.5f){
					bossX=-3+CCRANDOM_0_1()*-9;
					if(type==9){
						this->setPositionY(CCRANDOM_0_1()*(size.height*0.5f)+size.height*0.5f);
					}else if(type==10){
						this->setPositionY(CCRANDOM_0_1()*size.height);
					}
				}else if(this->getPositionX()<=-this->getContentSize().width*0.5f){
					bossX=3+CCRANDOM_0_1()*9;
					if(type==9){
						this->setPositionY(CCRANDOM_0_1()*(size.height*0.5f)+size.height*0.5f);
					}else if(type==10){
						this->setPositionY(CCRANDOM_0_1()*size.height);
					}
				}
				this->setPosition(ccpAdd(this->getPosition(), ccp(bossX,0)));
			}
		}
	}else{
		if(timeStop<=0){
			this->setColor(ccc3(255, 255, 255));
			isStop=false;
		}else{
			timeStop--;
		}
	}
}
bool EnemySprite::attackEnemy(int value){
	maxHp-=value;
	if(maxHp<=0){
		return true;
	}
	return false;
}
void EnemySprite::autoCreateBullet(float dt){
	if(type==10 || type==9){
		if(GameScene::sharedWorld()->getPlayer()){
			float r=90;
			int state=1;
			if(this->getPositionY()<GameScene::sharedWorld()->getPlayer()->getPositionY()){
				r=270;
				state=0;
			}
			for(int i=0; i<2; i++){
				getParent()->addChild(PBulletSprite1::createBullet(state, ccpAdd(this->getPosition(), ccp(-50+i*100, 0)), r));
			}
		}
		
	}else{
		if(!isStop){
			for(int i=0; i<12; i++){
				getParent()->addChild(PBulletSprite1::createBullet(1, this->getPosition(), i*30));
			}
		}
	}
}
void EnemySprite::setStop(int _time){
	if(!isBoss){
		if(!isStop){
			this->setColor(ccc3(92, 235, 239));
			isStop=true;
		}
		timeStop=_time;
	}
}
void EnemySprite::onExit(){
	unscheduleUpdate();
	if(type==2){
		unschedule(schedule_selector(EnemySprite::autoCreateBullet));
	}
	CCSprite::onExit();
}