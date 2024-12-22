#pragma once
#include "UnitSprite.h"

class CBulletSprite : public CUnitSprite
{
public:
	/**	初始化函数	*/
	bool init(int iPlaneType, int iType, int iAllNum, int iCurNum, float x, float y);

	/**	创建函数	*/
	static CBulletSprite* create(int iPlaneType, int iType, int iAllNum, int iCurNum, float x, float y);

private:
	/**	更新函数	*/
	void update(float dt);

	/**	 设置精灵消亡	*/
	void setDelFunc(CCNode* sender);
	
	/**	设置属性	*/
	void setAttr();

	/**	设置移动轨迹	*/
	void setMoveTrack(int iAllNum, int iCurNum);

	int m_iType;			///<子弹类型
	int m_iPlaneType;		///<发出子弹的飞机类型，0为玩家，1为敌人
	int m_iHarm;			///<子弹的伤害
};
