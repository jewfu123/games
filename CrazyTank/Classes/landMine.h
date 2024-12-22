#ifndef __LANDMINESCENE_SCENE_H__
#define __LANDMINESCENE_SCENE_H__

#include "cocos2d.h"
#include "userdataconsistencyccsprite.h"
#include "cocos-ext.h"
#include "xml/XMLParser.h"
#include "Tool.h"


class landMine :public UserDataConsistencyCCSprite
{
private:

	cocos2d::extension::CCBReader  * ccbreader;

public:
	landMine(void);
	~landMine(void);

	int            lv;//地雷等级
	char*          name;
	bool           destroy;
	const char*    layerName;

	void    myInit(int _lv);
	static landMine* spriteCreate(int _lv);

	void setName(char *name);
	virtual   const char*   getName();

	void setDestroy(bool destroy);
	virtual     bool         getDestroy(); //判断是否销毁

	void setLayerName(const char *name);
	virtual    const char*   getLayerName(); //所在层的名字

	int  getLandMineAttack();//地雷伤害值

	void setState(int state);
	void goEnd();

	int landMineState;//状态

	 enum bulletState{//子弹状态

		Normal,//正常
		EXPLODE = 2,//爆炸
		END = 3,//结束
	};
};

#endif