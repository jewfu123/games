#include "AudioUtil.h"
#include "KeyId.h"

static bool _isPlay;

bool AudioUtil::getMusicPlay(){
	_isPlay=CCUserDefault::sharedUserDefault()->getBoolForKey(KEY_MUSIC, true);
	if(_isPlay){
		SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	}else{ 
		SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	}
	return _isPlay;
    
}
void AudioUtil::setMusicPlay(bool isPlay){
	_isPlay=isPlay;
	if(_isPlay){
		SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	}else{
		SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	}
	CCUserDefault::sharedUserDefault()->setBoolForKey(KEY_MUSIC, isPlay);
	CCUserDefault::sharedUserDefault()->flush();
}

void AudioUtil::loadEffect(){
	SimpleAudioEngine::sharedEngine()->preloadEffect("sound/menu_out.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("sound/menu_in.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("sound/bomb0.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("sound/bomb1.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("sound/coin.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("sound/up_level.mp3");
}
void AudioUtil::soundMenuInOut(int state){
	if(_isPlay){
		if(state==0){
			SimpleAudioEngine::sharedEngine()->playEffect("sound/menu_out.mp3");
		}else if(state==1){
			SimpleAudioEngine::sharedEngine()->playEffect("sound/menu_in.mp3");
		}
	}
}
void AudioUtil::soundBomb(int type){
	if(_isPlay){
		SimpleAudioEngine::sharedEngine()->playEffect(CCString::createWithFormat("sound/bomb%d.mp3", type)->getCString());
	}
}
void AudioUtil::soundGift(){
	if(_isPlay){
		SimpleAudioEngine::sharedEngine()->playEffect("sound/coin.mp3");
	}
}
	
void AudioUtil::soundUpLevel(){
	if(_isPlay){
		SimpleAudioEngine::sharedEngine()->playEffect("sound/up_level.mp3");
	}
}