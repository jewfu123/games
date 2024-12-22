#include "MenuLayer.h"
#include "MenuCocosBuilder/MenuCocosBuilderLayerLoader.h"
#include "MenuCocosBuilder/MapSelectCocosBuilderLoader.h"

using namespace CocosDenshion;
using namespace cocos2d::extension; 
USING_NS_CC;
/*MenuLayer::MenuLayer(void)
{
}


MenuLayer::~MenuLayer(void)
{
}*/

CCScene* MenuLayer::scene()
{
    CCScene * scene = NULL;
    do 
    {

		SimpleAudioEngine::sharedEngine()->preloadEffect("sound/31.ogg");
        // 'scene' is an autorelease object
		scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
		MenuLayer *layer = MenuLayer::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);

		
    } while (0);

    // return the scene
    return scene;
}

bool MenuLayer::init()
{
    bool bRet = false;
    do {      
		CCSize size = CCDirector::sharedDirector()->getWinSize();

		CCNodeLoaderLibrary* lib = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	    lib->registerCCNodeLoader("MenuCocosBuilderLayer", MenuCocosBuilderLayerLoader::loader());//MenuCocosBuilderLayer
        reader = new cocos2d::extension::CCBReader(lib);
        CCNode * node = (CCNode*)reader->readNodeGraphFromFile("menu/MainMenuScene.ccbi", this);// MainMenu_pressmore.ccbi MainMenuScene.ccbi
		//reader->autorelease();
        if (NULL!=node) {
           this->addChild(node,1,321);
        }
	   reader->getAnimationManager()->runAnimationsForSequenceNamed("1st");
	   

	   char lable[30];
	   UserRecord *gradeUR = new UserRecord("tank.db","tank");

	   passCheckNum =  gradeUR->readIntFromSQLite("passCheckNum");//通过关卡数
	   for(int i=0;i<43;i++){
			sprintf(lable,"passCheck%d",i);
			passCheck[i]= gradeUR->readIntFromSQLite(lable);//关卡是否开启

			sprintf(lable,"passCheckIndex%d",i);
			passCheckIndex[i]= gradeUR->readIntFromSQLite(lable);////每一关卡等级
		 }
		 CCLog("passCheckNum:%d",passCheckNum);
		 gradeUR->closeDB();
		 free(gradeUR);

		actIndex = 0;
	    move = false;
	    this->setTouchEnabled(true); 
		this->setKeypadEnabled(true);      

		selectMapBool = false;

	    teach = false;

		
		if(LoadBooleanFromXML("survival")){
			survivalModeTime.setMillis(0);
		}else{
			CCLog("survival_time:%d",survival_time);//37078546
			CCLog("millisecondNow:%ld",millisecondNow());//35643275
		    CCLog("millisecondNow:%d",(survival_time-(millisecondNow()))/1000);

			int second = (survival_time-(millisecondNow()))/1000;

			CCLog("second:%d",second);//
			if(second>0){
				survivalModeTime.setMillis(second*1000);
				survivalModeTime.start();
			}else{
				survivalModeTime.setMillis(0);
				SaveBooleanToXML("survival",true);
			}
		}

		/*UserRecord *tankUR = new UserRecord("tank.db","tank");
		int  tankCont = tankUR->readIntFromSQLite("tankCont");
		

		if(tankCont<5){
			int second = (millisecondNow()+regainLife_time-leaveGame_time)/1000;
			int num = second/(15*60);
			tankCont = (tankCont+num)>5?5:(tankCont+num);

			if(tankCont>=5){
				regainLifeTime.setMillis(0);
				tankUR->saveInt("tankCont",5);
			}else{
				CCLog("second:%d",second);
				CCLog("second(15*60*1000):%d",(second%(15*60)));//18640
				regainLifeTime.setMillis((second%(15*60))*1000);
				regainLifeTime.start();
			}
		}else{
			regainLifeTime.setMillis(0);
		}

		tankUR->closeDB();
		free(tankUR);*/

		this->schedule(schedule_selector(MenuLayer::showTime),1.0f);// void showTime(float  t);
		

		gameScene = true;
		
		sound = LoadBooleanFromXML("sound");

		SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("sound/2.ogg");
		SimpleAudioEngine::sharedEngine()->preloadEffect("sound/10.ogg");
		SimpleAudioEngine::sharedEngine()->preloadEffect("sound/20.ogg");
		SimpleAudioEngine::sharedEngine()->preloadEffect("sound/24.ogg");
		SimpleAudioEngine::sharedEngine()->preloadEffect("sound/25.ogg");
		SimpleAudioEngine::sharedEngine()->preloadEffect("sound/26.ogg");
		
		SimpleAudioEngine::sharedEngine()->preloadEffect("sound/22.ogg");
		if(sound){
			SimpleAudioEngine::sharedEngine()->playEffect("sound/31.ogg",false);

			if(!SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()){
				SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sound/2.ogg",true);
			}
		}

	    bRet = true;
    } while (0);
    return bRet;
}
void MenuLayer::registerWithTouchDispatcher(void)
{
     CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);//0, 
} 

