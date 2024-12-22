#ifndef _MAGAZINECTCOCOSBUILDERLAYERLOADER_H_
#define _MAGAZINECTCOCOSBUILDERLAYERLOADER_H_

#include "magazine_frameCocosBuilderLayer.h"
class CCBReader;

class magazine_frameCocosBuilderLayerLoader : public cocos2d::extension::CCLayerLoader 
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(magazine_frameCocosBuilderLayerLoader, loader);

protected:
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(magazine_frameCocosBuilderLayer);
};
#endif
