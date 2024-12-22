#ifndef _SHOPTEACHLAYER2LOADER_H_
#define _SHOPTEACHLAYER2LOADER_H_

#include "shopTeach_2.h"

/* Forward declaration. */
class CCBReader;

class shopTeach_2Loader : public cocos2d::extension::CCLayerLoader {
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(shopTeach_2Loader, loader);

protected:
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(shopTeach_2);
};
#endif