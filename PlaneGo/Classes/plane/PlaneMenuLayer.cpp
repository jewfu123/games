#include "PlaneMenuLayer.h"
#include "../select/SelectScene.h"
#include "../game/GameScene.h"
#include "GameResource.h"
#include "StringUtil.h"
#include "AudioUtil.h"
#include "KeyId.h"

#define ADJUST_ANIM_VELOCITY 3200; 

enum{
	titleId, bottomId, tab0Id, tab1Id, tab2Id, tab3Id, tab4Id, tab5Id, leftId, rightId, textId
};

PlaneMenuLayer *PlaneMenuLayer::create(int _num){
	PlaneMenuLayer *pRet =new PlaneMenuLayer();
	if (pRet && pRet->init(_num)){  
		pRet->autorelease(); 
		return pRet;
	} else { 
		delete pRet; 
		pRet = NULL; 
		return NULL; 
	} 
}
bool PlaneMenuLayer::init(int _num){
	if(!CCLayer::init()){
		return false;
	}
	num=_num;
	setKeypadEnabled(true);
	CCSize size=CCDirector::sharedDirector()->getWinSize();
	CCPoint oPoint=CCDirector::sharedDirector()->getVisibleOrigin();
	//设置背景
	CCSprite *bg=CCSprite::createWithSpriteFrameName(s_select_bg);
	bg->setPosition(ccp(size.width*0.5f, size.height*0.5f));
	this->addChild(bg);

	//创建向左箭头
	CCSprite *left=CCSprite::createWithSpriteFrameName(s_select_left);
	left->setPosition(ccp(oPoint.x+20, size.height*0.5f+100));
	this->addChild(left, 0, leftId);
	CCMoveBy *action0=CCMoveBy::create(0.1f, ccp(-8, 0));
	left->runAction(CCRepeatForever::create(CCSequence::create(action0, action0->reverse(), NULL)));
	//创建向右箭头
	CCSprite *right=CCSprite::createWithSpriteFrameName(s_select_right);
	right->setPosition(ccp(size.width-oPoint.x-20, size.height*0.5f+100));
	this->addChild(right, 0, rightId);
	CCMoveBy *action1=CCMoveBy::create(0.1f, ccp(8, 0));
	right->runAction(CCRepeatForever::create(CCSequence::create(action1, action1->reverse(), NULL)));

	//根据屏幕比例，计算出缩放大小
	float s=(size.width-2*oPoint.x)/size.width;
	CCSprite *textBg=CCSprite::createWithSpriteFrameName(s_plane_text_bg);
	textBg->setPosition(ccp(size.width*0.5f, size.height*0.5f-200));
	textBg->setScale(s);
	this->addChild(textBg);
	CCLabelTTF *text=CCLabelTTF::create("", "MarkerFelt", 28);
	text->setPosition(textBg->getPosition());
	this->addChild(text, 0, textId);


	//创建滑动视图
	scroll=CCScrollView::create(CCSizeMake(size.width, size.height));
	scroll->setDirection(kCCScrollViewDirectionHorizontal);
	scroll->setBounceable(false);
	scroll->setContentSize(CCSizeMake(size.width*6, size.height));
	scroll->setTouchEnabled(true);
	this->addChild(scroll, 1);
	//创建滑动视图中内容图层
	maxNum=CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_NUM, 0)/10;
	if (maxNum>5) {
		maxNum = 5;
	}
	CCLayer *contentLayer=CCLayer::create();
	for(int i=0; i<6; i++){
		CCSprite *plane;
		if(i<=maxNum){
			plane=CCSprite::createWithSpriteFrameName(CCString::createWithFormat(s_plane_img_d, i)->getCString());
		}else{
			plane=CCSprite::createWithSpriteFrameName(s_plane_unable);
		}
		plane->setPosition(ccp(size.width*0.5f+i*size.width, size.height*0.5f+100));
		plane->setScale(s);
		contentLayer->addChild(plane);
	}
	int w=size.width;
	CCPoint adjustPos = ccpSub(scroll->getContentOffset(), ccp(w*maxNum, 0));  
	float adjustAnimDelay = (float) (w*maxNum) / ADJUST_ANIM_VELOCITY;
	scroll->setContentOffsetInDuration(adjustPos, adjustAnimDelay); 
	scroll->addChild(contentLayer);
	return  true;
}
void PlaneMenuLayer::onEnterTransitionDidFinish(){
	AudioUtil::soundMenuInOut(1);
	CCSize size=CCDirector::sharedDirector()->getWinSize();
	CCPoint oPoint=CCDirector::sharedDirector()->getVisibleOrigin();
	//创建标题
	CCSprite *title=CCSprite::createWithSpriteFrameName(s_plane_title);
	title->setPosition(ccp(size.width*0.5f, size.height+title->getContentSize().height));
	this->addChild(title, 0, titleId);
	title->runAction(CCMoveTo::create(0.3f, ccp(size.width*0.5f, size.height-oPoint.y-60)));

	//根据屏幕比例，计算出缩放大小
	float s=(size.width-2*oPoint.x)/size.width;
	//创建下部视图
	CCSprite *bottom=CCSprite::createWithSpriteFrameName(s_select_bottom);
	bottom->setScale(s);
	bottom->setPosition(ccp(size.width*0.5f, -bottom->getContentSize().height));
	this->addChild(bottom, 0, bottomId);
	bottom->runAction(CCMoveTo::create(0.3f, ccp(size.width*0.5f, oPoint.y+bottom->getContentSize().height*s*0.5f)));
	//创建返回选项
	back=CCMenuItemImage::create(NULL, NULL, this, menu_selector(PlaneMenuLayer::ccMenuCall));
	back->setNormalSpriteFrame(
		CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(s_select_back));
	back->setSelectedSpriteFrame(
		CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(s_select_back_pressed));
	back->setScale(s);
	back->setTag(0);
	back->setPosition(ccp(-back->getContentSize().width, back->getContentSize().height*s*0.5f+oPoint.y));
	back->runAction(CCMoveTo::create(0.3f, ccp(back->getContentSize().width*s*0.5f*s+oPoint.x, back->getContentSize().height*s*0.5f+oPoint.y)));
	//创建开始选项
	start=CCMenuItemImage::create(NULL, NULL, NULL
		, this, menu_selector(PlaneMenuLayer::ccMenuCall));
	start->setNormalSpriteFrame(
		CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(s_select_start));
	start->setSelectedSpriteFrame(
		CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(s_select_start_pressed));
	start->setDisabledSpriteFrame(
		CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(s_select_start_unable));
	start->setScale(s);
	start->setTag(1);
	start->setPosition(ccp(size.width+start->getContentSize().width, start->getContentSize().height*s*0.5f+oPoint.y));
	start->runAction(CCMoveTo::create(0.3f, ccp(size.width-start->getContentSize().width*s*0.5f*s-oPoint.x, start->getContentSize().height*s*0.5f+oPoint.y)));
	//创建按钮菜单
	buttonMenu=CCMenu::create(back, start, NULL);
	buttonMenu->setPosition(ccp(0, 0));
	this->addChild(buttonMenu);

	int ids[]={tab0Id, tab1Id, tab2Id, tab3Id, tab4Id, tab5Id};
	float w=40;
	//创建页数标签
	for(int i=0; i<6; i++){
		CCSprite *page=CCSprite::createWithSpriteFrameName(CCString::createWithFormat(s_select_page_d, i)->getCString());
		page->setPosition(ccp(size.width*0.5f+i*w-5/2*w-w*0.5f, 150));
		bottom->addChild(page, 0, ids[i]);
	}
	updatePageTab(maxNum);
	CCLayer::onEnterTransitionDidFinish();
}
void PlaneMenuLayer::onEnter(){
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
	CCLayer::onEnter();
}
void PlaneMenuLayer::onExit(){
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCLayer::onExit();
}
bool PlaneMenuLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *event){
	CCSize size=CCDirector::sharedDirector()->getWinSize();
	CCPoint oPoint=CCDirector::sharedDirector()->getVisibleOrigin();
	float x=pTouch->getLocation().x;
	float y=pTouch->getLocation().y;
	if(x<=oPoint.x+40 && y>oPoint.y+200){
		//点击左边缘，向后翻页
		int w=size.width;
		int x = scroll->getContentOffset().x;  
		int page=-x/w;
		int offset = (int) x %w; 
		if(page>0 && offset==0){
			page--;
			CCPoint adjustPos = ccpSub(scroll->getContentOffset(), ccp(-w, 0));  
			float adjustAnimDelay = (float) (w) / ADJUST_ANIM_VELOCITY;
			scroll->setContentOffsetInDuration(adjustPos, adjustAnimDelay); 
			updatePageTab(page);
		}
		isTouchLeftOrRight=true;
	}else if(x>=size.width-oPoint.x-40 && y>oPoint.y+200){
		//点击右边缘，向前翻页
		int w=size.width;
		int x = scroll->getContentOffset().x;  
		int page=-x/w;
		int offset = (int) x %w;
		if(page<5 && offset==0){
			page++;
			CCPoint adjustPos = ccpSub(scroll->getContentOffset(), ccp(w, 0));  
			float adjustAnimDelay = (float) (w) / ADJUST_ANIM_VELOCITY;
			scroll->setContentOffsetInDuration(adjustPos, adjustAnimDelay); 
			updatePageTab(page);
		}
		isTouchLeftOrRight=true;
	}else{
		isTouchLeftOrRight=false;
		tempX=scroll->getContentOffset().x;
	}
	return true;
}
void PlaneMenuLayer::ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event){

}
void PlaneMenuLayer::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event){
	if(!isTouchLeftOrRight){
		adjustScrollView(scroll);
	}
}
void PlaneMenuLayer::adjustScrollView(CCScrollView *scrollView){
	scroll->unscheduleAllSelectors();
	CCSize size=CCDirector::sharedDirector()->getWinSize();
	int w=size.width;
	int x = scrollView->getContentOffset().x;  
	int page=-x/w;
	int temp=tempX-x;
	int offset = (int) x %w; 
	CCPoint adjustPos;  
	float adjustAnimDelay;  
	if (temp <0 ) { 
		if(temp<-50){
			//向左滑动超过50, 自动调整向后翻一页
			adjustPos = ccpSub(scrollView->getContentOffset(), ccp(offset, 0));  
			adjustAnimDelay = (float) abs(offset) / ADJUST_ANIM_VELOCITY;  
		}else{
			//向左滑动小于50, 自动返回原来页
			adjustPos = ccpSub(scrollView->getContentOffset(), ccp(w+offset, 0));  
			adjustAnimDelay = (float) (w+offset) / ADJUST_ANIM_VELOCITY;  
			page++;
		}
		scrollView->setContentOffsetInDuration(adjustPos, adjustAnimDelay); 
	}else{
		if(temp>50){
			//向右滑动超过50, 自动调整向前翻一页
			adjustPos = ccpSub(scrollView->getContentOffset(), ccp(w+offset, 0));  
			adjustAnimDelay = (float) (w+offset) / ADJUST_ANIM_VELOCITY;  
			page++;
		}else{
			//向右滑动小于50, 自动返回原来页
			adjustPos = ccpSub(scrollView->getContentOffset(), ccp(offset, 0));  
			adjustAnimDelay = (float) abs(offset) / ADJUST_ANIM_VELOCITY;  

		}
		scrollView->setContentOffsetInDuration(adjustPos, adjustAnimDelay); 
	}
	updatePageTab(page);
}
void PlaneMenuLayer::updatePageTab(int page){
	if(page<0){
		page=0;
	}if(page>4){
		page=5;
	}
	if(page>maxNum){
		start->setEnabled(false);
	}else{
		start->setEnabled(true);
	}

	int ids[]={tab0Id, tab1Id, tab2Id, tab3Id, tab4Id, tab5Id};
	CCSprite *bottom=(CCSprite *)this->getChildByTag(bottomId);
	for(int i=0; i<6; i++){
		CCSprite *tab=(CCSprite *)bottom->getChildByTag(ids[i]);
		if(i==page){
			tab->setColor(ccc3(255, 255, 255));
		}else{
			tab->setColor(ccc3(100, 100, 100));
		}
	}
	CCSprite *left=(CCSprite *)this->getChildByTag(leftId);
	CCSprite *right=(CCSprite *)this->getChildByTag(rightId);
	//如果页数是0时，隐藏左箭头否则显示
	if(page==0){
		if(left->isVisible()){
			left->setVisible(false);
		}
	}else{
		if(!left->isVisible()){
			left->setVisible(true);
		}
	}
	//如果页数是5时，隐藏右箭头否则显示
	if(page==5){
		if(right->isVisible()){
			right->setVisible(false);
		}
	}else{
		if(!right->isVisible()){
			right->setVisible(true);
		}
	}
	CCLabelTTF *text=(CCLabelTTF *)this->getChildByTag(textId);
	text->setString(StringUtil::getString(CCString::createWithFormat("plane_text%d", page)->getCString()));
}
void PlaneMenuLayer::ccMenuCall(CCObject *object){
	CCMenuItemImage *item=(CCMenuItemImage *)object;
	int tag=item->getTag();
	if(tag==0){
		toScene(0);
	}else if(tag==1){
		toScene(1);
	}
}
void PlaneMenuLayer::toScene(int state){
	AudioUtil::soundMenuInOut(0);
	CCSize size=CCDirector::sharedDirector()->getWinSize();
	CCSprite *title=(CCSprite *)this->getChildByTag(titleId);
	CCSprite *bottom=(CCSprite *)this->getChildByTag(bottomId);
	bottom->runAction(CCMoveTo::create(0.2f, ccp(size.width*0.5f, -bottom->getContentSize().height)));
	if(start){
		start->runAction(CCMoveTo::create(0.2f, ccp(size.width+start->getContentSize().width, start->getPositionY())));
	}
	if(back){
		back->runAction(CCMoveTo::create(0.2f, ccp(-back->getContentSize().width, back->getPositionY())));
	}
	if(state==0){
		title->runAction(CCSequence::create(
			CCMoveTo::create(0.2f, ccp(size.width*0.5f, size.height+title->getContentSize().height))
			, CCCallFunc::create(this, callfunc_selector(PlaneMenuLayer::toSelect))
			, NULL));
	}else{
		title->runAction(CCSequence::create(
			CCMoveTo::create(0.2f, ccp(size.width*0.5f, size.height+title->getContentSize().height))
			, CCCallFunc::create(this, callfunc_selector(PlaneMenuLayer::toGame))
			, NULL));
	}
}
void PlaneMenuLayer::toSelect(){
	CCDirector::sharedDirector()->replaceScene(SelectScene::create());
}
void PlaneMenuLayer::toGame(){
	CCSize size=CCDirector::sharedDirector()->getWinSize();
	int w=size.width;
	int x = scroll->getContentOffset().x;  
	int type=-x/w;
	CCDirector::sharedDirector()->replaceScene(GameScene::create(num, type, 1));
}
void PlaneMenuLayer::keyBackClicked(){
	toScene(0);
}