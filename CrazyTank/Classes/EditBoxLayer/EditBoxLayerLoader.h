#ifndef _EDITCOCOSBUILDERLAYERLOADER_H_
#define _EDITCOCOSBUILDERLAYERLOADER_H_

#include "EditBoxLayer.h"

/* Forward declaration. */
class CCBReader;

class EditBoxLayerLoader : public cocos2d::extension::CCLayerLoader {
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(EditBoxLayerLoader, loader);

protected:
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(EditBoxLayer);
};

#endif
