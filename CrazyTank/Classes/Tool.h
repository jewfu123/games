#pragma once

#ifndef _TOOLS_H_   
#define  _TOOLS_H_   
#include "cocos2d.h" 
#include "SimpleTimer/SimpleTimer.h" 


bool PointInRound(float sx, float sy, float tx, float ty, int radius);
unsigned long millisecondNow();
int returnInt(const char* p);
const char *  timeData();//��������

int random(int start, int end);//start��end�����
int random(int n);//0����֮��������
bool randomBool(int rate);//%����

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



extern  bool sound;//����ȫ�ֱ���
extern  bool explainbool;//����ȫ�ֱ���
extern  int  mapIndex;//��ͼ��ѡ��

extern int loseBloodNumPos_1x_start;
extern int loseBloodNumPos_1y_start;
extern int loseBloodNumPos_1x_end;
extern int loseBloodNumPos_1y_end;

extern int loseBloodNumPos_2x_start;
extern int loseBloodNumPos_2y_start;
extern int loseBloodNumPos_2x_end;
extern int loseBloodNumPos_2y_end;
extern float loseBloodNumTime;


extern int addCritIndex;//�����ȼ�
extern int addDefenseIndex;//�����ȼ�
extern int addAttackIndex;//�����ȼ�
extern int addMoveIndex;//�ƶ��ȼ�
extern int doubleAttack;//˫������
extern int doubleDefense;//˫������
extern int addLifeIndex;//����

extern int sceneIndex;
extern bool  paused;//��ͣ
extern SimpleTimer  gameTime;//��Ϸʱ��
extern CountdownTimer    emergeTime;//����ʱ����ʱ��


extern CountdownTimer  survivalModeTime;//����ģʽ����ʱ��
extern long  survival_time;//   

extern CountdownTimer regainLifeTime;//�ָ�����ʱ��
extern long    regainLife_time;//
extern long    leaveGame_time;//�뿪��Ϸ��ʱ��

extern bool  gameScene;//�Ƿ�����Ϸ��
extern bool  gameSceneMagazine;//�Ƿ�����Ϸ��

#endif 