#ifndef _OPTIONCOCOSBUILDERLAYER_H_
#define _OPTIONCOCOSBUILDERLAYER_H_

#include "cocos2d.h"
#include "cocos-ext.h"

class option
	: public cocos2d::CCLayer
	, public cocos2d::extension::CCBSelectorResolver
	, public cocos2d::extension::CCBMemberVariableAssigner
	, public cocos2d::extension::CCNodeLoaderListener
{
public:
	 CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(option, create);
	option();
	virtual ~option();

	//菜单事件绑定
	virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	//控制按钮绑定
	virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	//成员变量绑定
	virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
	//属性绑定
	virtual bool onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue);
	//初始化
	virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);

	void onBackClicked(CCObject * pSender);

	void onshengyinkaiClicked(CCObject * pSender);
	void onshengyinguanClicked(CCObject * pSender);

	void onbiaojikaiClicked(CCObject * pSender);
	void onbiaojiguanClicked(CCObject * pSender);

	void onzhonglikongzhiClicked(CCObject * pSender);
	void onyaogankongzhiClicked(CCObject * pSender);

private:
	cocos2d::CCMenuItemImage  *button_shengyinkai_1;
	cocos2d::CCMenuItemImage  *button_shengyinguan_1;

	cocos2d::CCMenuItemImage  *button_biaojikai_1;
	cocos2d::CCMenuItemImage  *button_biaojiguan_1;

	cocos2d::CCMenuItemImage  *button_zhonglikongzhi_1;
	cocos2d::CCMenuItemImage  *button_yaogankongzhi_1;

};
#endif
