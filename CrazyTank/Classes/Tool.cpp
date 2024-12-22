#include "Tool.h"

using namespace std;
using namespace cocos2d;

bool sound;
bool explainbool;//����ȫ�ֱ���
int  mapIndex;
int sceneIndex;
bool  paused;//��ͣ
bool  gameScene;//�Ƿ�����Ϸ��
bool  gameSceneMagazine;//�Ƿ�����Ϸ��
SimpleTimer  gameTime;//��Ϸʱ��
CountdownTimer    emergeTime;//����ʱ����ʱ��




CountdownTimer  survivalModeTime;//����ģʽ����ʱ��
long    survival_time;//

CountdownTimer regainLifeTime;//�ָ�����ʱ��
long    regainLife_time;//
long  leaveGame_time;//�뿪��Ϸ��ʱ��

int loseBloodNumPos_1x_start;
int loseBloodNumPos_1y_start;
int loseBloodNumPos_1x_end;
int loseBloodNumPos_1y_end;

int loseBloodNumPos_2x_start;
int loseBloodNumPos_2y_start;
int loseBloodNumPos_2x_end;
int loseBloodNumPos_2y_end;
float loseBloodNumTime;

int addLifeIndex;//���� 
int addCritIndex;//�����ȼ�
int addDefenseIndex;//�����ȼ�
int addAttackIndex;//�����ȼ�
int addMoveIndex;//�ƶ��ȼ�
int doubleAttack;//˫������
int doubleDefense;//˫������

/**
	 * �ж�һ�����Ƿ���һ��Բ��
	 * 
	 * @param sx
	 *            ���X����
	 * @param sy
	 *            ���Y����
	 * @param tx
	 *            Բ�ĵ�X����
	 * @param ty
	 *            Բ�ĵ�Y����
	 * @param radius
	 *            Բ�뾶
	 * @return true��Բ�� falseû����Բ��
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
�����ַ���������
*/
int  returnInt(const char* p){
	 char result[100];/*����������*/
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
 ���ֳ���
 */
 int  numLenth(int num){
	int leng=1;
	if(num<0){//����
		leng =2;
	}
	while(abs(num)>=10){
		num = num/10;
        leng++;
	}
	return leng;
}

/*
���� ����ʾ������
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

//��ȡϵͳ������
unsigned long millisecondNow()  {
	struct cc_timeval now; 
	CCTime::gettimeofdayCocos2d(&now, NULL);
	unsigned long  time = ((unsigned long)now.tv_sec) * 1000+ now.tv_usec / 1000;
	return time ;//long long((long long)now.tv_sec * 1000 + now.tv_usec / 1000)
}

int random(int start, int end){//start��end�����,����start,end
	float _int = CCRANDOM_0_1();
	while(_int==1.0f){
		_int = CCRANDOM_0_1();
	}
	_int = _int*(end-start+1)+start;
	return (int)_int;
}

int random(int n){//0����֮��������
	int  de = (int)(CCRANDOM_0_1()*n);
	CCLog("random===============:%d",de);
	return de;
}

bool randomBool(int rate){//%����
	int m= random(100);
    return m>rate?false:true;
}

 /*
  ��������
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
//����ת��:��һ�ֱ���תΪ��һ�ֱ���   ��Ҫicovn

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

//UNICODE��תΪGB2312��   

//�ɹ��򷵻�һ����̬�����char*��������Ҫ��ʹ����Ϻ��ֶ�free��ʧ�ܷ���NULL

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

//GB2312��תΪUNICODE��   

//�ɹ��򷵻�һ����̬�����char*��������Ҫ��ʹ����Ϻ��ֶ�free��ʧ�ܷ���NULL

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
