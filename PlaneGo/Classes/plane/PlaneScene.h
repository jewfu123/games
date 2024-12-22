#ifndef PlaneScene_h__
#define PlaneScene_h__
#include "cocos2d.h"
USING_NS_CC;

class PlaneScene : public CCScene {
public:
	static PlaneScene *create(int _num);
	bool init(int _num);
};

#endif // PlaneScene_h__
