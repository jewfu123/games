#ifndef __GAMESCENE_SCENE_H__
#define __GAMESCENE_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "DrawDebug/GLES-Render.h"

#include "SimpleAudioEngine.h"
#include "xml/XMLParser.h"
#include "UserRecord/UserRecord.h"
#include "SkillButton/ImmediateButton.h"
#include "SkillButton/SkillButton.h"
#include "Bullet.h"
#include "Box.h"
#include "tanker.h"
#include "enemyTank.h"
#include "AStar/Astar.h"
#include "ShopCocosBuilder/ShopCocosBuilderLayerLoader.h"
#include "ShopCocosBuilder/ShopCocosBuilderLayer.h"
#include "cocos-ext.h"  

#define  loadNum 13
using namespace cocos2d::extension; 

class GameScene: public cocos2d::CCLayer,b2ContactListener
{
  public:
     ~GameScene(void);
     virtual bool init();  


	 void registerWithTouchDispatcher(void);

	// bool  paused;//暂停
     static cocos2d::CCScene* scene();

     void loadTextureCache();
	 virtual void draw();

	 void createWorld();
	 void upDataWorld(float dt);
	 void updateMap();
	 void updateBossMap();//boss地图关卡 

	 void setPlayerSpeed(float x,float y);


	void BeginContact(b2Contact*  contact);
	void EndContact(b2Contact*  contact);
	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	void PostSolve(b2Contact* contact, const b2Manifold* oldManifold);

	void setBoxTileCallBack(CCNode* sender);//当血量剩一半修改BoxTile
	void loadingCallBack(CCObject *obj);
	void startGame();

	

	void uiShow(float time);
	void loading3();

	bool showQiangGouBool;//显示抢购
	void showQiangGou(float time);
	void yesQiangGouCallBack(CCObject* sender);
	void noQiangGouCallBack(CCObject* sender);

	void yesSecondQiangGouPay(CCObject* sender);
	void noSecondQiangGouPay(CCObject* sender);
	void updateBillingPoint(float time);

	void qiangGouSave();//抢购成功，保存

	void guangMingQiShi(float time);
	void yesSecondGuangMingQiShiPay(CCObject* sender);
	void noSecondGuangMingQiShiPay(CCObject* sender);
	void guangMingQiShiSave();

	void tianShenXiaFan(float time);
	void yesSecondtianShenXiaFanPay(CCObject* sender);
	void noSecondtianShenXiaFanPay(CCObject* sender);
	void tianShenXiaFanSave();
	void deadTianShenXiaFanSave();//死亡后天神下凡


	bool  showDeadTianShenXiafan;
	void tianShenXiaFan();
    void yesShowtianShenXiaFan(CCObject* sender);
	void noShowtianShenXiaFan(CCObject* sender);

	 void cleanDestroy();
	 void setViewpointCenter(cocos2d::CCPoint position);
	 cocos2d::CCPoint getViewpointCenter(cocos2d::CCPoint position);
	 void endMoveView();

	 void showPayEnd();
	 void ysePay(CCObject* pSender);
	 void noPay(CCObject* pSender);
	 bool qiangGouShow;
	 void yseSecondPay(CCObject* pSender);
	 void noSecondPay(CCObject* pSender);

	 cocos2d::CCPoint tilePosFromLocation(cocos2d::CCPoint loction);
	
	 
	 void createLandMing();//地雷初始化
	 void bomBeEnemy();//敌人被炸
	 void bomOneEnemy();//其中一个敌人被炸
	 void bomOneEnemyBack();
	 void bomOneEnemyEnd();//结束动画
	 void tankSpeed();//调整坦克速度
	 void tankBulletCrit();//玩家子弹暴击几率
	 void setEnemyTankStanding();//设置敌人坦克站立不动

	 void changeTank();//更换坦克
	 void changeBack(float time);//变回

	 void showExplodeAll(float time);//全屏爆炸
	 void callBackremoveExplodeAll(CCNode* sender);

	 void upDataTankStanding(float dt);//检测无敌状态的时间

	 void pausedScene();//暂停
	 void startScene();//开始

	 cocos2d::CCPoint propsPoint;//道具出现位置
	 bool    proposShowBool;//是否显示
	 int   propsNum;//计数器
	 void showProps(cocos2d::CCPoint point);//显示道具
	 void showProps3(cocos2d::CCPoint point);//显示道具3

	 void showAchieve(int index);//显示成就
	 void showAchieveEnd();
	 bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	 //virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	 //virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	 //virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);  
     //virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);  
     //virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);  
     //virtual void ccTouchesCancelled(cocos2d::CCSet*pTouches,cocos2d::CCEvent *pEvent); 
	  void repositionSprite(float dt);

	  void ourBaseWall();//我方基地的墙
	  //cocos2d::CCPoint   ourBaseWall[8];

	 struct Base//基地的结构体  
     {  
	   cocos2d::CCPoint    pointBase;//在Layer的坐标
	   const char*         layerName;//所属地图图层
	   bool  show;          //是否显示
     };

