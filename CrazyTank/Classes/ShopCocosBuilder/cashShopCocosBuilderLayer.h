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

	  //�˵��¼���
     virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
     //���ư�ť��
     virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	 //��Ա������
     virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
		//���԰�
      virtual bool onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue);
	 //��ʼ��
     virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);

	 virtual void registerWithTouchDispatcher(void);

	  
private:
	  int   billingPointIndex;//�Ʒѵ���
	 // bool  payGameOver;//���ѽ���
	 // bool  payResultBool;//���ѽ��
	  void updateBillingPoint(float time);

	  bool secondPay;//�ڶ���ȷ�Ϲ���

	  void showTime(float  t);
	  cocos2d::CCSprite*           star_box;
	  cocos2d::CCSprite*           gold_box;
	  cocos2d::extension::CCBReader  *buySuccessBreader;//����ɹ�

	  void playBottnEffect();
	  void onBackClicked(CCObject * pSender);
	  //���أ��˳�
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
