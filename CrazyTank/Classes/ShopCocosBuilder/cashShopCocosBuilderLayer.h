#ifndef _CASHSHOPCOCOSBUILDERLAYER_H_
#define _CASHSHOPCOCOSBUILDERLAYER_H_

#include "cocos2d.h"
#include "cocos-ext.h"

class cashShopCocosBuilderLayer
: public cocos2d::CCLayer
, public cocos2d::extension::CCBSelectorResolver
, public cocos2d::extension::CCBMemberVariableAssigner
, public cocos2d::extension::CCNodeLoaderListener
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(cashShopCocosBuilderLayer, create);
	cashShopCocosBuilderLayer(void);
	~cashShopCocosBuilderLayer(void);

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

	 virtual void registerWithTouchDispatcher(void);

	  
private:
	  int   billingPointIndex;//计费点编号
	 // bool  payGameOver;//付费结束
	 // bool  payResultBool;//付费结果
	  void updateBillingPoint(float time);

	  bool secondPay;//第二次确认购买

	  void showTime(float  t);
	  cocos2d::CCSprite*           star_box;
	  cocos2d::CCSprite*           gold_box;
	  cocos2d::extension::CCBReader  *buySuccessBreader;//购买成功

	  void playBottnEffect();
	  void onBackClicked(CCObject * pSender);
	  //返回，退出
	  virtual void  keyBackClicked();

	  void onButtonShopClicked(CCObject * pSender);

	  void onPage1Clicked(CCObject * pSender);
	  void onPage2Clicked(CCObject * pSender);

	  void onGold0Clicked(CCObject * pSender);
	  void onGold1Clicked(CCObject * pSender);
	  void onGold2Clicked(CCObject * pSender);
	  void onGold3Clicked(CCObject * pSender);
	  void onGold4Clicked(CCObject * pSender);
	  void onGold5Clicked(CCObject * pSender);
	  void onSuite2Clicked(CCObject * pSender);
	  void onSuite3Clicked(CCObject * pSender);

	  void onShowPay0Clicked(CCObject * pSender);
	  void onShowPay1Clicked(CCObject * pSender);
	  void onShowPay2Clicked(CCObject * pSender);
	  void onShowPay3Clicked(CCObject * pSender);
	  void onShowPay4Clicked(CCObject * pSender);
	  void onShowPay5Clicked(CCObject * pSender);
	 // void onShowPay6Clicked(CCObject * pSender);
	  void yijinggoumaiEnd();

	  void  leftOrRight(CCObject * pSender);

	  cocos2d::CCMenuItemImage*    page1;
	  cocos2d::CCMenuItemImage*    page2;


	  void  no(CCObject* pSender);

	  void addGold(int goldNum);
};

#endif
