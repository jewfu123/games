#include "GameMenuLayer.h"
#include "GameResource.h"
#include "GameScene.h"
#include "StringUtil.h"
#include "KeyId.h"
#include "AudioUtil.h"

enum{
	topId, lProgressId, rProgressId, backId, settingId, bombId
};

bool GameMenuLayer::init(){
	if(!CCLayer::init()){
		return false;
	}
	setKeypadEnabled(true);
	lifeArray=CCArray::create();
	CC_SAFE_RETAIN(lifeArray);
	thumb=NULL;

	scoreValue=0;
	coinValue=CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_COIN, 2000);
	scoreTime=scoreValue;
	coinTime=coinValue;
	speed=0;

	CCSize size=CCDirector::sharedDirector()->getWinSize();
	CCPoint oPoint=CCDirector::sharedDirector()->getVisibleOrigin();
	CCSprite *coin=CCSprite::createWithSpriteFrameName(s_game_coin);
	coin->setPosition(ccp(oPoint.x+30, size.height-oPoint.y-120));
	this->addChild(coin);
	coinLabel=CCLabelTTF::create(CCString::createWithFormat("%d", coinValue)->getCString(), "MarkerFelt", 28);
	coinLabel->setAnchorPoint(ccp(0, 0.5f));
	coinLabel->setPosition(ccpAdd(coin->getPosition(), ccp(30, 0)));
	this->addChild(coinLabel);
	scoreLabel=CCLabelTTF::create(
		CCString::createWithFormat(StringUtil::getString("game_score"), 0)->getCString()
		, "MarkerFelt", 28);
	scoreLabel->setPosition(ccp(size.width*0.5f, size.height-oPoint.y-90));
	this->addChild(scoreLabel);

	CCMenuItemSprite *on=CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName(s_music_open), CCSprite::createWithSpriteFrameName(s_music_open));
	CCMenuItemSprite *off=CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName(s_music_close), CCSprite::createWithSpriteFrameName(s_music_close));
	CCMenuItemToggle *tgMusic=CCMenuItemToggle::createWithTarget(this, menu_selector(GameMenuLayer::togglemenuMusic), on, off, NULL);
	tgMusic->setPosition(ccp(size.width-oPoint.x-40, size.height-oPoint.y-120));
	CCMenu *menu=CCMenu::create(tgMusic, NULL);
	menu->setPosition(ccp(0, 0));
	addChild(menu);
	if(AudioUtil::getMusicPlay()){
		tgMusic->setSelectedIndex(0);
	}else{
		tgMusic->setSelectedIndex(1);
	}
	isArrive=false;
	isHalfway=false;
	
	return true;
}
void GameMenuLayer::onEnterTransitionDidFinish(){
	AudioUtil::soundMenuInOut(1);
	scoreValue=0;
	scoreTime=scoreValue;
	coinTime=-1;
	numBomb=-1;
	speed=0.05f;
	level=1;

	int num=GameScene::sharedWorld()->getNum();
	CCSize size=CCDirector::sharedDirector()->getWinSize();
	CCPoint oPoint=CCDirector::sharedDirector()->getVisibleOrigin();

	CCSprite *top=CCSprite::createWithSpriteFrameName(s_game_top);
	top->setPosition(ccp(size.width*0.5f, size.height+top->getContentSize().height));
	this->addChild(top, 0, topId);

	CCLabelTTF *numLabel=CCLabelTTF::create(
		CCString::createWithFormat(StringUtil::getString("game_num"), num+1)->getCString()
		, "MarkerFelt", 32);
	numLabel->setColor(ccc3(41, 173, 160));
	numLabel->setPosition(ccp(top->getContentSize().width*0.5f, top->getContentSize().height*0.5f));
	top->addChild(numLabel);

	CCControlButton *back=CCControlButton::create(CCScale9Sprite::createWithSpriteFrameName(s_game_back));
	back->setBackgroundSpriteForState(CCScale9Sprite::createWithSpriteFrameName(s_game_back_pressed), CCControlStateHighlighted);
	back->setPreferredSize(CCSizeMake(86, 42));
	back->setPosition(ccp(back->getPreferredSize().width*0.5f+10+oPoint.x, top->getContentSize().height*0.5f+5));
	back->setTag(backId);
	back->addTargetWithActionForControlEvents(this
		, cccontrol_selector(GameMenuLayer::ccTouchButton)
		, CCControlEventTouchUpInside);
	top->addChild(back);

	CCControlButton *setting=CCControlButton::create(CCScale9Sprite::createWithSpriteFrameName(s_game_setting));
	setting->setBackgroundSpriteForState(CCScale9Sprite::createWithSpriteFrameName(s_game_setting_pressed), CCControlStateHighlighted);
	setting->setPreferredSize(CCSizeMake(86, 42));
	setting->setPosition(ccp(top->getContentSize().width-back->getPreferredSize().width*0.5f-10-oPoint.x
		, top->getContentSize().height*0.5f+5));
	setting->setTag(settingId);
	setting->addTargetWithActionForControlEvents(this
		, cccontrol_selector(GameMenuLayer::ccTouchButton)
		, CCControlEventTouchUpInside);
	top->addChild(setting);
	top->runAction(CCMoveTo::create(0.5f, ccp(size.width*0.5f, size.height-oPoint.y-top->getContentSize().height*0.5f+2)));

	CCSprite *lProgress=CCSprite::createWithSpriteFrameName(s_left_progress);
	lProgress->setPosition(ccp(-lProgress->getContentSize().width, size.height*0.5f-50));
	this->addChild(lProgress, 0, lProgressId);
	lProgress->runAction(CCMoveTo::create(0.5f, ccp(oPoint.x+20, size.height*0.5f-50)));

	thumb=CCSprite::createWithSpriteFrameName(s_game_thumb);
	thumb->setPosition(ccp(lProgress->getContentSize().width*0.5f, 0));
	lProgress->addChild(thumb);

	CCSprite *rProgress=CCSprite::createWithSpriteFrameName(s_right_progress);
	rProgress->setPosition(ccp(size.width+rProgress->getContentSize().width, oPoint.y+300));
	this->addChild(rProgress, 0, rProgressId);
	rProgress->runAction(CCMoveTo::create(0.5f, ccp(size.width-oPoint.x-20, oPoint.y+300)));

	CCControlButton *bomb=CCControlButton::create(CCScale9Sprite::createWithSpriteFrameName(s_game_bomb));
	bomb->setBackgroundSpriteForState(CCScale9Sprite::createWithSpriteFrameName(s_game_bomb_pressed), CCControlStateHighlighted);
	bomb->setBackgroundSpriteForState(CCScale9Sprite::createWithSpriteFrameName(s_game_bomb_unable), CCControlStateDisabled);
	bomb->setPreferredSize(CCSizeMake(133, 115));
	bomb->setPosition(ccp(size.width-bomb->getPreferredSize().width*0.5f-oPoint.x-10
		, -bomb->getPreferredSize().height));
	bomb->addTargetWithActionForControlEvents(this
		, cccontrol_selector(GameMenuLayer::ccTouchButton)
		, CCControlEventTouchUpInside);
	this->addChild(bomb, 0, bombId);
	bombLabel=CCLabelTTF::create("0", "MarkerFelt", 20);
	bombLabel->setPosition(ccp(20, bomb->getPreferredSize().height*0.5f-15));
	bomb->addChild(bombLabel);
	bomb->runAction(CCMoveTo::create(0.5f, ccp(size.width-bomb->getPreferredSize().width*0.5f-oPoint.x-10
		, bomb->getPreferredSize().height*0.5f+oPoint.y+10)));
	for(int i=0; i<3; i++){
		CCSprite *life=CCSprite::createWithSpriteFrameName(s_game_life);
		life->setPosition(ccp(oPoint.x+150-i*50, -life->getContentSize().height));
		this->addChild(life);
		life->runAction(CCMoveTo::create(0.5f, ccp(oPoint.x+150-i*50, oPoint.y+50)));
		lifeArray->addObject(life);
	}
	this->scheduleUpdate();
	CCLayer::onEnterTransitionDidFinish();
}
void GameMenuLayer::onExit(){
	CC_SAFE_RELEASE(lifeArray);
	CCUserDefault::sharedUserDefault()->setIntegerForKey(KEY_COIN, coinValue);
	CCUserDefault::sharedUserDefault()->setIntegerForKey(KEY_BOMB, numBomb);
	CCUserDefault::sharedUserDefault()->flush();
	CCLayer::onExit();
}
void GameMenuLayer::update(float td){;
	if(thumb){
		if(thumb->getPositionY()<600){
			thumb->setPosition(ccpAdd(thumb->getPosition(), ccp(0, speed)));
			if(thumb->getPositionY()>=250){
				if(!isHalfway){
					isHalfway=true;
					GameScene::sharedWorld()->goBoss(0);
				}
			}
		}else {
			if(!isArrive){
				isArrive=true;
				GameScene::sharedWorld()->goBoss(1);
			}
		}
	}
	if(scoreTime<scoreValue){
		scoreTime+=100;
		scoreLabel->setString(CCString::createWithFormat(
			StringUtil::getString("game_score"), scoreTime)->getCString());
		if(scoreTime%(10000*level)==0){
			level+=2;
			GameScene::sharedWorld()->upLevel();
		}
	}
	if(coinTime<0){
		coinValue=CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_COIN, 2000);
		coinTime=coinValue;
		coinLabel->setString(CCString::createWithFormat("%d", coinTime)->getCString());
	}
	if(numBomb<0){
		numBomb=CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_BOMB, 3);
		bombLabel->setString(CCString::createWithFormat("%d", numBomb)->getCString());
		CCControlButton *bomb=(CCControlButton *)this->getChildByTag(bombId);
		if(numBomb<=0){
			bomb->setEnabled(false);
		}
	}
	if(coinTime<coinValue){
		coinTime+=2;
		coinLabel->setString(CCString::createWithFormat("%d", coinTime)->getCString());
	}
}

