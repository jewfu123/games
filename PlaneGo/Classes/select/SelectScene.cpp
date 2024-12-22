#include "SelectScene.h"
#include "SelectMenuLayer.h"

bool SelectScene::init(){
	if(!CCScene::init()){
		return false;
	}
	CCLayer *selectLayer=SelectMenuLayer::create();
	this->addChild(selectLayer);
	return true;
} 