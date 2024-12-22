#include "cocos2d.h"
#include "CCEGLView.h"
#include "AppDelegate.h"
#include "SimpleAudioEngine.h"
#include "MenuLayer.h"
#include "GCLayer.h"
#include "PauseLayer.h"
#include "GameScene.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
#include <jni.h>
#include "platform/android/jni/JniHelper.h" 
#include <android/log.h> 
#endif 

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

	//_tsetlocale(LC_ALL,_T("")); 
    //::AllocConsole(); 
    //::freopen("conout$","w",stdout); 

    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

	 /*CCSize frameSize = CCEGLView::sharedOpenGLView()->getFrameSize();
	// ���� LsSize �̶�ֵ
	CCSize lsSize = CCSizeMake(800, 480);

	float scaleX = (float) frameSize.width / lsSize.width;
	float scaleY = (float) frameSize.height / lsSize.height;

	// ���� scale ����
	float scale = 0.0f; // MAX(scaleX, scaleY);
	if (scaleX > scaleY) {
		// ����� X ����ƫ����ô scaleX ��Ҫ����һ���Ŵ�ϵ�����Ŵ�ϵ���������ȷ����ȡ��
		// ��Ϊ��ʱ FrameSize �� LsSize �����±����ص���
		scale = scaleX / (frameSize.height / (float) lsSize.height);
	} else {
		scale = scaleY / (frameSize.width / (float) lsSize.width);
	}

	CCLog("x: %f; y: %f; scale: %f", scaleX, scaleY, scale);

	// ���� LsSize ����Ļ��߱ȶ�̬�趨 WinSize
	CCEGLView::sharedOpenGLView()->setDesignResolutionSize(lsSize.width * scale,
		lsSize.height * scale, kResolutionNoBorder);*/


	CCEGLView::sharedOpenGLView()->setDesignResolutionSize(800, 480,kResolutionExactFit);  
    // turn on display FPS
  //  pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
  //  pDirector->setAnimationInterval(1.0 / 60);


	
	CCScene *scene = CCScene::create();
	//MenuLayer *node = MenuLayer::create();
	GCLayer *node = GCLayer::create();
	scene->addChild(node);
    //run
    pDirector->runWithScene(scene);

/*#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  

	 JniMethodInfo minfo;
	 CCLog("============"); 
	 bool isHave = JniHelper::getMethodInfo(minfo,"quyou/game/tank/tank","show_java_dialog", "(I)I");//"com/game/test/JniFruitHelper","show_java_dialog"
	 CCLog("------------"); 
	 jint _int=0;
	 if (isHave) {
		 CCLog("jni-java����ִ�����"); 
		// _int = minfo.env->CallStaticIntMethod(minfo.classID, minfo.methodID,1);//
		 minfo.env->CallVoidMethod(minfo, classID,  minfo.methodID,1);

	 }else{
		 CCLog("jjni:�˺�������"); 
	 }
#endif*/


    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
   // CCDirector::sharedDirector()->stopAnimation();


	CCLog("millisecondNow:%d",(int)regainLifeTime.getSurplusMillis());
	SaveIntegerToXML("survival_time",(int)(survivalModeTime.getSurplusMillis()+millisecondNow()));
	//SaveIntegerToXML("survival_time",millisecondNow());
	CCLog("leaveGame_time:%lu",millisecondNow());
	SaveIntegerToXML("leaveGame_time",millisecondNow());
	SaveIntegerToXML("regainLife_time",(int)regainLifeTime.getSurplusMillis());

	CCUserDefault::sharedUserDefault()->flush();//���ݱ���


    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();

	//if(puuseLayerShow){
		SimpleAudioEngine::sharedEngine()->pauseAllEffects();
		(CCDirector::sharedDirector()->getRunningScene())->pauseSchedulerAndActions();
		if((CCDirector::sharedDirector()->getRunningScene())->getTag()!=10000){
			sceneIndex = 0;


			if((CCDirector::sharedDirector()->getRunningScene())->getTag()==30000){
				CCLog("-----------30000");
				// ((GameScene*)((CCDirector::sharedDirector()->getRunningScene())->getChildByTag(1)))->pausedScene();
				if(!paused){
					((UIlayer*)((CCDirector::sharedDirector()->getRunningScene())->getChildByTag(2)))->pauseMenuCallback(CCDirector::sharedDirector()->getRunningScene());
					sceneIndex = 30000;
				}
				
			}

			CCScene* pause  = PauseLayer::scene();
			pause->setTag(10000);
			CCDirector::sharedDirector()->pushScene(pause);
		}
	//}
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    //CCDirector::sharedDirector()->startAnimation();

    //SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
