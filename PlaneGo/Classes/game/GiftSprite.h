#ifndef GiftSprite_h__
#define GiftSprite_h__
#include "cocos2d.h"
USING_NS_CC;

class GiftSprite : public CCSprite{
public:
	static GiftSprite* create(int _type, CCPoint point);
private:
	int type;
	void init(int _type, CCPoint point);
	void update(float time);
};


#endif // GiftSprite_h__
