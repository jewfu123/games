#include "PlaneScene.h"
#include "PlaneMenuLayer.h"

PlaneScene *PlaneScene::create(int _num){
	PlaneScene *pRet =new PlaneScene();
	if (pRet && pRet->init(_num)){  
		pRet->autorelease(); 
		return pRet;
	} else { 
		delete pRet; 
		pRet = NULL; 
		return NULL; 
	} 
}
bool PlaneScene::init(int _num){
	if(!CCScene::init()){ 
		return false;
	}
	CCLayer *selectLayer=PlaneMenuLayer::create(_num);
	this->addChild(selectLayer);
	return true;
}