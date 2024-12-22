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
	 static enemyTank* spriteCreate(int king); //��̬����
	 //enemyTank* CopyEnemyTank();//���󿽱�
	  virtual CCObject* copyWithZone(cocos2d::CCZone* pZone);//copy() ��ȡʱ����30ms

	 void setTankerAction();//������ʼ��


	  char*    name;
	  bool      destroy;
	  const char*    layerName;

	  bool  loseBloodNumDirection;//����


	  void setName(char *name);
	  virtual   const char*   getName();

	  void setDestroy(bool destroy);
	  virtual     bool         getDestroy(); //�ж��Ƿ�����

	  void setLayerName(const char *name);
	  virtual    const char*   getLayerName(); //���ڲ������

	  float  getSpeed();
	  int    getTankState();//״̬

	  void  loseBloodNumEnd(CCNode* sender);
	  void  loseBloodCritNumEnd(CCNode* sender);
	  void  beAttack(int attack,bool critRateBool);//������,�Ƿ񱩻�
	   void setState(int state);//���÷���
	   int  getState();
	   bool openFire();//����

	   bool  isChangeDir();//�ж��Ƿ���Ըı䷽��
	   bool  isTrackRadius(cocos2d::CCPoint enemyTilePot,cocos2d::CCPoint playerTilePot);//�ж��Ƿ��ڸ��ٰ뾶��
	   bool  getTrackState();//�Ƿ��Ǹ���״̬
	   void  setTrackState(bool trackBool);

	   void  setTrackPath(cocos2d::CCArray  *path);//���ø���·��
	   cocos2d::CCArray*  getTrackPath();//�õ�����·��
	   void  setTrackPathStep(int step);//����
	   int   getTrackPathStep();//��ò���

	   int   getKing();//�������
	   int   getPenetrate();//��ô�͸
	   int   getCritRate();//��������
	   int   getBulletSpeed();//����ӵ��ٶ�
	   int   getBulletStrength();//����ӵ�ǿ��
	   int   getBulletAttack();//�ӵ�������
	   int   getBulletRange();//

	   int   getLifeNum();//
	   int   getDefense();//

	   void setShow(bool  show);
	   bool getShow();

	   void setBulletArrayIndex(int  index);
	   int  getBulletArrayIndex();

	   CountdownTimer    rateOfFireTime;//����ʱ����

	    void hideTank(float dt);//����̹�ˣ���ʾ��ը
		void goEnd();
		void bomIsAttack();//ը��������
		void bomIsAttack2nd();

		void setShowStand(bool  isStand);//�Ƿ���ʾվ��
		bool getShow4();//�Ƿ����4
	  // void  setMapTag(int tag);//����tag
	 //  int   getMapTag();//����tag

	   enum tankState{
		  moveUp   = 1,//�����ƶ�
		  moveDown = 3,//�����ƶ�
		  moveLeft = 2,//���ƶ�
		  moveRight = 4,//���ƶ�

		  explode = 5,//��ը
		  END = 6,//����

		  BOMBING = 7,//����ը
		  BOMATTACK = 8,//ը������
		  BEATTACK = 9,//������
	    }; 
private:
		 int enemyKing;
		 cocos2d::extension::CCBReader  *ccbreader;
		 cocos2d::extension::CCBReader  *enemyFireBreader;//����
		 cocos2d::extension::CCBReader  *enemyExplodeBreader;//��ը
		 cocos2d::extension::CCBReader  *bomBreader;//����ը
		 cocos2d::extension::CCBReader  *standBreader;//վ��
		 cocos2d::extension::CCBReader  *appearBreader;//����

		 int tankState;//̹��״̬

		 int   defense;//����
	     int   lifeNum;//����ֵ
	     float moveSpeed;//�ƶ��ٶ�
		 int   trackRadius; //���ٰ뾶

		 int   penetrate;//��͸
		 int   critRate;//��������
		 float   rateOfFire;//�ӵ�����
		 int   bulletStrength;//�ӵ�ǿ��
		 int   bulletRange;//�ӵ���Χ
		 int   bulletAttack;//�ӵ�������
		 int   bulletSpeed;//�ӵ��ٶ�

		 bool  show;//�Ƿ���ʾ

		 int  recordLifeNum;//��¼����ֵ
		

		 int   bulletArrayIndex;//�ӵ���������id


		CountdownTimer    changeDirTime;//�´θı䷽����ʱ��

		bool     trackState;//�Ƿ��Ǹ���״̬��true�������߲�ת��
		cocos2d::CCArray *trackPath;//����·��
		int trackPathStep;//����·������
};

