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

	// bool  paused;//��ͣ
     static cocos2d::CCScene* scene();

     void loadTextureCache();
	 virtual void draw();

	 void createWorld();
	 void upDataWorld(float dt);
	 void updateMap();
	 void updateBossMap();//boss��ͼ�ؿ� 

	 void setPlayerSpeed(float x,float y);


	void BeginContact(b2Contact*  contact);
	void EndContact(b2Contact*  contact);
	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	void PostSolve(b2Contact* contact, const b2Manifold* oldManifold);

	void setBoxTileCallBack(CCNode* sender);//��Ѫ��ʣһ���޸�BoxTile
	void loadingCallBack(CCObject *obj);
	void startGame();

	

	void uiShow(float time);
	void loading3();

	bool showQiangGouBool;//��ʾ����
	void showQiangGou(float time);
	void yesQiangGouCallBack(CCObject* sender);
	void noQiangGouCallBack(CCObject* sender);

	void yesSecondQiangGouPay(CCObject* sender);
	void noSecondQiangGouPay(CCObject* sender);
	void updateBillingPoint(float time);

	void qiangGouSave();//�����ɹ�������

	void guangMingQiShi(float time);
	void yesSecondGuangMingQiShiPay(CCObject* sender);
	void noSecondGuangMingQiShiPay(CCObject* sender);
	void guangMingQiShiSave();

	void tianShenXiaFan(float time);
	void yesSecondtianShenXiaFanPay(CCObject* sender);
	void noSecondtianShenXiaFanPay(CCObject* sender);
	void tianShenXiaFanSave();
	void deadTianShenXiaFanSave();//�����������·�


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
	
	 
	 void createLandMing();//���׳�ʼ��
	 void bomBeEnemy();//���˱�ը
	 void bomOneEnemy();//����һ�����˱�ը
	 void bomOneEnemyBack();
	 void bomOneEnemyEnd();//��������
	 void tankSpeed();//����̹���ٶ�
	 void tankBulletCrit();//����ӵ���������
	 void setEnemyTankStanding();//���õ���̹��վ������

	 void changeTank();//����̹��
	 void changeBack(float time);//���

	 void showExplodeAll(float time);//ȫ����ը
	 void callBackremoveExplodeAll(CCNode* sender);

	 void upDataTankStanding(float dt);//����޵�״̬��ʱ��

	 void pausedScene();//��ͣ
	 void startScene();//��ʼ

	 cocos2d::CCPoint propsPoint;//���߳���λ��
	 bool    proposShowBool;//�Ƿ���ʾ
	 int   propsNum;//������
	 void showProps(cocos2d::CCPoint point);//��ʾ����
	 void showProps3(cocos2d::CCPoint point);//��ʾ����3

	 void showAchieve(int index);//��ʾ�ɾ�
	 void showAchieveEnd();
	 bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	 //virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	 //virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	 //virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);  
     //virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);  
     //virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);  
     //virtual void ccTouchesCancelled(cocos2d::CCSet*pTouches,cocos2d::CCEvent *pEvent); 
	  void repositionSprite(float dt);

	  void ourBaseWall();//�ҷ����ص�ǽ
	  //cocos2d::CCPoint   ourBaseWall[8];

	 struct Base//���صĽṹ��  
     {  
	   cocos2d::CCPoint    pointBase;//��Layer������
	   const char*         layerName;//������ͼͼ��
	   bool  show;          //�Ƿ���ʾ
     };

	 Base ourBaseInfo;//
	 Base enemyBaseInfo;//

	 bool ourBaseIsHurt;//�ҷ������Ƿ��ܵ��˺�

	 b2World* world;
	 b2Body *playBody;//���̹��
	 int  tankCont;//���̹������

	 int playBody_x;//���̹��λ��
	 int playBody_y;//���̹��λ��
	 int turretId;//���̹������
	 int chassisId;//����
	 int turretGrade;
	 int chassisGrade;
	 bool changeTankBool;
	 int landmineIndex;//���׵ȼ�

	 bool  isMoveView;//�Ƿ��ƶ���Ļ

	 bool playerDeath;//�Ƿ�����

	 int countEnemy_death;//���˵�����

	 cocos2d::CCTMXTiledMap *tileMap;//��ͼ

	//vector<b2Body *> tankerBulletArray;//����ӵ�
	 cocos2d::CCArray  *tankerBulletArray;//����ӵ�
	 cocos2d::CCArray  *tankerBulletArray6;//����ӵ�6

	 cocos2d::CCArray  *enemyTankArray;//����̹��
	  cocos2d::CCArray *eneymTankBullArray;//����̹�˵����ӵ�

	 cocos2d::CCArray  *enemy_King_Num;//�������� ����������

	 int   enemyNum;//���ٵ���̹��ͳ��
	 


	 int  enemysCount;//����������
	 int  emergeNum;  //ÿ��ͬʱ���ֵĵ�������
	 int  limit;//���� ���ӵĵ���
	 int  limitTime;//�������ӵ���ʱ����
	 CountdownTimer    limitCountTime;

	 bool showPayPoint;//��ʾ�Ʒѵ�
	 //CountdownTimer    emergeTime;//����ʱ����ʱ��

	 int   landmineEnemy;//���׻���̹������
	  enum tileGID{
		  sachet = 41,//����25
		  mound = 42,//����
		  stone =43,//ʯǽ
		  oilkeg = 44,//��Ͱ
		  console = 45,//����̨
		  commander = 46,//˾��
		  radar = 47,//�״�
		  skull = 48,//����
		  stab = 49,//�ش�
		  signpost = 50,//·��
		  zsignpost = 69,//��ɫ·��

		  barrier = 58,//·��
		  metalBox = 59,//����
		  stakes = 60,//ľ׮
		  withered = 61,//����

		  generator = 67,//�����
		  tree = 71,//��ɫ��
		  hstakes=65,//��ľ׮

		  fountain = 81,//��Ȫ
		  enemyBase = 75,//���˻���
		  ourBase = 85,//�ҷ�����

	  };

     CREATE_FUNC(GameScene);

  private:

	int   billingPointIndex;//�Ʒѵ���
	int  m_nNumberOfLoadedSprites;
	
	

	int randEnemyPos;//�����ϴγ��ֵ�λ��

	bool    drawWorldDebug;//��ʼ����
	GLESDebugDraw* m_debugDraw; //����
	
	vector<b2Body *>  toDestroy ; //layer����Ҫ���ٵ�
	vector<b2Body *>  enemys ;//���еĵ��� 
	//vector<b2Body *>  ourBase_Wall;//�ҷ�������Χǽ

	int  *mapObstruct;//��ͼ�赲���� 

	Astar * myastar ;//A*�㷨

	int  baseFrameFrequency;//���ر仯

	void enemysEmerge();//���˳�
	void enemysAIAStar(float dt);//���е���A*�㷨
	void enemyAIChangeDirection(b2Body* enemyBody);//����̹�������ϰ���󣬸ı䷽��
	void enemysAIupDate();//���е���A*·������

	bool isEnemyTankStanding;//����վ������

	void createOurBaseExplode(Box* box);//��ʼ���ҷ����ر�ը
	void callBackremoveOurBaseExplode(CCNode* sender);//���ӱ�ը���Ƴ�

	void createBoxExplode(Box* box);//��ʼ�����ӱ�ը
	void callBackremoveBoxExplode(CCNode* sender);//���ӱ�ը���Ƴ�

     cocos2d::CCArray  *enemyKingNum_DictElement;//�������� ����������
	 cocos2d::CCPoint  enemyPoint[3];//���˳��ֵĵص�

	 void gameTeach(CCPoint point);//��Ϸ�н�ѧ,��ʾ��̹��
	 void teach();//��ѧ
	 void goTeachEnd();

	 void showCoins(cocos2d::CCPoint point);
	 void callBackshowCoins(CCNode* sender);

	 void gameTeach3Over();

	 void teachBom();//ը����ѧ

	 void battleStart();//��ʼս��
	 
	 void propsTeach(int index);//��ѧ
	 void propsTeachEnd();

	 cocos2d::extension::CCBReader  *loadingBreader;
	 cocos2d::extension::CCBReader  *teachBreader;//��ѧ
	 cocos2d::extension::CCBReader  *battlestart;//��ʼս��
	  cocos2d::extension::CCBReader *bomReader;//ը��
	 int  teachIndex;

	 bool teachEnd;
	 
	 bool taechBom;//ը����ѧ
	 bool teachProps;//

	

	 int  teachPropsIndex;//
	 int teachBomIndex;//
};

