#pragma once
#ifndef __TANKSCENE_SCENE_H__
#define __TANKSCENE_SCENE_H__

#include "cocos2d.h"
#include "UserDataConsistencyCCSprite.h"
#include "xml/XMLParser.h"
#include "Box2D/Box2D.h"
#include "DrawDebug/GLES-Render.h"
#include "cocos-ext.h"
#include "SimpleTimer/SimpleTimer.h"
#include "Bullet.h"

class tanker :public UserDataConsistencyCCSprite //public cocos2d::CCSprite//
{
private:

	int tankState;//坦克状态
	cocos2d::extension::CCBReader  *ccbreader;
	cocos2d::extension::CCBReader  *firebreader;//开火
	cocos2d::extension::CCBReader  *lvupbreader;//升级
	cocos2d::extension::CCBReader  *godbreader;//无敌
	cocos2d::extension::CCBReader  *tankExplodeBreader;//爆炸
	cocos2d::extension::CCBReader  *cureBreader;//加血
	cocos2d::extension::CCBReader  *moveSpeedBreader;//提速
	cocos2d::extension::CCBReader  *criticalBreader;//暴击
	
	//cocos2d::extension::CCBReader  *newBirthBreader;//出生时，无敌

	//CountdownTimer    godTime;//无敌时间
	bool              godBool;//是否是无敌状态
	bool              newBirthGod;//出生无敌
	//CountdownTimer    moveSpeedTime;//提升速度时间

	 bool  loseBloodNumDirection;//方向
public:
	 tanker(void);
	~tanker(void);

	  void myInit(int chassis,int turret,int chassisGrade,int turretGrade);
	  static tanker* spriteCreate(int chassis,int turret,int chassisGrade,int turretGrade); //静态创建

	  void   setTankerAction();//动画初始化

	 // virtual void onEnter();    
	 // virtual void onExit();

	  char*    name;
	  bool      destroy;
	  const char*    layerName;

	  void setName(char *name);
	  virtual   const char*   getName();

	  void setDestroy(bool destroy);
	  virtual     bool         getDestroy(); //判断是否销毁

	  void setLayerName(const char *name);
	  virtual    const char*   getLayerName(); //所在层的名字


	  void  beAttack(int attack,bool critRateBool);//被攻击
	  void  goEnd();

	  void  loseBloodNumEnd(CCNode* sender);
	  void  loseBloodCritNumEnd(CCNode* sender);

	  void setState(int state);//设置方向
	  int  getState();
	  void openFire();//坦克开火

	  void setLvUp(int lv);//升级增加移动速度，攻击力

	  void addFullLift();//加满生命值
	  void upDataTankCure(float time);

	  void setGod();//设置无敌状态
	  bool isGod();//是否无敌状态
	  void upDataGod(float dt);//检测无敌状态的时间

	  void setPayGod();//付费无敌

	  void setNewBirth();//设置新生命
	  void upDataNewBirth(float dt);//

	  void setMoveSpeed();//道具提升速度
	  void upDataMoveSpeed(float dt);//检测提升速度的时间

	  void setCritical();//道具提升暴击
	  void upDataCritical(float dt);//检测提升暴击的时间

	  void goLeft();

	 int chassisKind;//底盘种类
	 int turretKing;//炮塔种类
	 int chassisGrade;
	 int turretGrade;
	

	 int   defense;//防御
	 int   lifeNum;//生命值
	 float moveSpeed;//移动速度
	 int  weight;

	 int   MAXLife;//最大生命值
	 float recordMoveSpeed;//移动速度

	 int getMAXLift();//得到生命最大值
	 int  getLife();
	 float getMoveSpeed();//获得移动速度
	 
	  enum tankState{
		  moveUp   = 1,//向上移动
		  moveDown = 2,//向下移动
		  moveLeft = 3,//左移动
		  moveRight = 4,//右移动

		  holdUp = 5,//静止
		  holdDown=6,//静止
		  holdLeft=7,//静止
		  holdRight=8,//静止

		  explode = 9//爆炸,死亡
	 }; 
};
#endif
