#ifndef _BOXCOCOSBUILDERLAYER_H_
#define _BOXCOCOSBUILDERLAYER_H_

#include "cocos2d.h"
#include "cocos-ext.h"

class BoxCocosBuilderLayer
: public cocos2d::CCLayer
, public cocos2d::extension::CCBSelectorResolver
, public cocos2d::extension::CCBMemberVariableAssigner
, public cocos2d::extension::CCNodeLoaderListener
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(BoxCocosBuilderLayer, create);
	BoxCocosBuilderLayer(void);
	~BoxCocosBuilderLayer(void);

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

	void setAnimationManager(cocos2d::extension::CCBAnimationManager *pAnimationManager);
	cocos2d::extension::CCBAnimationManager * getAnimationManager();

	void onControlShopBox(CCObject * pSender,cocos2d::extension::CCControlEvent pCCControlEvent);

	/*购买*/
	 void onbutton_buyClicked(CCObject * pSender);
	 void callBackButton_Buy();
	 //装备
	 void onbutton_equipClicked(CCObject * pSender);
	 //升级
	 void onbutton_upgradeClicked(CCObject * pSender);
	 void callBackButton_Upgrade();

	 void onbutton_buyLcokClicked(CCObject * pSender);
	 void showPayEnd();//显示完成
	 void ysePay(CCObject* pSender);
	 void noPay(CCObject* pSender);

	 void no(CCObject* pSender);
	 //设置 种类,id
	 void  setKingANDId(int k,int i);

	 /*Item购买*/
	 void onbutton_buyItemClicked(CCObject * pSender);
	 //Item装备
	 void onbutton_equipItemClicked(CCObject * pSender);
     //Item升级
	 void onbutton_upgradeItemClicked(CCObject * pSender);

	 void showAchieve(int index);//显示成就
	 void showAchieveEnd();

	 void removeChildEffect_level(float time);

	 void onShowPaySecondClicked(CCObject *pSender);

private:
	 cocos2d::extension::CCBAnimationManager *mAnimationManager;
	 int   billingPointIndex;//计费点编号
	 // bool  payGameOver;//付费结束
	 // bool  payResultBool;//付费结果
	 void updateBillingPoint(float time);

	 
	 int  king;//种类
	 int  id;  //编号id

	 bool buyClickBool;//按键
	 bool secondPay;//第二次确认购买
};

#endif
