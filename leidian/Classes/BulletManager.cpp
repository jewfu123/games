#include "BulletManager.h"
#include "BulletSprite.h"
#include "GameManager.h"

bool CBulletManager::init()
{
	bool bRet = false;
	do{
		CC_BREAK_IF(! CCLayer::init());
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		m_iBulletCout = 0;
		m_pBulletList = CCSpriteBatchNode::create("Image/Bullet.png");
		this->addChild(m_pBulletList);
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Image/Bullet.plist");
		this->scheduleUpdate();

		bRet = true;
	}while(0);

	return bRet;
}

void CBulletManager::createBullet(int iPlaneType, int iType, int iNum, float x, float y)
{
	for(int i = 1; i <= iNum; i++)
	{
		CBulletSprite* pBullet = (CBulletSprite*)CBulletSprite::create(iPlaneType, iType, iNum, i, x, y); 
		int iTag = getFreeTag();
		m_pBulletList->addChild(pBullet->getSprite(), 1, iTag);
		this->addChild(pBullet, 1, iTag);
	}
}
void CBulletManager::update(float dt)
{
	CCSprite *pBullet = NULL;
	for(int i = 0; i < m_iBulletCout; i++)
	{
		pBullet = (CCSprite*)m_pBulletList->getChildByTag(i);
		if(pBullet && pBullet->isVisible() == false)
		{
			m_pBulletList->removeChildByTag(i, true);
			this->removeChildByTag(i, true);
		}
	}
}
int CBulletManager::getFreeTag()
{
	int iTag = -1;
	for(int i = 0; i < m_iBulletCout; i++)
	{
		CCSprite* pBullet = (CCSprite*)m_pBulletList->getChildByTag(i);
		if(!pBullet)
		{
			iTag = i;
			break;
		}
	}
	if(iTag == -1)
	{
		iTag = m_iBulletCout;
		m_iBulletCout++;
	}
	return iTag;
}