	 Base ourBaseInfo;//
	 Base enemyBaseInfo;//

	 bool ourBaseIsHurt;//我方基地是否受到伤害

	 b2World* world;
	 b2Body *playBody;//玩家坦克
	 int  tankCont;//玩家坦克数量

	 int playBody_x;//玩家坦克位置
	 int playBody_y;//玩家坦克位置
	 int turretId;//玩家坦克炮塔
	 int chassisId;//底盘
	 int turretGrade;
	 int chassisGrade;
	 bool changeTankBool;
	 int landmineIndex;//地雷等级

	 bool  isMoveView;//是否移动屏幕

	 bool playerDeath;//是否死亡

	 int countEnemy_death;//敌人的死亡

	 cocos2d::CCTMXTiledMap *tileMap;//地图

	//vector<b2Body *> tankerBulletArray;//玩家子弹
	 cocos2d::CCArray  *tankerBulletArray;//玩家子弹
	 cocos2d::CCArray  *tankerBulletArray6;//玩家子弹6

	 cocos2d::CCArray  *enemyTankArray;//敌人坦克
	  cocos2d::CCArray *eneymTankBullArray;//敌人坦克的字子弹

	 cocos2d::CCArray  *enemy_King_Num;//敌人种类 数量的数量

	 int   enemyNum;//击毁敌人坦克统计
	 


	 int  enemysCount;//敌人总数量
	 int  emergeNum;  //每关同时出现的敌人数量
	 int  limit;//出现 增加的敌人
	 int  limitTime;//出现增加敌人时间间隔
	 CountdownTimer    limitCountTime;

	 bool showPayPoint;//显示计费点
	 //CountdownTimer    emergeTime;//出现时间间隔时间

	 int   landmineEnemy;//地雷击毁坦克数量
	  enum tileGID{
		  sachet = 41,//箱子25
		  mound = 42,//土堆
		  stone =43,//石墙
		  oilkeg = 44,//油桶
		  console = 45,//控制台
		  commander = 46,//司令
		  radar = 47,//雷达
		  skull = 48,//骷髅
		  stab = 49,//地刺
		  signpost = 50,//路标
		  zsignpost = 69,//紫色路标

		  barrier = 58,//路障
		  metalBox = 59,//铁箱
		  stakes = 60,//木桩
		  withered = 61,//枯树

		  generator = 67,//发电机
		  tree = 71,//绿色树
		  hstakes=65,//横木桩

		  fountain = 81,//喷泉
		  enemyBase = 75,//敌人基地
		  ourBase = 85,//我方基地

	  };

     CREATE_FUNC(GameScene);

  private:

	int   billingPointIndex;//计费点编号
	int  m_nNumberOfLoadedSprites;
	
	

	int randEnemyPos;//敌人上次出现的位置

	bool    drawWorldDebug;//开始绘制
	GLESDebugDraw* m_debugDraw; //绘制
	
	vector<b2Body *>  toDestroy ; //layer层需要销毁的
	vector<b2Body *>  enemys ;//所有的敌人 
	//vector<b2Body *>  ourBase_Wall;//我方基地周围墙

	int  *mapObstruct;//地图阻挡区域 

	Astar * myastar ;//A*算法

	int  baseFrameFrequency;//基地变化

	void enemysEmerge();//敌人出
	void enemysAIAStar(float dt);//所有敌人A*算法
	void enemyAIChangeDirection(b2Body* enemyBody);//敌人坦克碰到障碍物后，改变方向
	void enemysAIupDate();//所有敌人A*路径行走

	bool isEnemyTankStanding;//敌人站立不动

	void createOurBaseExplode(Box* box);//初始化我方基地爆炸
	void callBackremoveOurBaseExplode(CCNode* sender);//箱子爆炸后移除

	void createBoxExplode(Box* box);//初始化箱子爆炸
	void callBackremoveBoxExplode(CCNode* sender);//箱子爆炸后移除

     cocos2d::CCArray  *enemyKingNum_DictElement;//敌人种类 数量的数量
	 cocos2d::CCPoint  enemyPoint[3];//敌人出现的地点

	 void gameTeach(CCPoint point);//游戏中教学,显示换坦克
	 void teach();//教学
	 void goTeachEnd();

	 void showCoins(cocos2d::CCPoint point);
	 void callBackshowCoins(CCNode* sender);

	 void gameTeach3Over();

	 void teachBom();//炸弹教学

	 void battleStart();//开始战斗
	 
	 void propsTeach(int index);//教学
	 void propsTeachEnd();

	 cocos2d::extension::CCBReader  *loadingBreader;
	 cocos2d::extension::CCBReader  *teachBreader;//教学
	 cocos2d::extension::CCBReader  *battlestart;//开始战斗
	  cocos2d::extension::CCBReader *bomReader;//炸弹
	 int  teachIndex;

	 bool teachEnd;
	 
	 bool taechBom;//炸弹教学
	 bool teachProps;//

	

	 int  teachPropsIndex;//
	 int teachBomIndex;//
};

class UIlayer : public cocos2d::CCLayer
{
 public: 
   
