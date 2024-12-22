#include "cocos2d.h"
using namespace cocos2d;
class CEffect : public CCLayer
{
public:
	/**	爆炸特效*/
	static void sharedExplosion();
	
	/**	金币闪烁特效	*/
	static void sharedGold();

	/**	创建函数	*/
	CREATE_FUNC(CEffect);

};

