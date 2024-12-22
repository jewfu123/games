#include "RoleSprite.h"
#include "Effect.h"

void CRoleSprite::setDelFunc(CCNode* sender) 
{
	m_pSprite->setVisible(false);
	m_bDestroy = true;
}

void CRoleSprite::setFlyAnimation(int iDirection)
{
	
	CCArray *pArray = CCArray::create();
	char str[100] = {0};
	int iBegin, iEnd;
	if(iDirection == STRAIGHT)
	{
		iBegin = 0;
		iEnd = m_iFlyStraightNum;
	}
	else
	{
		iBegin = m_iFlyStraightNum;
		iEnd = m_iFlyStraightNum + m_iFlyLeftNum;
	}

	if(iDirection == RIGHT)
	{
		m_pSprite->setFlipX(true);
	}
	else
	{
		m_pSprite->setFlipX(false);
	}
	for(int i = iBegin; i < iEnd; i++) 
	{
		sprintf(str, "Plane%d_%d.png", m_iType,i);
		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str);
		pArray->addObject(frame);
	}
	CCAnimation *pWalkAnim = CCAnimation::createWithSpriteFrames(pArray, 0.2f);
	CCRepeatForever *pAciton = CCRepeatForever::create(CCAnimate::create(pWalkAnim));
	pAciton->setTag(iDirection);
	m_pSprite->runAction(pAciton);
}

void CRoleSprite::hurt(int iHarm)
{
	if(m_pSprite->isVisible() == false) return;
	m_iHp -= iHarm;
	if(m_iHp <= 0)
	{
		m_pSprite->setVisible(false);
		m_pSprite->stopAllActions();
		this->unscheduleAllSelectors();
		m_iLife--;
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("Music/Explosion.mp3");
		explosion();
	}
}
void CRoleSprite::explosion()
{
	CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("explosion_01.png");
	m_pExplosion = CCSprite::createWithSpriteFrame(pFrame);
	this->addChild(m_pExplosion);
	m_pExplosion->setPosition(m_pSprite->getPosition());
	CCCallFuncN *destroyFunc =  CCCallFuncN::create(this, callfuncN_selector(CRoleSprite::destroy));
	CCAnimation *animation = CCAnimationCache::sharedAnimationCache()->animationByName("Explosion");
	m_pExplosion->runAction(CCSequence::create(CCAnimate::create(animation), destroyFunc, NULL));
}
int CRoleSprite::getDirection()
{
	float dx = m_pSprite->getPosition().x - m_pos.x;
	m_pos.x = m_pSprite->getPosition().x;
	m_pos.y = m_pSprite->getPosition().y; 
	if(dx < 0)
		return LEFT;
	else if(dx == 0)
		return STRAIGHT;
	else if(dx > 0)
		return RIGHT;

}
void CRoleSprite::changeFlyAnimation(float dt)
{
	int iDirection = getDirection();
	if(m_iDirection != iDirection)
	{
		m_pSprite->stopActionByTag(m_iDirection);
		setFlyAnimation(iDirection);
		m_iDirection = iDirection;
	}
}