void GameMenuLayer::ccTouchButton(CCObject *object, CCControlEvent event){
	CCControlButton *button=(CCControlButton *)object;
	int tag=button->getTag();
	if(tag==backId){
		toScene(0);
	}else if(tag==settingId){
		GameScene::sharedWorld()->showSetting();
	}else if(tag==bombId){
		//button->setEnabled(false);
		numBomb--;
		bombLabel->setString(CCString::createWithFormat("%d", numBomb)->getCString());
		if(numBomb<=0){
			CCControlButton *bomb=(CCControlButton *)this->getChildByTag(bombId);
			bomb->setEnabled(false);
		}
		GameScene::sharedWorld()->bomb();
	}
}
void GameMenuLayer::keyBackClicked(){
	toScene(0);
}

void GameMenuLayer::toScene(int _state){
	AudioUtil::soundMenuInOut(0);
	CCSize size=CCDirector::sharedDirector()->getWinSize();
	CCPoint oPoint=CCDirector::sharedDirector()->getVisibleOrigin();

	CCSprite *top=(CCSprite *)this->getChildByTag(topId);
	CCSprite *lProgress=(CCSprite *)this->getChildByTag(lProgressId);
	CCSprite *rProgress=(CCSprite *)this->getChildByTag(rProgressId);
	CCControlButton *bomb=(CCControlButton *)this->getChildByTag(bombId);

	CCObject *object=NULL;
	CCARRAY_FOREACH(lifeArray, object){
		CCSprite *life=(CCSprite *)object;
		life->runAction(CCMoveTo::create(0.3f, ccp(life->getPositionX(), -life->getContentSize().height)));
	}
	bomb->runAction(CCMoveTo::create(0.3f, ccp(bomb->getPositionX(), -bomb->getPreferredSize().height)));
	rProgress->runAction(CCMoveTo::create(0.3f, ccp(size.width+rProgress->getContentSize().width,rProgress->getPositionY())));
	lProgress->runAction(CCMoveTo::create(0.3f, ccp(-lProgress->getContentSize().width, lProgress->getPositionY())));
	top->runAction(CCSequence::create(CCMoveTo::create(0.3f, ccp(size.width*0.5f, size.height+top->getContentSize().height)), CCCallFunc::create(this, callfunc_selector(GameMenuLayer::toBack)), NULL));

}
void GameMenuLayer::toBack(){
	GameScene::sharedWorld()->gameBack();
}
void GameMenuLayer::reduceLife(){
	if(lifeArray->count()>0){
		CCSprite *life=(CCSprite *)lifeArray->objectAtIndex(0);
		this->removeChild(life);
		lifeArray->removeObjectAtIndex(0);
	}
}
void GameMenuLayer::updateCoin(int _coin){
	coinValue=_coin;
	coinTime=_coin;
	coinLabel->setString(CCString::createWithFormat("%d", coinTime)->getCString());
	CCUserDefault::sharedUserDefault()->setIntegerForKey(KEY_COIN, coinValue);
	CCUserDefault::sharedUserDefault()->flush();
}
void GameMenuLayer::addBomb(int value){
	numBomb+=value;
	bombLabel->setString(CCString::createWithFormat("%d", numBomb)->getCString());
	CCControlButton *bomb=(CCControlButton *)this->getChildByTag(bombId);
	bomb->setEnabled(true);
}
void GameMenuLayer::togglemenuMusic(CCObject *pSender){
	CCMenuItemToggle* temp=(CCMenuItemToggle*)pSender;  
	if (temp->getSelectedIndex()==0){   
		AudioUtil::setMusicPlay(true);
	}else if (temp->getSelectedIndex()==1){  
		AudioUtil::setMusicPlay(false);
	}  
}