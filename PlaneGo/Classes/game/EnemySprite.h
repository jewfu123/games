#ifndef EnemySprite_h__
#define EnemySprite_h__
#include "cocos2d.h"
USING_NS_CC;

class EnemySprite : public CCSprite
{
public:
	static EnemySprite* createEnemy(int _type, bool _isBoss=false);
	bool attackEnemy(int value);
    void setStop(int _time);
	CC_SYNTHESIZE_READONLY(int , coin, Coin);
	CC_SYNTHESIZE_READONLY(int , score, Score);
	CC_SYNTHESIZE_READONLY(int , type, Type);
	CC_SYNTHESIZE(bool , lock, Lock);
	CC_SYNTHESIZE_READONLY(bool , isBoss, IsBoss);
private:
	bool isStop;
	int timeStop;
	int maxHp;
	int speed;
	int bossX, bossY;
	bool isActed;
	virtual void onExit();
	void enemyInit(int _type, bool _isBoss);
	void update(float time);
	void autoCreateBullet(float dt);
	
};
#endif // EnemySprite_h__
