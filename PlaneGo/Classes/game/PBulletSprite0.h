#ifndef PBulletSprite0_h__
#define PBulletSprite0_h__
#include "cocos2d.h"
USING_NS_CC;

class PBulletSprite0 : public CCSprite {
public:
	static PBulletSprite0* createBullet(int _state, CCPoint _position, float _rotation, float _scale);
private:
	float scale;
	int state;
	float speed;
	float roation;
	void bulletInit(int _state,CCPoint _position, float _rotation, float _scale);
	void update(float time);
	virtual void onExit();
};

#endif // PBulletSprite0_h__
