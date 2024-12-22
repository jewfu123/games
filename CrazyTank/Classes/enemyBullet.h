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

	int   penetrate;//穿透
	int   critRate;//暴击几率
	//float rateOfFire;//子弹射速
	int   bulletStrength;//子弹强度
	int   bulletRange;//子弹范围
	int   bulletAttack;//子弹攻击力
	int   bulletSpeed;//子弹速度
	int   king;   //子弹种类

	bool  critRateBool;//是否是暴击
public:
	enemyBullet(void);
	~enemyBullet(void);

    void    myInit(enemyTank* _enemyTank);
	static  enemyBullet* spriteCreate(enemyTank* _enemyTank);
	void    setBulletAction();

	void setName(char *name);
	virtual   const char*   getName();

	void setDestroy(bool destroy);
	virtual     bool         getDestroy(); //判断是否销毁

	void setLayerName(const char *name);
	virtual    const char*   getLayerName(); //所在层的名字

	int   getBulletSpeed();//子弹移动速度
	int   getBulletAttack();//子弹的攻击力
	int   getBullStrenght();

	void setState(int state);
	int  getState();
	void goEnd();

	bool  getCritRateBool();//是否是暴击

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
		END = 6,//结束

		
	};

};


#endif
