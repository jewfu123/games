#include "SelectMenuLayer.h"
#include "GameResource.h"
#include "KeyId.h"
#include "AudioUtil.h"
#include "../index/IndexScene.h"
#include "../plane/PlaneScene.h"


#define ADJUST_ANIM_VELOCITY 3200;

enum{
	titleId, bottomId, tab0Id, tab1Id, tab2Id, tab3Id, tab4Id, leftId, rightId
};


bool SelectMenuLayer::init(){
	if(!CCLayer::init()){ 
		return false;
	}
	//初始化
	back=NULL;
	start=NULL;
	kuang=NULL;
	buttonMenu=NULL;
	numMenu=NULL;
	scroll=NULL;

	setKeypadEnabled(true);

	CCSize size=CCDirector::sharedDirector()->getWinSize();
	CCPoint oPoint=CCDirector::sharedDirector()->getVisibleOrigin();
	//设置背景
	CCSprite *bg=CCSprite::createWithSpriteFrameName(s_select_bg);
	bg->setPosition(ccp(size.width*0.5f, size.height*0.5f));
	this->addChild(bg);
	//创建滑动视图
	scroll=CCScrollView::create(CCSizeMake(size.width, size.height));
	scroll->setDirection(kCCScrollViewDirectionHorizontal);
	scroll->setBounceable(false);
	scroll->setContentSize(CCSizeMake(size.width*5, size.height));
	scroll->setTouchEnabled(true);
	this->addChild(scroll, 1);

	//创建滑动视图中内容图层
	CCLayer *contentLayer=CCLayer::create();
	//创建选中框
	kuang=CCSprite::createWithSpriteFrameName(s_select_kunag);
	contentLayer->addChild(kuang, 1);
	 
	CCArray *itemArray=CCArray::create();
	float w=90.0f;
	float h=140.0f;
	int maxNum=CCUserDefault::sharedUserDefault()->getIntegerForKey(KEY_NUM, 0);
	if(maxNum >= 100){
		maxNum = 99;
	}
	num=maxNum; 
	//创建选择关卡内容
	for(int p=0; p<5; p++){
		for(int i=0; i<4; i++){
			for(int j=0; j<5; j++){
				float tempH=0;
				int n=p*20+i*5+j;
				if(j%2==0){
					tempH=50;
				}else{
					tempH=0;
				}
				if(n<=maxNum){
					CCMenuItemImage *item=CCMenuItemImage::create(NULL, NULL, this, menu_selector(SelectMenuLayer::ccMenuCallNum));
					item->setNormalSpriteFrame(
						CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(s_select_item));
					item->setSelectedSpriteFrame(
						CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(s_select_item));

					item->setPosition(ccp(p*size.width+size.width*0.5f+j*w-5/2*w, size.height*0.5f+(h+h*0.5f-i*h)+tempH));
					item->setTag(n);

					CCLabelAtlas* text=CCLabelAtlas::create(CCString::createWithFormat("%d", n+1)->getCString()
						, s_select_nums_font, 45, 57, '0');
					text->setAnchorPoint(ccp(0.5f, 0.5f));
					text->setPosition(ccp(item->getContentSize().width*0.5f, item->getContentSize().height*0.5f));
					if(n!=100){
						text->setScale(0.7f);
					}else{
						text->setScale(0.6f);
					}
					item->addChild(text);
					itemArray->addObject(item);
					if(n==maxNum){
						kuang->setPosition(item->getPosition());
					}
				}else{
					CCSprite *item=CCSprite::createWithSpriteFrameName(s_select_item_unable);
					item->setPosition(ccp(p*size.width+size.width*0.5f+j*w-5/2*w, size.height*0.5f+(h+h*0.5f-i*h)+tempH));
					contentLayer->addChild(item);
					CCLabelAtlas* text=CCLabelAtlas::create(CCString::createWithFormat("%d", n+1)->getCString()
						, s_select_nums_font, 45, 57, '0');
					text->setAnchorPoint(ccp(0.5f, 0.5f));
					if(n!=100){
						text->setScale(0.7f);
					}else{
						text->setScale(0.6f);
					}
					text->setPosition(ccp(item->getContentSize().width*0.5f, item->getContentSize().height*0.5f));
					item->addChild(text);
				}
			}
		}
	}
	numMenu=CCMenu::createWithArray(itemArray);
	numMenu->setPosition(ccp(0, 0));
	contentLayer->addChild(numMenu);
	scroll->addChild(contentLayer);
	//创建向左箭头
	CCSprite *left=CCSprite::createWithSpriteFrameName(s_select_left);
	left->setPosition(ccp(oPoint.x+20, size.height*0.5f+25));
	this->addChild(left, 0, leftId);
	CCMoveBy *action0=CCMoveBy::create(0.1f, ccp(-8, 0));
	left->runAction(CCRepeatForever::create(CCSequence::create(action0, action0->reverse(), NULL)));
	//创建向右箭头
	CCSprite *right=CCSprite::createWithSpriteFrameName(s_select_right);
	right->setPosition(ccp(size.width-oPoint.x-20, size.height*0.5f+25));
	this->addChild(right, 0, rightId);
	CCMoveBy *action1=CCMoveBy::create(0.1f, ccp(8, 0));
	right->runAction(CCRepeatForever::create(CCSequence::create(action1, action1->reverse(), NULL)));
	return  true;
}

