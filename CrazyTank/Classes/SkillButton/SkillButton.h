/** 
    ʵ�ּ�����ȴЧ�� 

    Notice:���childʱҪע�����²��ϵ
    ���·��ǰ�ť ����ǰ�͸����ģ��ͼƬ ���Ϸ���CCProgressTimer����������
*/
#ifndef _SKILLBUTTON_H_
#define _SKILLBUTTON_H_
#include <cocos2d.h>

class SkillButton : public cocos2d::CCNode
{
public:
    SkillButton();
    virtual ~SkillButton();

    /** ����һ��SkillButton����
        */
    static SkillButton* createSkillButton(float cdTime,
                                        const char* stencil_file_name, 
                                        const char* button_normal_name, 
                                        const char* button_click_name);

    /** CDʱ������
        */
    void    setCDTime(float time) { mCDTime = time; }
    float   getCDTime() const { return mCDTime; }

    /** ���ܰ�ť����ص� */
    void    skillClickCallBack(cocos2d::CCObject* obj);

    /** ������ȴ��ϻص�*/
    void    skillCoolDownCallBack(cocos2d::CCNode* node);

private:

    /** ��ʼ�� */
    bool    init(float cdTime, const char* stencil_file_name, const char* button_normal_name, const char* button_click_name);

private:
    cocos2d::CCMenuItemImage*   mItemSkill;     // ���ܰ�ť
    cocos2d::CCMenu*            mMenuSkill;     // ���ܰ�ť����menu
    cocos2d::CCSprite*          mStencil;       // �ɰ徫��,��ɫ��͸��(�����Ϊ����ʾһ����ȴ��ʱЧ��)
    cocos2d::CCProgressTimer*   mProgressTimer; // ʱ�����������(360����ת)
    float                       mCDTime;          // CDʱ��
};


#endif
