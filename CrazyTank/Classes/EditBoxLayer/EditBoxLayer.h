#ifndef __EBLAYER_SCENE_H__
#define __EBLAYER_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
class EditBoxLayer : public cocos2d::CCLayer
	, public cocos2d::extension::CCBSelectorResolver
	, public cocos2d::extension::CCBMemberVariableAssigner
	, public cocos2d::extension::CCNodeLoaderListener
	, public cocos2d::extension::CCEditBoxDelegate
{
public:

	CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(EditBoxLayer, create);
	EditBoxLayer();
	virtual ~EditBoxLayer();
	//virtual bool init(); 
	//static cocos2d::CCScene* scene();
	

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

	void toExtensionsMainLayer(cocos2d::CCObject *sender);

	virtual void editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox);
	virtual void editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox);
	virtual void editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text);
	virtual void editBoxReturn(cocos2d::extension::CCEditBox* editBox);

	virtual void menuCloseCallback(CCObject* pSender);
	virtual void yesCallback(CCObject* pSender);


	void  backClicked();
	void menuCallback(CCObject* pSender);
	//���أ��˳�
	virtual void  keyBackClicked();
	void onGiftClicked(CCObject* pSender);

	bool isLingQu;//�Ƿ���ȡ

private:
	cocos2d::extension::CCEditBox* m_pEditName;

	cocos2d::CCSprite*           gold_box;
	cocos2d::CCSprite*           mine_box;
	cocos2d::CCSprite*           nuke_box;

	 char      activeNumtext[50];
	//cocos2d::CCString *fontStr;
	//cocos2d::CCString *codeStr;
};

#endif