void MenuLayer::playBottnEffect(){
	if(sound){
		SimpleAudioEngine::sharedEngine()->playEffect("sound/10.ogg",false);
	}
}

 void MenuLayer::playEffect(int index){
  if(sound){
	  char lable[30];
	  sprintf(lable,"sound/%d.ogg",index);
	  SimpleAudioEngine::sharedEngine()->playEffect(lable,false);
   }
 }
CCNode *  MenuLayer::openCBBI(const char * pCCBFileName,const char *actionName) 
{ 
    //创建一个自动释放的CCNode加载库 
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary(); 
    ccNodeLoaderLibrary->registerCCNodeLoader("MapSelectCocosBuilderLayer", MapSelectCocosBuilderLoader::loader());//
        
    //创建一个自动释放的CCB读取类（CCBReader） 
    cocos2d::extension::CCBReader *ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary); 
    ccbReader->autorelease(); 
        
    //读取ccbi文件获取根节点 
    CCNode * node = ccbReader->readNodeGraphFromFile(pCCBFileName, this); 
    
	CCArray* allSeq = ccbReader->getAnimationManager()->getSequences(); 
	  CCLog("MenuCocosBuilderLayer============================") ;
	  for (int i=0; i<allSeq->count(); i++)
	  {     
		 //获取到每一个Seq 
		 CCBSequence* everySeq = (CCBSequence*)allSeq->objectAtIndex(i);
		    //获取每个序列动作周期、名字以及id 
		 // everySeq->getDuration(); 
		  CCLog("MenuCocosBuilderLayer===========%s",everySeq->getName()) ; 
		  CCLog("MenuCocosBuilderLayer===========%d",everySeq->getSequenceId()) ;
		  
	  } 

	//ccbReader->getAnimationManager()->runAnimationsForSequenceNamed(actionName);

    return node; 
}

