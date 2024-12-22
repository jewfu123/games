#ifndef _OPTIONCOCOSBUILDERLAYERLOADER_H_
#define _OPTIONCOCOSBUILDERLAYERLOADER_H_

#include "option.h"

/* Forward declaration. */
class CCBReader;

class optionLoader : public cocos2d::extension::CCLayerLoader {
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(optionLoader, loader);

protected:
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(option);
};

#endif
