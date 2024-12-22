#include "PlayerSprite.h"
#include "GameManager.h"
#include "GameEvents.h"

CPlayerSprite* CPlayerSprite::create(int iType)
{
	CPlayerSprite *pRet = new CPlayerSprite();
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

bool CPlayerSprite::init(int iType)
{
	bool bRet = false;

	do{
		CC_BREAK_IF(! CCLayer::init());

		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Image/Plane.plist");
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Image/Shield.plist");
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Image/Shoot.plist");
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Image/Missile.plist");
		m_pSprite = CCSprite::createWithSpriteFrameName("Plane0_0.png");
		this->addChild(m_pSprite, 1);
		
		m_iWidth = m_pSprite->getContentSize().width;
		m_iHeight = m_pSprite->getContentSize().height;

		m_bWin = false;
		m_bMissile = false;
		m_iType = iType;
		m_iHpMax = 10;
		m_iFlyStraightNum = 2;
		m_iFlyLeftNum = 2;
		m_iScore = 0;
		m_iLife = 3;
		m_iAddLifeScore = 100;
		initAttrWithDie();
		this->setTouchEnabled(true);
		bRet = true;
	}
	while(0);

	return bRet;
}

void CPlayerSprite::initAttrWithDie()
{
	m_pSprite->setVisible(true);
	m_bDestroy = false;
	m_fShootSpace = 0.2f;
	m_iBulletType = 0;
	m_iBulletNum = 3;
	m_iMissileNum = 2;
	initAttrWithWin();

}
void CPlayerSprite::initAttrWithWin()
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	m_pSprite->setPosition(ccp(size.width/2, -size.height/10));
	m_pSprite->runAction(CCMoveBy::create(2.0f, ccp(0, size.height/5)));
	m_iHp = m_iHpMax;
	m_iDirection = STRAIGHT;
	setFlyAnimation(m_iDirection);
	setShield();
	setShoot();
	this->schedule(schedule_selector(CPlayerSprite::shoot), m_fShootSpace);
	this->schedule(schedule_selector(CPlayerSprite::changeFlyAnimation), 0.1f);
}

void CPlayerSprite::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
	CCTouch* touch = (CCTouch*)(* pTouches->begin());
	m_PosBack = touch->getLocation();
}

void CPlayerSprite::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	CCTouch* touch = (CCTouch*)(* pTouches->begin());
	CCPoint pos = touch->getLocation();
	
	CCSize size = CCDirector::sharedDirector()->getWinSize();

	float dx = pos.x - m_PosBack.x;
	float dy = pos.y - m_PosBack.y;
	m_PosBack = pos;

	CCPoint spritePos = m_pSprite->getPosition();
	spritePos.x += dx;
	spritePos.y += dy;

	if(spritePos.x < m_iWidth/2) spritePos.x = m_iWidth/2;
	else if(spritePos.x > size.width - m_iWidth/2) spritePos.x = size.width - m_iWidth/2;
	if(spritePos.y < m_iHeight/2) spritePos.y = m_iHeight/2;
	else if(spritePos.y > size.height*7/8 - m_iHeight/2) spritePos.y = size.height*7/8 - m_iHeight/2;

	m_pSprite->setPosition(spritePos);
}

void CPlayerSprite::setWinMove()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("Music/Fly.mp3");
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	this->unscheduleAllSelectors();
	this->setTouchEnabled(false);
	m_pSprite->stopAllActions();
	setFlyAnimation(STRAIGHT);
	clearShoot();
	CCMoveBy *pAction = CCMoveBy::create(size.height/300.0, ccp(0, size.height));
	CCCallFuncN *pFunc = CCCallFuncN::create(this, callfuncN_selector(CPlayerSprite::win));
	CCFiniteTimeAction *pMove = CCSequence::create(pAction, pFunc, NULL);  
	m_pSprite->runAction(pMove);
}

void CPlayerSprite::win(CCNode *pSender)
{
	clearShield(0);
	m_bWin = true;
}

void CPlayerSprite::shoot(float dt)
{
	m_iShootSoundId = CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("Music/Shoot.mp3");
	CCPoint pos = m_pSprite->getPosition();
	CGameManager::getBulletManager()->createBullet(PLAYER, m_iBulletType, m_iBulletNum, pos.x, pos.y);
}

void CPlayerSprite::destroy(CCNode *pSender)
{
	m_bDestroy = true;
	this->removeChild(m_pExplosion,true);
	clearShield(0);
	clearShoot();
	if(m_iLife <= 0)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Music/GameOver.mp3");
		CGameEvents::startFlipInterface(GAME_OVER);
	}
	else 
	{
		initAttrWithDie();
	}
}

void CPlayerSprite::gainItem(int iType, int iScore)
{
	m_iScore += iScore;
	switch(iType)
	{
	case 0:
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("Music/GainGold.mp3");
		break;
	case 1:
		if(m_iBulletNum < 6)
			m_iBulletNum++;
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("Music/GainJewel.mp3");
		break;
	case 2:
		if(m_bShield)
		{
			this->unschedule(schedule_selector(CPlayerSprite::clearShield));
			this->scheduleOnce(schedule_selector(CPlayerSprite::clearShield), 10.0f);
		}
		else
		{
			setShield();
		}
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("Music/GainJewel.mp3");
		break;
	case 3:
		m_iHp = m_iHpMax;
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("Music/GainJewel.mp3");
		break;
	case 4:
		m_iMissileNum++;
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("Music/GainMissile.mp3");
		break;
	default:
		break;
	}
}

