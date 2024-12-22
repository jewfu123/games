#include "IndexMenuLayer.h"
#include "GameResource.h"
#include "../select/SelectScene.h"
#include "AudioUtil.h"
#include "JniUtil.h"


bool IndexMenuLayer::init(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
// 	std::string keyId = "tSNsfmbSNMXmuG7gIENg9iv+u3I=";
// 	std::string result = getKeyIdJNI();
// 	if (result != keyId) {
// 		CCDirector::sharedDirector()->end();
// 	}
#endif

	if(!CCLayer::init()){
		return false;
	}
	AudioUtil::loadEffect();
	setKeypadEnabled(true);
	CCSize size=CCDirector::sharedDirector()->getWinSize();
	CCPoint oPoint=CCDirector::sharedDirector()->getVisibleOrigin();

	//创建背景
	CCSprite *bg=CCSprite::createWithSpriteFrameName(s_index_bg);
	bg->setPosition(ccp(size.width*0.5f, size.height*0.5f));
	this->addChild(bg);
                     
	//创建标题
	CCSprite *title=CCSprite::createWithSpriteFrameName(s_index_title);
	title->setPosition(ccp(size.width*0.5f, size.height*0.5f+260));
	this->addChild(title);
	//创建按钮
	for(int i=0; i<3; i++){
		CCControlButton *button=CCControlButton::create(CCScale9Sprite::createWithSpriteFrameName(
			CCString::createWithFormat(s_index_button_d, i)->getCString()));
		button->setPosition(ccp(size.width*0.5f, size.height*0.5f-200-i*100));
		button->setPreferredSize(CCSizeMake(374, 79));
		button->setTag(i);
		button->addTargetWithActionForControlEvents(this, cccontrol_selector(IndexMenuLayer::ccTouchButton), CCControlEventTouchUpInside);
		this->addChild(button);
	}
	return  true;
}

void IndexMenuLayer::ccTouchButton(CCObject *object, CCControlEvent event){
	CCControlButton *button=(CCControlButton *)object;
	int tag=button->getTag();
	if(tag==0){
		CCDirector::sharedDirector()->replaceScene(SelectScene::create());
	}else if(tag==1){
		JniUtil::sendStringJNI("feed_back");
	}else if(tag==2){
		JniUtil::sendStringJNI("ad_list");
	}
}
void IndexMenuLayer::keyBackClicked(){
	CCDirector::sharedDirector()->end();
}