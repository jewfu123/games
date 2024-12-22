#pragma once

#include "EnemySprite.h"

class CBossSprite : public CEnemySprite
{
public:
	/**	初始化函数	*/
	bool init(int iType);
	
	/**	更新函数	*/
	void update(float dt);

	/**	设置属性	*/
	void setAttr();

	/**	设置移动轨迹	*/
	void setRun(float dt);

	/**	子弹发射	*/
	virtual void shoot(float dt);

	/**	子弹2发射	*/
	virtual void shoot2(float dt);

	/**	销毁	*/
	virtual void destroy(CCNode *pSender);

	/**	创建函数	*/
	static CBossSprite* create(int iType);

private:

	int m_iBullet2Num;		///<子弹2数量
	int m_iBullet2Type;		///<子弹2类型
	int m_fShoot2Space;		///<子弹2发射间隔
	bool m_bHpChange1;		///<HP改变1（HP是否小于2/3）
	bool m_bHpChange2;		///<HP改变2（HP是否小于1/3）
};