void   MenuLayer::firstSelectMap(int index){
	for(int i=0;i<10;i++){
		if(this->getChildByTag(200+i)){
			this->removeChildByTag(200+i);
		}
	}

	CCLog("1passCheckNum:%d",passCheckNum);
	int startMap = 0;
	int maplong = 10;

    actIndex = index;

	if(index==0){
		if(passCheckNum>=10){
			selsecMap = 0;
		}else{
			selsecMap = passCheckNum;
			CCLog("1selsecMap:%d",selsecMap);
		}
		startMap = 0;
		maplong = 10;
	}else if(index==1){
		if(passCheckNum>=20){
			selsecMap = 0;
		}else if(passCheckNum>10&&passCheckNum<20){
			selsecMap = passCheckNum-10;
		}else{
			selsecMap = 1;
		}
		startMap = 10;
		maplong = 10;
	}else if(index==2){
		if(passCheckNum>=30){
			selsecMap = 0;
		}else if(passCheckNum>20&&passCheckNum<30){
			selsecMap = passCheckNum-20;
		}else{
			selsecMap = 1;
		}
		startMap = 20;
		maplong = 10;
	}else if(index==3){
		if(passCheckNum>=40){
			selsecMap = 0;
		}else if(passCheckNum>30&&passCheckNum<40){
			selsecMap = passCheckNum-30;
		}else{
			selsecMap = 1;
		}
		startMap = 30;
		maplong = 10;
	}else if(index==4){
		if(passCheckNum>=42){
			selsecMap = 0;
		}else if(passCheckNum>40&&passCheckNum<43){
			selsecMap = passCheckNum-40;
		}else{
			selsecMap = 1;
		}
		startMap = 40;
		maplong = 3;
	}

	CCLog("selsecMap:%d",selsecMap);
	CCLog("maplong:%d",maplong);
	for(int i=0;i<maplong;i++){
		char lable[50];
		if(i>selsecMap){
			if(passCheck[i+startMap]==1){
		     // sprintf(lable,"menu/UI_misson_entrance%d.ccbi",i+startMap);
				sprintf(lable,"menu/UI_misson_entrance.ccbi");
			}else{
			  if(i+startMap>=40){
				 sprintf(lable,"menu/U_%d.ccbi",i+startMap); 
			  }else{
				 sprintf(lable,"menu/UI_misson_close.ccbi",i);
			  }
			  
			}
			CCLog("lable:%s",lable);
		   CCNode * node1 =openCBBI(lable,"first");
	       node1->setPosition(ccp((i-1)!=selsecMap?960:660,214));//
	       node1->setScale(0.8f);
	       this->addChild(node1,1,200+i);
           
		   sprintf(lable,"%d%d",i/10,i%10);
		   CCLabelAtlas* num1=CCLabelAtlas::create(lable, "menu/num_yello.png", 18, 21, '0');
   	       num1->setOpacity(0);
	       num1->setAnchorPoint(ccp(0.5,0.5));
	       num1->setPosition(0,110);
	       num1->runAction(CCSequence::create(CCDelayTime::create(0.2f),CCFadeTo::create(0.1f,255),NULL));
	       node1->addChild(num1, 1,1);
		   if(passCheck[i+startMap]==1){
			  for(int n=0;n<passCheckIndex[i+startMap];n++){//passCheckIndex[i+startMap]
		        CCSprite *star=CCSprite::create("menu/star_yellow.png");
		        star->setOpacity(0);
		        star->setAnchorPoint(ccp(0.5,0.5));
				star->setPosition(ccp(-32+n*28,-103));
			    star->runAction(CCSequence::create(CCDelayTime::create(0.2f),CCFadeTo::create(0.1f,255),NULL));
			    node1->addChild(star,1); 
			 }
			//生存之旅
			if(index==4){
				if(!LoadBooleanFromXML("survival")){
				  CCSprite *time = CCSprite::create("menu/shijian.png");
				  time->setPosition(ccp(0,30));
				  node1->addChild(time,1,2);

				  int second = (int)(survivalModeTime.getSurplusMillis()/1000);
				  char tm[50];
				  sprintf(tm, "%d%d:%d%d",(second/60)/10,(second/60)%10,(second%60)/10,(second%60)%10); 

				  CCLabelAtlas* gameTimeLable=CCLabelAtlas::create(tm, "menu/shuzi.png",  18, 21, '0');
				  gameTimeLable->setAnchorPoint(ccp(0,0));
				  gameTimeLable->setPosition(ccp(-45,-20));
				  node1->addChild(gameTimeLable,1,3);
				}
			  }
		   }
		}else if(i<selsecMap){
			if(passCheck[i+startMap]==1){
				//sprintf(lable,"menu/UI_misson_entrance%d.ccbi",i+startMap);
				sprintf(lable,"menu/UI_misson_entrance.ccbi");
			}else{
				if(i+startMap>=40){
					sprintf(lable,"menu/U_%d.ccbi",i+startMap); 
				}else{
					sprintf(lable,"menu/UI_misson_close.ccbi",i);
				}
			}
			CCLog("lable:%s",lable);
		   CCNode * node1 =openCBBI(lable,"first");
	       node1->setPosition(ccp((i+1)!=selsecMap?-160:140,214));//
	       node1->setScale(0.8f);
	       this->addChild(node1,1,200+i);

		   sprintf(lable,"%d%d",i/10,i%10);
		   CCLabelAtlas* num1=CCLabelAtlas::create(lable, "menu/num_yello.png", 18, 21, '0');
   	       num1->setOpacity(0);
	       num1->setAnchorPoint(ccp(0.5,0.5));
	       num1->setPosition(0,110);
	       num1->runAction(CCSequence::create(CCDelayTime::create(0.2f),CCFadeTo::create(0.1f,255),NULL));
	       node1->addChild(num1, 1,1);

		   if(passCheck[i+startMap]==1){
			   for(int n=0;n<passCheckIndex[i+startMap];n++){//passCheckIndex[i+startMap]
				   CCSprite *star=CCSprite::create("menu/star_yellow.png");
				   star->setOpacity(0);
				   star->setAnchorPoint(ccp(0.5,0.5));
				   star->setPosition(ccp(-32+n*28,-103));
				   star->runAction(CCSequence::create(CCDelayTime::create(0.2f),CCFadeTo::create(0.1f,255),NULL));
				   node1->addChild(star,1); 
			   }
			   if(index==4){
				   if(!LoadBooleanFromXML("survival")){
					   CCSprite *time = CCSprite::create("menu/shijian.png");
					   time->setPosition(ccp(0,30));
					   node1->addChild(time,1,2);

					   int second = (int)(survivalModeTime.getSurplusMillis()/1000);
					   char tm[50];
					   sprintf(tm, "%d%d:%d%d",(second/60)/10,(second/60)%10,(second%60)/10,(second%60)%10); 

					   CCLabelAtlas* gameTimeLable=CCLabelAtlas::create(tm, "menu/shuzi.png",  18, 21, '0');
					   gameTimeLable->setAnchorPoint(ccp(0,0));
					   gameTimeLable->setPosition(ccp(-45,-20));
					   node1->addChild(gameTimeLable,1,3);
				   }
			   }
		   }
		}else{
			if(passCheck[i+startMap]==1){
				//sprintf(lable,"menu/UI_misson_entrance%d.ccbi",i+startMap);
				sprintf(lable,"menu/UI_misson_entrance.ccbi");
			}else{
				if(i+startMap>=40){
					sprintf(lable,"menu/U_%d.ccbi",i+startMap); 
				}else{
					sprintf(lable,"menu/UI_misson_close.ccbi",i);
				}
			}
			CCLog("lable:%s",lable);
		   CCNode * node1 =openCBBI(lable,"first");
	       node1->setPosition(ccp(400,214));//
	       this->addChild(node1,1,200+i);

		   sprintf(lable,"%d%d",i/10,i%10);
		   CCLabelAtlas* num1=CCLabelAtlas::create(lable, "menu/num_yello.png", 18, 21, '0');
   	       num1->setOpacity(0);
	       num1->setAnchorPoint(ccp(0.5,0.5));
	       num1->setPosition(0,110);
	       num1->runAction(CCSequence::create(CCDelayTime::create(0.2f),CCFadeTo::create(0.1f,255),NULL));
	       node1->addChild(num1, 1,1);

		   if(passCheck[i+startMap]==1){
			   for(int n=0;n<passCheckIndex[i+startMap];n++){//
				   CCSprite *star=CCSprite::create("menu/star_yellow.png");
				   star->setOpacity(0);
				   star->setAnchorPoint(ccp(0.5,0.5));
				   star->setPosition(ccp(-32+n*28,-103));
				   star->runAction(CCSequence::create(CCDelayTime::create(0.2f),CCFadeTo::create(0.1f,255),NULL));
				   node1->addChild(star,1); 
			   }

			   if(index==4){
				   if(!LoadBooleanFromXML("survival")){
					   CCSprite *time = CCSprite::create("menu/shijian.png");
					   time->setPosition(ccp(0,30));
					   node1->addChild(time,1,2);

					   int second = (int)(survivalModeTime.getSurplusMillis()/1000);
					   char tm[50];
					   sprintf(tm, "%d%d:%d%d",(second/60)/10,(second/60)%10,(second%60)/10,(second%60)%10); 

					   CCLabelAtlas* gameTimeLable=CCLabelAtlas::create(tm, "menu/shuzi.png",  18, 21, '0');
					   gameTimeLable->setAnchorPoint(ccp(0,0));
					   gameTimeLable->setPosition(ccp(-45,-20));
					   node1->addChild(gameTimeLable,1,3);
				   }
			   }
		   }
		}
	}

	if (this->getChildByTag(900))
	{
		this->removeChildByTag(900);
	}

	if(passCheck[actIndex*10+selsecMap]==1||(actIndex*10+selsecMap)==0||(actIndex*10+selsecMap)==10||(actIndex*10+selsecMap)==20||(actIndex*10+selsecMap)==30
		||(actIndex*10+selsecMap)==40||(actIndex*10+selsecMap)==41||(actIndex*10+selsecMap)==42){//任务
		showMission(actIndex*10+selsecMap);
	}
}

