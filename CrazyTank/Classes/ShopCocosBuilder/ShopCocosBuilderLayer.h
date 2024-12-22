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

	 bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	 virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	 virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	 void registerWithTouchDispatcher(void);

	 void onBackClicked(CCObject * pSender);
	 void onWeaponClicked(CCObject * pSender);
	 void onBodyClicked(CCObject * pSender);
	 void onItemClicked(CCObject * pSender);
	  void onButton_cashshopClicked(CCObject * pSender);

	 void setChangeAnimation(int dex,int g);//dex��ţ��ȼ�
	 void setShowPre_buy(int dex);//dex���

	 void showTanker();//��ʾ̹��

	 void  setShowGold(int goldNum);//������ʾ�������

	 int  turretId;//����
	 int  turretGrade;//�����ȼ�

	 int  chassisId;//����
	 int  chassisGrade;//���̵ȼ�

	 int  gold;//���
	 int  star;//�Ǳ�
	 int  price[12][4];//�������� �۸�

	 void leftButton(CCObject* pSender);
	 void rightButton(CCObject* pSender);

	 void  setShowBar();//��ʾ̹����Ϣ

	 void buySuccess();//����ɹ�
	 void noMoney();//��Ǯ����
	 void yes(CCObject* pSender);
	 void no(CCObject* pSender);

	 void yesPay(CCObject* pSender);
	 void noPay(CCObject* pSender);
	 bool no_moneyShow;//��ʾû�н��
	 void addGold(int _gold);
	 void updateBillingPoint(float time);

	 void  upGrade();//�����ɹ�
	 void  upFull();
	 void  playBottnEffect();//
	 void  playBuyAndUpgradeEffect();//��������
	 void  playEquipEffect();//
	 void  playUpgradeEffect();//����

	 void showGradeMoney(int index,int grade);//��ʾ������Ҫ�Ľ�Ǯ

	 int   index;//�ƶ�����
	 bool isPay;//����ʱ��������ť������
	 //���أ��˳�
	 virtual void  keyBackClicked();
private:
	  cocos2d::CCMenuItemImage*    button_weapon;
	  cocos2d::CCMenuItemImage*    button_body;
	  cocos2d::CCMenuItemImage*    button_item;
	  cocos2d::CCSprite*           star_box;
	  cocos2d::CCSprite*           gold_box;
	  cocos2d::CCSprite*           tank_box;//̹�˱���

	  cocos2d::extension::CCBReader  *buySuccessBreader;//����ɹ�
	  cocos2d::extension::CCBReader  *noMoneyBreader;//��Ǯ����
	  cocos2d::extension::CCBReader  *upGradeBreader;//�����ɹ�
	  cocos2d::extension::CCBReader  *fullBreader;//����

	  void WeapShoew();//��ʾ����
	  void BodyShoew();//��ʾ����
	  void ItemShow();//װ��

	  void showTime(float  t);

	  void callBackMoveAction();

	  bool  move;//�Ƿ����ڻ���
	 
	  int   SeqLength;//���г���

	  float   max[4];

	  int   billingPointIndex;//�Ʒѵ���
	  

	  //int ATTACK_MAX;//������
	  //int DEFENSE_MAX;//���װ��
	  //int MOVE_MAX;//����ƶ��ٶ�
	  //int BULLETRANGE_MAX;//������

	  //cocos2d::CCSprite*    shechengSprite;

	  

};

#endif