class UIlayer : public cocos2d::CCLayer
{
 public: 
   
   UIlayer();
   virtual ~UIlayer();  
   bool secondPay;//�ڶ���ȷ�Ϲ���

   int bombNum;//ը������
   int landmineNum;//��������

   int   passLevel;//���غ�ȼ�
   bool  task_Bool;//�����Ƿ����

   bool gameOver;//��Ϸ����

   bool againPayLiftGame;//����

   bool  failMenuBool;//
   bool  isFireBool;//�Ƿ񿪻�
   bool  showReturnMainMenu;

   CCPoint didAcceleratePos;
   int enemyCountNum;//��������

   CountdownTimer    fireTime;//����ʱ����ʱ��


   virtual bool init();  
   void registerWithTouchDispatcher(void);
   void  playEffect(int index);
   float  rateOfFire;

   void menuFireCallback(float t);
   void menuFireEventCallback(CCObject *sender, CCControlEvent event);
   void menuFireStopCallback(CCObject *sender, CCControlEvent event);

   void menuBombEventCallback(CCObject *sender, CCControlEvent event);
   void menuBombCallback(CCObject* pSender);//ը��

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

   void payBomb();//����ը��
   void showPayBombButton();//����ը����ť
   void yesPayBomButton(CCObject* pSender);//
   void noPayBomButton(CCObject* pSender);//
   void yesPayBomSecondButton(CCObject* pSender);//����ȷ��
   void noPayBomSecondButton(CCObject* pSender);

