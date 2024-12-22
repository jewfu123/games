#ifndef __MENULAYER_LAYER_H__
#define __MENULAYER_LAYER_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "Tool.h"
#include "cocos-ext.h"
#include "UserRecord/UserRecord.h"

class MenuLayer :public cocos2d::CCLayer
{
public:
	//MenuLayer(void);
	//~MenuLayer(void);

	 static cocos2d::CCScene* scene();
	 virtual bool init();  
	 void   firstSelectMap(int index);//��һ��ѡ���ͼ
	  void  backMenu();//����

	 cocos2d::extension::CCBReader* reader;

	 int selsecMap;

	 int actIndex;//ģʽѡ��

  	 int passCheckNum;
	 int passCheck[43];//�ؿ��Ƿ���
	 int passCheckIndex[43];//ÿһ�ؿ��ȼ�

	 bool teach;//�Ƿ��ǽ�ѧ

	 virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
     virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
     virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	 void registerWithTouchDispatcher(void);
	 void playBottnEffect();
	 void playEffect(int index);

	 void showTime(float  t);

	 void moveMenu(bool isLeft);//�����˵�
	 void showMission(int n);//��ʾ����
	 //���أ��˳�
	 virtual void  keyBackClicked();

	  bool selectMapBool;
	  CREATE_FUNC(MenuLayer);
private:
	CCNode *  openCBBI(const char * pCCBFileName,const char *actionName) ;
	void callBackEndAction();
	bool  move;
	//cocos2d::CCSpriteFrameCache  *numSFC;
};

#endif