void MenuLayer::showTime(float  t){
	if(actIndex==4){
	  if(!LoadBooleanFromXML("survival")){
		 int second = (int)(survivalModeTime.getSurplusMillis()/1000);
		//CCLog("===========second:%d",second);
		 char tm[50];
		 sprintf(tm, "%d%d:%d%d",(second/60)/10,(second/60)%10,(second%60)/10,(second%60)%10); 
		 CCLabelAtlas* gameTimeLable2 = (CCLabelAtlas*)((this->getChildByTag(202))->getChildByTag(3));
		 if(gameTimeLable2){
			  gameTimeLable2->setString(tm);
		 }
		 CCLabelAtlas* gameTimeLable1 = (CCLabelAtlas*)((this->getChildByTag(201))->getChildByTag(3));

		 if(gameTimeLable1){
			 gameTimeLable1->setString(tm);
		 }
		 CCLabelAtlas* gameTimeLable0 = (CCLabelAtlas*)((this->getChildByTag(200))->getChildByTag(3));

		 if(gameTimeLable0){
			 gameTimeLable0->setString(tm);
		 }

		 if(second==0){
			 SaveBooleanToXML("survival",true);
			((this->getChildByTag(200))->getChildByTag(3))->setVisible(false);
			((this->getChildByTag(200))->getChildByTag(2))->setVisible(false);
			((this->getChildByTag(201))->getChildByTag(3))->setVisible(false);
			((this->getChildByTag(201))->getChildByTag(2))->setVisible(false);
			((this->getChildByTag(202))->getChildByTag(3))->setVisible(false);
			((this->getChildByTag(202))->getChildByTag(2))->setVisible(false);
		 }

	  }
	}

	if(this->getChildByTag(321)){
		 char lable[50];
		 CCLabelAtlas* lifeTimeM= (CCLabelAtlas*)((((MenuCocosBuilderLayer*)this->getChildByTag(321))->life_box)->getChildByTag(2));
		 sprintf(lable,"%d%d",(regainLifeTime.getSurplusMillis()/(1000*60))/10,(regainLifeTime.getSurplusMillis()/(1000*60))%10);
		 lifeTimeM->setString(lable);

		 CCLabelAtlas* lifeTimeS= (CCLabelAtlas*)((((MenuCocosBuilderLayer*)this->getChildByTag(321))->life_box)->getChildByTag(3));
		 sprintf(lable,"%d%d",((regainLifeTime.getSurplusMillis()/1000)%60)/10,((regainLifeTime.getSurplusMillis()/1000)%60)%10);
		 lifeTimeS->setString(lable);

		 if(regainLifeTime.getSurplusMillis()<=0){
			 UserRecord *gradeUR = new UserRecord("tank.db","tank");
			 int  tankCont = gradeUR->readIntFromSQLite("tankCont");
			 CCLog("tankCont:%d",tankCont);
			 tankCont++;
			 if(tankCont<5){
				// regainLifeTime.setMillis(15*60*1000);
				 regainLifeTime.setSurplusMillis(15*60*1000);
				 regainLifeTime.start();
			 }
			 if(tankCont<=5){
				 CCLabelAtlas* lifeTimeN= (CCLabelAtlas*)((((MenuCocosBuilderLayer*)this->getChildByTag(321))->life_box)->getChildByTag(1));
				 sprintf(lable,"%d",tankCont);
				 lifeTimeN->setString(lable);

				 gradeUR->saveInt("tankCont",tankCont);
			 }
			 
			 gradeUR->closeDB();
			 free(gradeUR);
		 }
	}
}

