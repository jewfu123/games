#ifndef _SHOPCOCOSBUILDERLAYER_H_
#define _SHOPCOCOSBUILDERLAYER_H_

#include "cocos2d.h"
#include "cocos-ext.h"

class ShopCocosBuilderLayer
: public cocos2d::CCLayer
, public cocos2d::extension::CCBSelectorResolver
, public cocos2d::extension::CCBMemberVariableAssigner
, public cocos2d::extension::CCNodeLoaderListener
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(ShopCocosBuilderLayer, create);
	ShopCocosBuilderLayer(void);
	~ShopCocosBuilderLayer(void);
	/*virtual void  onEnter ();*/

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

	 bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	 virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	 virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	 void registerWithTouchDispatcher(void);

	 void onBackClicked(CCObject * pSender);
	 void onWeaponClicked(CCObject * pSender);
	 void onBodyClicked(CCObject * pSender);
	 void onItemClicked(CCObject * pSender);
	  void onButton_cashshopClicked(CCObject * pSender);

	 void setChangeAnimation(int dex,int g);//dex编号，等级
	 void setShowPre_buy(int dex);//dex编号

	 void showTanker();//显示坦克

	 void  setShowGold(int goldNum);//设置显示金币数量

	 int  turretId;//炮塔
	 int  turretGrade;//炮塔等级

	 int  chassisId;//底盘
	 int  chassisGrade;//底盘等级

	 int  gold;//金币
	 int  star;//星币
	 int  price[12][4];//购买升级 价格

	 void leftButton(CCObject* pSender);
	 void rightButton(CCObject* pSender);

	 void  setShowBar();//显示坦克信息

	 void buySuccess();//购买成功
	 void noMoney();//金钱不足
	 void yes(CCObject* pSender);
	 void no(CCObject* pSender);

	 void yesPay(CCObject* pSender);
	 void noPay(CCObject* pSender);
	 bool no_moneyShow;//显示没有金币
	 void addGold(int _gold);
	 void updateBillingPoint(float time);

	 void  upGrade();//升级成功
	 void  upFull();
	 void  playBottnEffect();//
	 void  playBuyAndUpgradeEffect();//购买升级
	 void  playEquipEffect();//
	 void  playUpgradeEffect();//升级

	 void showGradeMoney(int index,int grade);//显示升级需要的金钱

	 int   index;//移动序列
	 bool isPay;//付费时，其它按钮不可用
	 //返回，退出
	 virtual void  keyBackClicked();
private:
	  cocos2d::CCMenuItemImage*    button_weapon;
	  cocos2d::CCMenuItemImage*    button_body;
	  cocos2d::CCMenuItemImage*    button_item;
	  cocos2d::CCSprite*           star_box;
	  cocos2d::CCSprite*           gold_box;
	  cocos2d::CCSprite*           tank_box;//坦克背景

	  cocos2d::extension::CCBReader  *buySuccessBreader;//购买成功
	  cocos2d::extension::CCBReader  *noMoneyBreader;//金钱不足
	  cocos2d::extension::CCBReader  *upGradeBreader;//升级成功
	  cocos2d::extension::CCBReader  *fullBreader;//已满

	  void WeapShoew();//显示炮塔
	  void BodyShoew();//显示底盘
	  void ItemShow();//装备

	  void showTime(float  t);

	  void callBackMoveAction();

	  bool  move;//是否正在滑动
	 
	  int   SeqLength;//序列长度

	  float   max[4];

	  int   billingPointIndex;//计费点编号
	  

	  //int ATTACK_MAX;//最大火力
	  //int DEFENSE_MAX;//最大装甲
	  //int MOVE_MAX;//最大移动速度
	  //int BULLETRANGE_MAX;//最大射程

	  //cocos2d::CCSprite*    shechengSprite;

	  

};

#endif
