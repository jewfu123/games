#include "MoveTrackArray.h"

CCPointArray * CMoveTrackArray::getEnemyArray(int iType)
{
	CCPointArray *pArray = NULL;
	switch(iType)
	{
	case 1:
		pArray = getEnemyArray1();
		break;
	case 2:
		pArray = getEnemyArray2(1);
		break;
	case 3:
		pArray = getEnemyArray2(-1);
		break;
	default:
		break;
	}
	return pArray;
}

CCPointArray * CMoveTrackArray::getBossArray(int iType, int iHeight, int iWidth)
{
	CCPointArray *pArray = NULL;
	switch(iType)
	{
	case 1:
		pArray = getBossArray1(iHeight, iWidth);
		break;
	case 2:
		pArray = getBossArray2(iHeight, iWidth);
		break;
	case 3:
		pArray = getBossArray3(iHeight, iWidth);
		break;
	default:
		break;
	}
	return pArray;
}

CCPointArray * CMoveTrackArray::getEnemyArray1()
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCPointArray *pArray = CCPointArray::create(10);
	pArray->addControlPoint(ccp(0, 0));
	int iFlag = -1;
	for(int i = 1; i < 7; i++)
	{
		iFlag = -iFlag;
		pArray->addControlPoint(ccp(size.width/3.0f*iFlag, -size.height*i/5.0f));
	}
	return pArray;
}

CCPointArray * CMoveTrackArray::getEnemyArray2(int iDirection)
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCPointArray *pArray = CCPointArray::create(4);
	pArray->addControlPoint(ccp(0, 0));
	pArray->addControlPoint(ccp(iDirection*size.width*3/5.0f, -size.height*1/5.0f));
	pArray->addControlPoint(ccp(iDirection*size.width*4/5.0f, -size.height*1/5.0f));
	pArray->addControlPoint(ccp(-1*iDirection*size.width*1/5.0f, -size.height*6/5.0f));
	return pArray;
}

CCPointArray * CMoveTrackArray::getBossArray1(int iHeight, int iWidth)
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCPointArray *pArray = CCPointArray::create(15);
	pArray->addControlPoint(ccp(size.width/2, size.height*3/4));
	for(int i = 0; i < 3; i++)
	{
		pArray->addControlPoint(ccp(iWidth/2, size.height*3/4));
		pArray->addControlPoint(ccp(size.width - iWidth/2, size.height*3/4));
	}
	pArray->addControlPoint(ccp(size.width/2, size.height*3/4));
	pArray->addControlPoint(ccp(size.width/2, iHeight/2));
	pArray->addControlPoint(ccp(iWidth/2, (size.height*3/4 - iHeight/2)*1/3 + iHeight/2));
	pArray->addControlPoint(ccp(size.width - iWidth/2, (size.height*3/4 - iHeight/2)*2/3 + iHeight/2));
	pArray->addControlPoint(ccp(size.width/2, size.height*3/4));

	return pArray;
}
CCPointArray * CMoveTrackArray::getBossArray2(int iHeight, int iWidth)
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCPointArray *pArray = CCPointArray::create(15);
	pArray->addControlPoint(ccp(size.width/2, size.height*3/4));
	for(int i = 0; i < 2; i++)
	{
		pArray->addControlPoint(ccp(iWidth/2, size.height*2/3));
		pArray->addControlPoint(ccp(size.width/2, size.height*3/4));
		pArray->addControlPoint(ccp(size.width - iWidth/2, size.height*2/3));
	}
	pArray->addControlPoint(ccp(iWidth/2, (size.height*3/4 - iHeight/2)*2/3 + iHeight/2));
	pArray->addControlPoint(ccp(size.width - iWidth/2, (size.height*3/4 - iHeight/2)*1/3 + iHeight/2));
	pArray->addControlPoint(ccp(size.width/2, iHeight/2));
	pArray->addControlPoint(ccp(iWidth/2, (size.height*3/4 - iHeight/2)*1/3 + iHeight/2));
	pArray->addControlPoint(ccp(size.width - iWidth/2, (size.height*3/4 - iHeight/2)*2/3 + iHeight/2));
	pArray->addControlPoint(ccp(size.width/2, size.height*3/4));

	return pArray;
}
CCPointArray * CMoveTrackArray::getBossArray3(int iHeight, int iWidth)
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCPointArray *pArray = CCPointArray::create(15);
	pArray->addControlPoint(ccp(size.width/2, size.height*3/4));
	pArray->addControlPoint(ccp(iWidth/2, (size.height*3/4 - iHeight/2)*1/2 + iHeight/2));
	pArray->addControlPoint(ccp(size.width/2, iHeight/2));
	pArray->addControlPoint(ccp(size.width - iWidth/2, (size.height*3/4 - iHeight/2)*1/2 + iHeight/2));
	pArray->addControlPoint(ccp(size.width/2, size.height*3/4));
	pArray->addControlPoint(ccp(size.width - iWidth/2, size.height*3/4));
	pArray->addControlPoint(ccp(size.width - iWidth/2, iHeight/2));
	pArray->addControlPoint(ccp(iWidth/2, iHeight/2));
	pArray->addControlPoint(ccp(iWidth/2, size.height*3/4));
	pArray->addControlPoint(ccp(size.width/2, size.height*3/4));

	return pArray;
}