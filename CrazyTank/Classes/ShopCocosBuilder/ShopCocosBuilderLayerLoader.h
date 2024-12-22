#ifndef _MAPSELECTCOCOSBUILDERLAYERLOADER_H_
#define _MAPSELECTCOCOSBUILDERLAYERLOADER_H_

#include "ShopCocosBuilderLayer.h"

/* Forward declaration. */
class CCBReader;

class ShopCocosBuilderLayerLoader : public cocos2d::extension::CCLayerLoader {
    public:
        CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(ShopCocosBuilderLayerLoader, loader);

    protected:
        CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(ShopCocosBuilderLayer);
};

#endif