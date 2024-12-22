
#ifndef __PROPSSCENE_SCENE_H__
#define __PROPSSCENE_SCENE_H__

#include "cocos2d.h"
#include "UserDataConsistencyCCSprite.h"
#include "cocos-ext.h"
#include "Box2D/Box2D.h"
#include "DrawDebug/GLES-Render.h"
#include "SimpleTimer/SimpleTimer.h"

class Props :public UserDataConsistencyCCSprite
{
private :
	//cocos2d::extension::CCBReader  *explodeBreader;

	CountdownTimer    showTime;//出现时间间隔时间
public:
	Props(void);
	~Props(void);

	cocos2d::extension::CCBReader *propsBreader;

	void myInit(int bfitemIndex);
	static Props* spriteCreate(int bfitemIndex);//cocos2d::CCSpriteFrame *pSpriteFrame

	int                     index;
	bool                    destroy;
	char*                   name;
	const char*             layerName;
	cocos2d::CCPoint        boxPosLayer;//box在layer上的位置

	int  getIndex();
	void setPoropsAction();

	void setName(char *name);
	virtual   const char*   getName();

	void setDestroy(bool destroy);
	virtual     bool         getDestroy(); //判断是否销毁

	void setLayerName(const char *name);
	virtual    const char*   getLayerName(); //所在层的名字

	void   setBoxPosLayer(cocos2d::CCPoint pos);//box在layer上的位置
	cocos2d::CCPoint getBoxPosLayer();

	void setExplode();//爆炸
	void end(float time);
	void goEnd();

	void startTime();//时间开始
	void pausedTime();//时间暂停
};

#endif