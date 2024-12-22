#ifndef PBulletSprite1_h__
#define PBulletSprite1_h__
#include "cocos2d.h"
USING_NS_CC;

class PBulletSprite1 : public CCSprite {
public:
	static PBulletSprite1 *createBullet(int _state, CCPoint point, float r);
	
private:
	int state;
	float radius;
	int speed;
	void initBullet(int _state, CCPoint point, float r);
	void removeOneself();
	virtual void update(float dt);
	virtual void onExit();
};

#endif // EBulletSprite0_h__
