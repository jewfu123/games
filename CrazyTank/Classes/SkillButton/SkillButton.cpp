#include "SkillButton.h"


USING_NS_CC;

SkillButton::SkillButton():
    mItemSkill(NULL),
    mMenuSkill(NULL),
    mStencil(NULL),
    mProgressTimer(NULL),
    mCDTime(1.f)
{

}

SkillButton::~SkillButton()
{

}

SkillButton* SkillButton::createSkillButton(float cdTime, const char* stencil_file_name, const char* button_normal_name, const char* button_click_name)
{
    SkillButton* skillButton = new SkillButton();
    if (skillButton && skillButton->init(cdTime, stencil_file_name, button_normal_name, button_click_name))
    {
        skillButton->autorelease();
        return skillButton;
    }
    else
    {
        delete skillButton;
        skillButton = NULL;
    }

    return NULL;
}

bool SkillButton::init(float cdTime, const char* stencil_file_name, const char* button_normal_name, const char* button_click_name)
{
    CCAssert(stencil_file_name, "SkillButton::init stencil_file_name != NULL");
    CCAssert(button_normal_name, "SkillButton::init button_normal_name != NULL");
    CCAssert(button_click_name, "SkillButton::init button_click_name != NULL");

    // Notice:���childʱҪע�����²�
    // ���·���CCMenuItemImage �����ģ��ͼƬ ���Ϸ���CCProgressTimer

    // ��Ӽ��ܰ�ť
    mItemSkill = CCMenuItemImage::create(button_normal_name, button_click_name, this, menu_selector(SkillButton::skillClickCallBack));
    mItemSkill->setPosition(CCPointZero);

    mMenuSkill = CCMenu::create(mItemSkill, NULL);
    mMenuSkill->setPosition(CCPointZero);
    addChild(mMenuSkill, -100);

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
    return true;
}

/** ���ܰ�ť����ص� */
void SkillButton::skillClickCallBack(cocos2d::CCObject* obj)
{
    // ��ȴ��ʱ����ʱ״̬���ܰ�ť���ɵ��
    mItemSkill->setEnabled(false);

    // ģ��ɼ�
    mStencil->setVisible(true);

    // ���þ��������Ϊ˳ʱ��
    mProgressTimer->setVisible(true);
    mProgressTimer->setType(kCCProgressTimerTypeRadial);

    //׼��һ��5����ת360�ȵĶ���(�𽥸��ǰ�͸ģ���γ���ȴЧ��;������м�ʱ��ȴ������ʵ�ֺ�ʱ�����)
    CCActionInterval* action_progress_to = CCProgressTo::create(mCDTime, 100);
    CCCallFunc* action_callback = CCCallFuncN::create(this, callfuncN_selector(SkillButton::skillCoolDownCallBack));
    mProgressTimer->runAction(CCSequence::create(action_progress_to, action_callback, NULL));
}

/** ������ȴ��ɻص� */
void SkillButton::skillCoolDownCallBack(CCNode* node)
{
    // �����ɰ岻�ɼ�
    mStencil->setVisible(false);

    // ���������ܲ��ɼ�
    mProgressTimer->setVisible(false);

    // ��ť��Ϊ����
    mItemSkill->setEnabled(true);
}

