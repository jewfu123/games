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

	// �����Ӱģ��
    mStencil = CCSprite::create(stencil_file_name);
    mStencil->setPosition(CCPointZero);
    mStencil->setVisible(false);
    addChild(mStencil);

    // �����ת����������
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
	  //ִ�лص�����
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

/** ������ȴ��ɻص� */
void ImmediateButton::skillCoolDownCallBack(CCNode* node)
{
	if(!this->isTouchEnabled()){
		buttonPress = false;
		unschedule(schedule_selector(ImmediateButton::upDataSEL_CallFunc));
	}

  if(!buttonPress){
    // �����ɰ岻�ɼ�
    mStencil->setVisible(false);
    // ���������ܲ��ɼ�
    mProgressTimer->setVisible(false);
    // ��ť��Ϊ����
	//this->setTouchEnabled(true);
	CDTimeBool = false;
   }
}

void ImmediateButton::upDataSEL_CallFunc(float dt){
	// ģ��ɼ�
    mStencil->setVisible(true);
    // ���þ��������Ϊ˳ʱ��
    mProgressTimer->setVisible(true);
    mProgressTimer->setType(kCCProgressTimerTypeRadial);

    //׼��һ��5����ת360�ȵĶ���(�𽥸��ǰ�͸ģ���γ���ȴЧ��;������м�ʱ��ȴ������ʵ�ֺ�ʱ�����)
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
