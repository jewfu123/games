/*��Ӧ����״̬*/

#ifndef _IMMEDIATEBUTTON_H_
#define _IMMEDIATEBUTTON_H_
#include "cocos2d.h"


class ImmediateButton :public cocos2d::CCLayer
{
public:
	ImmediateButton(void);
	~ImmediateButton(void);

	/** ����һ��ImmediateButton����*/
    static ImmediateButton* createSkillButton(float cdTime, const char* stencil_file_name,
	                                          const char* button_normal_name,CCObject *rec,cocos2d::SEL_CallFuncO selector);
	 /** CDʱ������*/
    void    setCDTime(float time) { mCDTime = time; }
    float   getCDTime() const { return mCDTime; }

    /** ���ܰ�ť����ص� */
    void    skillClickCallBack(cocos2d::CCObject* obj);

    /** ������ȴ��ϻص�*/
    void   skillCoolDownCallBack(cocos2d::CCNode* node);


	void registerWithTouchDispatcher(void);
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	 virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	 virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
private:
    cocos2d::CCSprite*   mItemSkill;     // ���ܰ�ť
    cocos2d::CCSprite*          mStencil;       // �ɰ徫��,��ɫ��͸��(�����Ϊ����ʾһ����ȴ��ʱЧ��)
    cocos2d::CCProgressTimer*   mProgressTimer; // ʱ�����������(360����ת)
    float                       mCDTime;          // CDʱ��

	bool    init(float cdTime, const char* stencil_file_name,const char* button_normal_name,CCObject *rec,cocos2d::SEL_CallFuncO selector);
	bool    isTouch(cocos2d::CCTouch *touch);

	CCObject*       m_pListener;
	cocos2d::SEL_CallFuncO    m_pfnSelector;

	void upDataSEL_CallFunc(float dt);//�ص�����ִ�м��
	bool buttonPress;//�����Ƿ���
	bool CDTimeBool;//CDʱ���Ƿ����
};

#endif