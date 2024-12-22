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

    // Notice:添加child时要注意上下层
    // 最下方是CCMenuItemImage 其次是模版图片 最上方是CCProgressTimer

    // 添加技能按钮
    mItemSkill = CCMenuItemImage::create(button_normal_name, button_click_name, this, menu_selector(SkillButton::skillClickCallBack));
    mItemSkill->setPosition(CCPointZero);

    mMenuSkill = CCMenu::create(mItemSkill, NULL);
    mMenuSkill->setPosition(CCPointZero);
    addChild(mMenuSkill, -100);

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
    return true;
}

/** 技能按钮点击回调 */
void SkillButton::skillClickCallBack(cocos2d::CCObject* obj)
{
    // 冷却计时，即时状态技能按钮不可点击
    mItemSkill->setEnabled(false);

    // 模版可见
    mStencil->setVisible(true);

    // 设置精灵进度条为顺时针
    mProgressTimer->setVisible(true);
    mProgressTimer->setType(kCCProgressTimerTypeRadial);

    //准备一个5秒旋转360度的动画(逐渐覆盖半透模板形成冷却效果;这里进行计时冷却动画的实现和时间控制)
    CCActionInterval* action_progress_to = CCProgressTo::create(mCDTime, 100);
    CCCallFunc* action_callback = CCCallFuncN::create(this, callfuncN_selector(SkillButton::skillCoolDownCallBack));
    mProgressTimer->runAction(CCSequence::create(action_progress_to, action_callback, NULL));
}

/** 技能冷却完成回调 */
void SkillButton::skillCoolDownCallBack(CCNode* node)
{
    // 设置蒙板不可见
    mStencil->setVisible(false);

    // 进度条技能不可见
    mProgressTimer->setVisible(false);

    // 按钮置为可用
    mItemSkill->setEnabled(true);
}

