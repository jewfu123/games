#ifndef _MAGAZINECOCOSBUILDERLAYER_H_
#define _MAGAZINECOCOSBUILDERLAYER_H_

#include "cocos2d.h"
#include "cocos-ext.h"

class magazine_frameCocosBuilderLayer
	: public cocos2d::CCLayer
	, public cocos2d::extension::CCBSelectorResolver
	, public cocos2d::extension::CCBMemberVariableAssigner
	, public cocos2d::extension::CCNodeLoaderListener
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(magazine_frameCocosBuilderLayer, create);
	magazine_frameCocosBuilderLayer(void);
	~magazine_frameCocosBuilderLayer(void);

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

	void onbutton_page1Clicked(CCObject * pSender);
	void onbutton_page2Clicked(CCObject * pSender);
	void onbutton_page3Clicked(CCObject * pSender);
	void onBackClicked(CCObject * pSender);
	//���أ��˳�
	virtual void  keyBackClicked();

	void onbutton_buy1Clicked(CCObject * pSender);
	void onbutton_buy2Clicked(CCObject * pSender);
	void onbutton_buy3Clicked(CCObject * pSender);
	void onbutton_buy4Clicked(CCObject * pSender);
	void onbutton_buy5Clicked(CCObject * pSender);
	void onbutton_buy6Clicked(CCObject * pSender);
	void onbutton_startClicked(CCObject * pSender);

	void button_lingqu1Clicked(CCObject * pSender);
	void button_lingqu2Clicked(CCObject * pSender);
	void button_lingqu3Clicked(CCObject * pSender);

	void yijinggoumaiEnd();
	void showTime(float  t);
	void addGold(int goldNum);

	void onbutton_LiftClicked(CCObject * pSender);
	void onbutton_RightClicked(CCObject * pSender);

	void end();
	void playBottnEffect();
private:
	int   billingPointIndex;//�Ʒѵ���

	int   pageIndex;
	cocos2d::CCSprite*           star_box;
	cocos2d::CCSprite*           gold_box;
	cocos2d::CCSprite*           mine_box;
	cocos2d::CCSprite*           nuke_box;

	cocos2d::CCMenuItemImage*    button_page1;
	cocos2d::CCMenuItemImage*    button_page2;
	cocos2d::CCMenuItemImage*    button_page3;

	cocos2d::extension::CCBReader  *buySuccessBreader;//����ɹ�

	cocos2d::CCMenuItemImage*    button_lingqu1;
	cocos2d::CCMenuItemImage*    button_lingqu2;
	cocos2d::CCMenuItemImage*    button_lingqu3;

	void updateBillingPoint(float time);
};

#endif