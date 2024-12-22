
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

	CountdownTimer    showTime;//����ʱ����ʱ��
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
	cocos2d::CCPoint        boxPosLayer;//box��layer�ϵ�λ��

	int  getIndex();
	void setPoropsAction();

	void setName(char *name);
	virtual   const char*   getName();

	void setDestroy(bool destroy);
	virtual     bool         getDestroy(); //�ж��Ƿ�����

	void setLayerName(const char *name);
	virtual    const char*   getLayerName(); //���ڲ������

	void   setBoxPosLayer(cocos2d::CCPoint pos);//box��layer�ϵ�λ��
	cocos2d::CCPoint getBoxPosLayer();

	void setExplode();//��ը
	void end(float time);
	void goEnd();

	void startTime();//ʱ�俪ʼ
	void pausedTime();//ʱ����ͣ
};

#endif