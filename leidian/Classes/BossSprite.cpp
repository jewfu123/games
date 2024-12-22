#include "BossSprite.h"
#include "GameManager.h"
#include "MoveTrackArray.h"

CBossSprite* CBossSprite::create(int iType)
{
	CBossSprite *pRet = new CBossSprite();
    if (pRet && pRet->init(iType))
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

bool CBossSprite::init(int iType)
{
	bool bRet = false;

	do{
		CC_BREAK_IF(! CCLayer::init());

		m_iType = iType;
		sprintf(m_szName,"Boss%d_0.png",m_iType);
		m_pSprite = CCSprite::createWithSpriteFrameName(m_szName);
		m_iWidth = m_pSprite->getContentSize().width;
		m_iHeight = m_pSprite->getContentSize().height;
		setAttr();
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		this->addChild(m_pSprite);

		this->scheduleUpdate();
		this->schedule(schedule_selector(CBossSprite::shoot), m_fShootSpace);
		this->schedule(schedule_selector(CBossSprite::shoot2), m_fShoot2Space);
		this->scheduleOnce(schedule_selector(CBossSprite::setRun), 5.0f);

		bRet = true;
	}while(0);

	return bRet;
	
}
void CBossSprite::update(float dt)
{
	CEnemySprite::update(dt);
	if(!m_bHpChange1 && m_iHp < m_iHpMax/3.0)
	{
		m_bHpChange1 = true;
		m_fShootSpace /= 2.0;
		this->unschedule(schedule_selector(CBossSprite::shoot));
		this->schedule(schedule_selector(CBossSprite::shoot), m_fShootSpace);
	}
	if(!m_bHpChange2 && m_iHp < m_iHpMax*2/3.0)
	{
		m_bHpChange2 = true;
		m_iBulletNum = 5;
	}
	
}
void CBossSprite::setAttr()
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	switch(m_iType)
	{
	case 0:
		m_iHpMax = 600;
		m_iBulletNum = 3;
		m_iBulletType = 2;
		m_fShootSpace = 3.0f;
		m_iBullet2Num = 18;
		m_iBullet2Type = 5;
		m_fShoot2Space = 5.0f;
		m_iMoveType = 1;
		m_iScore = 50;
		break;
	case 1:
		m_iHpMax = 800;
		m_iBulletNum = 3;
		m_iBulletType = 3;
		m_fShootSpace = 3.0f;
		m_iBullet2Num = 18;
		m_iBullet2Type = 6;
		m_fShoot2Space = 5.0f;
		m_iMoveType = 2;
		m_iScore = 50;
		break;
	case 2:
		m_iHpMax = 1200;
		m_iBulletNum = 3;
		m_iBulletType = 4;
		m_fShootSpace = 3.0f;
		m_iBullet2Num = 18;
		m_iBullet2Type = 7;
		m_fShoot2Space = 5.0f;
		m_iMoveType = 3;
		m_iScore = 50;
		break;
	}
	m_iHp = m_iHpMax;
	m_bHpChange1 = false;
	m_bHpChange2 = false;
	m_pSprite->setPosition(ccp(size.width/2, size.height + m_iHeight/2));
	m_pSprite->runAction(CCMoveTo::create(5, ccp(size.width/2, size.height*3/4)));
}

void CBossSprite::setRun(float dt)
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCPointArray *pArray = CMoveTrackArray::getBossArray(m_iMoveType, m_iHeight, m_iWidth);
	CCCatmullRomTo *pAction = CCCatmullRomTo::create(20, pArray);
	CCRepeatForever *pMove = CCRepeatForever::create(pAction);

	m_pSprite->runAction(pMove);

}
void CBossSprite::shoot(float dt)
{
	CCPoint pos = m_pSprite->getPosition();
	CGameManager::getBulletManager()->createBullet(ENEMY, m_iBulletType, m_iBulletNum, pos.x, pos.y);
}

void CBossSprite::shoot2(float dt)
{
	CCPoint pos = m_pSprite->getPosition();
	CGameManager::getBulletManager()->createBullet(ENEMY, m_iBullet2Type, m_iBullet2Num, pos.x - m_iWidth/4, pos.y);
	CGameManager::getBulletManager()->createBullet(ENEMY, m_iBullet2Type, m_iBullet2Num, pos.x + m_iWidth/4, pos.y);
}

void CBossSprite::destroy(CCNode *pSender)
{
	m_pSprite->setVisible(false);
	m_bDestroy = true;
	this->removeChild(m_pExplosion,true);
	CGameManager::getPlayer()->addScore(m_iScore);
	CGameManager::getPlayer()->setWinMove();
	
}