//
//  CCRadioMenu.cpp
//  SceneHello
//
//  Created by Orangef on 12-12-24.
//
//

#include "CCRadioMenu.h"
using namespace cocos2d;

CCRadioMenu* CCRadioMenu::create()
{
    return CCRadioMenu::create(NULL, NULL);
}

CCRadioMenu * CCRadioMenu::create(CCMenuItem* item, ...)
{
    va_list args;
    va_start(args,item);
    
    CCRadioMenu *pRet = CCRadioMenu::createWithItems(item, args);
    
    va_end(args);
    
    return pRet;
}

CCRadioMenu* CCRadioMenu::createWithItems(CCMenuItem* item, va_list args)
{
    CCArray* pArray = NULL;
    if( item )
    {
        pArray = CCArray::create(item, NULL);
        CCMenuItem *i = va_arg(args, CCMenuItem*);
        while(i)
        {
            pArray->addObject(i);
            i = va_arg(args, CCMenuItem*);
        }
    }
    
    return CCRadioMenu::createWithArray(pArray);
}

CCRadioMenu* CCRadioMenu::createWithArray(CCArray* pArrayOfItems)
{
    CCRadioMenu *pRet = new CCRadioMenu();
    if (pRet && pRet->initWithArray(pArrayOfItems))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

void CCRadioMenu::setSelectedItem_(CCMenuItem *item){
    m_pSelectedItem = NULL;
    m_pSelectedItem = item;
}

bool CCRadioMenu::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if (m_eState != kCCMenuStateWaiting) return false;
    CCMenuItem *curSelection = this->itemForTouch(pTouch);
    if (curSelection) {
        curSelection->selected();
        _curHighlighted = curSelection;
        
        if (_curHighlighted) {
            if (m_pSelectedItem && m_pSelectedItem != curSelection) {
                m_pSelectedItem->unselected();
            }
            m_eState = kCCMenuStateTrackingTouch;
            return true;
        }
    }
    return false;
}

void CCRadioMenu::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    
    CCAssert(m_eState == kCCMenuStateTrackingTouch, "[Menu ccTouchEnded] -- invalid state");
    CCMenuItem *curSelection = this->itemForTouch(pTouch);
    if (curSelection != _curHighlighted && curSelection != NULL) {
        m_pSelectedItem->selected();
        _curHighlighted->unselected();
        _curHighlighted = NULL;
        m_eState = kCCMenuStateWaiting;
        return;
    }
    
    m_pSelectedItem = _curHighlighted;
    _curHighlighted->activate();
    _curHighlighted = NULL;
    
    m_eState = kCCMenuStateWaiting;
}

void CCRadioMenu::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){
    CCAssert(m_eState == kCCMenuStateTrackingTouch, "[Menu ccTouchCancelled] -- invalid state");
    m_pSelectedItem->selected();
    _curHighlighted->unselected();
    _curHighlighted = NULL;
    
    m_eState = kCCMenuStateWaiting;
}

void CCRadioMenu::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    CCAssert(m_eState == kCCMenuStateTrackingTouch, "[Menu ccTouchMoved] -- invalid state");
    CCMenuItem *curSelection = this->itemForTouch(pTouch);
    if (curSelection != _curHighlighted && curSelection != NULL) {
        _curHighlighted->unselected();
        curSelection->selected();
        _curHighlighted = curSelection;
        return;
    }
}