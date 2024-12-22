#ifndef _CCSHOPTEACHLAYER_H_
#define _CCSHOPTEACHLAYER_H_

#include "cocos2d.h"
#include "cocos-ext.h"


class shopTeach_1 : public cocos2d::CCLayer
	, public cocos2d::extension::CCBSelectorResolver
	, public cocos2d::extension::CCBMemberVariableAssigner
	, public cocos2d::extension::CCNodeLoaderListener
{
public:
	 shopTeach_1(void);
	~shopTeach_1(void);

	CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(shopTeach_1, create);


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

	void button_shop(CCObject * pSender);
};

#endif