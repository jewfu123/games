#ifndef _CCSHOPTEACH2LAYER_H_
#define _CCSHOPTEACH2LAYER_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "../UserRecord/UserRecord.h"
#include "GameScene.h"

class shopTeach_2: public cocos2d::CCLayer
	, public cocos2d::extension::CCBSelectorResolver
	, public cocos2d::extension::CCBMemberVariableAssigner
	, public cocos2d::extension::CCNodeLoaderListener
{
public:
	shopTeach_2(void);
	~shopTeach_2(void);

	CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(shopTeach_2, create);


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

	 void registerWithTouchDispatcher(void);
	 bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	 void setAnimationManager(cocos2d::extension::CCBAnimationManager *pAnimationManager);

	 int seqIndex;//��������

	 cocos2d::extension::CCBAnimationManager *mAnimationManager;

	  void removeChildEffect_level(float time);

	  void goumai1(CCObject * pSender);
	  void zhuangbei1(CCObject * pSender);
	  void button_body(CCObject * pSender);
	  void goumai2(CCObject * pSender);
	  void zhuangbei2(CCObject * pSender);
	   void back1(CCObject * pSender);
	   void addGold(int goldNum);
	   cocos2d::CCSprite*           star_box;
	   cocos2d::CCSprite*           gold_box;
};

#endif