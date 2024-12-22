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

	void setAnimationManager(cocos2d::extension::CCBAnimationManager *pAnimationManager);
	cocos2d::extension::CCBAnimationManager * getAnimationManager();

	void onControlShopBox(CCObject * pSender,cocos2d::extension::CCControlEvent pCCControlEvent);

	/*����*/
	 void onbutton_buyClicked(CCObject * pSender);
	 void callBackButton_Buy();
	 //װ��
	 void onbutton_equipClicked(CCObject * pSender);
	 //����
	 void onbutton_upgradeClicked(CCObject * pSender);
	 void callBackButton_Upgrade();

	 void onbutton_buyLcokClicked(CCObject * pSender);
	 void showPayEnd();//��ʾ���
	 void ysePay(CCObject* pSender);
	 void noPay(CCObject* pSender);

	 void no(CCObject* pSender);
	 //���� ����,id
	 void  setKingANDId(int k,int i);

	 /*Item����*/
	 void onbutton_buyItemClicked(CCObject * pSender);
	 //Itemװ��
	 void onbutton_equipItemClicked(CCObject * pSender);
     //Item����
	 void onbutton_upgradeItemClicked(CCObject * pSender);

	 void showAchieve(int index);//��ʾ�ɾ�
	 void showAchieveEnd();

	 void removeChildEffect_level(float time);

	 void onShowPaySecondClicked(CCObject *pSender);

private:
	 cocos2d::extension::CCBAnimationManager *mAnimationManager;
	 int   billingPointIndex;//�Ʒѵ���
	 // bool  payGameOver;//���ѽ���
	 // bool  payResultBool;//���ѽ��
	 void updateBillingPoint(float time);

	 
	 int  king;//����
	 int  id;  //���id

	 bool buyClickBool;//����
	 bool secondPay;//�ڶ���ȷ�Ϲ���
};

#endif
