#ifndef _HELLOCOCOSBUILDERLAYER_H_
#define _HELLOCOCOSBUILDERLAYER_H_

#include "cocos2d.h"
#include "cocos-ext.h"

/*
 * Note: for some pretty hard fucked up reason, the order of inheritance is important!
 * When CCLayer is the 'first' inherited object:
 * During runtime the method call to the (pure virtual) 'interfaces' fails jumping into a bogus method or just doing nothing: 
 *  #0    0x000cf840 in non-virtual thunk to HelloCocos....
 *  #1    ....
 *
 * This thread describes the problem:
 * http://www.cocoabuilder.com/archive/xcode/265549-crash-in-virtual-method-call.html
 */
class MenuCocosBuilderLayer
: public cocos2d::CCLayer
, public cocos2d::extension::CCBSelectorResolver
, public cocos2d::extension::CCBMemberVariableAssigner
, public cocos2d::extension::CCNodeLoaderListener
{
    public:
		//CREATE_FUNC(MenuCocosBuilderLayer);

        CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(MenuCocosBuilderLayer, create);

        MenuCocosBuilderLayer();
        virtual ~MenuCocosBuilderLayer();

        //void openTest(const char * pCCBFileName, const char * pCCNodeName = NULL, cocos2d::extension::CCNodeLoader * pCCNodeLoader = NULL);
		//菜单事件绑定
        virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
		//控制按钮绑定
        virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
		//成员变量绑定
        virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
		//属性绑定
        virtual bool onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue);
		//初始化
        virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);

		void onMenuClicked(CCObject * pSender);
		void onAboutClicked(CCObject * pSender);
		void onHelpClicked(CCObject * pSender);
		void onOptionClicked(CCObject * pSender);
		void onAchineveClicked(CCObject * pSender);
		void onStartClicked(CCObject * pSender);
		void onShopClicked(CCObject * pSender);
		void onBackClicked(CCObject * pSender);
		void onQuitClicked(CCObject * pSender);
		void onMoreGameClicked(CCObject * pSender);
		void showBackMenu();
		void yse(CCObject* pSender);
		void no(CCObject* pSender);


		void onButton_act1Clicked(CCObject * pSender);
		void onButton_act2Clicked(CCObject * pSender);
		void onButton_act3Clicked(CCObject * pSender);
		void onButton_act4Clicked(CCObject * pSender);
		void onButton_act5Clicked(CCObject * pSender);
		void onButton_cashshopClicked(CCObject * pSender);

        void onMenuTestClicked(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent);
        void onSpriteTestClicked(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent);
        void onButtonTestClicked(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent);
        void onAnimationsTestClicked(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent);
        void onParticleSystemTestClicked(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent);
        void onScrollViewTestClicked(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent);

		//返回，退出
		virtual void  keyBackClicked();

	    void onGiftClicked(CCObject * pSender);

		cocos2d::CCMenu   *Button_act;

		cocos2d::CCMenuItemImage  *act1;
		cocos2d::CCMenuItemImage  *act2;
		cocos2d::CCMenuItemImage  *act3;
		cocos2d::CCMenuItemImage  *act4;
		cocos2d::CCMenuItemImage  *act5;
		cocos2d::CCMenuItemImage  *button_cashshop;

		cocos2d::CCMenuItemImage  *button_voice;

		cocos2d::CCSprite *life_box;
		cocos2d::CCMenuItemImage  *layer1;
		cocos2d::CCNode  *layer2;
		cocos2d::CCSprite *gold_box;
    private:
		bool menuLayerShow;
		cocos2d::CCLayer  *menuLayer;
		

        cocos2d::CCSprite *white1Sprite;

		
		//cocos2d::CCSprite *life_box;
		

        int mCustomPropertyInt;
        float mCustomPropertyFloat;
        bool mCustomPropertyBoolean;
        std::string mCustomPropertyString;

		bool  startBool;//开始
		//CCNode*  openCBBI(const char * pCCBFileName,const char *actionName) ;
		void showSelectMap(float dt);

		bool exitShow;//显示退出

		
};

#endif