void CPlayerSprite::setShield()
{
	m_bShield = true;
	
	CCSprite *pShield1 = CCSprite::createWithSpriteFrameName("Shield_10.png");
	CCSprite *pShield2 = CCSprite::createWithSpriteFrameName("Shield_10.png");
	pShield1->setPosition(ccp(m_iWidth/2, m_iHeight/2));
	pShield2->setPosition(ccp(m_iWidth/2, m_iHeight/2));

	CCArray *animFrames = CCArray::create();
	char str[64] = {0};
	for (int i = 1; i < 11; i++) 
	{
		sprintf(str, "Shield_%02d.png", i); 
		CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str);
		animFrames->addObject(frame);
	}
	CCAnimation *animation = CCAnimation::createWithSpriteFrames(animFrames, 0.05f);
	CCRepeatForever *pAciton = CCRepeatForever::create(CCAnimate::create(animation));
	pShield2->runAction(pAciton);

	m_pSprite->addChild(pShield1, -1, 0);
	m_pSprite->addChild(pShield2, -1, 1);

	this->scheduleOnce(schedule_selector(CPlayerSprite::clearShield), 10.0f);
}

void CPlayerSprite::clearShield(float dt)
{
	if(!m_bShield) return;
	m_bShield = false;
	m_pSprite->removeChildByTag(0, true);
	m_pSprite->removeChildByTag(1, true);
}

void CPlayerSprite::setShoot()
{
	CCSprite *pShoot = CCSprite::createWithSpriteFrameName("Shoot_01.png");
	pShoot->setPosition(ccp(m_iWidth/2, m_iHeight));
	
	CCArray *animFrames = CCArray::create();
	char str[64] = {0};
	for (int i = 1; i < 5; i++)
	{
		sprintf(str, "Shoot_%02d.png", i); 
		CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str);
		animFrames->addObject(frame);
	}
	CCAnimation *animation = CCAnimation::createWithSpriteFrames(animFrames, m_fShootSpace/4.0);
	CCRepeatForever *pAciton = CCRepeatForever::create(CCAnimate::create(animation));
	pShoot->runAction(pAciton);

	m_pSprite->addChild(pShoot, -2, 2);
}

void CPlayerSprite::clearShoot()
{
	m_pSprite->removeChildByTag(2, true);
}

void CPlayerSprite::addScore(int iValue)
{
	m_iScore += iValue;
	while(m_iScore >= m_iAddLifeScore)
	{
		m_iLife++;
		m_iAddLifeScore += 100;
	}
}

void CPlayerSprite::shootMissile()
{
	if(m_bMissile || m_iMissileNum <= 0 || !m_pSprite->isVisible()) return;
	m_bMissile = true;
	if(m_iMissileNum > 0)
		m_iMissileNum--;
	createMissile();
}
void CPlayerSprite::createMissile()
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCPoint pos = m_pSprite->getPosition();
	m_pMissile = CCSprite::createWithSpriteFrameName("Missile_00.png");
	m_pMissile->setPosition(ccp(pos.x, pos.y));
	this->addChild(m_pMissile, 0);
	CCMoveBy *pMoveBy = CCMoveBy::create(0.2f, ccp(0, size.height/4));
	CCCallFuncN *Func= CCCallFuncN::create(this, callfuncN_selector(CPlayerSprite::MissileExplore));
	
	CCFiniteTimeAction *pMove = CCSequence::create(pMoveBy, Func, NULL);  
	m_pMissile->runAction(pMove);
}

void CPlayerSprite::MissileExplore(CCNode *pSender)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("Music/MissileExplosion.mp3");
	CCArray *pAnimFrames = CCArray::create();
	char str[64] = {0};
	for (int i = 1; i < 13; i++)
	{
		sprintf(str, "Missile_%02d.png", i); 
		CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str);
		pAnimFrames->addObject(frame);
	}
	CCAnimate *pAnimate = CCAnimate::create(CCAnimation::createWithSpriteFrames(pAnimFrames, 0.07f));
	CCCallFuncN *Func = CCCallFuncN::create(this, callfuncN_selector(CPlayerSprite::MissileDestroy));
	CCFiniteTimeAction *pExplore = CCSequence::create(pAnimate, Func, NULL);  
	m_pMissile->runAction(pExplore);
	CGameManager::getGameMap()->shake();
	hurtEnemy();
}
void CPlayerSprite::MissileDestroy(CCNode *pSender)
{
	m_bMissile = false;
	this->removeChild(m_pMissile, true);
}
void CPlayerSprite::hurtEnemy()
{
	CEnemyManager *pEnemyManager = CGameManager::getEnemyManager();
	for(int i = 0; i < pEnemyManager->getCount(); i++)
	{
		CEnemySprite *pEnemy = (CEnemySprite*)pEnemyManager->getChildByTag(i);
		if(pEnemy && pEnemy->getSprite()->isVisible() == true)
		{
			pEnemy->hurt(20);
		}
	}
	CBossSprite *pBoss = (CBossSprite*)pEnemyManager->getChildByTag(100);
	if(pBoss && pBoss->getSprite()->isVisible() == true)
	{
		pBoss->hurt(20);
	}
}