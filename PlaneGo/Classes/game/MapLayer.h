#ifndef MapLayer_h__
#define MapLayer_h__
#include "cocos2d.h"
USING_NS_CC;

class MapLayer : public CCLayer {
public:
	virtual bool init();
	CREATE_FUNC(MapLayer);
private:
	float speed;
	virtual void update(float dt);
	virtual void onExit();
};



#endif // MapLayer_h__
