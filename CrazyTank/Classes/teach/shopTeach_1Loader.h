#ifndef _SHOPTEACHLAYERLOADER_H_
#define _SHOPTEACHLAYERLOADER_H_

#include "shopTeach_1.h"

/* Forward declaration. */
class CCBReader;

class shopTeach_1Loader : public cocos2d::extension::CCLayerLoader {
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(shopTeach_1Loader, loader);

protected:
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(shopTeach_1);
};

#endif