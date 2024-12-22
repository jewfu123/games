#pragma once
#include "UnitSprite.h"

enum
{
	LEFT,
	STRAIGHT,
	RIGHT,
};
enum
{
	PLAYER,
	ENEMY,
};

class CRoleSprite : public CUnitSprite
{
public:
	/**	受伤害	*/
	void hurt(int iHarm);

	/**	是否销毁	*/
	bool isDestroy(){return m_bDestroy;}

protected:
	/**	设置飞行动画	*/
	void setFlyAnimation(int iDirection);

	/**	 设置精灵消亡	*/
	void setDelFunc(CCNode* sender);

	/**	发射子弹	*/
	virtual void shoot(float dt) = 0;

	/**	爆炸	*/
	void explosion();

	/**	销毁	*/
	virtual void destroy(CCNode *pSender) = 0;

	/**	改变飞行动画	*/
	void changeFlyAnimation(float dt);

	/**	获取飞行方向	*/
	int getDirection();

	int m_iHpMax;			///<HP上限
	int m_iHp;				///<HP
	int m_iType;			///<飞机类型
	bool m_bDestroy;		///<是否销毁
	float m_fShootSpace;	///<发射子弹间隔
	CCPoint m_pos;			///<初始化坐标
	int m_iBulletType;		///<子弹类型
	int m_iFlyStraightNum;	///<直飞动作帧数
	int m_iFlyLeftNum;		///<左飞动作帧数
	int m_iLife;			///<生命数
	int m_iDirection;		///<飞行方向
	int m_iBulletNum;		///<子弹数量
	CCSprite *m_pExplosion;	///<爆炸精灵
	
};
