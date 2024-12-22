#include "ItemManager.h"
#include "ItemSprite.h"

bool CItemManager::init()
{
	bool bRet = false;
	do{
		CC_BREAK_IF(! CCLayer::init());
		m_iItemCout = 0;
		m_pItemList = CCSpriteBatchNode::create("Image/Item.png");
		this->addChild(m_pItemList);
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Image/Item.plist");
		this->scheduleUpdate();
		bRet = true;
	}
	while(0);

	return bRet;
}

void CItemManager::createItem(int iType, float x, float y)
{
	int iTag = -1;
	CItemSprite* pItem = (CItemSprite*)CItemSprite::create(iType, x, y); 
	for(int i = 0; i < m_iItemCout; i++)
	{
		CCSprite* pSprite = (CCSprite*)m_pItemList->getChildByTag(i);
		if(!pSprite)
		{
			iTag = i;
			break;
		}
	}
	if(iTag == -1)
	{
		iTag = m_iItemCout;
		m_iItemCout++;
	}
	m_pItemList->addChild(pItem->getSprite(), 1, iTag);
	this->addChild(pItem, 1, iTag);
}
void CItemManager::update(float dt)
{
	CCSprite *pSprite = NULL;
	for(int i = 0; i < m_iItemCout; i++)
	{
		pSprite = (CCSprite*)m_pItemList->getChildByTag(i);
		if(pSprite && pSprite->isVisible() == false)
		{
			m_pItemList->removeChildByTag(i, true);
			this->removeChildByTag(i, true);
		}
	}
}
