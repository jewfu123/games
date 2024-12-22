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

	int            lv;//���׵ȼ�
	char*          name;
	bool           destroy;
	const char*    layerName;

	void    myInit(int _lv);
	static landMine* spriteCreate(int _lv);

	void setName(char *name);
	virtual   const char*   getName();

	void setDestroy(bool destroy);
	virtual     bool         getDestroy(); //�ж��Ƿ�����

	void setLayerName(const char *name);
	virtual    const char*   getLayerName(); //���ڲ������

	int  getLandMineAttack();//�����˺�ֵ

	void setState(int state);
	void goEnd();

	int landMineState;//״̬

	 enum bulletState{//�ӵ�״̬

		Normal,//����
		EXPLODE = 2,//��ը
		END = 3,//����
	};
};

#endif