   UIlayer();
   virtual ~UIlayer();  
   bool secondPay;//第二次确认购买

   int bombNum;//炸弹数量
   int landmineNum;//地雷数量

   int   passLevel;//过关后等级
   bool  task_Bool;//任务是否完成

   bool gameOver;//游戏结束

   bool againPayLiftGame;//重玩

   bool  failMenuBool;//
   bool  isFireBool;//是否开火
   bool  showReturnMainMenu;

   CCPoint didAcceleratePos;
   int enemyCountNum;//敌人总数

   CountdownTimer    fireTime;//开火时间间隔时间


   virtual bool init();  
   void registerWithTouchDispatcher(void);
   void  playEffect(int index);
   float  rateOfFire;

   void menuFireCallback(float t);
   void menuFireEventCallback(CCObject *sender, CCControlEvent event);
   void menuFireStopCallback(CCObject *sender, CCControlEvent event);

   void menuBombEventCallback(CCObject *sender, CCControlEvent event);
   void menuBombCallback(CCObject* pSender);//炸弹

   void menulandmineEventCallback(CCObject *sender, CCControlEvent event);
   void menulandmineCallback(CCObject* pSender);
   void pauseMenuCallback(CCObject* pSender);

   void tuhaojinCallback(CCObject* pSender);
   void noPaytuhaojin(CCObject* pSender);
   void showPay();

   void onShowPaytuhaojinSecondClicked(CCObject *pSender);
   void noShowPaytuhaojinSecond(CCObject *pSender);

   void ysePay(CCObject* pSender);
   void noPay(CCObject* pSender);
   void showBuyEnd();

   void payBomb();//购买炸弹
   void showPayBombButton();//购买炸弹按钮
   void yesPayBomButton(CCObject* pSender);//
   void noPayBomButton(CCObject* pSender);//
   void yesPayBomSecondButton(CCObject* pSender);//二次确认
   void noPayBomSecondButton(CCObject* pSender);

   void payLandmine();//购买地雷
   void showPayLandmineButton();//购买地雷按钮
   void yesPayLandmineButton(CCObject* pSender);//
   void noPayLandmineButton(CCObject* pSender);//
   void yesPayLandmineSecondButton(CCObject* pSender);//二次确认
   void noPayLandmineSecondButton(CCObject* pSender);

   void wudiCallback(CCObject* pSender);//无敌
   void showPayWuDiBotton();
   void yesPayWuDiButton(CCObject* pSender);//
   void noPayWuDiButton(CCObject* pSender);//
   void yesPayWuDiSecondButton(CCObject* pSender);//二次确认
   void noPayWuDiSecondButton(CCObject* pSender);

   void pauseAddMenu();
   void successMenu();//成功界面
   void failMenu();//失败界面

   bool  addMenuBool;//回调函数执行一次

   void showSuccessMenu(float time);
   void showFailMenu(float time);

   void successAddMenu();
   void failAddMenu();

   void nextCallback(CCObject* pSender);//下一关按钮
   void againCallback(CCObject* pSender);//重玩
   void shopCallback(CCObject* pSender);//商店按钮

   void showPay_0();
   void onShowPay_0SecondClicked(CCObject *pSender);
   void onNoPay_0SecondClicked(CCObject *pSender);
   void ysePay_0(CCObject* pSender);
   void noPay_0(CCObject* pSender);

   void menuCloseCallback(CCObject* pSender);

   void goOnCallback(CCObject* pSender);
   void mainMenuCallback(CCObject* pSender);
   void setCallback(CCObject* pSender);

   void showBackMenu();
   void yse(CCObject* pSender);
   void no(CCObject* pSender);
   //static cocos2d::CCScene* scene();
   void  touchEnd();

   int showTablePropsIndex;//显示table
   void  showTableProps(int index);//显示道具说明
   void  showTableEnd(CCNode* sender);//道具说明结束

   //SimpleTimer  gameTime;//游戏时间
   void showShopTeach();

   void showGameTime(float dt);
   void setTankCount(int num);//设置坦克数量
   void setTankLift(int lift);//设置生命值
   void setEnemyCount(int num);//敌人坦克数量

   virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
   virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
   virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
   virtual void didAccelerate( CCAcceleration* pAccelerationValue );
   virtual void keyBackClicked();

   int   billingPointIndex;//计费点编号
   // bool  payGameOver;//付费结束
   // bool  payResultBool;//付费结果
   void updateBillingPoint(float time);

   CREATE_FUNC(UIlayer); 
private:
	   cocos2d::CCPoint startPoint;//按键起始点

	  
	  
};
class enemyKingNumDictElement : public cocos2d::CCObject{
	 public: 
		  enemyKingNumDictElement();
          virtual ~enemyKingNumDictElement();  

		  void myInit(int k,int n,int index);
		  static enemyKingNumDictElement* enemyKingNumDictElementCreate(int k,int n,int index); //静态创建

		 int King;//敌人坦克种类
		 int Num;//敌人坦克数量
		 int index;//enemyTankArray数组中id
		// enemyTank *enemytank;
};
#endif 