   void payLandmine();//�������
   void showPayLandmineButton();//������װ�ť
   void yesPayLandmineButton(CCObject* pSender);//
   void noPayLandmineButton(CCObject* pSender);//
   void yesPayLandmineSecondButton(CCObject* pSender);//����ȷ��
   void noPayLandmineSecondButton(CCObject* pSender);

   void wudiCallback(CCObject* pSender);//�޵�
   void showPayWuDiBotton();
   void yesPayWuDiButton(CCObject* pSender);//
   void noPayWuDiButton(CCObject* pSender);//
   void yesPayWuDiSecondButton(CCObject* pSender);//����ȷ��
   void noPayWuDiSecondButton(CCObject* pSender);

   void pauseAddMenu();
   void successMenu();//�ɹ�����
   void failMenu();//ʧ�ܽ���

   bool  addMenuBool;//�ص�����ִ��һ��

   void showSuccessMenu(float time);
   void showFailMenu(float time);

   void successAddMenu();
   void failAddMenu();

   void nextCallback(CCObject* pSender);//��һ�ذ�ť
   void againCallback(CCObject* pSender);//����
   void shopCallback(CCObject* pSender);//�̵갴ť

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

   int showTablePropsIndex;//��ʾtable
   void  showTableProps(int index);//��ʾ����˵��
   void  showTableEnd(CCNode* sender);//����˵������

   //SimpleTimer  gameTime;//��Ϸʱ��
   void showShopTeach();

   void showGameTime(float dt);
   void setTankCount(int num);//����̹������
   void setTankLift(int lift);//��������ֵ
   void setEnemyCount(int num);//����̹������

   virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
   virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
   virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
   virtual void didAccelerate( CCAcceleration* pAccelerationValue );
   virtual void keyBackClicked();

   int   billingPointIndex;//�Ʒѵ���
   // bool  payGameOver;//���ѽ���
   // bool  payResultBool;//���ѽ��
   void updateBillingPoint(float time);

   CREATE_FUNC(UIlayer); 
private:
	   cocos2d::CCPoint startPoint;//������ʼ��

	  
	  
};
class enemyKingNumDictElement : public cocos2d::CCObject{
	 public: 
		  enemyKingNumDictElement();
          virtual ~enemyKingNumDictElement();  

		  void myInit(int k,int n,int index);
		  static enemyKingNumDictElement* enemyKingNumDictElementCreate(int k,int n,int index); //��̬����

		 int King;//����̹������
		 int Num;//����̹������
		 int index;//enemyTankArray������id
		// enemyTank *enemytank;
};
#endif 
