
#ifndef __BULLETSCENE_SCENE_H__
#define __BULLETSCENE_SCENE_H__

#include "cocos2d.h"
#include "userdataconsistencyccsprite.h"
#include "cocos-ext.h"
#include "xml/XMLParser.h"
#include "Tool.h"
#include "SimpleTimer/SimpleTimer.h"
#include "UserRecord/UserRecord.h"

class Bullet :public UserDataConsistencyCCSprite
{
private:
	cocos2d::extension::CCBReader  * ccbreader;

	int  bulletSpeed;//������ӵ��ƶ��ٶ�
    int    bulletAttack;//�ӵ��Ĺ�����
	int  bulletRange;//�ӵ������
	int  bulletStrength;//�ӵ�ǿ��
	float  rateOfFire;//����
	int  critRate;//��������
	bool penetrate;//�ӵ��Ƿ���Թᴩ
	static int  weight;//����������

	bool  critRateBool;//�Ƿ��Ǳ���


	CountdownTimer      critTime;//����ʱ��
public:
	 Bullet(void);
	~Bullet(void);

	 

	void    myInit(int turret,int turretGrade);
	static Bullet* spriteCreate(int turret,int turretGrade);
	void    setBulletAction();

	void setName(char *name);
	virtual   const char*   getName();

	void setDestroy(bool destroy);
	virtual     bool         getDestroy(); //�ж��Ƿ�����

	void setLayerName(const char *name);
	virtual    const char*   getLayerName(); //���ڲ������

	int   getBulletSpeed();//�ӵ��ƶ��ٶ�
	int   getBulletAttack();//�ӵ��Ĺ�����
	static int  getWeight();//��ȡ����
	float   getRateOfFire();//�������

	bool  getCritRateBool();//�Ƿ��Ǳ���
	int   getBullStrenght();

	void setState(int state);
	int  getState();
	void goEnd();
	void go5End();

	void static setBulletAttackByLvUp(int _bulletAttack);//���ݵȼ����ù�����

	void setUpCrit();//����������������
	//void upDataCrit(float dt);//��������������ʵ�ʱ��

	int turretKing;//��������
	int turretGrade;
	char*          name;
	bool           destroy;
    const char*    layerName;
	int  bulletState;//״̬
	 enum bulletState{//�ӵ�״̬ 
		moveUp   = 1,//�����ƶ�
		moveDown = 2,//�����ƶ�
		moveLeft = 3,//���ƶ�
		moveRight = 4,//���ƶ�

		EXPLODE = 5,//��ը
		END = 6//����
	};
};

#endif