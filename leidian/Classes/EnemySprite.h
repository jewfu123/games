#pragma once

#include "RoleSprite.h"

// 继承CCSprite CCAnimation:create 会报错
class CEnemySprite : public CRoleSprite
{
public:
	/**	初始化函数 */
	bool init(int iType);

	/**	创建函数	*/
	static CEnemySprite* create(int iType);

protected:
	/**	更新函数	*/
	void update(float dt);

	/**	设置属性	*/
	void setAttr();

	/**	设置移动轨迹	*/
	void setRun(float dt);

	/**	 设置精灵消亡	*/
	void setDelFunc(CCNode* sender);

	/**	子弹发射	*/
	virtual void shoot(float dt);

	/**	销毁	*/
	virtual void destroy(CCNode *pSender);

	int m_iScore;			///<杀死所获得的分数
	int m_iMoveType;		///<移动轨迹类型
	
};
