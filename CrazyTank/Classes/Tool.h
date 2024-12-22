#pragma once

#ifndef _TOOLS_H_   
#define  _TOOLS_H_   
#include "cocos2d.h" 
#include "SimpleTimer/SimpleTimer.h" 


bool PointInRound(float sx, float sy, float tx, float ty, int radius);
unsigned long millisecondNow();
int returnInt(const char* p);
const char *  timeData();//返回日期

int random(int start, int end);//start到end随机数
int random(int n);//0到ｎ之间的随机数
bool randomBool(int rate);//%几率

 cocos2d::CCPoint operator - (const cocos2d::CCPoint& v1, const cocos2d::CCPoint v2);
 cocos2d::CCPoint operator + (const cocos2d::CCPoint& v1, const cocos2d::CCPoint v2);
 cocos2d::CCPoint operator - (const cocos2d::CCPoint& v);
 cocos2d::CCPoint operator * (const cocos2d::CCPoint& v1, float scale);
 cocos2d::CCPoint operator * (float scale, const cocos2d::CCPoint& v1);
 cocos2d::CCPoint operator / (const cocos2d::CCPoint& v1, float scale);
 bool operator != (const cocos2d::CCPoint& v1, const cocos2d::CCPoint& v2);
 bool operator == (const cocos2d::CCPoint& v1, const cocos2d::CCPoint& v2);

#define SaveStringToXML CCUserDefault::sharedUserDefault()->setStringForKey 
#define SaveIntegerToXML CCUserDefault::sharedUserDefault()->setIntegerForKey  
#define SaveBooleanToXML CCUserDefault::sharedUserDefault()->setBoolForKey
#define LoadStringFromXML CCUserDefault::sharedUserDefault()->getStringForKey
#define LoadIntegerFromXML CCUserDefault::sharedUserDefault()->getIntegerForKey
#define LoadBooleanFromXML CCUserDefault::sharedUserDefault()->getBoolForKey



extern  bool sound;//声明全局变量
extern  bool explainbool;//声明全局变量
extern  int  mapIndex;//地图的选择

extern int loseBloodNumPos_1x_start;
extern int loseBloodNumPos_1y_start;
extern int loseBloodNumPos_1x_end;
extern int loseBloodNumPos_1y_end;

extern int loseBloodNumPos_2x_start;
extern int loseBloodNumPos_2y_start;
extern int loseBloodNumPos_2x_end;
extern int loseBloodNumPos_2y_end;
extern float loseBloodNumTime;


extern int addCritIndex;//暴击等级
extern int addDefenseIndex;//防御等级
extern int addAttackIndex;//攻击等级
extern int addMoveIndex;//移动等级
extern int doubleAttack;//双倍攻击
extern int doubleDefense;//双倍防御
extern int addLifeIndex;//生命

extern int sceneIndex;
extern bool  paused;//暂停
extern SimpleTimer  gameTime;//游戏时间
extern CountdownTimer    emergeTime;//出现时间间隔时间


extern CountdownTimer  survivalModeTime;//生存模式开启时间
extern long  survival_time;//   

extern CountdownTimer regainLifeTime;//恢复生命时间
extern long    regainLife_time;//
extern long    leaveGame_time;//离开游戏的时间

extern bool  gameScene;//是否是游戏中
extern bool  gameSceneMagazine;//是否是游戏中

#endif 