#include "Props.h"

using namespace cocos2d::extension; 
using namespace cocos2d;


Props::Props(void):propsBreader(NULL)
{
}


Props::~Props(void)
{
	CC_SAFE_RELEASE_NULL( propsBreader);
}

void Props::myInit(int bfitemIndex)//const char *pszFileName
{
	index = bfitemIndex;
	this->destroy = false;
	showTime.setMillis(10.0*1000);
	showTime.start();

	schedule(schedule_selector(Props::end),0.5f);
}

Props* Props::spriteCreate(int bfitemIndex){
	Props *pobSprite = new Props();       
	if (pobSprite && pobSprite->init())//±¸×¢1     initWithSpriteFrame(pSpriteFrame)
	{   
		pobSprite->myInit(bfitemIndex);        
		pobSprite->autorelease();        
		return pobSprite;     
	}     
	CC_SAFE_DELETE(pobSprite);     
	return NULL;
}

void Props::setPoropsAction(){

	char pszFileName[50];
	sprintf(pszFileName,"map/bfitem_%d.ccbi", index);

	CCNodeLoaderLibrary *godnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	propsBreader = new cocos2d::extension::CCBReader(godnodeloaderlibrary); 
	//explodeBreader->autorelease();
	CCNode  *animationsBfitem  = propsBreader->readNodeGraphFromFile(pszFileName,this);//map/bfitem_1.ccbi
	
//	animationsBfitem->setPosition(p);

	if(animationsBfitem  != NULL){     
		this->addChild(animationsBfitem,10);
	}
	propsBreader->getAnimationManager()->runAnimationsForSequenceNamed("1st");
}

void Props:: end(float time){

	if(showTime.getSurplusMillis()<500){
		this->unschedule(schedule_selector(Props::end));

		 CCActionInterval*  actionBlink = CCBlink::create(0.5, 3);
		 this->runAction(CCSequence::create(actionBlink,
			 CCCallFunc::create(this,callfunc_selector(Props::goEnd)),NULL));
	}
}

void Props::setExplode(){
	propsBreader->getAnimationManager()->runAnimationsForSequenceNamed("2nd");
	propsBreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(Props::goEnd)); 

}

void Props::goEnd(){
	setDestroy(true);
}

int  Props::getIndex(){

	return index;
}

void Props::setName(char *name){
	this->name = name;
}

const char* Props::getName(){
	return name;
}

void Props::setDestroy(bool destroy){
	this->destroy = destroy;
}

bool Props::getDestroy(){
	return destroy;
}

void Props::setLayerName(const char *layerName){
	this->layerName = layerName;
}

const char* Props::getLayerName(){
	return layerName;
}

void  Props::setBoxPosLayer(cocos2d::CCPoint pos){
	boxPosLayer =pos;
}

cocos2d::CCPoint Props::getBoxPosLayer(){
	return boxPosLayer;
}

void Props::pausedTime(){
	showTime.pause();
}

void Props::startTime(){
	showTime.start();
}
