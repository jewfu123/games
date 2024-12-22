#pragma once

#include "RoleSprite.h"

extern int g_iGameLevel;

class CPlayerSprite : public CRoleSprite
{
public:
	/**	初始化函数	*/
	bool init(int iType);

	/**	玩家通关后的初始化	*/
	void initAttrWithWin();
	
	/**	获取生命数	*/
	int getLife(){return m_iLife;}

	/**	获取HP上限值	*/
	int getHpMax(){return m_iHpMax;}

	/**	获取HP值*/
	int getHp(){return m_iHp;}

	/**	获取分数	*/
	int getScore(){return m_iScore;}

	/**	获得物品	*/
	void gainItem(int iType, int iScore);

	/**	增加分数	*/
	void addScore(int iValue);

	/**	是否通关	*/
	bool isWin(){return m_bWin;}

	/**	设置通关后的移动	*/
	void setWinMove();

	/**	设置通关的布尔值*/
	void setWin(bool bValue){m_bWin = bValue;}

	/**	发射导弹	*/
	void shootMissile();

	/**	是否有保护盾	*/
	bool isShield(){return m_bShield;}

	/**	获取导弹数量	*/
	int getMissileNum(){return m_iMissileNum;}

	/**	创建函数	*/
	static CPlayerSprite* create(int iType);

private:
	/**	触摸屏幕	*/
	virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

	/**	划动屏幕	*/
    virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);

	/**	子弹发射	*/
	virtual void shoot(float dt);

	/**	玩家死亡后的初始化	*/
	void initAttrWithDie();

	/**	销毁	*/
	virtual void destroy(CCNode *pSender);

	/**	通关	*/
	void win(CCNode *pSender);

	/**	设置保护盾特效	*/
	void setShield();

	/**	清除保护盾特效	*/
	void clearShield(float dt);

	/**	设置子弹发射特效	*/
	void setShoot();

	/**	清除子弹发射特效	*/
	void clearShoot();
	
	/**	创建导弹	*/
	void createMissile();

	/**	导弹爆炸	*/
	void MissileExplore(CCNode *pSender);

	/**	导弹销毁	*/
	void MissileDestroy(CCNode *pSender);

	/**	导弹对敌人的伤害	*/
	void hurtEnemy();

	CCPoint m_PosBack;					///<备份触摸点坐标
	bool m_bMissile;					///<是否发射导弹中
	int m_iScore;						///<分数
	bool m_bShield;						///<是否保护盾
	bool m_bWin;						///<是否通过
	int m_iAddLifeScore;				///<增加生命所需分数
	int m_iMissileNum;					///<导弹个数
	unsigned int m_iShootSoundId;		///<发射子弹音效ID
	CCActionInterval*  m_pMoveAction;	///<移动
	CCSprite *m_pMissile;				///<导弹精灵
};
