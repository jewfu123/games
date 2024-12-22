#ifndef _HELLOCOCOSBUILDERLAYERLOADER_H_
#define _HELLOCOCOSBUILDERLAYERLOADER_H_

#include "MenuCocosBuilderLayer.h"

/* Forward declaration. */
class CCBReader;

class MenuCocosBuilderLayerLoader : public cocos2d::extension::CCLayerLoader {
    public:
        CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(MenuCocosBuilderLayerLoader, loader);

    protected:
        CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(MenuCocosBuilderLayer);
};

#endif
