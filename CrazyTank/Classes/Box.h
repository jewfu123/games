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
	static Box* spriteCreate(const char *pszFileName); //��̬����
	static Box* spriteCreate(cocos2d::CCSpriteFrame *pSpriteFrame);

	bool                    destroy;
	char*                   name;
	const char*             layerName;
	cocos2d::CCPoint        boxPosLayer;//box��layer�ϵ�λ��

	void setName(char *name);
    virtual   const char*   getName();

	void setDestroy(bool destroy);
	virtual     bool         getDestroy(); //�ж��Ƿ�����

	 void setLayerName(const char *name);
	 virtual    const char*   getLayerName(); //���ڲ������

	 void   setBoxPosLayer(cocos2d::CCPoint pos);//box��layer�ϵ�λ��
	 cocos2d::CCPoint getBoxPosLayer();

	 void  beAttack(int attack,CCObject *rec,cocos2d::SEL_CallFuncN selector);//�ܵ�������,attack�ܵ��Ĺ�����

	 void setStrength(int streng);//����ǿ��
     int  getStrength();//ǿ��
	

	void  setId(int id);
	int  getId();
private:
	bool     SEL_Bool;//�ص�����ִֻ��һ��


	int     id;//box����	
	int     lifeNum;//����ֵ
	int     strength;//ǿ�ȣ�ֻ���ӵ����ڴ�ֵ�Ż����˺�

	//CCObject*       m_pListener;
	//cocos2d::SEL_CallFuncN    m_pfnSelector;//��ִ���ߵĻص�
	
};

#endif