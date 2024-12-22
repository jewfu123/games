#ifndef _CASHSELECTCOCOSBUILDERLAYERLOADER_H_
#define _CASHSELECTCOCOSBUILDERLAYERLOADER_H_

#include "cashShopCocosBuilderLayer.h"

/* Forward declaration. */
class CCBReader;

class cashShopCocosBuilderLayerLoader : public cocos2d::extension::CCLayerLoader {
    public:
        CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(cashShopCocosBuilderLayerLoader, loader);

    protected:
        CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(cashShopCocosBuilderLayer);
};

#endif