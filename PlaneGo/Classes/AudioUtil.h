#ifndef AudioUtil_h__
#define AudioUtil_h__
#include "cocos2d.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

class AudioUtil {
public:
	static bool getMusicPlay();
	static void setMusicPlay(bool isPlay);
	static void loadEffect();
	/**菜单进出的音效，state：0为出，state：1为进*/
	static void soundMenuInOut(int state);
	static void soundBomb(int type);
	static void soundGift();
	static void soundUpLevel();
};

#endif // AudioUtil_h__
