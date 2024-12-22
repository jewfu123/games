#include "ImmediateButton.h"

USING_NS_CC;

ImmediateButton::ImmediateButton(void):
    mStencil(NULL),
    mProgressTimer(NULL),
	buttonPress(false),
	CDTimeBool(false),
    mCDTime(1.f)
	//isUse(true)
{

}


ImmediateButton::~ImmediateButton(void)
{
}


ImmediateButton* ImmediateButton::createSkillButton(float cdTime, const char* stencil_file_name,const char* button_normal_name,CCObject *rec,cocos2d::SEL_CallFuncO selector){

	 ImmediateButton* immediateButton = new ImmediateButton();
    if (immediateButton && immediateButton->init(cdTime, stencil_file_name,button_normal_name,rec,selector))
    {
        immediateButton->autorelease();
        return immediateButton;
    }
    else
    {
        delete immediateButton;
        immediateButton = NULL;
    }

    return NULL;

}

bool    ImmediateButton::init(float cdTime,const char* stencil_file_name, const char* button_normal_name,CCObject *rec,cocos2d::SEL_CallFuncO selector){

	m_pListener = rec;
    m_pfnSelector = selector;

	mItemSkill = CCSprite::create(button_normal_name);
    mItemSkill->setPosition(CCPointZero);
    addChild(mItemSkill);

	// 添加阴影模版
    mStencil = CCSprite::create(stencil_file_name);
    mStencil->setPosition(CCPointZero);
    mStencil->setVisible(false);
    addChild(mStencil);

    // 添加旋转进度条精灵
    CCSprite* progressSprite = CCSprite::create(button_normal_name);
    mProgressTimer = CCProgressTimer::create(progressSprite);
    mProgressTimer->setPosition(CCPointZero);
    mProgressTimer->setVisible(false);
    addChild(mProgressTimer, 100);

    mCDTime = cdTime;

	this->setTouchEnabled(true);
    return true;
}

void ImmediateButton::registerWithTouchDispatcher(void)
{
     CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1, true);
}
bool ImmediateButton::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){

	//this->setTouchEnabled(false);
	if(isTouch(pTouch)&&!CDTimeBool){
	   buttonPress = true;

	   schedule(schedule_selector(ImmediateButton::upDataSEL_CallFunc),mCDTime,kCCRepeatForever,0.0001f);
	  //执行回调函数
	   return true;
	}
	return false;
}
void ImmediateButton::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
	if(isTouch(pTouch)){
		if(!buttonPress){
		 // buttonPress = true;
		 // schedule(schedule_selector(ImmediateButton::upDataSEL_CallFunc),mCDTime,kCCRepeatForever,0.0001f);
		 }
	}else{
		if(buttonPress){
		  buttonPress = false;
		  unschedule(schedule_selector(ImmediateButton::upDataSEL_CallFunc));
		}
	}


}

void ImmediateButton::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
	buttonPress = false;
	unschedule(schedule_selector(ImmediateButton::upDataSEL_CallFunc));
}

/** 技能冷却完成回调 */
void ImmediateButton::skillCoolDownCallBack(CCNode* node)
{
	if(!this->isTouchEnabled()){
		buttonPress = false;
		unschedule(schedule_selector(ImmediateButton::upDataSEL_CallFunc));
	}

  if(!buttonPress){
    // 设置蒙板不可见
    mStencil->setVisible(false);
    // 进度条技能不可见
    mProgressTimer->setVisible(false);
    // 按钮置为可用
	//this->setTouchEnabled(true);
	CDTimeBool = false;
   }
}

void ImmediateButton::upDataSEL_CallFunc(float dt){
	// 模版可见
    mStencil->setVisible(true);
    // 设置精灵进度条为顺时针
    mProgressTimer->setVisible(true);
    mProgressTimer->setType(kCCProgressTimerTypeRadial);

    //准备一个5秒旋转360度的动画(逐渐覆盖半透模板形成冷却效果;这里进行计时冷却动画的实现和时间控制)
    CCActionInterval* action_progress_to = CCProgressTo::create(mCDTime, 100);
    CCCallFunc* action_callback = CCCallFuncN::create(this, callfuncN_selector(ImmediateButton::skillCoolDownCallBack));
    mProgressTimer->runAction(CCSequence::create(action_progress_to, action_callback, NULL));

	CDTimeBool = true;
	if (m_pListener && m_pfnSelector)
    {
        (m_pListener->*m_pfnSelector)(this);
	}
}
bool ImmediateButton::isTouch(cocos2d::CCTouch *touch){
	CCPoint touchLocation = touch->getLocation();

	CCPoint local = mItemSkill->convertToNodeSpace(touchLocation);

	CCRect r = mItemSkill->getTextureRect();
    r.origin = CCPointZero;
	if (r.containsPoint(local))
     {
         return true;
     }
	return  false;
}
