#ifndef __ENEMYSCENE_SCENE_H__
#define __ENEMYSCENE_SCENE_H__

#include "cocos2d.h"
#include "userdataconsistencyccsprite.h"
#include "cocos-ext.h"
#include "xml/XMLParser.h"
#include "enemyTank.h"
#include "Tool.h"


class enemyBullet:public UserDataConsistencyCCSprite
{
private:
	cocos2d::extension::CCBReader  * ccbreader;

	int   penetrate;//��͸
	int   critRate;//��������
	//float rateOfFire;//�ӵ�����
	int   bulletStrength;//�ӵ�ǿ��
	int   bulletRange;//�ӵ���Χ
	int   bulletAttack;//�ӵ�������
	int   bulletSpeed;//�ӵ��ٶ�
	int   king;   //�ӵ�����

	bool  critRateBool;//�Ƿ��Ǳ���
public:
	enemyBullet(void);
	~enemyBullet(void);

    void    myInit(enemyTank* _enemyTank);
	static  enemyBullet* spriteCreate(enemyTank* _enemyTank);
	void    setBulletAction();

	void setName(char *name);
	virtual   const char*   getName();

	void setDestroy(bool destroy);
	virtual     bool         getDestroy(); //�ж��Ƿ�����

	void setLayerName(const char *name);
	virtual    const char*   getLayerName(); //���ڲ������

	int   getBulletSpeed();//�ӵ��ƶ��ٶ�
	int   getBulletAttack();//�ӵ��Ĺ�����
	int   getBullStrenght();

	void setState(int state);
	int  getState();
	void goEnd();

	bool  getCritRateBool();//�Ƿ��Ǳ���

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
		END = 6,//����

		
	};

};


#endif
