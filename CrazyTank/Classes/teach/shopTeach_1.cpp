#include "shopTeach_1.h"
#include "shopTeach_2Loader.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace CocosDenshion;
shopTeach_1::shopTeach_1(void)
{
}


shopTeach_1::~shopTeach_1(void)
{
}
void shopTeach_1::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader) {

	SimpleAudioEngine::sharedEngine()->preloadEffect("sound/24.ogg");

}
SEL_MenuHandler shopTeach_1::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName) {
	 CCLog("pSelectorName:%s",pSelectorName);

	 CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_shop:", shopTeach_1::button_shop);
	 return NULL;
}
SEL_CCControlHandler shopTeach_1::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName) {

	 return NULL;
}
bool shopTeach_1::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode) {

	 return false;
}
bool shopTeach_1::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue){
	bool bRet = false;

	 return bRet;
}

void shopTeach_1::button_shop(CCObject * pSender){
	if(sound){
		SimpleAudioEngine::sharedEngine()->playEffect("sound/24.ogg",false);
	}



	CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary(); 
	ccNodeLoaderLibrary->registerCCNodeLoader("instruction_202", shopTeach_2Loader::loader());//

	//创建一个自动释放的CCB读取类（CCBReader） 
	cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary); 
	ccbReader->autorelease(); 

	//读取ccbi文件获取根节点 
	CCNode * node = ccbReader->readNodeGraphFromFile("teach/instruction_202.ccbi", this); 

	 ((shopTeach_2*)node)->setAnimationManager(ccbReader->getAnimationManager());

	CCArray* allSeq = ccbReader->getAnimationManager()->getSequences(); 
	CCLog("MenuCocosBuilderLayer============================");

	for (int i=0; i<allSeq->count(); i++)
	{     
		//获取到每一个Seq 
		CCBSequence* everySeq = (CCBSequence*)allSeq->objectAtIndex(i);
		//获取每个序列动作周期、名字以及id 
		//everySeq->getDuration(); 
		CCLog("MenuCocosBuilderLayer===========%s",everySeq->getName()) ; 
		CCLog("MenuCocosBuilderLayer===========%d",everySeq->getSequenceId()) ;
	} 
	CCScene * scene = CCScene::create();

	CCNodeLoaderLibrary *backloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	cocos2d::extension::CCBReader *backBreader= new cocos2d::extension::CCBReader(backloaderlibrary); 
	backBreader->autorelease(); 

	CCNode  *backNode  = backBreader->readNodeGraphFromFile("shop/scene_shop_background.ccbi",this);
	backNode->setPosition(ccp(0,0));
	scene->addChild(backNode,0,12300);

	if(node != NULL) {
		scene->addChild(node,1,1);
	}
	CCDirector::sharedDirector()->pushScene(scene);

}