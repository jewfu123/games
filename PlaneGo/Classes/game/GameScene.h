#ifndef GameScene_h__
#define GameScene_h__
#include "cocos2d.h"
USING_NS_CC;

class MapLayer;
class EnemyLayer;
class PlayerLayer;
class GameMenuLayer;
class EnemySprite;
class BulletSprite;
class PlaneSprite;

class GameScene : public CCScene {
public:
	static GameScene *sharedWorld();
	static GameScene *create(int _num, int _type, int _level);
	bool init(int _num, int _type, int _level);
	CC_SYNTHESIZE(CCArray*, enemys, Enemys);
	CC_SYNTHESIZE(int , num, Num);
	CC_SYNTHESIZE(int , type, Type);
	CC_SYNTHESIZE(int , level, Level);
	bool killEnemy(EnemySprite *_enemy, int value);
	PlaneSprite* getPlayer();
	void killPlayer();
	void gameBack();
	void gameAgain();
	void gameNext();
	void gamePause();
	void gameResume();
	void toSelect();
	void showSetting();
	void showSucceed();
	void showFail();
	void reduceCoin(int _coin);
	void goBoss(int type);
	void bomb();
	void upLevel();
	void addCoin(int coin);
	void addBomb();
	~GameScene();
private:
	int life;
	MapLayer *mapLayer;
	EnemyLayer *enemyLayer;
	PlayerLayer *playerLayer;
	GameMenuLayer *menuLayer;
	void resumePoint();
};


#endif // GameScene_h__
