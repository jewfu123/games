#include "landMine.h"

using namespace cocos2d::extension; 
using namespace cocos2d;


landMine::landMine(void)
{
}


landMine::~landMine(void)
{
}

void landMine::myInit(int _lv){

	lv = _lv;
	this->destroy = false;

	char pszFileName[30];
	sprintf(pszFileName,"UI/minelv%d.ccbi",lv);

	CCNodeLoaderLibrary *godnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	ccbreader = new cocos2d::extension::CCBReader(godnodeloaderlibrary); 
	//ccbreader->autorelease();
	CCNode  *animationsLand = ccbreader->readNodeGraphFromFile(pszFileName,this);

	CCArray* allSeq = ccbreader->getAnimationManager()->getSequences(); 
	CCLog("============================") ;
	for (int i=0; i<(allSeq->count()); i++)
	{
		//获取到每一个Seq 
		CCBSequence* everySeq = (CCBSequence*)allSeq->objectAtIndex(i);
		//获取每个序列动作周期、名字以及id 
		// everySeq->getDuration(); 
		CCLog("===========%s",everySeq->getName()) ; 
		CCLog("===========%d",everySeq->getSequenceId()) ;
	} 
	ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("1st");


	if(animationsLand  != NULL){     
		this->addChild(animationsLand,0,0);
	}




	//ming->setPosition(ccp(point.x-20.0f,point.y));
}
landMine* landMine::spriteCreate(int _lv){
	landMine *pobSprite = new landMine();       
	if (pobSprite && pobSprite->init())//备注1
	{        

		pobSprite->myInit(_lv);        
		pobSprite->autorelease(); 
		return pobSprite;     
	}     
	CC_SAFE_DELETE(pobSprite); 
	return NULL;

}

void landMine::setName(char *name){
	this->name = name;
}

const char* landMine::getName(){
	return name;
}

void landMine::setDestroy(bool destroy){
	this->destroy = destroy;
}

bool landMine::getDestroy(){
	return destroy;
}

void landMine::setLayerName(const char *layerName){
	this->layerName = layerName;
}

const char* landMine::getLayerName(){
	return layerName;
}

int  landMine::getLandMineAttack(){
	if(lv==1){
		return 200;
	}else if(lv==2){
		return 350;
	}else{
		return 800;
	}
	
}
void landMine::setState(int state){
	landMineState = state;

	switch(landMineState){
	case Normal:
		{
			ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("1st");
		}
		break;
	case EXPLODE:
		{
			ccbreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(landMine::goEnd)); 
			ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("2nd");
			this->destroy = true;
		}
		break;
	case END:
		{
		
		}
		break;

	}
}

void landMine::goEnd(){
	if(landMineState==EXPLODE){
		setState(END);
		this->destroy = true;
	}
}