void  MenuLayer::backMenu(){
	for(int i=0;i<10;i++){
        this->removeChildByTag(200+i);
	}
}

bool MenuLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	return true;
}

void MenuLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
	if(!selectMapBool){
		 return ;
	}
	CCPoint startLocation = pTouch->getLocationInView();
	CCPoint endLocation = pTouch->getPreviousLocation();

	startLocation =  CCDirector::sharedDirector()->convertToGL(startLocation);
	endLocation =  CCDirector::sharedDirector()->convertToGL(endLocation);

	float distance = ccpDistance(startLocation, endLocation);
	CCSize size = CCDirector::sharedDirector()->getWinSize();

	if(abs(endLocation.x-startLocation.x)<10.0){
		return ;
	}

   if(!move){
	if(startLocation.x<endLocation.x){//向左滑动
		int maplong = 10;
		if(actIndex<4){
			 maplong = 10;
		}else{
			maplong = 3;
		}

	  if(selsecMap<maplong-1){
		move = true;
		//CCLog("=========================selsecMap:%d",selsecMap);

		if(selsecMap>0){
		  CCActionInterval* move = CCMoveTo::create(0.5, ccp(-160,214));
		  CCActionInterval* move_ease_in = CCEaseElasticOut::create (move, 1.5);
		  this->getChildByTag(200+selsecMap-1)->runAction(move_ease_in);
		}

		CCActionInterval* move1 = CCMoveTo::create(0.5, ccp(140,214));
		CCActionInterval* move_ease_in1 = CCEaseElasticOut::create (move1, 1.5f);

		this->getChildByTag(200+selsecMap)->runAction(CCSequence::create(//move_ease_in1,
			                                                   CCSpawn::create(move_ease_in1,CCScaleTo::create(0.4f,0.8f),NULL),//CCSpawn(move_ease_in1,NULL)
			                                                  CCCallFunc::create(this,callfunc_selector(MenuLayer::callBackEndAction)),
															   NULL));

		CCActionInterval* move2 = CCMoveTo::create(0.5f, ccp(400,214));
		CCActionInterval* move_ease_in2 = CCEaseElasticOut::create (move2, 1.5);
		this->getChildByTag(200+1+selsecMap)->runAction(CCSequence::create(CCSpawn::create(move_ease_in2,CCScaleTo::create(0.4f,1.0f),NULL)//CCSpawn(move_ease_in1,NULL)			                                                 
			                                                  ,NULL));

		if(selsecMap+2<maplong){
		  CCActionInterval* move3 = CCMoveTo::create(0.5f, ccp(660,214));
		  CCActionInterval* move_ease_in3 = CCEaseElasticOut::create (move3, 1.5f);
		  this->getChildByTag(200+2+selsecMap)->runAction(move_ease_in3);
		}

		
		   selsecMap++;
	    }
	}else{//向右
		//CCLog("-----------------------selsecMap:%d",selsecMap);
		if(selsecMap!=0){
			move = true;
		}

		if(selsecMap-2>=0){
		  CCActionInterval* move = CCMoveTo::create(0.5, ccp(140,214));
		  CCActionInterval* move_ease_in = CCEaseElasticOut::create (move, 1.5);
		  this->getChildByTag(200+selsecMap-2)->runAction(move_ease_in);
		}

		if(selsecMap-1>=0){
			CCActionInterval* move2 = CCMoveTo::create(0.5f, ccp(400,214));
		    CCActionInterval* move_ease_in2 = CCEaseElasticOut::create (move2, 1.5);
			this->getChildByTag(200+selsecMap-1)->runAction(CCSequence::create(CCSpawn::create(move_ease_in2,CCScaleTo::create(0.4f,1.0),NULL)//CCSpawn(move_ease_in1,NULL)			                                                 
			                                                  ,NULL));
		}

		if(selsecMap>0){
		CCActionInterval* move3 = CCMoveTo::create(0.5, ccp(660,214));
		CCActionInterval* move_ease_in3 = CCEaseElasticOut::create (move3, 1.5);
		this->getChildByTag(200+selsecMap)->runAction(CCSequence::create(CCSpawn::create(move_ease_in3,CCScaleTo::create(0.4f,0.8f),NULL)//CCSpawn(move_ease_in1,NULL)	
			                                                 ,CCCallFunc::create(this,callfunc_selector(MenuLayer::callBackEndAction))
			                                                  ,NULL));
		}

		int maplong = 10;
		if(actIndex<4){
			maplong = 10;
		}else{
			maplong = 3;
		}

		if(selsecMap!=0&&(selsecMap+1<maplong)){
		   CCActionInterval* move3 = CCMoveTo::create(0.5, ccp(960,214));
		   CCActionInterval* move_ease_in3 = CCEaseElasticOut::create (move3, 1.5);
		   this->getChildByTag(200+selsecMap+1)->runAction(move_ease_in3);

		}

		if(selsecMap>0){
		   selsecMap--;
	    }

	}

  }
}

