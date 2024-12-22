#ifndef PBulletSprite3_h__
#define PBulletSprite3_h__
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

class PBulletSprite3 : public CCScale9Sprite { 
public:
	static PBulletSprite3* createBullet(int _state, CCPoint point, int _level);
	CC_SYNTHESIZE(int, level, Level);
private:
	int state;
	void bulletInit(int _state, CCPoint point, int _level);
	virtual void update(float time);
	virtual void onExit();
};

#endif // PBulletSprite3_h__
