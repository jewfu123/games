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

	int tankState;//̹��״̬
	cocos2d::extension::CCBReader  *ccbreader;
	cocos2d::extension::CCBReader  *firebreader;//����
	cocos2d::extension::CCBReader  *lvupbreader;//����
	cocos2d::extension::CCBReader  *godbreader;//�޵�
	cocos2d::extension::CCBReader  *tankExplodeBreader;//��ը
	cocos2d::extension::CCBReader  *cureBreader;//��Ѫ
	cocos2d::extension::CCBReader  *moveSpeedBreader;//����
	cocos2d::extension::CCBReader  *criticalBreader;//����
	
	//cocos2d::extension::CCBReader  *newBirthBreader;//����ʱ���޵�

	//CountdownTimer    godTime;//�޵�ʱ��
	bool              godBool;//�Ƿ����޵�״̬
	bool              newBirthGod;//�����޵�
	//CountdownTimer    moveSpeedTime;//�����ٶ�ʱ��

	 bool  loseBloodNumDirection;//����
public:
	 tanker(void);
	~tanker(void);

	  void myInit(int chassis,int turret,int chassisGrade,int turretGrade);
	  static tanker* spriteCreate(int chassis,int turret,int chassisGrade,int turretGrade); //��̬����

	  void   setTankerAction();//������ʼ��

	 // virtual void onEnter();    
	 // virtual void onExit();

	  char*    name;
	  bool      destroy;
	  const char*    layerName;

	  void setName(char *name);
	  virtual   const char*   getName();

	  void setDestroy(bool destroy);
	  virtual     bool         getDestroy(); //�ж��Ƿ�����

	  void setLayerName(const char *name);
	  virtual    const char*   getLayerName(); //���ڲ������


	  void  beAttack(int attack,bool critRateBool);//������
	  void  goEnd();

	  void  loseBloodNumEnd(CCNode* sender);
	  void  loseBloodCritNumEnd(CCNode* sender);

	  void setState(int state);//���÷���
	  int  getState();
	  void openFire();//̹�˿���

	  void setLvUp(int lv);//���������ƶ��ٶȣ�������

	  void addFullLift();//��������ֵ
	  void upDataTankCure(float time);

	  void setGod();//�����޵�״̬
	  bool isGod();//�Ƿ��޵�״̬
	  void upDataGod(float dt);//����޵�״̬��ʱ��

	  void setPayGod();//�����޵�

	  void setNewBirth();//����������
	  void upDataNewBirth(float dt);//

	  void setMoveSpeed();//���������ٶ�
	  void upDataMoveSpeed(float dt);//��������ٶȵ�ʱ��

	  void setCritical();//������������
	  void upDataCritical(float dt);//�������������ʱ��

	  void goLeft();

	 int chassisKind;//��������
	 int turretKing;//��������
	 int chassisGrade;
	 int turretGrade;
	

	 int   defense;//����
	 int   lifeNum;//����ֵ
	 float moveSpeed;//�ƶ��ٶ�
	 int  weight;

	 int   MAXLife;//�������ֵ
	 float recordMoveSpeed;//�ƶ��ٶ�

	 int getMAXLift();//�õ��������ֵ
	 int  getLife();
	 float getMoveSpeed();//����ƶ��ٶ�
	 
	  enum tankState{
		  moveUp   = 1,//�����ƶ�
		  moveDown = 2,//�����ƶ�
		  moveLeft = 3,//���ƶ�
		  moveRight = 4,//���ƶ�

		  holdUp = 5,//��ֹ
		  holdDown=6,//��ֹ
		  holdLeft=7,//��ֹ
		  holdRight=8,//��ֹ

		  explode = 9//��ը,����
	 }; 
};
#endif
