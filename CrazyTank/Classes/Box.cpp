#include "Box.h"
#include "xml/XMLParser.h"


using namespace cocos2d;

Box::~Box(void)
{
}
void Box::myInit(void)//const char *pszFileName
{
	this->destroy = false;
	SEL_Bool = false;
	strength = 0;
}

Box* Box::spriteCreate(const char *pszFileName){
	 Box *pobSprite = new Box();       
	 if (pobSprite && pobSprite->initWithFile(pszFileName))//备注1     
	 {        
		 pobSprite->myInit();        
		 pobSprite->autorelease();        
		 return pobSprite;     
	 }     
	 CC_SAFE_DELETE(pobSprite);     
	 return NULL;
}

Box* Box::spriteCreate(cocos2d::CCSpriteFrame *pSpriteFrame){
	 Box *pobSprite = new Box();       
	 if (pobSprite && pobSprite->initWithSpriteFrame(pSpriteFrame))//备注1     
	 {        
		 pobSprite->myInit();        
		 pobSprite->autorelease();        
		 return pobSprite;     
	 }     
	 CC_SAFE_DELETE(pobSprite);     
	 return NULL;
}



void Box::setName(char *name){
	this->name = name;
}

const char* Box::getName(){
	return name;
}

void Box::setDestroy(bool destroy){
	this->destroy = destroy;
}

bool Box::getDestroy(){
	return destroy;
}

void Box::setLayerName(const char *layerName){
	this->layerName = layerName;
}

const char* Box::getLayerName(){
	return layerName;
}
void  Box::setId(int id){
	this->id = id;
	//lifeNum = 100;
	//strength = 5;
	XMLParser *pXmlParser = XMLParser::parseWithFile("box.xml");

	char layerName[30];
	sprintf(layerName,"box%dLife", id);

	lifeNum = (pXmlParser->getString(layerName))->intValue();
	sprintf(layerName,"box%dStrength", id);
	strength = (pXmlParser->getString(layerName))->intValue();
	
}

int  Box::getId(){
	return id;
}
void  Box::setBoxPosLayer(cocos2d::CCPoint pos){
	boxPosLayer =pos;
}

cocos2d::CCPoint Box::getBoxPosLayer(){
	return boxPosLayer;
}
void Box::setStrength(int streng){
	strength = streng;
}
int  Box::getStrength(){
	return strength;
}
 void Box::beAttack(int attack,CCObject *m_pListener,cocos2d::SEL_CallFuncN m_pfnSelector){

	 lifeNum-=attack;
	 if(lifeNum<=0){
		 lifeNum = 0;
		 setDestroy(true);
		 //if(!SEL_Bool){
			 if (m_pListener && m_pfnSelector)//执行回调函数
			 {
				 (m_pListener->*m_pfnSelector)(this);
				 SEL_Bool = true;
			 }
		// }
	 }else if(lifeNum<50&&!SEL_Bool){
		 if (m_pListener && m_pfnSelector)//执行回调函数
         {
           (m_pListener->*m_pfnSelector)(this);
		   SEL_Bool = true;
         }
	 }
	 cocos2d::CCLog("=====lifeNum:%d",lifeNum);
}