void SelectMenuLayer::onEnter(){
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, INT_MIN, true);  
	CCLayer::onEnter();
}
void SelectMenuLayer::onEnterTransitionDidFinish(){
	AudioUtil::soundMenuInOut(1);
	CCSize size=CCDirector::sharedDirector()->getWinSize();
	CCPoint oPoint=CCDirector::sharedDirector()->getVisibleOrigin();
	//创建标题
	CCSprite *title=CCSprite::createWithSpriteFrameName(s_select_title);
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
	back=CCMenuItemImage::create(NULL, NULL, this, menu_selector(SelectMenuLayer::ccMenuCall));
	back->setNormalSpriteFrame(
		CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(s_select_back));
	back->setSelectedSpriteFrame(
		CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(s_select_back_pressed));
	back->setScale(s);
	back->setTag(0);
	back->setPosition(ccp(-back->getContentSize().width, back->getContentSize().height*s*0.5f+oPoint.y));
	back->runAction(CCMoveTo::create(0.3f, ccp(back->getContentSize().width*s*0.5f*s+oPoint.x, back->getContentSize().height*s*0.5f+oPoint.y)));
	//创建开始选项
	start=CCMenuItemImage::create(NULL, NULL, this, menu_selector(SelectMenuLayer::ccMenuCall));
	start->setNormalSpriteFrame(
		CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(s_select_next));
	start->setSelectedSpriteFrame(
		CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(s_select_next_pressed));
	start->setScale(s);
	start->setTag(1);
	start->setPosition(ccp(size.width+start->getContentSize().width, start->getContentSize().height*s*0.5f+oPoint.y));
	start->runAction(CCMoveTo::create(0.3f, ccp(size.width-start->getContentSize().width*s*0.5f*s-oPoint.x, start->getContentSize().height*s*0.5f+oPoint.y)));
	//创建按钮菜单
	buttonMenu=CCMenu::create(back, start, NULL);
	buttonMenu->setPosition(ccp(0, 0));
	this->addChild(buttonMenu);

	int ids[]={tab0Id, tab1Id, tab2Id, tab3Id, tab4Id};
	float w=40;
	//创建页数标签
	for(int i=0; i<5; i++){
		CCSprite *page=CCSprite::createWithSpriteFrameName(CCString::createWithFormat(s_select_page_d, i)->getCString());
		page->setPosition(ccp(size.width*0.5f+i*w-5/2*w, 150));
		bottom->addChild(page, 0, ids[i]);
	}
	updatePageTab(0);
	CCLayer::onEnterTransitionDidFinish();
}
void SelectMenuLayer::onExit(){
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCLayer::onExit();
}

