#ifndef _MAPCOCOSBUILDERLAYER_H_
#define _MAPCOCOSBUILDERLAYER_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "MenuLayer.h"

class MapSelectCocosBuilderLayer
: public cocos2d::CCLayer
, public cocos2d::extension::CCBSelectorResolver
, public cocos2d::extension::CCBMemberVariableAssigner
, public cocos2d::extension::CCNodeLoaderListener
{
public:
	  CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(MapSelectCocosBuilderLayer, create);
	  MapSelectCocosBuilderLayer(void);
	 ~MapSelectCocosBuilderLayer(void);

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

	 void onCCControlButtonFunkyClicked(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent);

	 bool  EntranceClickedBool;//按键是否按下

	 void onEntranceClicked(CCObject * pSender);


	 void showPayEnd();//显示完成
	 void ysePay(CCObject* pSender);
	 void noPay(CCObject* pSender);

	 void onShowPayEndSecondClicked(CCObject * pSender);//二次确认
	 void no(CCObject* pSender);

	 void showPayTank();//显示完成
	 void yseTankPay(CCObject* pSender);
	 void noTankPay(CCObject* pSender);

	 void onShowPayTankSecondClicked(CCObject * pSender);//二次确认
	



	 void showPaySurvival();//显示完成
	 void yseSurvivalPay(CCObject* pSender);
	 void noSurvivalPay(CCObject* pSender);




	 void shopTeachShow();//显示教学
	 void end();

private:
	int   billingPointIndex;//计费点编号
	// bool  payGameOver;//付费结束
	// bool  payResultBool;//付费结果
	void updateBillingPoint(float time);

	bool secondPay;
};

#endif