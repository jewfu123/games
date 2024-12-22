#include "EditBoxLayer.h"
#include "EditBoxLayerLoader.h"
#include "../xml/XMLParser.h"
#include "../UserRecord/UserRecord.h"
#include "Tool.h"
#include "MenuLayer.h"
USING_NS_CC;
USING_NS_CC_EXT;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
#include <jni.h>
#include "platform/android/jni/JniHelper.h" 
#include <android/log.h> 
#endif 

EditBoxLayer::EditBoxLayer():
gold_box(NULL),
mine_box(NULL),
nuke_box(NULL)
{
}

EditBoxLayer::~EditBoxLayer()
{ 
}

/*bool EditBoxLayer::init(){
	bool bRet = false;
	do 
	{
     CCPoint visibleOrigin = CCEGLView::sharedOpenGLView()->getVisibleOrigin();
     CCSize visibleSize = CCEGLView::sharedOpenGLView()->getVisibleSize();
    
	 CCSpriteFrameCache *UI_mission=CCSpriteFrameCache::sharedSpriteFrameCache();
	 UI_mission->addSpriteFramesWithFile("ccbResources/UI_mission.plist");
    // Back Menu
	 CCMenuItemSprite *backMenu= CCMenuItemSprite::create(CCSprite::createWithSpriteFrame(UI_mission->spriteFrameByName("button_back.png")),CCSprite::createWithSpriteFrame(UI_mission->spriteFrameByName("button_back_1.png"))
		 ,this,menu_selector(EditBoxLayer::menuCloseCallback));
	 backMenu->setPosition(ccp(82,30));
	 CCMenu* menu = CCMenu::create(backMenu,NULL);
	 menu->setPosition(CCPointZero);
	 this->addChild(menu, 101,201);

    CCSize editBoxSize = CCSizeMake(visibleSize.width - 200, 60);

    // top
    m_pEditName = CCEditBox::create(editBoxSize, CCScale9Sprite::create());
    m_pEditName->setPosition(ccp(visibleOrigin.x+visibleSize.width/2, visibleOrigin.y+visibleSize.height/2));//ccp(visibleOrigin.x+visibleSize.width/2, visibleOrigin.y+visibleSize.height*3/4)

    m_pEditName->setFontSize(25);
    m_pEditName->setFontColor(ccWHITE);//ccBLUE,ccRED
    m_pEditName->setPlaceHolder("Name");
    m_pEditName->setPlaceholderFontColor(ccBLUE);
    m_pEditName->setMaxLength(8);
    m_pEditName->setReturnType(kKeyboardReturnTypeDone);
	m_pEditName->setInputMode(kEditBoxInputModeNumeric);//数字
    m_pEditName->setDelegate(this);
    this->addChild(m_pEditName);
    this->setPosition(ccp(10, 20));
	

	bRet = true;
	} while (0);
	return bRet;
}*/

void EditBoxLayer::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader) {
	UserRecord *gradeUR = new UserRecord("tank.db","tank");
	int gold = gradeUR->readIntFromSQLite("gold");
	int tankCont = gradeUR->readIntFromSQLite("tankCont");
	int landmine = gradeUR->readIntFromSQLite("landmine");//地雷
	int bomb = gradeUR->readIntFromSQLite("bomb");//炸弹
	gradeUR->closeDB();
	free(gradeUR);
	char lable[30];
	//金钱
	sprintf(lable,"%d",gold);
	CCLabelAtlas* moneyNum=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
	moneyNum->setAnchorPoint(ccp(0.5,0.5));
	moneyNum->setPosition(gold_box->getContentSize().width/2+10,gold_box->getContentSize().height/2);
	gold_box->addChild(moneyNum,1,2);

	//地雷
	sprintf(lable,"%d",landmine);
	CCLabelAtlas* landmineNum=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
	landmineNum->setAnchorPoint(ccp(0.5,0.5));
	landmineNum->setPosition(mine_box->getContentSize().width/2+10,mine_box->getContentSize().height/2);
	mine_box->addChild(landmineNum,1,2);

	//炸弹
	sprintf(lable,"%d",bomb);
	CCLabelAtlas* bombNum=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
	bombNum->setAnchorPoint(ccp(0.5,0.5));
	bombNum->setPosition(nuke_box->getContentSize().width/2+10,nuke_box->getContentSize().height/2);
	nuke_box->addChild(bombNum,1,2);

	CCSpriteFrameCache *UI_mission=CCSpriteFrameCache::sharedSpriteFrameCache();
	UI_mission->addSpriteFramesWithFile("ccbResources/UI_mission.plist");
	// Back Menu
	CCMenuItemSprite *backMenu= CCMenuItemSprite::create(CCSprite::createWithSpriteFrame(UI_mission->spriteFrameByName("button_back.png")),CCSprite::createWithSpriteFrame(UI_mission->spriteFrameByName("button_back_1.png"))
		,this,menu_selector(EditBoxLayer::menuCallback));
	backMenu->setPosition(ccp(82,30));
	CCMenu* menu = CCMenu::create(backMenu,NULL);
	menu->setPosition(CCPointZero);
	this->addChild(menu, 101,201);

	isLingQu = false;
}

