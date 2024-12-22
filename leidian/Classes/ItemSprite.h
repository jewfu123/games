#pragma once
#include "UnitSprite.h"

class CItemSprite : public CUnitSprite
{
public:
	/**	初始化函数	*/
	bool init(int iType, float x, float y);

	/**	创建函数	*/
	static CItemSprite* create(int iType, float x, float y);

private:
	/**	更新函数	*/
	void update(float dt);

	/**	随机移动	*/
	void randomMove(float dt);

	/**	设置属性	*/
	void setAttr();

	/**	 设置精灵消亡	*/
	void setDelFunc(CCNode* sender);

	/**	改变宝石类型	*/
	void changeType(float dt);
	
	/**	设置宝石的动画	*/
	void setAnimation();

	int m_iType;			///<物品类型
	int m_iScore;			///<得到该物品所得的分数
};