void MenuLayer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){

}

void MenuLayer::callBackEndAction(){
	CCLog("-------------------callBackEndAction:%d",actIndex*10+selsecMap);
	move = false;
	if (this->getChildByTag(900))
	{
		this->removeChildByTag(900);
	}
	if(passCheck[actIndex*10+selsecMap]==1||(actIndex*10+selsecMap)==0||(actIndex*10+selsecMap)==10||(actIndex*10+selsecMap)==20||(actIndex*10+selsecMap)==30
		||(actIndex*10+selsecMap)==40||(actIndex*10+selsecMap)==41||(actIndex*10+selsecMap)==42){
		showMission(actIndex*10+selsecMap);
	}

}

 void MenuLayer::showMission(int n){
	 CCSize size = CCDirector::sharedDirector()->getWinSize();

	 //创建一个自动释放的CCNode加载库 
	 CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary(); 

	 //创建一个自动释放的CCB读取类（CCBReader） 
	 cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary); 
	 ccbReader->autorelease(); 
	 //读取ccbi文件获取根节点
	 char lable[30];
	 sprintf(lable,"menu/mission%d_text.ccbi",n);
	 CCNode * node = ccbReader->readNodeGraphFromFile(lable, this); 
	 node->setPosition(ccp(size.width/2-50,0));
	 if (NULL!=node) {
		 this->addChild(node,1,900);
	 }

 }
