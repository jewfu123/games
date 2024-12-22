#ifndef PlaneSprite_h__
#define PlaneSprite_h__
#include "cocos2d.h"
USING_NS_CC;

class PlaneSprite : public CCSprite {
public:
	static PlaneSprite* createPlane(int _type, int _level);
	CC_SYNTHESIZE(bool , isAlive, IsAlive)
private:
	int type;
	int level;
	void planeInit(int _type, int _level);
	void setAlive();
};

#endif // PlaneSprite_h__
