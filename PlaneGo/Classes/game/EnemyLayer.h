#ifndef EnemyLayer_h__
#define EnemyLayer_h__
#include "cocos2d.h"
#include "EnemySprite.h"
USING_NS_CC;

class EnemyLayer : public CCLayer {
public:
	virtual bool init();
	CREATE_FUNC(EnemyLayer);
	void removeEnemy(EnemySprite *_enemy);
	void stopCreatEnemy();
	void startCreateEnemy();
	void goBoss(int type);
	CC_SYNTHESIZE(int , level, Level);
	
private:
	int bossCount;
	CCSpriteBatchNode *batchNode;
	void autoCreateEnemy(float dt);
	virtual void onExit();
	
};


#endif // EnemyLayer_h__
