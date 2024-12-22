#ifndef PBulletSprite2_h__
#define PBulletSprite2_h__
#include "cocos2d.h"
USING_NS_CC;

class EnemySprite;

class PBulletSprite2 : public CCSprite {
public:
	static PBulletSprite2 *createBullet(CCPoint point, EnemySprite *_enemy);
	void initBullet(CCPoint point, EnemySprite *_enemy);
private:
	CCPoint location;
	EnemySprite *enemy; 
	float radius;
	int speed;
	bool isFirst;
	void removeOneself();
	virtual void update(float dt);
	virtual void onExit();
};

#endif // PBulletSprite2_h__