void MenuLayer::moveMenu(bool isLeft){
  if(!move){
	if(isLeft){//向左滑动
		int maplong = 10;
		if(actIndex<3){
			maplong = 10;
		}else{
			maplong = 3;
		}

		if(selsecMap<maplong-1){
			move = true;
			//CCLog("=========================selsecMap:%d",selsecMap);

			if(selsecMap>0){
				CCActionInterval* move = CCMoveTo::create(0.5, ccp(-160,214));
				CCActionInterval* move_ease_in = CCEaseElasticOut::create (move, 1.5);
				this->getChildByTag(200+selsecMap-1)->runAction(move_ease_in);
			}

			CCActionInterval* move1 = CCMoveTo::create(0.5, ccp(140,214));
			CCActionInterval* move_ease_in1 = CCEaseElasticOut::create (move1, 1.5f);

			this->getChildByTag(200+selsecMap)->runAction(CCSequence::create(//move_ease_in1,
				CCSpawn::create(move_ease_in1,CCScaleTo::create(0.4f,0.8f),NULL),//CCSpawn(move_ease_in1,NULL)
				CCCallFunc::create(this,callfunc_selector(MenuLayer::callBackEndAction)),
				NULL));

			CCActionInterval* move2 = CCMoveTo::create(0.5f, ccp(400,214));
			CCActionInterval* move_ease_in2 = CCEaseElasticOut::create (move2, 1.5);
			this->getChildByTag(200+1+selsecMap)->runAction(CCSequence::create(CCSpawn::create(move_ease_in2,CCScaleTo::create(0.4f,1.0f),NULL)//CCSpawn(move_ease_in1,NULL)			                                                 
				,NULL));

			if(selsecMap+2<maplong){
				CCActionInterval* move3 = CCMoveTo::create(0.5f, ccp(660,214));
				CCActionInterval* move_ease_in3 = CCEaseElasticOut::create (move3, 1.5f);
				this->getChildByTag(200+2+selsecMap)->runAction(move_ease_in3);
			}


			selsecMap++;
		}
	}else{
		if(selsecMap!=0){
			move = true;
		}

		if(selsecMap-2>=0){
			CCActionInterval* move = CCMoveTo::create(0.5, ccp(140,214));
			CCActionInterval* move_ease_in = CCEaseElasticOut::create (move, 1.5);
			this->getChildByTag(200+selsecMap-2)->runAction(move_ease_in);
		}

		if(selsecMap-1>=0){
			CCActionInterval* move2 = CCMoveTo::create(0.5f, ccp(400,214));
			CCActionInterval* move_ease_in2 = CCEaseElasticOut::create (move2, 1.5);
			this->getChildByTag(200+selsecMap-1)->runAction(CCSequence::create(CCSpawn::create(move_ease_in2,CCScaleTo::create(0.4f,1.0),NULL)//CCSpawn(move_ease_in1,NULL)			                                                 
				,NULL));
		}

		if(selsecMap>0){
			CCActionInterval* move3 = CCMoveTo::create(0.5, ccp(660,214));
			CCActionInterval* move_ease_in3 = CCEaseElasticOut::create (move3, 1.5);
			this->getChildByTag(200+selsecMap)->runAction(CCSequence::create(CCSpawn::create(move_ease_in3,CCScaleTo::create(0.4f,0.8f),NULL)//CCSpawn(move_ease_in1,NULL)	
				,CCCallFunc::create(this,callfunc_selector(MenuLayer::callBackEndAction))
				,NULL));
		}

		int maplong = 10;
		if(actIndex<3){
			maplong = 10;
		}else{
			maplong = 3;
		}

		if(selsecMap!=0&&(selsecMap+1<maplong)){
			CCActionInterval* move3 = CCMoveTo::create(0.5, ccp(960,214));
			CCActionInterval* move_ease_in3 = CCEaseElasticOut::create (move3, 1.5);
			this->getChildByTag(200+selsecMap+1)->runAction(move_ease_in3);

		}

		if(selsecMap>0){
			selsecMap--;
		}

	}
  }
}


void  MenuLayer::keyBackClicked(){
	if(selectMapBool){
		return ;
	}
	((MenuCocosBuilderLayer*)this->getChildByTag(321))->onQuitClicked(this);
   /* #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	  exit(0);
    #endif
	CCTextureCache::sharedTextureCache()->removeAllTextures();
	CCDirector::sharedDirector()->end();*/
	
}