SEL_MenuHandler EditBoxLayer::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){

	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_gift", EditBoxLayer::onGiftClicked);
	return NULL;
}
SEL_CCControlHandler EditBoxLayer::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName) {
	return NULL;
}
bool EditBoxLayer::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode) {
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "gold_box", CCSprite*, this->gold_box);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mine_box", CCSprite*, this->mine_box);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "nuke_box", CCSprite*, this->nuke_box);

	 return false;
}

bool EditBoxLayer::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue){
	 return false;
}

void EditBoxLayer::toExtensionsMainLayer(cocos2d::CCObject *sender)
{
   // ExtensionsTestScene *pScene = new ExtensionsTestScene();
  //  pScene->runThisTest();
  //  pScene->release();
}

void EditBoxLayer::onGiftClicked(CCObject* pSender){

 if(!this->getChildByTag(10)){
	 CCSize size =CCDirector::sharedDirector()->getWinSize();

	ccColor4B color = ccc4(0, 0, 0, 210);  
    CCLayerColor *color_layer = CCLayerColor::create(color);  
	this->addChild(color_layer,10,10);


	CCPoint visibleOrigin = CCEGLView::sharedOpenGLView()->getVisibleOrigin();
	CCSize visibleSize = CCEGLView::sharedOpenGLView()->getVisibleSize();

	CCSpriteFrameCache *UI_mission=CCSpriteFrameCache::sharedSpriteFrameCache();
	UI_mission->addSpriteFramesWithFile("ccbResources/UI_mission.plist");
	// Back Menu
	CCMenuItemSprite *backMenu= CCMenuItemSprite::create(CCSprite::createWithSpriteFrame(UI_mission->spriteFrameByName("button_back.png")),CCSprite::createWithSpriteFrame(UI_mission->spriteFrameByName("button_back_1.png"))
		,this,menu_selector(EditBoxLayer::menuCloseCallback));
	backMenu->setPosition(ccp(82,30));
	//确定
	CCMenuItemSprite *yesMenu= CCMenuItemSprite::create(CCSprite::create("EditBox/sure.png"),CCSprite::create("EditBox/sure.png")
		,this,menu_selector(EditBoxLayer::yesCallback));
	yesMenu->setPosition(ccp(size.width-82,30));

	CCMenu* menu = CCMenu::create(backMenu,yesMenu,NULL);
	menu->setPosition(CCPointZero);
	color_layer->addChild(menu, 101,201);

	XMLParser *pXmlParser = XMLParser::parseWithFile("activeNum.xml");
	CCString *fontStr = pXmlParser->getString("font");
	//CCString *codeStr = pXmlParser->getString("code");

	CCSize editBoxSize = CCSizeMake(visibleSize.width - 200, 60);
	// top
	m_pEditName = CCEditBox::create(editBoxSize, CCScale9Sprite::create("buttonHighlighted.png"));
	m_pEditName->setPosition(ccp(visibleOrigin.x+visibleSize.width/2, visibleOrigin.y+visibleSize.height*3/4));//ccp(visibleOrigin.x+visibleSize.width/2, visibleOrigin.y+visibleSize.height*3/4)

	m_pEditName->setFontSize(25);
	m_pEditName->setFontColor(ccWHITE);//ccBLUE,ccRED
	m_pEditName->setPlaceHolder(fontStr->getCString());
	CCLog("fontStr:%s",fontStr->getCString());
	m_pEditName->setPlaceholderFontColor(ccWHITE);
	m_pEditName->setMaxLength(8);
	m_pEditName->setReturnType(kKeyboardReturnTypeDefault);// kKeyboardReturnTypeDone
	m_pEditName->setInputMode(kEditBoxInputModeAny);//数字  kEditBoxInputModeNumeric
	m_pEditName->setDelegate(this);
	color_layer->addChild(m_pEditName);
  }
}

