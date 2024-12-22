#ifndef _MAPSELECTCOCOSBUILDERLAYERLOADER_H_
#define _MAPSELECTCOCOSBUILDERLAYERLOADER_H_

#include "MapSelectCocosBuilderLayer.h"

/* Forward declaration. */
class CCBReader;

class MapSelectCocosBuilderLoader : public cocos2d::extension::CCLayerLoader {
    public:
        CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(MapSelectCocosBuilderLoader, loader);

    protected:
        CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(MapSelectCocosBuilderLayer);
};

#endif