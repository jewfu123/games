#include "AchieveCocosBuilder.h"
#include "CustomTableViewCell.h"
#include "../UserRecord/UserRecord.h"
#include "../xml/XMLParser.h"

USING_NS_CC;
USING_NS_CC_EXT;

AchieveCocosBuilder::AchieveCocosBuilder(void):
gold_achievement(NULL)
{
	 UserRecord *achieveUR = new UserRecord("tank.db","achieveRecord");
	 for(int i=0;i<10;i++){
		 char taskname[30];
		 sprintf(taskname,"achieveRecord%d", i);
		 achieveRecord[i]=achieveUR->readIntFromSQLite(taskname);
		//CCLog("achieveRecord:%d",achieveRecord[i]);
		sprintf(taskname,"isRecordGold%d", i);
		isRecordGold[i]=achieveUR->readIntFromSQLite(taskname);
		CCLog("isRecordGold:%d",isRecordGold[i]);
	}
	achieveUR->closeDB();
	free(achieveUR);

	XMLParser *pXmlParser = XMLParser::parseWithFile("achieve/achieve.xml");
	for(int i=0;i<10;i++){
	   char taskname[30];

	   sprintf(taskname,"task_%d", i+1);
	   task[i]= (pXmlParser->getString(taskname))->intValue();
	  // CCLog("task_:%d",task[i]);

	   sprintf(taskname,"task_gold_%d", i+1);
	   task_gold[i]= (pXmlParser->getString(taskname))->intValue();
	   CCLog("task_gold:%d",task_gold[i]);
	}
}


AchieveCocosBuilder::~AchieveCocosBuilder(void)
{
	CC_SAFE_RELEASE(gold_achievement);
}
void AchieveCocosBuilder::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader) {
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	UserRecord *gradeUR = new UserRecord("tank.db","tank");
	int gold = gradeUR->readIntFromSQLite("gold");

	gradeUR->closeDB();
	free(gradeUR);

	char lable[30];
    sprintf(lable,"%d",gold);

    CCLabelAtlas* starNum=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
	starNum->setAnchorPoint(ccp(0.5,0.5));
	starNum->setPosition(gold_achievement->getContentSize().width/2+10,gold_achievement->getContentSize().height/2);
	gold_achievement->addChild(starNum,1,1);


	CCTableView* 	tableView = CCTableView::create(this, CCSizeMake(600, 350));
	tableView->setDirection(kCCScrollViewDirectionVertical);
	tableView->setPosition(ccp(winSize.width/2-300,winSize.height/2-190));
	tableView->setDelegate(this);
	tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
	this->addChild(tableView);
	tableView->reloadData();
}

SEL_MenuHandler AchieveCocosBuilder::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "button_back", AchieveCocosBuilder::onBackClicked);

	return NULL;
}

SEL_CCControlHandler AchieveCocosBuilder::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName){
	return NULL;
}

bool AchieveCocosBuilder::onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode){
	CCLog("onAssignCCBMemberVariable:%s",pMemberVariableName);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "gold_achievement", CCSprite*, this->gold_achievement);
	return false;
}
bool AchieveCocosBuilder::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue){
	   bool bRet = false;

	   return bRet;
}
void  AchieveCocosBuilder::keyBackClicked(){
	onBackClicked(this);
}
void AchieveCocosBuilder::onBackClicked(CCObject * pSender){

	 CCDirector::sharedDirector()->popScene();
}


