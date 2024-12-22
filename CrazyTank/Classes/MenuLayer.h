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
	 void   firstSelectMap(int index);//第一次选择地图
	  void  backMenu();//返回

	 cocos2d::extension::CCBReader* reader;

	 int selsecMap;

	 int actIndex;//模式选择

  	 int passCheckNum;
	 int passCheck[43];//关卡是否开启
	 int passCheckIndex[43];//每一关卡等级

	 bool teach;//是否是教学

	 virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
     virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
     virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	 void registerWithTouchDispatcher(void);
	 void playBottnEffect();
	 void playEffect(int index);

	 void showTime(float  t);

	 void moveMenu(bool isLeft);//滑动菜单
	 void showMission(int n);//显示任务
	 //返回，退出
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