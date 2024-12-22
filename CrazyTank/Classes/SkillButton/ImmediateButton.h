/*响应按下状态*/

#ifndef _IMMEDIATEBUTTON_H_
#define _IMMEDIATEBUTTON_H_
#include "cocos2d.h"


class ImmediateButton :public cocos2d::CCLayer
{
public:
	ImmediateButton(void);
	~ImmediateButton(void);

	/** 创建一个ImmediateButton对象*/
    static ImmediateButton* createSkillButton(float cdTime, const char* stencil_file_name,
	                                          const char* button_normal_name,CCObject *rec,cocos2d::SEL_CallFuncO selector);
	 /** CD时间设置*/
    void    setCDTime(float time) { mCDTime = time; }
    float   getCDTime() const { return mCDTime; }

    /** 技能按钮点击回调 */
    void    skillClickCallBack(cocos2d::CCObject* obj);

    /** 技能冷却完毕回调*/
    void   skillCoolDownCallBack(cocos2d::CCNode* node);


	void registerWithTouchDispatcher(void);
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	 virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	 virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
private:
    cocos2d::CCSprite*   mItemSkill;     // 技能按钮
    cocos2d::CCSprite*          mStencil;       // 蒙板精灵,黑色半透明(这个是为了显示一个冷却计时效果)
    cocos2d::CCProgressTimer*   mProgressTimer; // 时间进度条精灵(360度旋转)
    float                       mCDTime;          // CD时间

	bool    init(float cdTime, const char* stencil_file_name,const char* button_normal_name,CCObject *rec,cocos2d::SEL_CallFuncO selector);
	bool    isTouch(cocos2d::CCTouch *touch);

	CCObject*       m_pListener;
	cocos2d::SEL_CallFuncO    m_pfnSelector;

	void upDataSEL_CallFunc(float dt);//回调函数执行间隔
	bool buttonPress;//按键是否按下
	bool CDTimeBool;//CD时间是否完成
};

#endif