bool SelectMenuLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
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
		isScroll=false;
		isButtonMenu=false;
		isNumMenu=false;
		beganX=0;
		tempX=0;
		isMove=false;
	}else if(x>=size.width-oPoint.x-40 && y>oPoint.y+200){
		//点击右边缘，向前翻页
		int w=size.width;
		int x = scroll->getContentOffset().x;  
		int page=-x/w;
		int offset = (int) x %w;
		if(page<4 && offset==0){
			page++;
			CCPoint adjustPos = ccpSub(scroll->getContentOffset(), ccp(w, 0));  
			float adjustAnimDelay = (float) (w) / ADJUST_ANIM_VELOCITY;
			scroll->setContentOffsetInDuration(adjustPos, adjustAnimDelay); 
			updatePageTab(page);
		}
		isScroll=false;
		isButtonMenu=false;
		isNumMenu=false;
		beganX=0;
		tempX=0;
		isMove=false;
	}else{
		isScroll=scroll->ccTouchBegan(pTouch, pEvent);
		isButtonMenu=buttonMenu->ccTouchBegan(pTouch, pEvent);
		isNumMenu=numMenu->ccTouchBegan(pTouch, pEvent);
		beganX=pTouch->getLocation().x;
		tempX=scroll->getContentOffset().x;
		isMove=false;
	}
	return true;
}
void SelectMenuLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
	float x=pTouch->getLocation().x;
	if(x-beganX>5 || x-beganX<-5){
		isMove=true;
	}
	if(isScroll){
		scroll->ccTouchMoved(pTouch, pEvent);
	}
	if(isButtonMenu){
		buttonMenu->ccTouchMoved(pTouch, pEvent);
	}
	if(isNumMenu){
		numMenu->ccTouchMoved(pTouch, pEvent);
	}
}
void SelectMenuLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
	if(isScroll){
		scroll->ccTouchEnded(pTouch, pEvent);
		adjustScrollView(scroll);
	}
	if(isButtonMenu){
		buttonMenu->ccTouchEnded(pTouch, pEvent);
	}
	if(isNumMenu){
		numMenu->ccTouchEnded(pTouch, pEvent);
	}
}

void SelectMenuLayer::ccMenuCall(CCObject *object){
	CCMenuItemImage *item=(CCMenuItemImage *)object;
	int tag=item->getTag();
	if(tag==0){
		toScene(0);
	}else if(tag==1){
		toScene(1);
	}
}
void SelectMenuLayer::ccMenuCallNum(CCObject *object){
	if(!isMove){
		CCMenuItemImage *item=(CCMenuItemImage *)object;
		num=item->getTag();
		kuang->setPosition(item->getPosition());
	}
}

void SelectMenuLayer::adjustScrollView(CCScrollView *scrollView){
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
void SelectMenuLayer::updatePageTab(int page){
	int ids[]={tab0Id, tab1Id, tab2Id, tab3Id, tab4Id};
	CCSprite *bottom=(CCSprite *)this->getChildByTag(bottomId);
	for(int i=0; i<5; i++){
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
	//如果页数是4时，隐藏右箭头否则显示
	if(page==4){
		if(right->isVisible()){
			right->setVisible(false);
		}
	}else{
		if(!right->isVisible()){
			right->setVisible(true);
		}
	}
}
void SelectMenuLayer::keyBackClicked(){
	toScene(0);
}
void SelectMenuLayer::toScene(int state){
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
			, CCCallFunc::create(this, callfunc_selector(SelectMenuLayer::toIndex))
			, NULL));
	}else{
		title->runAction(CCSequence::create(
			CCMoveTo::create(0.2f, ccp(size.width*0.5f, size.height+title->getContentSize().height))
			, CCCallFunc::create(this, callfunc_selector(SelectMenuLayer::toPlane))
			, NULL));
	}
}

void SelectMenuLayer::toIndex(){
	CCDirector::sharedDirector()->replaceScene(IndexScene::create());
}
void SelectMenuLayer::toPlane(){
	CCDirector::sharedDirector()->replaceScene(PlaneScene::create(num));
}