void EditBoxLayer::editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox)
{
    CCLog("editBox %p DidBegin !", editBox);
}

void EditBoxLayer::editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox)
{
    CCLog("editBox %p DidEnd !", editBox);
}

void EditBoxLayer::editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text)
{
    CCLog("editBox %p TextChanged, text:%s ", editBox, text.c_str());

	
	strcpy(activeNumtext,text.c_str());
	CCLog("activeNumtext:%s ", activeNumtext);

}

void EditBoxLayer::editBoxReturn(CCEditBox* editBox)
{
    CCLog("editBox %p was returned !",editBox);
}


void EditBoxLayer::yesCallback(CCObject* pSender){
	CCLog("activeNumtext:%s ", activeNumtext);
	CCLog("activeNumtext:%d",atoi(activeNumtext));


	XMLParser *pXmlParser = XMLParser::parseWithFile("activeNum.xml");
	CCString *codeStr = pXmlParser->getString("code");
	CCLog("codeStr->getCString():%s ", codeStr->getCString());


	if(atoi(codeStr->getCString())==atoi(activeNumtext)){//成功

	 if(!isLingQu){
		   isLingQu = true;
		UserRecord *gradeUR = new UserRecord("tank.db","tank");
		int gold = gradeUR->readIntFromSQLite("gold");
		int landmine = gradeUR->readIntFromSQLite("landmine");
		int bomb = gradeUR->readIntFromSQLite("bomb");

		gold+=800;
		landmine+=5;
		bomb+=3;
		char lable[30];
		sprintf(lable,"%d",gold);
		((CCLabelAtlas*)(gold_box->getChildByTag(2)))->setString(lable);


		char tm[50];
		sprintf(tm, "%d",landmine); 
		CCLabelAtlas* _landmineNum =(CCLabelAtlas*) (mine_box->getChildByTag(2));
		_landmineNum->setString(tm);

		sprintf(tm, "%d",bomb); 
		CCLabelAtlas* _bombNum =(CCLabelAtlas*) (nuke_box->getChildByTag(2));
		_bombNum->setString(tm);

		gradeUR->saveInt("gold",gold);
		gradeUR->saveInt("landmine",landmine);
		gradeUR->saveInt("bomb",bomb);

		gradeUR->closeDB();
		free(gradeUR);
		SaveBooleanToXML("activeNum",true);
		

		CCSize size =CCDirector::sharedDirector()->getWinSize();
		CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		cocos2d::extension::CCBReader *ccbreader = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
		ccbreader->autorelease();
		//读取一个ccbi的文件,生成一个ccnode实例
		CCNode  *animationsTest  = ccbreader->readNodeGraphFromFile("EditBox/UI_box_lingquchenggong.ccbi",this);//
		//设置坐标
		//animationsTest->setPosition(ccp(0,0));
		animationsTest->setPosition(ccp(size.width/2,size.height/2));
		if(animationsTest!= NULL) {
			(this->getChildByTag(10))->addChild(animationsTest,1,2986);
		}
	    ccbreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(EditBoxLayer::backClicked)); 
	   }
		//keyBackClicked();
	}else{//失败
		CCSize size =CCDirector::sharedDirector()->getWinSize();
		CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		cocos2d::extension::CCBReader *ccbreader = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
		ccbreader->autorelease();
		//读取一个ccbi的文件,生成一个ccnode实例
		CCNode  *animationsTest  = ccbreader->readNodeGraphFromFile("EditBox/UI_box_shurubuzhengque.ccbi",this);//
		//设置坐标
		//animationsTest->setPosition(ccp(0,0));
		animationsTest->setPosition(ccp(size.width/2,size.height/2));
		if(animationsTest!= NULL) {
			(this->getChildByTag(10))->addChild(animationsTest,1,2986);
		}
	}
 
}

void EditBoxLayer::menuCloseCallback(CCObject* pSender){

	this->removeChildByTag(10,-1);
//	CCDirector::sharedDirector()->popScene();
}
void  EditBoxLayer::backClicked(){
	keyBackClicked();
}
void EditBoxLayer::menuCallback(CCObject* pSender){

	keyBackClicked();
}
//返回，退出
void  EditBoxLayer::keyBackClicked(){
	//menuCloseCallback(this);
	//CCDirector::sharedDirector()->popScene();
	CCScene *scene = CCScene::create();
	MenuLayer *node = MenuLayer::create();
	scene->addChild(node);
	CCDirector::sharedDirector()->replaceScene(scene);
}
