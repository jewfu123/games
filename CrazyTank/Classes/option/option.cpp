#include "option.h"
#include "Tool.h"
USING_NS_CC;
USING_NS_CC_EXT;

option::option(void):
button_shengyinkai_1(NULL),
button_shengyinguan_1(NULL),
button_biaojikai_1(NULL),
button_biaojiguan_1(NULL),
button_zhonglikongzhi_1(NULL),
button_yaogankongzhi_1(NULL)
{
}


option::~option(void)
{
}

void option::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader) {
	//CCLOG("option=========onResolveCCBCCMenuItemSelector");
	if(LoadBooleanFromXML("sound")){
		button_shengyinkai_1->selected();
		button_shengyinguan_1->unselected();
	}else{
		button_shengyinguan_1->selected();
		button_shengyinkai_1->unselected();
	}

	if(LoadBooleanFromXML("moveMark")){
		button_biaojikai_1->selected();
		button_biaojiguan_1->unselected();
	}else{
		button_biaojikai_1->unselected();
		button_biaojiguan_1->selected();
	}
	
	if(LoadBooleanFromXML("moveControlHandle")){
		button_zhonglikongzhi_1->selected();
		button_yaogankongzhi_1->unselected();
	}else{
		button_zhonglikongzhi_1->unselected();
		button_yaogankongzhi_1->selected();
	}

}
SEL_MenuHandler option::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName) {
	//CCLOG("option=========onResolveCCBCCMenuItemSelector:%s",pSelectorName);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_back", option::onBackClicked);//·µ»Ø

	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_shengyinkai", option::onshengyinkaiClicked);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_shengyinguan", option::onshengyinguanClicked);//

	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_biaojikai", option::onbiaojikaiClicked);//
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_biaojiguan", option::onbiaojiguanClicked);//

	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_zhonglikongzhi", option::onzhonglikongzhiClicked);//
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_yaogankongzhi", option::onyaogankongzhiClicked);//

	return NULL;
}
SEL_CCControlHandler option::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName) {
	//CCLOG("option=========onResolveCCBCCControlSelector:%s",pSelectorName);
	return NULL;
}
bool option::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode) {

	 //CCLOG("option=========onAssignCCBMemberVariable:%s",pMemberVariableName);
	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_shengyinkai_1", CCMenuItemImage*, this->button_shengyinkai_1);
	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_shengyinguan_1", CCMenuItemImage*, this->button_shengyinguan_1);

	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_biaojikai_1", CCMenuItemImage*, this->button_biaojikai_1);
	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_biaojiguan_1", CCMenuItemImage*, this->button_biaojiguan_1);

	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_zhonglikongzhi_1", CCMenuItemImage*, this->button_zhonglikongzhi_1);
	 CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_yaogankongzhi_1", CCMenuItemImage*, this->button_yaogankongzhi_1);
	 return false;
}
bool option::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue)
{
    bool bRet = false;

    return bRet;
}

void option::onBackClicked(CCObject * pSender){
	 CCDirector::sharedDirector()->popScene();
}

void option::onshengyinkaiClicked(CCObject * pSender){
	button_shengyinkai_1->selected();
	button_shengyinguan_1->unselected();

	SaveBooleanToXML("sound", true);   
	
}

void option::onshengyinguanClicked(CCObject * pSender){
	button_shengyinguan_1->selected();
	button_shengyinkai_1->unselected();

	SaveBooleanToXML("sound", false);   
}

void option::onbiaojikaiClicked(CCObject * pSender){
	button_biaojikai_1->selected();
	button_biaojiguan_1->unselected();

	SaveBooleanToXML("moveMark", true);
}
void option::onbiaojiguanClicked(CCObject * pSender){
	button_biaojikai_1->unselected();
	button_biaojiguan_1->selected();

	SaveBooleanToXML("moveMark", false);
}

void option::onzhonglikongzhiClicked(CCObject * pSender){
	button_zhonglikongzhi_1->selected();
	button_yaogankongzhi_1->unselected();

	SaveBooleanToXML("moveControlHandle", true);
}

void option::onyaogankongzhiClicked(CCObject * pSender){
	button_zhonglikongzhi_1->unselected();
	button_yaogankongzhi_1->selected();

	SaveBooleanToXML("moveControlHandle", false);
}