#include "BulletSprite.h"
#include "GameManager.h"

CBulletSprite* CBulletSprite::create(int iPlaneType, int iType, int iAllNum, int iCurNum, float x, float y)
{
	CBulletSprite *pRet = new CBulletSprite();
    if (pRet && pRet->init(iPlaneType, iType, iAllNum, iCurNum, x, y))
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

bool CBulletSprite::init(int iPlaneType, int iType, int iAllNum, int iCurNum, float x, float y)
{
	bool bRet = false;

	do{
		CC_BREAK_IF(! CCLayer::init());

		m_iPlaneType = iPlaneType;
		m_iType = iType;
		setAttr();
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		m_pSprite = CCSprite::createWithSpriteFrameName(m_szName);
		m_iWidth = m_pSprite->getContentSize().width;
		m_iHeight = m_pSprite->getContentSize().height;
		m_pSprite->setPosition(ccp(x, y));
		setMoveTrack(iAllNum, iCurNum);
		this->scheduleUpdate();

		bRet = true;
	}
	while(0);

	return bRet;
}

void CBulletSprite::setAttr()
{
	switch(m_iType)
	{
	case 0:
		strcpy(m_szName, "orangeCircle.png");
		m_iHarm = 1;
		break;
	case 1:
		strcpy(m_szName, "blueBall2.png");
		m_iHarm = 1;
		break;
	case 2:
		strcpy(m_szName, "monsterBullet1.png");
		m_iHarm = 1;
		break;
	case 3:
		strcpy(m_szName, "helperBullet.png");
		m_iHarm = 1;
		break;
	case 4:
		strcpy(m_szName, "purpleCircle.png");
		m_iHarm = 1;
		break;
	case 5:
		strcpy(m_szName, "yellowBall.png");
		m_iHarm = 1;
		break;
	case 6:
		strcpy(m_szName, "silverBall.png");
		m_iHarm = 1;
		break;
	case 7:
		strcpy(m_szName, "greenBall.png");
		m_iHarm = 1;
		break;
	default:
		break;
	}
	
}

void CBulletSprite::setMoveTrack(int iAllNum, int iCurNum)
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	float dx, dy;
	CCActionInterval* pAction;
	if(m_iPlaneType == 0)
	{
		if(m_iType == 5)
		{
			dx = 0;
			dy = size.height/5;
		}
		else
		{
			dx =  (iCurNum - 1)*20 - (iAllNum - 1)*10;
			dy = size.height;
			
		}
		pAction = CCMoveBy::create(1, ccp(dx, dy));
	}
	else
	{	
		float iSpace = 20;
		float iRadians = CC_DEGREES_TO_RADIANS(270 + (iCurNum - iAllNum%2/2.0 - 0.5 - iAllNum/2)*iSpace);
		dx = size.height*5/4*cos(iRadians);
		dy = size.height*5/4*sin(iRadians);
		pAction = CCMoveBy::create(4, ccp(dx, dy));
	}
	CCCallFuncN *pFunc = CCCallFuncN::create(this, callfuncN_selector(CBulletSprite::setDelFunc));
	CCFiniteTimeAction *pMove = CCSequence::create(pAction, pFunc, NULL);  
	m_pSprite->runAction(pMove);
}

void CBulletSprite::update(float dt)
{
	if(m_pSprite->isVisible() == false) return;
	if(m_iPlaneType == 0)
	{
		CEnemyManager *pEnemyManager = CGameManager::getEnemyManager();
		for(int i = 0; i < pEnemyManager->getCount(); i++)
		{
			CEnemySprite *pEnemy = (CEnemySprite*)pEnemyManager->getChildByTag(i);
			if(!pEnemy || pEnemy->getSprite()->isVisible() == false) continue;
			if(pEnemy->getSprite()->boundingBox().intersectsRect(m_pSprite->boundingBox()))
			{
				m_pSprite->setVisible(false);
				pEnemy->hurt(m_iHarm);
			}
			
		}
		CBossSprite *pBoss = (CBossSprite*)pEnemyManager->getChildByTag(100);
		if(pBoss && pBoss->getSprite()->isVisible() == true)
		{
			if(pBoss->getSprite()->boundingBox().intersectsRect(m_pSprite->boundingBox()))
			{
				m_pSprite->setVisible(false);
				pBoss->hurt(m_iHarm);
			}
		}
	}
	else
	{
		CPlayerSprite *pPlayer = CGameManager::getPlayer();	
		if(!pPlayer->isWin())
		{
			if(m_pSprite->boundingBox().containsPoint(pPlayer->getSprite()->getPosition()))
			//if(CCRect::CCRectIntersectsRect(pPlayer->getSprite()->boundingBox(), m_pSprite->boundingBox()))
			{
				m_pSprite->setVisible(false);
				if(pPlayer->isShield() == false)
				{
					pPlayer->hurt(m_iHarm);
				}
			}
		}
	}
}

void CBulletSprite::setDelFunc(CCNode* sender) 
{
	m_pSprite->setVisible(false);
}