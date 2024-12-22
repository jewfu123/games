#include "Tool.h"

using namespace std;
using namespace cocos2d;

bool sound;
bool explainbool;//声明全局变量
int  mapIndex;
int sceneIndex;
bool  paused;//暂停
bool  gameScene;//是否是游戏中
bool  gameSceneMagazine;//是否是游戏中
SimpleTimer  gameTime;//游戏时间
CountdownTimer    emergeTime;//出现时间间隔时间




CountdownTimer  survivalModeTime;//生存模式开启时间
long    survival_time;//

CountdownTimer regainLifeTime;//恢复生命时间
long    regainLife_time;//
long  leaveGame_time;//离开游戏的时间

int loseBloodNumPos_1x_start;
int loseBloodNumPos_1y_start;
int loseBloodNumPos_1x_end;
int loseBloodNumPos_1y_end;

int loseBloodNumPos_2x_start;
int loseBloodNumPos_2y_start;
int loseBloodNumPos_2x_end;
int loseBloodNumPos_2y_end;
float loseBloodNumTime;

int addLifeIndex;//生命 
int addCritIndex;//暴击等级
int addDefenseIndex;//防御等级
int addAttackIndex;//攻击等级
int addMoveIndex;//移动等级
int doubleAttack;//双倍攻击
int doubleDefense;//双倍防御

/**
	 * 判断一个点是否在一个圆内
	 * 
	 * @param sx
	 *            点的X坐标
	 * @param sy
	 *            点的Y坐标
	 * @param tx
	 *            圆心的X坐标
	 * @param ty
	 *            圆心的Y坐标
	 * @param radius
	 *            圆半径
	 * @return true在圆内 false没有在圆内
	 */
 bool PointInRound(float sx, float sy, float tx, float ty, int radius)
 {
	if((sx - tx) * (sx - tx) + (sy - ty) * (sy - ty) < radius * radius)
	{
			return true;
	}
	return false;
}
/*
返回字符串中数字
*/
int  returnInt(const char* p){
	 char result[100];/*用来保存结果*/
	 int i=0;

	 while(*p!=NULL)
	 {
		 CCLog("PPPPPPPPPPPPPPP:%s",p);
		 if(*p<='9'&&*p>='0')

		 {
			 result[i]=*p;
			 i++;
		 }
		 p++;
	 }

	 return atoi(result);

 }


 /*
 数字长度
 */
 int  numLenth(int num){
	int leng=1;
	if(num<0){//负数
		leng =2;
	}
	while(abs(num)>=10){
		num = num/10;
        leng++;
	}
	return leng;
}

/*
返回 ：显示年月日
*/
const char * timeData(){
	   struct cc_timeval now; 
	   CCTime::gettimeofdayCocos2d(&now, NULL);

	    char str[64]; 
		time_t timep = now.tv_sec; 
		tm* timeStuct;

		timeStuct =localtime(&timep);
		sprintf(str, "%02d:%02d:%02d", //%02d:%02d:%02d %02d:%02d
			timeStuct->tm_year + 1900, 
			timeStuct->tm_mon + 1, 
			timeStuct->tm_mday
			//timeStuct->tm_hour, 
			//timeStuct->tm_min
			); 
		 char *data = new char[strlen(str)+1];
         strcpy(data, str);

		CCLog("-------:%s",data);
		return data;
 }

//获取系统毫秒数
unsigned long millisecondNow()  {
	struct cc_timeval now; 
	CCTime::gettimeofdayCocos2d(&now, NULL);
	unsigned long  time = ((unsigned long)now.tv_sec) * 1000+ now.tv_usec / 1000;
	return time ;//long long((long long)now.tv_sec * 1000 + now.tv_usec / 1000)
}

int random(int start, int end){//start到end随机数,包括start,end
	float _int = CCRANDOM_0_1();
	while(_int==1.0f){
		_int = CCRANDOM_0_1();
	}
	_int = _int*(end-start+1)+start;
	return (int)_int;
}

int random(int n){//0到ｎ之间的随机数
	int  de = (int)(CCRANDOM_0_1()*n);
	CCLog("random===============:%d",de);
	return de;
}

bool randomBool(int rate){//%几率
	int m= random(100);
    return m>rate?false:true;
}

 /*
  符号重载
 */
  cocos2d::CCPoint operator + (const cocos2d::CCPoint& v1, const cocos2d::CCPoint v2)
    {
        return ccp(v1.x + v2.x, v1.y + v2.y);
    }

     cocos2d::CCPoint operator - (const cocos2d::CCPoint& v1, const cocos2d::CCPoint v2)
    {
        return ccp(v1.x - v2.x, v1.y - v2.y);
    }

     cocos2d::CCPoint operator - (const cocos2d::CCPoint& v)
    {
        return ccp(-v.x, -v.y);
    }

     cocos2d::CCPoint operator * (const cocos2d::CCPoint& v1, float scale)
    {
        return ccp(v1.x * scale, v1.y * scale);        
    }

     cocos2d::CCPoint operator * (float scale, const cocos2d::CCPoint& v1)
    {
        return ccp(v1.x * scale, v1.y * scale);        
    }
    
     cocos2d::CCPoint operator / (const cocos2d::CCPoint& v1, float scale)
    {
        return ccp(v1.x / scale, v1.y / scale);        
    }

     bool operator == (const cocos2d::CCPoint& v1, const cocos2d::CCPoint& v2)
    {
        return (v1.x == v2.x) && (v1.y == v2.y);
    }

     bool operator != (const cocos2d::CCPoint& v1, const cocos2d::CCPoint& v2)
    {
        return (v1.x != v2.x) || (v1.y != v2.y);
    }



/*
//代码转换:从一种编码转为另一种编码   需要icovn

int code_convert(char* from_charset, char* to_charset, char* inbuf,

               int inlen, char* outbuf, int outlen)

{

    iconv_t cd;

    char** pin = &inbuf;   

    char** pout = &outbuf;

    cd = iconv_open(to_charset,from_charset);   

    if(cd == 0)

       return -1;

    memset(outbuf,0,outlen);   

    if(iconv(cd,(const char**)pin,(unsigned int *)&inlen,pout,(unsigned int*)&outlen)

       == -1)

       return -1;   

    iconv_close(cd);

    return 0;   

}

//UNICODE码转为GB2312码   

//成功则返回一个动态分配的char*变量，需要在使用完毕后手动free，失败返回NULL

char* u2g(char *inbuf)   

{

    int nOutLen = 2 * strlen(inbuf) - 1;

    char* szOut = (char*)malloc(nOutLen);

    if (-1 == code_convert("utf-8","gb2312",inbuf,strlen(inbuf),szOut,nOutLen))

    {

       free(szOut);

       szOut = NULL;

    }

    return szOut;

}   

//GB2312码转为UNICODE码   

//成功则返回一个动态分配的char*变量，需要在使用完毕后手动free，失败返回NULL

char* g2u(char *inbuf)   

{

    int nOutLen = 2 * strlen(inbuf) - 1;

    char* szOut = (char*)malloc(nOutLen);

    if (-1 == code_convert("gb2312","utf-8",inbuf,strlen(inbuf),szOut,nOutLen))

    {

       free(szOut);

       szOut = NULL;

    }

    return szOut;

} */
//#endif 
