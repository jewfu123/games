
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

	int  bulletSpeed;//发射的子弹移动速度
    int    bulletAttack;//子弹的攻击力
	int  bulletRange;//子弹的射程
	int  bulletStrength;//子弹强度
	float  rateOfFire;//射速
	int  critRate;//暴击几率
	bool penetrate;//子弹是否可以贯穿
	static int  weight;//炮塔的重量

	bool  critRateBool;//是否是暴击


	CountdownTimer      critTime;//暴击时间
public:
	 Bullet(void);
	~Bullet(void);

	 

	void    myInit(int turret,int turretGrade);
	static Bullet* spriteCreate(int turret,int turretGrade);
	void    setBulletAction();

	void setName(char *name);
	virtual   const char*   getName();

	void setDestroy(bool destroy);
	virtual     bool         getDestroy(); //判断是否销毁

	void setLayerName(const char *name);
	virtual    const char*   getLayerName(); //所在层的名字

	int   getBulletSpeed();//子弹移动速度
	int   getBulletAttack();//子弹的攻击力
	static int  getWeight();//获取重量
	float   getRateOfFire();//获得射速

	bool  getCritRateBool();//是否是暴击
	int   getBullStrenght();

	void setState(int state);
	int  getState();
	void goEnd();
	void go5End();

	void static setBulletAttackByLvUp(int _bulletAttack);//根据等级设置攻击力

	void setUpCrit();//道具提升暴击几率
	//void upDataCrit(float dt);//检测提升暴击几率的时间

	int turretKing;//炮塔种类
	int turretGrade;
	char*          name;
	bool           destroy;
    const char*    layerName;
	int  bulletState;//状态
	 enum bulletState{//子弹状态 
		moveUp   = 1,//向上移动
		moveDown = 2,//向下移动
		moveLeft = 3,//左移动
		moveRight = 4,//右移动

		EXPLODE = 5,//爆炸
		END = 6//结束
	};
};

#endif