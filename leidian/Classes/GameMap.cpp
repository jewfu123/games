#include "GameMap.h"

bool CGameMap::init()
{
	bool bRet = false;
	do{
		CC_BREAK_IF(! CCLayer::init());
		this->removeAllChildrenWithCleanup(true);
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		/**	¼ÓÔØÓÎÏ·±³¾°µØÍ¼	*/
		char szText[128];
		sprintf(szText, "Image/Map%d.png", g_iGameLevel);
		m_pMap= CCSprite::create(szText);
		CC_BREAK_IF(! m_pMap);
		m_pMap->setPosition(ccp(size.width/2, 600));
		this->addChild(m_pMap, 0);
		m_pMap->runAction(CCMoveTo::create(100.0f, ccp(size.width/2, -(600 - size.height)))); //100.0f
		
		bRet = true;
	}
	while(0);

	return bRet;
}

void CGameMap::shake()
{
	m_pMap->runAction(CCJumpBy::create(1, ccp(0, 0), -15, 6));
}