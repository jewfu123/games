#include "GameSettingLayer.h"
#include "GameResource.h"
#include "GameScene.h"
#include "StringUtil.h"

bool GameSettingLayer::init(){
	if(!CCLayer::init()){
		return false;
	}
	CCSize size=CCDirector::sharedDirector()->getWinSize();
	CCSprite *bg=CCSprite::createWithSpriteFrameName(s_setting_bg);
	bg->setPosition(ccp(size.width*0.5f, size.height*0.5f));
	this->addChild(bg);
	CCArray *array=CCArray::create();
	for(int i=0; i<3; i++){
		CCMenuItemImage *item=CCMenuItemImage::create(NULL, NULL
			, this, menu_selector(GameSettingLayer::menuCall));
		item->setNormalSpriteFrame(
			CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(s_setting_button));
		item->setSelectedSpriteFrame(
			CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(s_setting_button_pressed));

		item->setTag(i);
		item->setPosition(ccp(bg->getContentSize().width*0.5f, bg->getContentSize().height*0.5f+(1-i)*130+5));
		CCLabelTTF *text=CCLabelTTF::create(StringUtil::getString(
			CCString::createWithFormat("setting_text%d", i)->getCString())
			, "MarkerFelt", 40);
		text->setPosition(ccp(item->getContentSize().width*0.5f, item->getContentSize().height*0.5f));
		text->setColor(ccc3(41, 173, 160));
		item->addChild(text);
		array->addObject(item);
	}
	menu=CCMenu::createWithArray(array);
	menu->setPosition(ccp(0, 0));
	bg->addChild(menu);
	return true;
}
void GameSettingLayer::menuCall(CCObject *object){
	CCMenuItemImage *item=(CCMenuItemImage *)object;
	int tag=item->getTag();
	switch(tag){
	case 0:
		this->removeFromParentAndCleanup(true);
		break;
	case 1:
		GameScene::sharedWorld()->gameAgain();
		break;
	case 2:
		GameScene::sharedWorld()->toSelect();
		break;
	}
}
void GameSettingLayer::onEnter(){
	CCDirector::sharedDirector()->pause();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, INT_MIN, true);  
	CCLayer::onEnter();
}
void GameSettingLayer::onExit(){
	CCDirector::sharedDirector()->resume();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCLayer::onExit();
}
bool GameSettingLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
	isMenu=menu->ccTouchBegan(pTouch, pEvent);
	return true;
}
void GameSettingLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
	if(isMenu){
		menu->ccTouchMoved(pTouch, pEvent);
	}
}
void GameSettingLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
	if(isMenu){
		menu->ccTouchEnded(pTouch, pEvent);
	}
}