#include "GameDialogLayer.h"
#include "GameResource.h"
#include "GameScene.h"
#include "StringUtil.h"

GameDialogLayer *GameDialogLayer::create(int _state, int _coin, int _score){
	GameDialogLayer *pRet =new GameDialogLayer();
	if (pRet && pRet->init(_state, _coin, _score)){  
		pRet->autorelease(); 
		return pRet;
	} else { 
		delete pRet; 
		pRet = NULL; 
		return NULL; 
	} 
}
bool GameDialogLayer::init(int _state, int _coin, int _score){
	if(!CCLayer::init()){
		return false;
	}
	state=_state;
	CCSize size=CCDirector::sharedDirector()->getWinSize();
	CCSprite *bg=CCSprite::createWithSpriteFrameName(CCString::createWithFormat(s_dialog_bg_d, _state)->getCString());
	bg->setPosition(ccp(size.width*0.5f, size.height*0.5f));
	this->addChild(bg);

	CCMenuItemImage *item0=CCMenuItemImage::create(NULL, NULL, NULL
		, this, menu_selector(GameDialogLayer::menuCall));
	item0->setNormalSpriteFrame(
		CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(s_dialog_button));
	item0->setSelectedSpriteFrame(
		CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(s_dialog_button_pressed));
	item0->setDisabledSpriteFrame(
		CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(s_dialog_button_unable));

	item0->setTag(0);
	item0->setPosition(ccp(120, 50));
	CCLabelTTF *text0=CCLabelTTF::create(
		StringUtil::getString(CCString::createWithFormat("diloag_text%d%d", _state, 0)->getCString())
		, "MarkerFelt", 26);
	text0->setPosition(ccp(item0->getContentSize().width*0.5f, item0->getContentSize().height*0.5f));
	item0->addChild(text0);

	CCMenuItemImage *item1=CCMenuItemImage::create(NULL, NULL, NULL
		, this, menu_selector(GameDialogLayer::menuCall));
	item1->setNormalSpriteFrame(
		CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(s_dialog_button));
	item1->setSelectedSpriteFrame(
		CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(s_dialog_button_pressed));
	item1->setDisabledSpriteFrame(
		CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(s_dialog_button_unable));

	item1->setTag(1);
	item1->setPosition(ccp(bg->getContentSize().width-120, 50));
	CCLabelTTF *text1=CCLabelTTF::create(
		StringUtil::getString(CCString::createWithFormat("diloag_text%d%d", _state, 1)->getCString())
		, "MarkerFelt", 26);
	text1->setPosition(ccp(item1->getContentSize().width*0.5f, item1->getContentSize().height*0.5f));
	item1->addChild(text1);

	menu=CCMenu::create(item0, item1, NULL);
	menu->setPosition(ccp(0, 0));
	bg->addChild(menu);

	if(_state==0){
		CCLabelTTF *text;
		if(_coin<2000){
			item0->setEnabled(false);
			text=CCLabelTTF::create(StringUtil::getString("dialog_no_coin"), "MarkerFelt", 34);
		}else{
			text=CCLabelTTF::create(StringUtil::getString("dialog_able_again"), "MarkerFelt", 34);
		}
		text->setPosition(ccp(bg->getContentSize().width*0.5f, bg->getContentSize().height *0.5f));
		bg->addChild(text);
	}else{
		CCLabelTTF *scoreLabel=CCLabelTTF::create(
			CCString::createWithFormat(StringUtil::getString("dialog_score"), _score)->getCString()
			, "MarkerFelt", 34);
		scoreLabel->setPosition(ccp(bg->getContentSize().width*0.5f, bg->getContentSize().height*0.5f+30));
		bg->addChild(scoreLabel);
		CCLabelTTF *coinLabel=CCLabelTTF::create(
			CCString::createWithFormat(StringUtil::getString("dialog_coin"), _coin)->getCString()
			, "MarkerFelt", 34);
		coinLabel->setPosition(ccp(bg->getContentSize().width*0.5f, bg->getContentSize().height*0.5f-30));
		bg->addChild(coinLabel);
	}

	return true;
}
void GameDialogLayer::onEnter(){
	//CCDirector::sharedDirector()->pause();
	GameScene::sharedWorld()->gamePause();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, INT_MIN, true);  
	CCLayer::onEnter();
}
void GameDialogLayer::onExit(){
	//CCDirector::sharedDirector()->resume();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCLayer::onExit();
}
bool GameDialogLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
	isMenu=menu->ccTouchBegan(pTouch, pEvent);
	return true;
}
void GameDialogLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
	if(isMenu){
		menu->ccTouchMoved(pTouch, pEvent);
	}
}
void GameDialogLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
	if(isMenu){
		menu->ccTouchEnded(pTouch, pEvent);
	}
}
void GameDialogLayer::menuCall(CCObject *object){
	CCMenuItemImage *item=(CCMenuItemImage *)object;
	int tag=item->getTag();
	if(state==0){
		if(tag==0){
			GameScene::sharedWorld()->reduceCoin(2000);
			GameScene::sharedWorld()->gameResume();
			this->removeFromParentAndCleanup(true);
		}else{
			GameScene::sharedWorld()->gameResume();
			GameScene::sharedWorld()->gameAgain();
		}
	}else{
		if(tag==0){
			GameScene::sharedWorld()->gameNext();
		}else{
			GameScene::sharedWorld()->gameBack();
		}
	}
}