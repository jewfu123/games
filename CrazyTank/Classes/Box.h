#pragma once
#ifndef __BOXSCENE_SCENE_H__
#define __BOXSCENE_SCENE_H__

#include "cocos2d.h"
#include "UserDataConsistencyCCSprite.h"

class Box:public UserDataConsistencyCCSprite
{
public:
	~Box(void);
	void myInit(void);
	static Box* spriteCreate(const char *pszFileName); //静态创建
	static Box* spriteCreate(cocos2d::CCSpriteFrame *pSpriteFrame);

	bool                    destroy;
	char*                   name;
	const char*             layerName;
	cocos2d::CCPoint        boxPosLayer;//box在layer上的位置

	void setName(char *name);
    virtual   const char*   getName();

	void setDestroy(bool destroy);
	virtual     bool         getDestroy(); //判断是否销毁

	 void setLayerName(const char *name);
	 virtual    const char*   getLayerName(); //所在层的名字

	 void   setBoxPosLayer(cocos2d::CCPoint pos);//box在layer上的位置
	 cocos2d::CCPoint getBoxPosLayer();

	 void  beAttack(int attack,CCObject *rec,cocos2d::SEL_CallFuncN selector);//受到攻击后,attack受到的攻击力

	 void setStrength(int streng);//设置强度
     int  getStrength();//强度
	

	void  setId(int id);
	int  getId();
private:
	bool     SEL_Bool;//回调函数只执行一次


	int     id;//box种类	
	int     lifeNum;//生命值
	int     strength;//强度，只有子弹大于此值才会有伤害

	//CCObject*       m_pListener;
	//cocos2d::SEL_CallFuncN    m_pfnSelector;//带执行者的回调
	
};

#endif