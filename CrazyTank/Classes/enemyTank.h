#pragma once
#include "UserDataConsistencyCCSprite.h"
#include "cocos2d.h"
#include "xml/XMLParser.h"
#include "cocos-ext.h"
#include "SimpleTimer/SimpleTimer.h"
#include "Bullet.h"

class enemyTank :public UserDataConsistencyCCSprite
{
public:
	enemyTank(void);
	~enemyTank(void);

	 void myInit(int king);
	 static enemyTank* spriteCreate(int king); //静态创建
	 //enemyTank* CopyEnemyTank();//对象拷贝
	  virtual CCObject* copyWithZone(cocos2d::CCZone* pZone);//copy() 读取时间大概30ms

	 void setTankerAction();//动画初始化


	  char*    name;
	  bool      destroy;
	  const char*    layerName;

	  bool  loseBloodNumDirection;//方向


	  void setName(char *name);
	  virtual   const char*   getName();

	  void setDestroy(bool destroy);
	  virtual     bool         getDestroy(); //判断是否销毁

	  void setLayerName(const char *name);
	  virtual    const char*   getLayerName(); //所在层的名字

	  float  getSpeed();
	  int    getTankState();//状态

	  void  loseBloodNumEnd(CCNode* sender);
	  void  loseBloodCritNumEnd(CCNode* sender);
	  void  beAttack(int attack,bool critRateBool);//被攻击,是否暴击
	   void setState(int state);//设置方向
	   int  getState();
	   bool openFire();//开火

	   bool  isChangeDir();//判断是否可以改变方向
	   bool  isTrackRadius(cocos2d::CCPoint enemyTilePot,cocos2d::CCPoint playerTilePot);//判断是否在跟踪半径内
	   bool  getTrackState();//是否是跟踪状态
	   void  setTrackState(bool trackBool);

	   void  setTrackPath(cocos2d::CCArray  *path);//设置跟踪路径
	   cocos2d::CCArray*  getTrackPath();//得到跟踪路径
	   void  setTrackPathStep(int step);//步进
	   int   getTrackPathStep();//或得步进

	   int   getKing();//获得种类
	   int   getPenetrate();//获得穿透
	   int   getCritRate();//暴击几率
	   int   getBulletSpeed();//获得子弹速度
	   int   getBulletStrength();//获得子弹强度
	   int   getBulletAttack();//子弹攻击力
	   int   getBulletRange();//

	   int   getLifeNum();//
	   int   getDefense();//

	   void setShow(bool  show);
	   bool getShow();

	   void setBulletArrayIndex(int  index);
	   int  getBulletArrayIndex();

	   CountdownTimer    rateOfFireTime;//开火时间间隔

	    void hideTank(float dt);//隐藏坦克，显示爆炸
		void goEnd();
		void bomIsAttack();//炸弹被攻击
		void bomIsAttack2nd();

		void setShowStand(bool  isStand);//是否显示站立
		bool getShow4();//是否可视4
	  // void  setMapTag(int tag);//设置tag
	 //  int   getMapTag();//返回tag

	   enum tankState{
		  moveUp   = 1,//向上移动
		  moveDown = 3,//向下移动
		  moveLeft = 2,//左移动
		  moveRight = 4,//右移动

		  explode = 5,//爆炸
		  END = 6,//结束

		  BOMBING = 7,//被轰炸
		  BOMATTACK = 8,//炸弹攻击
		  BEATTACK = 9,//被攻击
	    }; 
private:
		 int enemyKing;
		 cocos2d::extension::CCBReader  *ccbreader;
		 cocos2d::extension::CCBReader  *enemyFireBreader;//开火
		 cocos2d::extension::CCBReader  *enemyExplodeBreader;//爆炸
		 cocos2d::extension::CCBReader  *bomBreader;//被爆炸
		 cocos2d::extension::CCBReader  *standBreader;//站立
		 cocos2d::extension::CCBReader  *appearBreader;//出现

		 int tankState;//坦克状态

		 int   defense;//防御
	     int   lifeNum;//生命值
	     float moveSpeed;//移动速度
		 int   trackRadius; //跟踪半径

		 int   penetrate;//穿透
		 int   critRate;//暴击几率
		 float   rateOfFire;//子弹射速
		 int   bulletStrength;//子弹强度
		 int   bulletRange;//子弹范围
		 int   bulletAttack;//子弹攻击力
		 int   bulletSpeed;//子弹速度

		 bool  show;//是否显示

		 int  recordLifeNum;//记录生命值
		

		 int   bulletArrayIndex;//子弹在数组中id


		CountdownTimer    changeDirTime;//下次改变方向间隔时间

		bool     trackState;//是否是跟踪状态，true自由行走不转向
		cocos2d::CCArray *trackPath;//跟踪路径
		int trackPathStep;//跟踪路径步进
};