void AchieveCocosBuilder::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    CCLOG("cell touched at index: %i", cell->getIdx());
	int idx = cell->getIdx();
	//((CCNode*)(cell->getChildByTag(0)))->getUserData();
	
	if(cell->getChildByTag(0)){
		CCLOG("99999999999999999900000000000000000");
	}
	
	/*UserRecord *achieveUR = new UserRecord("tank.db","achieveRecord");//保存领取
	char taskname[30];
	sprintf(taskname,"isRecordGold%d", idx);
	int _reslut = achieveUR->readIntFromSQLite(taskname);
	achieveUR->closeDB();
	free(achieveUR);*/
	CCLOG("isRecordGold[%d]:%d",idx,isRecordGold[idx]);
	if( isRecordGold[idx]!=0){
		return;
	}

	CCBReader     *reader =(CCBReader*)((CCNode*)cell->getChildByTag(0))->getUserData();	
    if(achieveRecord[idx]>=task[idx]){
	   if(isRecordGold[idx]==0){
		   //reader->getAnimationManager()->runAnimationsForSequenceNamed("2nd_click");//
		  reader->getAnimationManager()->runAnimationsForSequenceNamed("3rd");
		  cell->getChildByTag(0)->removeChildByTag(2);
		  /*CCSprite *sprite = CCSprite::create("achieve/gold_achievement.png");
          sprite->setAnchorPoint(CCPointZero);
		  CCLog("cell->getPositionY():%f",cell->getPositionY());
		  sprite->setPosition(ccp(cell->getPositionX()+cell->getChildByTag(0)->getPositionX()+cell->getChildByTag(0)->getChildByTag(1)->getPositionX()+100, 
			                      cell->getChildByTag(0)->getPositionY()+cell->getChildByTag(0)->getChildByTag(1)->getPositionY()));
          this->addChild(sprite,0,1000);*/

		  cell->getChildByTag(0)->removeChildByTag(1);

		/* CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		 ccBezierConfig bezier;
	     bezier.controlPoint_1 = ccp(sprite->getPositionX(), sprite->getPositionY());  
		 bezier.controlPoint_2 = ccp(winSize.width-50, 50);
	     bezier.endPosition = ccp(winSize.width-20,50);  
		 CCActionInterval* move = CCBezierTo::create(1.01,bezier);
		 sprite->runAction(move);*/


		  UserRecord *gradeUR = new UserRecord("tank.db","tank");
		  int gold = gradeUR->readIntFromSQLite("gold");
		  gold = task_gold[idx]+gold;
		  gradeUR->saveInt("gold",gold);
		  gradeUR->closeDB();
		  free(gradeUR);

		  char lable[30];
		  sprintf(lable,"%d",gold);
		  ((CCLabelAtlas*)gold_achievement->getChildByTag(1))->setString(lable);

		  isRecordGold[idx] = 1;

		  UserRecord *achieveUR = new UserRecord("tank.db","achieveRecord");//保存领取
		  char taskname[30];
		  sprintf(taskname,"isRecordGold%d", idx);
		  achieveUR->saveInt(taskname,1);
		  achieveUR->closeDB();
		  free(achieveUR);
	   }
	}else{
		reader->getAnimationManager()->runAnimationsForSequenceNamed("1st_click");//1st 3rd  2nd
	}
}
CCSize AchieveCocosBuilder::cellSizeForTable(CCTableView *table){
	return CCSizeMake(300, 90);
}
CCTableViewCell* AchieveCocosBuilder::tableCellAtIndex(CCTableView *table, unsigned int idx){
	   CCTableViewCell *cell = table->dequeueCell();
	   CCLog("tableCellAtIndex===========idx:%d ",idx) ;
	   // CCString *string = CCString::createWithFormat("%d", idx);

		if (!cell) {
		   cell = new CustomTableViewCell();
		   cell->autorelease();

		   CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	       cocos2d::extension::CCBReader  *ccbreader = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
		   //CCLog("CCTableViewCell============================idx:%d",idx) ;
		   	char pszFileName[50];
	        sprintf(pszFileName,"achieve/scene_achievement_box%d.ccbi", idx+1);
	       //读取一个ccbi的文件,生成一个ccnode实例
	       CCNode  *animationsTest  = ccbreader->readNodeGraphFromFile(pszFileName,this);//tank11.ccbi
		   animationsTest->setUserData(ccbreader);	   
		  // ccbreader->autorelease();
	   
		    CCSprite *sprite = CCSprite::create("achieve/gold_achievement.png");
            sprite->setAnchorPoint(CCPointZero);
            sprite->setPosition(ccp(200, 0));
            animationsTest->addChild(sprite,0,1);

		   char lable[30];
		   CCLog("task_gold[idx]:%d      %d",task_gold[idx],idx);
           sprintf(lable,"%d",task_gold[idx]);
           CCLabelAtlas* goldNum=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
	       goldNum->setAnchorPoint(CCPointZero);
	       goldNum->setPosition(200,0);
	       animationsTest->addChild(goldNum,0,2);


			CCLog("task[idx]:%d      %d",task[idx],idx);
           sprintf(lable,"%d",task[idx]);
           CCLabelAtlas* taskNum=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
	       taskNum->setAnchorPoint(CCPointZero);
	       taskNum->setPosition(135,-10);
	       animationsTest->addChild(taskNum,0,4);

		   CCLog("achieveRecord[idx]:%d      %d",achieveRecord[idx],idx);
           sprintf(lable,"%d",achieveRecord[idx]);
           CCLabelAtlas* achieveRecordNum=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
	       achieveRecordNum->setAnchorPoint(ccp(1,0));
	       achieveRecordNum->setPosition(120,-10);
	       animationsTest->addChild(achieveRecordNum,0,3);


		    animationsTest->setAnchorPoint(ccp(0.5,0.5));
            animationsTest->setPosition(ccp(300, 50));
	        if(animationsTest  != NULL) {     
		      cell->addChild(animationsTest,0,0);
	        }
		   if(achieveRecord[idx]>=task[idx]){
			   if(isRecordGold[idx]==0){
				   ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("2nd");
			   }else{//接受金币
				   ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("3rd");
				   animationsTest->removeChildByTag(1);
				   animationsTest->removeChildByTag(2);
			   }
		   }else{
		       ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("1st");//1st 3rd  2nd
		   }
		   

           /*CCLabelTTF *label = CCLabelTTF::create(string->getCString(), "Helvetica", 20.0);
           label->setPosition(CCPointZero);
		   label->setAnchorPoint(CCPointZero);
           label->setTag(123);
           cell->addChild(label);*/
	   }else
       {
       // CCLabelTTF *label = (CCLabelTTF*)cell->getChildByTag(123);
       // label->setString(string->getCString());

		   cell = new CustomTableViewCell();
		   cell->autorelease();
		   cell->removeChildByTag(0);


		  CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	      cocos2d::extension::CCBReader  *ccbreader = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
		  //CCLog("CCTableViewCell-------------------------idx:%d",idx) ;
		  char pszFileName[50];
	      sprintf(pszFileName,"achieve/scene_achievement_box%d.ccbi", idx+1);
	       //读取一个ccbi的文件,生成一个ccnode实例
	       CCNode  *animationsTest  = ccbreader->readNodeGraphFromFile(pszFileName,this);//tank11.ccbi
		   animationsTest->setUserData(ccbreader);

		    CCSprite *sprite = CCSprite::create("achieve/gold_achievement.png");
            sprite->setAnchorPoint(CCPointZero);
            sprite->setPosition(ccp(200, 0));
            animationsTest->addChild(sprite,0,1);

			char lable[30];
			CCLog("task_gold[idx]:%d      %d",task_gold[idx],idx);
           sprintf(lable,"%d",task_gold[idx]);
           CCLabelAtlas* goldNum=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
	       goldNum->setAnchorPoint(CCPointZero);
	       goldNum->setPosition(200,0);
	       animationsTest->addChild(goldNum,0,2);


			CCLog("task[idx]:%d      %d",task[idx],idx);
           sprintf(lable,"%d",task[idx]);
           CCLabelAtlas* taskNum=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
	       taskNum->setAnchorPoint(CCPointZero);
	       taskNum->setPosition(135,-10);
	       animationsTest->addChild(taskNum,0,4);

		   CCLog("achieveRecord[idx]:%d      %d",achieveRecord[idx],idx);
           sprintf(lable,"%d",achieveRecord[idx]);
           CCLabelAtlas* achieveRecordNum=CCLabelAtlas::create(lable, "shop/num_money.png", 12, 13, '0');
	       achieveRecordNum->setAnchorPoint(ccp(1,0));
	       achieveRecordNum->setPosition(120,-10);
	       animationsTest->addChild(achieveRecordNum,0,3);
		  // ccbreader->autorelease();
	      //设置坐标 
		    animationsTest->setAnchorPoint(ccp(0.5,0.5));
            animationsTest->setPosition(ccp(300, 50));
	       if(animationsTest  != NULL) {     
		      cell->addChild(animationsTest,0,0);
	       }
		   //执行时间线
		   if(achieveRecord[idx]>=task[idx]){
			   if(isRecordGold[idx]==0){
				   ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("2nd");
			   }else{//接受金币
				   ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("3rd");
				   animationsTest->removeChildByTag(1);
				   animationsTest->removeChildByTag(2);
			   }
		   }else{
		       ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("1st");//1st 3rd  2nd
		   }
      }
	   return cell;
}
unsigned int AchieveCocosBuilder::numberOfCellsInTableView(CCTableView *table)
{
    return 10;
}