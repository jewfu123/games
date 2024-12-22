#include "IndexScene.h"
#include "IndexMenuLayer.h"
#include "JniUtil.h"



bool IndexScene::init(){
	if(!CCScene::init()){
		return false;
	}
	CCLayer *menuyLayer=IndexMenuLayer::create();
	this->addChild(menuyLayer);
	return true;
}