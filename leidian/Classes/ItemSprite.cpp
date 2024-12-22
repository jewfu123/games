#include "ItemSprite.h"
#include "GameManager.h"

CItemSprite* CItemSprite::create(int iType, float x, float y)
{
	CItemSprite *pRet = new CItemSprite();
    if (pRet && pRet->init(iType, x, y))
    { 
        pRet->autorelease(); 
        return pRet; 
    } 
    else 
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool CItemSprite::init(int iType, float x, float y)
{
	bool bRet = false;

	do{
		CC_BREAK_IF(! CCLayer::init());	

		m_iType = iType;
		setAttr();
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		char szName[64] = {0};
		sprintf(szName,m_szName,1);
		m_pSprite = CCSprite::createWithSpriteFrameName(szName);
		m_iWidth = m_pSprite->getContentSize().width;
		m_iHeight = m_pSprite->getContentSize().height;
		m_pSprite->setPosition(ccp(x, y));
		CCActionInterval* pAction;
		CCAnimation *animation;
		CCCallFuncN *pFunc;
		CCFiniteTimeAction *pMove;
		switch(m_iType)
		{
		case 0:
			animation = CCAnimationCache::sharedAnimationCache()->animationByName("Gold");
			m_pSprite->runAction(CCRepeatForever::create(CCAnimate::create(animation)));
			pAction = CCMoveBy::create(5, ccp(0, -size.height - 40));
			pFunc = CCCallFuncN::create(this, callfuncN_selector(CItemSprite::setDelFunc));
			pMove = CCSequence::create(pAction, pFunc, NULL);  
			m_pSprite->runAction(pMove);
			break;
		case 1:
		case 2:
		case 3:
			randomMove(0);
			this->schedule(schedule_selector(CItemSprite::randomMove), 1.0f); 
			this->schedule(schedule_selector(CItemSprite::changeType), 5.0f); 
			setAnimation();
			break;
		case 4:
			randomMove(0);
			this->schedule(schedule_selector(CItemSprite::randomMove), 1.0f); 
			break;
		}
		
		this->scheduleUpdate();

		bRet = true;
	}
	while(0);

	return bRet;
}
void CItemSprite::setAttr()
{
	switch(m_iType)
	{
	case 0:
		strcpy(m_szName, "Gold_%02d.png");
		m_iScore = 5;
		break;
	case 1:
		strcpy(m_szName, "Ruby_%02d.png");
		m_iScore = 10;
		break;
	case 2:
		strcpy(m_szName, "Sapphire_%02d.png");
		m_iScore = 10;
		break;
	case 3:
		strcpy(m_szName, "Amethyst_%02d.png");
		m_iScore = 10;
		break;
	case 4:
		strcpy(m_szName, "MissileItem_%02d.png");
		m_iScore = 10;
		break;
	default:
		break;
	}
}
void CItemSprite::setAnimation()
{
	CCArray *pAnimFrames = CCArray::create();
	char str[64] = {0};
	for (int i = 1; i < 5; i++) 
	{
		sprintf(str, m_szName, i);
		CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str);
		pAnimFrames->addObject(frame);
	}
	CCAnimation *pAnimation = CCAnimation::createWithSpriteFrames(pAnimFrames, 0.2f);
	CCRepeatForever *pRepeat = CCRepeatForever::create(CCAnimate::create(pAnimation));
	pRepeat->setTag(0);
	m_pSprite->runAction(pRepeat);
}
void CItemSprite::update(float dt)
{
	if(m_pSprite->isVisible() == false) return;
	CPlayerSprite *pPlayer = CGameManager::getPlayer();
	if(pPlayer->getSprite()->boundingBox().intersectsRect(m_pSprite->boundingBox()))
	{
		m_pSprite->setVisible(false);
		pPlayer->gainItem(m_iType, m_iScore);
	}
}

void CItemSprite::randomMove(float dt)
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	float x, y, fDist;
	do
	{
		x = rand()%(int)(size.width - m_iWidth) + m_iWidth/2;
		y = rand()%(int)(size.height - m_iHeight) + m_iHeight/2;
		fDist = getDist(x, y);
	}while(fDist < 50);
	m_pSprite->stopActionByTag(1);
	CCMoveTo *pMove = CCMoveTo::create(fDist/50, ccp(x, y));
	pMove->setTag(1);
	m_pSprite->runAction(pMove);
}

void CItemSprite::changeType(float dt)
{
	m_iType++;
	if(m_iType >= 4)
		m_iType = 1;
	setAttr();
	m_pSprite->stopActionByTag(0);
	setAnimation();
}
void CItemSprite::setDelFunc(CCNode* sender) 
{
	m_pSprite->setVisible(false);
}