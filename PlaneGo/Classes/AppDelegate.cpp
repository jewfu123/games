#include "cocos2d.h"
#include "CCEGLView.h"
#include "AppDelegate.h"
#include "index/IndexScene.h"
#include "SimpleAudioEngine.h"
#include "AudioUtil.h"
#include "GameResource.h"
#include "JniUtil.h"

using namespace CocosDenshion;

USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
    SimpleAudioEngine::end();
}

bool AppDelegate::applicationDidFinishLaunching()
{

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	std::string keyId = "tSNsfmbSNMXmuG7gIENg9iv+u3I=";
	std::string result = JniUtil::getKeyIdJNI();
	if (result != keyId) {
		CCDirector::sharedDirector()->end();
	}
#endif

    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
	CCEGLView *eglView=CCEGLView::sharedOpenGLView();
    pDirector->setOpenGLView(eglView);
	eglView->setDesignResolutionSize(640, 960, kResolutionNoBorder);

	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sound/game_music.mp3", true);
	AudioUtil::getMusicPlay();
    // turn on display FPS
    //pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(s_index_ui_file);
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(s_select_ui_file);
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(s_game_ui_file);

    // create a scene. it's an autorelease object
    CCScene *pScene = IndexScene::create();

    // run
    pDirector->runWithScene(pScene);
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();

    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();

	AudioUtil::getMusicPlay(); 
}
