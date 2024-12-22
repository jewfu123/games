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

	 void onCCControlButtonFunkyClicked(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent);

	 bool  EntranceClickedBool;//�����Ƿ���

	 void onEntranceClicked(CCObject * pSender);


	 void showPayEnd();//��ʾ���
	 void ysePay(CCObject* pSender);
	 void noPay(CCObject* pSender);

	 void onShowPayEndSecondClicked(CCObject * pSender);//����ȷ��
	 void no(CCObject* pSender);

	 void showPayTank();//��ʾ���
	 void yseTankPay(CCObject* pSender);
	 void noTankPay(CCObject* pSender);

	 void onShowPayTankSecondClicked(CCObject * pSender);//����ȷ��
	



	 void showPaySurvival();//��ʾ���
	 void yseSurvivalPay(CCObject* pSender);
	 void noSurvivalPay(CCObject* pSender);




	 void shopTeachShow();//��ʾ��ѧ
	 void end();

private:
	int   billingPointIndex;//�Ʒѵ���
	// bool  payGameOver;//���ѽ���
	// bool  payResultBool;//���ѽ��
	void updateBillingPoint(float time);

	bool secondPay;
};

#endif