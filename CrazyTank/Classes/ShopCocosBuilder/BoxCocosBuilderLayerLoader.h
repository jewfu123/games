#ifndef _BOXCOCOSBUILDERLAYERLOADER_H_
#define _BOXCOCOSBUILDERLAYERLOADER_H_

#include "BoxCocosBuilderLayer.h"

/* Forward declaration. */
class CCBReader;

class BoxCocosBuilderLayerLoader : public cocos2d::extension::CCLayerLoader {
    public:
        CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(BoxCocosBuilderLayerLoader, loader);

    protected:
        CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(BoxCocosBuilderLayer);
};

#endif