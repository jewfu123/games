#ifndef _ACHIEVERCOCOSBUILDERLAYERLOADER_H_
#define _ACHIEVERCOCOSBUILDERLAYERLOADER_H_

#include "AchieveCocosBuilder.h"

/* Forward declaration. */
class CCBReader;

class AchieveCocosBuilderLoader : public cocos2d::extension::CCLayerLoader {
    public:
        CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(AchieveCocosBuilderLoader, loader);

    protected:
        CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(AchieveCocosBuilder);
};

#endif