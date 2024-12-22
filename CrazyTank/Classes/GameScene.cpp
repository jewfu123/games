#include "GameScene.h"
#include "GB2ShapeCache/GB2ShapeCache-x.h"
#include "Tool.h"
#include "cocos-ext.h"
#include "enemyBullet.h"
#include "MenuLayer.h"
#include "landMine.h"
#include "MenuCocosBuilder/MapSelectCocosBuilderLoader.h"
#include "option/optionLoader.h"
#include "Props.h"
#include "SimpleAudioEngine.h"
#include "CCShake/CCShake.h"
#include "teach/shopTeach_1Loader.h"
#include "ShopCocosBuilder/magazine_frameCocosBuilderLayerLoader.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
#include <jni.h>
#include "platform/android/jni/JniHelper.h" 
#include <android/log.h> 
#endif 
//#include "MenuCocosBuilder/MenuCocosBuilderLayerLoader.h"

using namespace CocosDenshion;
USING_NS_CC;
USING_NS_CC_EXT;

//using namespace cocos2d::extension; 
//using namespace cocos2d;

#define PTM_RATIO 32.0f

CCScene* GameScene::scene()
{
	CCScene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(! scene);
		scene->setTag(30000);//��ͣʹ��
		// 'layer' is an autorelease object
		GameScene *layer = GameScene::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer,1,1);

	} while (0);

	// return the scene
	return scene;
}
// on "init" you need to initialize your instance
bool GameScene::init()
{
	bool bRet = false;
	do 
	{
		//////////////////////////////////////////////////////////////////////////
		// super init first
		//////////////////////////////////////////////////////////////////////////

		CC_BREAK_IF(! CCLayer::init());

		//////////////////////////////////////////////////////////////////////////
		// add your codes below...
		//////////////////////////////////////////////////////////////////////////

		// 1. Add a menu item with "X" image, which is clicked to quit the program.

		// Create a "close" menu item with close icon, it's an auto release object.
		CCSize size =CCDirector::sharedDirector()->getWinSize();
		/*CCLabelTTF *m_pLabelLoading = CCLabelTTF::create("loading...", "Arial", 15);
		CCLabelTTF *m_pLabelPercent = CCLabelTTF::create("%0", "Arial", 30);

		m_pLabelLoading->setPosition(ccp(size.width / 2, size.height / 2 - 20));
		m_pLabelPercent->setPosition(ccp(size.width / 2, size.height / 2 + 20));

		this->addChild(m_pLabelLoading,1,0);
		this->addChild(m_pLabelPercent,1,1);*/
		CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		loadingBreader = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
		CCNode  *loading  = loadingBreader->readNodeGraphFromFile("scene_loading.ccbi",this);
		//loadingBreader->autorelease();
		loading->setAnchorPoint(ccp(0.5,0.5));
		loading->setPosition(ccp(0,0));//this->getContentSize().width/2,this->getContentSize().height/2
		this->addChild(loading,1000,1);
		loadingBreader->getAnimationManager()->runAnimationsForSequenceNamed("2nd");//1st,2nd,3rd

		XMLParser *pXmlParser = XMLParser::parseWithFile("strings.xml");
		CCString  *pValue1 = pXmlParser->getString("hello");
		CCString  *pValue2 = pXmlParser->getString("name");


		char enemyN[30];
		sprintf(enemyN,"state%d", mapIndex);
		CCString  *pValue3 = pXmlParser->getString(enemyN);
		emergeNum = pValue3->intValue();//ͬʱ���ֵĵ�������

		CCString  *lValue = pXmlParser->getString("limit");
		limit = lValue->intValue()+emergeNum;
		CCString  *tValue = pXmlParser->getString("time");
		limitTime = tValue->intValue();

		//CCLog("%d",pValue2->intValue());
		CCLog("emergeNum:%d",emergeNum);
		limitCountTime.setMillis(limitTime*1000);

		/*CCString *pValue = CCString::createWithFormat("%s%s%s%s%s", "XML:", pValue1->getCString(), ",", pValue2->getCString(),pValue3->getCString());

		UserRecord *ur = new UserRecord("sql.db","ur");
		// ur->openTableInDB("sql.db","ur");
		ur->saveString("fgq",pValue1->getCString());
		ur->saveFloat("fg",2.5);
		ur->saveInt("sd",10);
		ur->saveBool("sdb",false);
		CCLog("==%f",ur->readFloatFromSQLite("fg"));
		//CCLog("==%d",ur->readIntFromSQLite("sd"));
		//CCLog("==%s",ur->readStringFromSQLite("fgq"));
		const char * string = ur->readStringFromSQLite("fgq");
		//CCLog("==%d",ur->readBoolFromSQLite("sdb"));
		ur->closeDB();
		free(ur);
		*/
		XMLParser * loseBloodNumPosXmlParser = XMLParser::parseWithFile("loseBloodNumPos.xml");
		loseBloodNumPos_1x_start = loseBloodNumPosXmlParser->getString("loseBloodNumPos_1x_start")->intValue();
		loseBloodNumPos_1y_start = loseBloodNumPosXmlParser->getString("loseBloodNumPos_1y_start")->intValue();

		loseBloodNumPos_1x_end = loseBloodNumPosXmlParser->getString("loseBloodNumPos_1x_end")->intValue();
		loseBloodNumPos_1y_end = loseBloodNumPosXmlParser->getString("loseBloodNumPos_1y_end")->intValue();
		loseBloodNumPos_2x_start = loseBloodNumPosXmlParser->getString("loseBloodNumPos_2x_start")->intValue();
		loseBloodNumPos_2y_start = loseBloodNumPosXmlParser->getString("loseBloodNumPos_2y_start")->intValue();
		loseBloodNumPos_2x_end = loseBloodNumPosXmlParser->getString("loseBloodNumPos_2x_end")->intValue();
		loseBloodNumPos_2y_end = loseBloodNumPosXmlParser->getString("loseBloodNumPos_2y_end")->intValue();

		loseBloodNumTime = loseBloodNumPosXmlParser->getString("loseBloodNumTime")->floatValue();

		drawWorldDebug = false;
		m_nNumberOfLoadedSprites = 0;
		loadTextureCache();
		//CC_POINT_POINTS_TO_PIXELS(pSprite->getAnchorPoint());

		playerDeath = false;
		enemyBaseInfo.show = false;
		ourBaseInfo.show = false;

		paused = false;
		isMoveView = false;
		proposShowBool = false;
		isEnemyTankStanding = false;
		propsNum = 1;

		landmineEnemy = 0;
		ourBaseIsHurt = false;
		changeTankBool = false;

		teachBreader = NULL;
		taechBom = false;
		teachBomIndex = 0;

		showDeadTianShenXiafan = false;

		teachProps = false;
		teachPropsIndex = 0;
		showPayPoint = false;
		this->setTouchEnabled(true);
		countEnemy_death = 0;
		SimpleAudioEngine::sharedEngine()->preloadEffect("sound/23.ogg");//
		SimpleAudioEngine::sharedEngine()->preloadEffect("sound/28.ogg");//����battlestart*.ccbi��1st������
		SimpleAudioEngine::sharedEngine()->preloadEffect("sound/29.ogg");//
		SimpleAudioEngine::sharedEngine()->preloadEffect("sound/30.ogg");//
		SimpleAudioEngine::sharedEngine()->preloadEffect("sound/32.ogg");
		SimpleAudioEngine::sharedEngine()->preloadEffect("sound/33.ogg");
		bRet = true;
	} while (0);

	return bRet;
}

void GameScene::registerWithTouchDispatcher(void)
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
} 

const char * loadName[loadNum]={
	"UI/button_menu_0.png",
	"UI/button_menu_1.png",
	"UI/button_nextlv_0.png",
	"UI/landmineNormal.png",
	"UI/bombNormal.png",
	"UI/fireNormal.png",
	"UI/statusBar.png",
	"UI/handlePoint.png",
	"UI/handleBack.png",
	"UI/statusBar.png",
	"UI/stencilL.png",
	"UI/stencilS.png",
	"UI/statusBar.png"
};


void GameScene::loadTextureCache(){
	for(int i=0;i<loadNum;i++){
		if(loadName[i]!=NULL){
			CCTextureCache::sharedTextureCache()->addImageAsync(loadName[i], this, callfuncO_selector(GameScene::loadingCallBack));
		}
	}
}

void GameScene::loadingCallBack(CCObject *obj){
	++m_nNumberOfLoadedSprites;
	// char tmp[10];
	// sprintf(tmp,"%%%d", (int)(((float)m_nNumberOfLoadedSprites / loadNum) * 100));
	//((CCLabelTTF*)this->getChildByTag(1))->setString(tmp);
	// long  start =millisecondNow();
	CCSize size =CCDirector::sharedDirector()->getWinSize();

	if (m_nNumberOfLoadedSprites == loadNum)
	{
		GB2ShapeCache::sharedGB2ShapeCache()->addShapesWithFile("image/player.plist");
		GB2ShapeCache::sharedGB2ShapeCache()->addShapesWithFile("image/enemy.plist");
		GB2ShapeCache::sharedGB2ShapeCache()->addShapesWithFile("image/bossBullet.plist");

		//mapIndex = 9;
		char mapName[30];
		sprintf(mapName,"map/map%d.tmx", mapIndex);//mapIndex
		tileMap = CCTMXTiledMap::create(mapName);

	    (tileMap->layerNamed("floor")->getTexture())->setAliasTexParameters();
		

		/*CCArray *pChildrenArray = tileMap->getChildren();
		CCSpriteBatchNode *child = NULL;
		CCObject *pObject = NULL;
		CCARRAY_FOREACH(pChildrenArray,pObject){
			child = (CCSpriteBatchNode *)pObject;
			if(child){
				child->getTexture()->setAntiAliasTexParameters();//setAntiAliasTexParameters
				(child->getTexture())->set
			}else{
				break;
			}

		}*/

		this->addChild(tileMap,1,0);

		const char * name[43]={
			"map/city.png",
			"map/desert.png",//1
			"map/city.png",//2
			"map/hill.png",
			"map/desert.png",
			"map/city.png",
			"map/hill.png",
			"map/desert.png",
			"map/city.png",
			"map/hill.png",
			"map/desert.png",
			"map/city.png",
			"map/hill.png",
			"map/desert.png",
			"map/city.png",
			"map/hill.png",
			"map/desert.png",
			"map/city.png",
			"map/hill.png",
			"map/desert.png",
			"map/city.png",//20
			"map/hill.png",
			"map/desert.png",
			"map/city.png",
			"map/hill.png",
			"map/desert.png",
			"map/city.png",
			"map/hill.png",
			"map/desert.png",
			"map/city.png",
			"map/hill.png",//30
			"map/desert.png",
			"map/city.png",
			"map/hill.png",
			"map/desert.png",
			"map/city.png",
			"map/hill.png",
			"map/desert.png",
			"map/city.png",
			"map/hill.png",
			"map/desert.png",
			"map/city.png",
			"map/hill.png",
		} ;
		const char * outline[43]={
			"map/outline_city.png",
			"map/ouline_desert.png",//1
			"map/outline_city.png",//2
			"map/outline_grass.png",
			"map/ouline_desert.png",//
			"map/outline_city.png",//
			"map/outline_grass.png",
			"map/ouline_desert.png",//
			"map/outline_city.png",//
			"map/outline_grass.png",
			"map/ouline_desert.png",//
			"map/outline_city.png",//10
			"map/outline_grass.png",
			"map/ouline_desert.png",//
			"map/outline_city.png",//
			"map/outline_grass.png",
			"map/ouline_desert.png",//
			"map/outline_city.png",//
			"map/outline_grass.png",
			"map/ouline_desert.png",//
			"map/outline_city.png",//
			"map/outline_grass.png",//20
			"map/ouline_desert.png",//
			"map/outline_city.png",//
			"map/outline_grass.png",
			"map/ouline_desert.png",//
			"map/outline_city.png",//
			"map/outline_grass.png",
			"map/ouline_desert.png",//
			"map/outline_city.png",//
			"map/outline_grass.png",
			"map/ouline_desert.png",//30
			"map/outline_city.png",//
			"map/outline_grass.png",
			"map/ouline_desert.png",//
			"map/outline_city.png",//
			"map/outline_grass.png",
			"map/ouline_desert.png",//
			"map/outline_city.png",//
			"map/outline_grass.png",
			"map/ouline_desert.png",//
			"map/outline_city.png",//
			"map/outline_grass.png",
		};


		//CCLOG("========================mapIndex:%d",mapIndex);
	 // if(mapIndex!=9&&mapIndex!=19&&mapIndex!=39){
		for(int i=0;i<3;i++){
			CCSprite *leftMap = CCSprite::create(name[mapIndex]);
			leftMap->setAnchorPoint(ccp(0,0));

			leftMap->setPosition(ccp(-leftMap->getContentSize().width/3,leftMap->getContentSize().height*i));
			tileMap->addChild(leftMap,1024);

			CCSprite *rightMap = CCSprite::createWithSpriteFrame(leftMap->displayFrame());
			rightMap->setAnchorPoint(ccp(0,0));
			rightMap->setFlipX(true);
			rightMap->setPosition(ccp(tileMap->getContentSize().width-(leftMap->getContentSize().width*2/3),leftMap->getContentSize().height*i));
			tileMap->addChild(rightMap,1024);

			CCSprite *leftoutline = CCSprite::create(outline[mapIndex]);
			leftoutline->setAnchorPoint(ccp(0,0));

			leftoutline->setPosition(ccp(-leftoutline->getContentSize().width,leftoutline->getContentSize().height*i));
			tileMap->addChild(leftoutline,0);

			CCSprite *rightoutline = CCSprite::createWithSpriteFrame(leftoutline->displayFrame());
			rightoutline->setAnchorPoint(ccp(0,0));
			rightoutline->setFlipX(true);
			rightoutline->setPosition(ccp(tileMap->getContentSize().width,rightoutline->getContentSize().height*i));
			tileMap->addChild(rightoutline,0);
		}
	 // }
		/*CCSprite *leftMap = CCSprite::create(name[mapIndex]);
		leftMap->setAnchorPoint(ccp(0,0));

		leftMap->setPosition(ccp(-leftMap->getContentSize().width/3,0));
		tileMap->addChild(leftMap,1024);

		CCSprite *leftMap1 = CCSprite::createWithSpriteFrame(leftMap->displayFrame());
		leftMap1->setAnchorPoint(ccp(0,0));
		leftMap1->setPosition(ccp(-leftMap1->getContentSize().width/3,leftMap->getContentSize().height));
		tileMap->addChild(leftMap1,1024);


		CCSprite *rightMap = CCSprite::createWithSpriteFrame(leftMap->displayFrame());
		rightMap->setAnchorPoint(ccp(0,0));
		rightMap->setFlipX(true);
		rightMap->setPosition(ccp(tileMap->getContentSize().width-(leftMap1->getContentSize().width*2/3),0));
		tileMap->addChild(rightMap,1024);


		CCSprite *rightMap1 = CCSprite::createWithSpriteFrame(leftMap->displayFrame());
		rightMap1->setAnchorPoint(ccp(0,0));
		rightMap1->setFlipX(true);
		rightMap1->setPosition(ccp(tileMap->getContentSize().width-rightMap1->getContentSize().width*2/3,rightMap1->getContentSize().height));
		tileMap->addChild(rightMap1,1024);*/


		tileMap->setPosition(ccp(0,0));

		enemysCount = 0;
		enemyKingNum_DictElement= CCArray::create();
		enemyKingNum_DictElement->retain();

		enemy_King_Num=CCArray::create();
		enemy_King_Num->retain();

		CCDictionary *mapDic = tileMap->getProperties();

		CCDictElement *keyString = NULL;  
		enemyTankArray = CCArray::create();
		enemyTankArray->retain();

		GB2ShapeCache::sharedGB2ShapeCache()->addShapesWithFile("image/enemyBullet.plist");
		GB2ShapeCache::sharedGB2ShapeCache()->addShapesWithFile("image/landming.plist");//����
		eneymTankBullArray = CCArray::create();
		eneymTankBullArray->retain();


		int index = 0;
		CCDICT_FOREACH(mapDic, keyString)//��ȡ���˵����࣬����  
		{   
			const char *enemyKings =  keyString->getStrKey(); 
			CCLog("dict:key %d", atoi(enemyKings)); 
			CCString  *enemyKingNum =  (CCString *)(keyString->getObject());   
			CCLog("dict:value %d", enemyKingNum->intValue());  
			enemysCount+=enemyKingNum->intValue();

			enemyKingNumDictElement *enemyKingNumDE = enemyKingNumDictElement::enemyKingNumDictElementCreate(atoi(enemyKings), enemyKingNum->intValue(),index);
			enemyKingNum_DictElement->addObject(enemyKingNumDE);

			enemyKingNumDictElement *_enemyKingNum = enemyKingNumDictElement::enemyKingNumDictElementCreate(atoi(enemyKings), enemyKingNum->intValue(),index);
			enemy_King_Num->addObject(_enemyKingNum);

			cocos2d::CCArray  *enemyCArray = CCArray::create();

			for(int i=0;i<(enemyKingNum->intValue()>17?17:enemyKingNum->intValue());i++){//����̹�����5��
				enemyTank* enemytank = enemyTank::spriteCreate(atoi(enemyKings));
				enemytank->setTankerAction();
				enemytank->setBulletArrayIndex(index);
				enemyCArray->addObject(enemytank);
			}

			enemyTankArray->addObject(enemyCArray);

			cocos2d::CCArray  *bulletCArray = CCArray::create();
			for(int i=0;i<20;i++){//ÿ��̹���ӵ���20
				enemyBullet *enemybullt = enemyBullet::spriteCreate((enemyTank*)enemyCArray->objectAtIndex(0));
				enemybullt->setBulletAction();
				enemybullt->setName("enemybullt");
				bulletCArray->addObject(enemybullt);
			}
			eneymTankBullArray->addObject(bulletCArray);

			index++;
		}

		/*enemy_King_Num=CCArray::create();
		enemy_King_Num->retain();
		enemy_King_Num->addObjectsFromArray(enemyKingNum_DictElement);

		CCObject* _object = NULL; 
		CCARRAY_FOREACH(enemy_King_Num, _object) {
		int king = ((enemyKingNumDictElement *)_object)->King;
		int num =  ((enemyKingNumDictElement *)_object)->Num;

		CCLOG("--------------------king:%d",king);
		CCLOG("--------------------num:%d",num);
		//(( enemyTank*)((CCArray*)_object)->objectAtIndex(0))->getKing();
		//CCLog("----------------king:%d",(( enemyTank*)((CCArray*)_object)->objectAtIndex(0))->getKing());
		}*/

		CCTMXObjectGroup *objects = tileMap->objectGroupNamed("Objects");
		//CCAssert(objects != NULL, "Objects' object group not found");

		for(int i=0;i<3;i++){
			char enemyN[30];
			sprintf(enemyN,"enemyPoint%d", i+1);

			CCDictionary *point = objects->objectNamed(enemyN);
			enemyPoint[i].x=  point->valueForKey("x")->floatValue();
			enemyPoint[i].y = point->valueForKey("y")->floatValue();
		}

		createWorld();//������������
		// CCTMXObjectGroup *objects = tileMap->objectGroupNamed("Objects");
		CCDictionary *spawnPoint = objects->objectNamed("plyartPoint");
		//CCAssert(spawnPoint != NULL, "SpawnPoint object not found");
		int x = spawnPoint->valueForKey("x")->intValue();
		int y = spawnPoint->valueForKey("y")->intValue();

		playBody_x = x;
		playBody_y = y;


		/*if(mapIndex>3){
		if(!LoadBooleanFromXML("once")){//
		UserRecord *shop = new UserRecord("tank.db","shop");//�̵���Ʒ�ȼ�
		shop->saveInt("shopbox6",0);//��ʼװ��
		shop->saveInt("shopbox12",0);//��ʼװ��

		shop->saveInt("shopbox1",1);//��ʼװ��
		shop->saveInt("shopbox7",1);//��ʼװ��
		shop->closeDB();
		free(shop);

		UserRecord *gradeUR = new UserRecord("tank.db","tank");
		gradeUR->saveInt("turretId",1);//����
		gradeUR->saveInt("turretGrade",1);//�����ȼ�
		gradeUR->saveInt("chassisId",1);//����
		gradeUR->saveInt("chassisGrade",1);//���̵ȼ�
		gradeUR->closeDB();
		free(gradeUR);

		SaveBooleanToXML("once",true);
		}
		}*/

		//��ȡ���̹�� id
		UserRecord *ur = new UserRecord("tank.db","tank");
		turretId= ur->readIntFromSQLite("turretId");
		chassisId =ur->readIntFromSQLite("chassisId");//����

		turretGrade = ur->readIntFromSQLite("turretGrade");//�����ȼ�
		chassisGrade = ur->readIntFromSQLite("chassisGrade");//���̵ȼ�
		enemyNum = ur->readIntFromSQLite("enemyNum");

		addLifeIndex = ur->readIntFromSQLite("addLifeIndex");//����
		addCritIndex = ur->readIntFromSQLite("addCritIndex");//����
		addDefenseIndex = ur->readIntFromSQLite("addDefenseIndex");//����
		addAttackIndex = ur->readIntFromSQLite("addAttackIndex");//����
		addMoveIndex = ur->readIntFromSQLite("addMoveIndex");//����
		landmineIndex = ur->readIntFromSQLite("landmineIndex");//���׵ȼ�
		ur->closeDB();
		free(ur);


		UserRecord *urTank = new UserRecord("tank.db","shop");//�̵���Ʒ
		doubleAttack = urTank->readIntFromSQLite("doubleAttack");//˫������
		doubleDefense = urTank->readIntFromSQLite("doubleDefense");//˫������
		if(doubleAttack>0){
			urTank->saveInt("doubleAttack",doubleAttack-1);
		}
		if(doubleDefense>0){
			urTank->saveInt("doubleDefense",doubleDefense-1);
		}
		urTank->closeDB();
		free(urTank);


		tanker *tank =tanker::spriteCreate(chassisId,turretId,chassisGrade,turretGrade);
		tank->setTankerAction();
		tank->setPosition(ccp(x, y));
		tileMap->addChild(tank,1,100);
		tank->setName("tank");

		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.fixedRotation = true;//��ֹ������ת


		bodyDef.position.Set(x/PTM_RATIO, y/PTM_RATIO);
		bodyDef.userData = tank;


		playBody = world->CreateBody(&bodyDef);


		//playBody->GetUserData();
		//playBody->SetUserData(tankMax);


		GB2ShapeCache *sc = GB2ShapeCache::sharedGB2ShapeCache();
		sc->addFixturesToBody(playBody, "player"); //player
		tank->setAnchorPoint(sc->anchorPointForShape("player"));//


		//CCLog("-------------:%s",((UserDataConsistencyCCSprite*)playBody->GetUserData())->getName());//

		emergeTime.setMillis(1.5*1000);
		emergeTime.start();

		/*if(mapIndex==9||mapIndex==19||mapIndex==39){
			updateBossMap();
		}else{
			updateMap();
		}*/
		updateMap();

		myastar = new Astar();
		CCLog("(int)tileMap->getMapSize().width:%d",(int)tileMap->getMapSize().width);
		CCLog("(int)tileMap->getMapSize().height:%d",(int)tileMap->getMapSize().height);

		/*for(int i=0;i<13;i++){
		CCLog(" %d %d %d %d %d %d %d %d %d %d %d %d %d %d",mapObstruct[i*14],mapObstruct[i*14+1],mapObstruct[i*14+2],mapObstruct[i*14+3],
		mapObstruct[i*14+4],mapObstruct[i*14+5],mapObstruct[i*14+6],mapObstruct[i*14+7],mapObstruct[i*14+8],
		mapObstruct[i*14+9],mapObstruct[i*14+10],mapObstruct[i*14+11],mapObstruct[i*14+12],mapObstruct[i*14+13]);
		}*/
		GB2ShapeCache::sharedGB2ShapeCache()->addShapesWithFile("image/tankerbullet.plist");
		//����ӵ�
		tankerBulletArray = CCArray::create();
		tankerBulletArray->retain();

		for(int i=0;i<30;i++){
			Bullet *tankBullet = Bullet::spriteCreate(turretId,turretGrade);
			tankBullet->setBulletAction();
			tankBullet->setPosition(ccp(-40, 0));
			tankBullet->setVisible(false);
			tileMap->addChild(tankBullet,20,110+i); 

			//char bulletName[30];
			//sprintf(bulletName,"eleball%d", turretId);
			tankBullet->setName("eleball");

			//tankerBulletArray.push_back(body);
			//push_back
			tankerBulletArray->addObject(tankBullet);
			/* CCTMXObjectGroup *objects = tileMap->objectGroupNamed("Objects");
			CCDictionary *spawnPoint = objects->objectNamed("plyartPoint");
			int x = spawnPoint->valueForKey("x")->intValue();
			int y = spawnPoint->valueForKey("y")->intValue();*/
		}
		//����ӵ�6
		tankerBulletArray6 = CCArray::create();
		tankerBulletArray6->retain();
		for(int i=0;i<20;i++){
			Bullet *tankBullet = Bullet::spriteCreate(6,2);
			tankBullet->setBulletAction();
			tankBullet->setPosition(ccp(0, 0));
			tankBullet->setVisible(false);
			tileMap->addChild(tankBullet,20,210+i); 

			//char bulletName[30];
			//sprintf(bulletName,"eleball%d", turretId);
			tankBullet->setName("eleball");

			//tankerBulletArray.push_back(body);
			//push_back
			tankerBulletArray6->addObject(tankBullet);
			/* CCTMXObjectGroup *objects = tileMap->objectGroupNamed("Objects");
			CCDictionary *spawnPoint = objects->objectNamed("plyartPoint");
			int x = spawnPoint->valueForKey("x")->intValue();
			int y = spawnPoint->valueForKey("y")->intValue();*/
		}

		//setViewpointCenter(ccp(x, y));
		loadingBreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(GameScene::loading3)); 
		//CCLog("----%d---000000000000000-----:%ld",m_nNumberOfLoadedSprites,millisecondNow()-start);
	}
}
void GameScene::loading3(){
	//CCLog("0000000000000000000000000000000000000000000000000000");
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	if(tileMap->getContentSize().width<size.width){
		//this->getChildByTag(1000)->setPosition(ccp(this->getChildByTag(1000)->getPositionX()-20,));
		this->getChildByTag(1)->setPositionX((this->getChildByTag(1))->getPositionX()-10);
		//���õ�ͼ
		setViewpointCenter(ccp(playBody->GetPosition().x*PTM_RATIO,playBody->GetPosition().y*PTM_RATIO));
	}
	loadingBreader->getAnimationManager()->runAnimationsForSequenceNamed("3rd");

	if(sound){
		SimpleAudioEngine::sharedEngine()->playEffect("sound/23.ogg",false);
	}
	if(mapIndex<3){
		if(mapIndex==1){
			if(!LoadBooleanFromXML("teach1")){
				loadingBreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(GameScene::teach)); 
				SaveBooleanToXML("teach1",true);
			}else{
				loadingBreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(GameScene::battleStart)); 
			}
		}else if(mapIndex==2){
			if(!LoadBooleanFromXML("teach2")){
				loadingBreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(GameScene::teach)); 
				SaveBooleanToXML("teach2",true);
			}else{
				loadingBreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(GameScene::battleStart)); 
			}
		}else{
			loadingBreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(GameScene::battleStart)); 
		}

	}else{
		loadingBreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(GameScene::battleStart)); 
	}
}

void GameScene::enemysEmerge(){
	//����
	if(emergeTime.getSurplusMillis()<=0){
		if(limitCountTime.getSurplusMillis()<=0){
			if(emergeNum<limit){
				emergeNum++;
				limitCountTime.reset();
				limitCountTime.start();
			}
		}

		//CCLog("===================enemysCount:%d",enemysCount);
		// CCLog("===================enemys.size():%d",enemys.size());
		if(enemysCount>0&&enemys.size()<emergeNum){//&&!isEnemyTankStanding
			if(isEnemyTankStanding){
				if(enemys.size()>2){
					return;
				}
			}
			int  _index=0;
			int  king =0 ;//����
			int  nudex=0;
			bool loop = true;
			while(loop){
				//CCLog("zzzz=============enemyKingNum_DictElement->count()-1:%d",enemyKingNum_DictElement->count()-1);
				_index=random(0,enemyKingNum_DictElement->count()-1);
			//	CCLog("=====we==============_index:%d",_index);
				enemyKingNumDictElement *_enemyKingNumDE = (enemyKingNumDictElement*)enemyKingNum_DictElement->objectAtIndex(_index);
				//CCLog("_enemyKingNumDE->Num:%d",_enemyKingNumDE->Num);
				if(_enemyKingNumDE->Num>0){
				//	CCLog("===================index:%d",((CCArray*)enemyTankArray->objectAtIndex(_enemyKingNumDE->index))->count());
					for(int i=0;i<((CCArray*)enemyTankArray->objectAtIndex(_enemyKingNumDE->index))->count();i++){//
						if(!((enemyTank*)(((CCArray*)enemyTankArray->objectAtIndex(_enemyKingNumDE->index))->objectAtIndex(i)))->getShow()){
							king = _enemyKingNumDE->King;
							_enemyKingNumDE->Num--;
							enemysCount--;
							emergeTime.reset();
							emergeTime.start();
							loop = false;//����ѭ
							nudex = i;
							break;
						}
					}
				}
			}
			//CCLog("vvvvvvvvvvvvvvvvvvvvv=randEnemyPos:%d",randEnemyPos);
			//λ��
			int  enemyPointIndex= random(0,2);
			while(randEnemyPos==enemyPointIndex){

				enemyPointIndex= random(0,2);
				//CCLog("enemyPointIndex:%d",enemyPointIndex);
			}
			randEnemyPos = enemyPointIndex;
			//CCLog("-----------------king:%d",king);
			//CCLog("-----------------enemyPointIndex:%d",enemyPointIndex);
			//long  startTime = millisecondNow();

			//enemyTank *enemyTank2 = enemyTank::spriteCreate(king);
			//enemyTank2->setTankerAction();
			// CCAssert(((enemyKingNumDictElement*)enemyKingNum_DictElement->objectAtIndex(index))->_enemyTank==NULL,"Paddle - Unexpected state!");
			// enemyTank *enemyTank2= (enemyTank*)((((enemyKingNumDictElement*)enemyKingNum_DictElement->objectAtIndex(index))->enemytank)->copy());

			// enemyTank *enemyTank2=((((enemyKingNumDictElement*)enemyKingNum_DictElement->objectAtIndex(index))->enemytank));
			int  enemyTankArrayIndex= ((enemyKingNumDictElement*)enemyKingNum_DictElement->objectAtIndex(_index))->index;

			//CCLog("-----------------enemyTankArrayIndex:%d",enemyTankArrayIndex);


			enemyTank *enemyTank2 = (enemyTank*)(((CCArray*)enemyTankArray->objectAtIndex(enemyTankArrayIndex))->objectAtIndex(nudex));
			//CCLog("-------------------------------nudex:%d",nudex);
			if(enemyTank2){
			 enemyTank2->setPosition(enemyPoint[enemyPointIndex]);//�����
			 enemyTank2->setDestroy(false);//������body
			 enemyTank2->setShow(true);//��ʾ

			 enemyTank2->setTag(200+enemysCount);
			 CCLog("enemyTank2-----------------time:%ld",millisecondNow());
			 tileMap->addChild(enemyTank2,1);
			//CCLog("-----------------enemyPoint[enemyPointIndex].x:%f        y:%f",enemyPoint[enemyPointIndex].x,enemyPoint[enemyPointIndex].y);
			 CCLog("-----------------------ddd--------nudex:%d",nudex);
			//char enemyN[30];
			//sprintf(enemyN,"enemy%d",king);
			 enemyTank2->setName("enemy");//����

			b2BodyDef enemyBodyDef;
			enemyBodyDef.type = b2_dynamicBody;

			enemyBodyDef.position.Set(enemyPoint[enemyPointIndex].x/PTM_RATIO, enemyPoint[enemyPointIndex].y/PTM_RATIO);
			enemyBodyDef.allowSleep  = false;
			enemyBodyDef.fixedRotation = true;//��ֹ������ת

			enemyBodyDef.userData = enemyTank2;
			b2Body* enemyBody = world->CreateBody(&enemyBodyDef);

			GB2ShapeCache *enemysc = GB2ShapeCache::sharedGB2ShapeCache();

			char lable[30];
			if(enemyTank2->getKing()>11){
				sprintf(lable,"boss",0);
			}else{
				sprintf(lable,"enemy",0);
			}

			enemysc->addFixturesToBody(enemyBody,lable); 
			enemyTank2->setAnchorPoint(enemysc->anchorPointForShape(lable));

			enemyAIChangeDirection(enemyBody);
			enemys.push_back(enemyBody);

			if(isEnemyTankStanding){
				enemyBody->SetLinearVelocity(b2Vec2(0/PTM_RATIO,0/PTM_RATIO));
				((enemyTank*)enemyBody->GetUserData())->setShowStand(true);
			}
		  }
			//  CCLog("-----------------enemyPointIndexenemy:%ld",millisecondNow());
		}

		if(enemysCount<=0&&mapIndex==2){//�ڶ��ؽ�ѧ������̹������5����ը
			if(!taechBom){
				teachBom();
			}
		}

		//��Ϸ����ʤ��
		if(enemysCount==0&&enemys.size()==0&&playBody!=NULL){
			CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
			((UIlayer *)scene->getChildByTag(2))->successMenu();
		}
	}
}


void GameScene::teachBom(){
	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	CCSize size = CCDirector::sharedDirector()->getWinSize();

	this->setTouchEnabled(true);

	taechBom = true;

	paused = true;
	this->pauseSchedulerAndActions();

	UIlayer *ui = ((UIlayer *)scene->getChildByTag(2));
	ui->menuFireStopCallback(ui,CCControlEventTouchDown);

	gameTime.pause();
	((CCMenu*)ui->getChildByTag(1))->setTouchEnabled(false);
	((ImmediateButton*)ui->getChildByTag(100))->setTouchEnabled(false);
	if(ui->getChildByTag(101)){
		((ImmediateButton*)ui->getChildByTag(101))->setTouchEnabled(false);
	}
	if(ui->getChildByTag(102)){
		((ImmediateButton*)ui->getChildByTag(102))->setTouchEnabled(false);
	}

	CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary(); 
	//����һ���Զ��ͷŵ�CCB��ȡ�ࣨCCBReader�� 
	bomReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary); 
	// ccbReader->autorelease(); 
	CCNode * node = bomReader->readNodeGraphFromFile("teach/instruction_204.ccbi", this);

	((UIlayer *)scene->getChildByTag(2))->addChild(node,10,798);

}

/*
��������
*/
void GameScene::createWorld(){
	CCSize size =CCDirector::sharedDirector()->getWinSize();
	b2Vec2 gravity;
	gravity.Set(0.0f, 0.0f);
	// Do we want to let bodies sleep?
	bool doSleep = true;
	// Construct a world object, which will hold and simulate the rigid bodies.
	world = new b2World(gravity);
	world->SetAllowSleeping(doSleep);    
	world->SetContinuousPhysics(true);

	world->SetContactListener(this);//���ô�������

	//����
	m_debugDraw = new GLESDebugDraw( PTM_RATIO );
	world->SetDebugDraw(m_debugDraw);

	uint32  flags = 0;
	/*flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_jointBit;
	flags += b2Draw::e_aabbBit;
	flags += b2Draw::e_pairBit;
	flags += b2Draw::e_centerOfMassBit;
	m_debugDraw->SetFlags(flags);	*/

	float  mapHeight = tileMap->getContentSize().height;
	float  mapWidth = tileMap->getContentSize().width;

	b2BodyDef groundBodyDef;
	//groundBodyDef.position.Set(mapHeight/2/PTM_RATIO, 
	//                           mapWidth/2/PTM_RATIO); // bottom-left corner
	groundBodyDef.position.Set(0, 0); 
	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape (also from a pool).
	// The body is also added to the world.
	b2Body* groundBody = world->CreateBody(&groundBodyDef);
	// Define the ground box shape.
	b2PolygonShape groundBox;		


	groundBox.SetAsBox(mapWidth/2/PTM_RATIO, 0, b2Vec2(mapWidth/2/PTM_RATIO, 0), 0);
	groundBody->CreateFixture(&groundBox, 0);

	// top
	groundBox.SetAsBox(mapWidth/2/PTM_RATIO, 0, b2Vec2(mapWidth/2/PTM_RATIO, mapHeight/PTM_RATIO), 0);
	groundBody->CreateFixture(&groundBox, 0);

	//left
	groundBox.SetAsBox(0, mapHeight/2/PTM_RATIO, b2Vec2(0, mapHeight/2/PTM_RATIO), 0);
	groundBody->CreateFixture(&groundBox, 0);

	// right
	groundBox.SetAsBox(0, mapHeight/2/PTM_RATIO, b2Vec2(mapWidth/PTM_RATIO, mapHeight/2/PTM_RATIO), 0);
	groundBody->CreateFixture(&groundBox, 0);

	//b
	groundBox.SetAsBox(mapWidth/2/PTM_RATIO, 0, b2Vec2((mapWidth+30)/2/PTM_RATIO, 0), 0);
	groundBody->CreateFixture(&groundBox, 0);

	// t
	groundBox.SetAsBox(mapWidth/2/PTM_RATIO, 0, b2Vec2(mapWidth/2/PTM_RATIO, (mapHeight+30)/PTM_RATIO), 0);
	groundBody->CreateFixture(&groundBox, 0);
	//l
	groundBox.SetAsBox(0, mapHeight/2/PTM_RATIO, b2Vec2(-30, mapHeight/2/PTM_RATIO), 0);
	groundBody->CreateFixture(&groundBox, 0);
	// r
	groundBox.SetAsBox(0, mapHeight/2/PTM_RATIO, b2Vec2((mapWidth+30)/PTM_RATIO, mapHeight/2/PTM_RATIO), 0);
	groundBody->CreateFixture(&groundBox, 0);

	// bottom
	/*  groundBox.SetAsBox(mapWidth/2/PTM_RATIO, 0, b2Vec2(32/PTM_RATIO, -(mapHeight+60)/2/PTM_RATIO), 0);
	groundBody->CreateFixture(&groundBox, 0);

	// top
	groundBox.SetAsBox(mapWidth/2/PTM_RATIO, 0, b2Vec2(32/PTM_RATIO, (mapHeight-70)/2/PTM_RATIO), 0);
	groundBody->CreateFixture(&groundBox, 0);

	// left
	groundBox.SetAsBox(0, mapHeight/2/PTM_RATIO, b2Vec2(-(mapWidth-70)/2/PTM_RATIO, -32/PTM_RATIO), 0);
	groundBody->CreateFixture(&groundBox, 0);

	// right
	groundBox.SetAsBox(0, mapHeight/2/PTM_RATIO, b2Vec2((mapWidth+60)/2/PTM_RATIO, -32/PTM_RATIO), 0);
	groundBody->CreateFixture(&groundBox, 0);

	//b
	groundBox.SetAsBox(mapWidth/2/PTM_RATIO, 0, b2Vec2(32/PTM_RATIO, -(mapHeight+100)/2/PTM_RATIO), 0);
	groundBody->CreateFixture(&groundBox, 0);
	//t
	groundBox.SetAsBox(mapWidth/2/PTM_RATIO, 0, b2Vec2(32/PTM_RATIO, (mapHeight-30)/2/PTM_RATIO), 0);
	groundBody->CreateFixture(&groundBox, 0);
	//l
	groundBox.SetAsBox(0, mapHeight/2/PTM_RATIO, b2Vec2(-(mapWidth-30)/2/PTM_RATIO, -32/PTM_RATIO), 0);
	groundBody->CreateFixture(&groundBox, 0);
	//r
	groundBox.SetAsBox(0, mapHeight/2/PTM_RATIO, b2Vec2((mapWidth+90)/2/PTM_RATIO, -32/PTM_RATIO), 0);
	groundBody->CreateFixture(&groundBox, 0);*/

	drawWorldDebug = true;
}

void GameScene::uiShow(float time){

	if (mapIndex==3)
	{
		char lable[30];
		UserRecord *urs = new UserRecord("tank.db","shop");
		sprintf(lable,"shopbox%d",4);
		int shopbox4= urs->readIntFromSQLite(lable);
		sprintf(lable,"shopbox%d",10);
	    int shopbox10 = urs->readIntFromSQLite(lable);

		if(shopbox4==0||shopbox10==0){
			this->removeChildByTag(1);
			unschedule(schedule_selector(GameScene::uiShow));
			CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
			UIlayer *ui = UIlayer::create(); 
			scene->addChild(ui,2,2);

			ui->setVisible(false);
			/*CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
			cocos2d::extension::CCBReader *ccbreaderPay = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
			CCNode  *payNode  = ccbreaderPay->readNodeGraphFromFile("pay/scene_buy_xianshiqianggou.ccbi",this);
			ccbreaderPay->autorelease();
			this->addChild(payNode,100,8510);
			showQiangGouBool = false;
			this->schedule(schedule_selector(GameScene::showQiangGou),3.6f,kCCRepeatForever,1.0f);
			*/
			yesSecondQiangGouPay(this);
		}else{
			this->removeChildByTag(1);

			unschedule(schedule_selector(GameScene::uiShow));
			CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
			UIlayer *ui = UIlayer::create(); 
			scene->addChild(ui,2,2);

			baseFrameFrequency = 0;

			schedule( schedule_selector(GameScene::repositionSprite),0.02f);
			schedule(schedule_selector(GameScene::upDataWorld),0.02f);
			schedule(schedule_selector(GameScene::enemysAIAStar),1.5f);
			showAchieve(2);

		}
	}else if(mapIndex==6){
		char lable[30];
		UserRecord *urs = new UserRecord("tank.db","shop");
		sprintf(lable,"shopbox%d",5);
		int shopbox5= urs->readIntFromSQLite(lable);
		sprintf(lable,"shopbox%d",11);
		int shopbox11 = urs->readIntFromSQLite(lable);

		if(shopbox5==0||shopbox11==0){
			this->removeChildByTag(1);
			unschedule(schedule_selector(GameScene::uiShow));
			CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
			UIlayer *ui = UIlayer::create(); 
			scene->addChild(ui,2,2);

			ui->setVisible(false);

			/*CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
			cocos2d::extension::CCBReader *ccbreaderPay = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
			CCNode  *payNode  = ccbreaderPay->readNodeGraphFromFile("pay/scene_buy_ziyouqishi.ccbi",this);
			ccbreaderPay->autorelease();
			this->addChild(payNode,100,8510);*/
			showQiangGouBool = false;
			//this->schedule(schedule_selector(GameScene::guangMingQiShi),3.6f,kCCRepeatForever,1.0f);
			//guangMingQiShi(0);
			yesSecondGuangMingQiShiPay(this);
		}else{
			this->removeChildByTag(1);
			unschedule(schedule_selector(GameScene::uiShow));
			CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
			UIlayer *ui = UIlayer::create(); 
			scene->addChild(ui,2,2);

			baseFrameFrequency = 0;

			schedule( schedule_selector(GameScene::repositionSprite),0.02f);
			schedule(schedule_selector(GameScene::upDataWorld),0.02f);
			schedule(schedule_selector(GameScene::enemysAIAStar),1.5f);
			showAchieve(2);
		}

	}else if(mapIndex==9){
		char lable[30];
		UserRecord *urs = new UserRecord("tank.db","shop");
		sprintf(lable,"shopbox%d",6);
		int shopbox6= urs->readIntFromSQLite(lable);
		sprintf(lable,"shopbox%d",12);
		int shopbox12 = urs->readIntFromSQLite(lable);

		if(shopbox6==0||shopbox12==0){
			this->removeChildByTag(1);
			unschedule(schedule_selector(GameScene::uiShow));
			CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
			UIlayer *ui = UIlayer::create(); 
			scene->addChild(ui,2,2);

			ui->setVisible(false);

			/*CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
			cocos2d::extension::CCBReader *ccbreaderPay = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
			CCNode  *payNode  = ccbreaderPay->readNodeGraphFromFile("pay/scene_buy_tianshenxiafan.ccbi",this);
			ccbreaderPay->autorelease();
			this->addChild(payNode,100,8510);*/
			showQiangGouBool = false;
			//this->schedule(schedule_selector(GameScene::guangMingQiShi),3.6f,kCCRepeatForever,1.0f);
			//tianShenXiaFan(0);
			yesSecondtianShenXiaFanPay(this);
		}else{
			this->removeChildByTag(1);
			unschedule(schedule_selector(GameScene::uiShow));
			CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
			UIlayer *ui = UIlayer::create(); 
			scene->addChild(ui,2,2);

			baseFrameFrequency = 0;

			schedule( schedule_selector(GameScene::repositionSprite),0.02f);
			schedule(schedule_selector(GameScene::upDataWorld),0.02f);
			schedule(schedule_selector(GameScene::enemysAIAStar),1.5f);
			showAchieve(2);
		}
       


	}else{
		this->removeChildByTag(1);

		unschedule(schedule_selector(GameScene::uiShow));
		CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
		UIlayer *ui = UIlayer::create(); 
		scene->addChild(ui,2,2);

		baseFrameFrequency = 0;

		schedule( schedule_selector(GameScene::repositionSprite),0.02f);
		schedule(schedule_selector(GameScene::upDataWorld),0.02f);
		schedule(schedule_selector(GameScene::enemysAIAStar),1.5f);
		showAchieve(2);
	}
	
}


void GameScene::showQiangGou(float time){
	if(this->getChildByTag(8511)){
		this->removeChildByTag(8511,-1);
	}
	CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	cocos2d::extension::CCBReader *ccbreaderPay = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
	CCNode  *payNode  = ccbreaderPay->readNodeGraphFromFile("pay/scene_buy_xianshiqianggou2.ccbi",this);
	ccbreaderPay->autorelease();
	this->addChild(payNode,100,8511);

	if(!showQiangGouBool){
		CCSize size =CCDirector::sharedDirector()->getWinSize();
		CCNode * node =this->getChildByTag(8510);
		CCMenuItemImage *yesItem = CCMenuItemImage::create(
			"UI/queding1.png",
			"UI/queding2.png",
			this,
			menu_selector(GameScene::yesSecondQiangGouPay));//yesQiangGouCallBack
		yesItem->setPosition(ccp(size.width/2, size.height/2-150));

		CCMenuItemImage *noItem = CCMenuItemImage::create(
			"UI/guanbi1.png",
			"UI/guanbi2.png",
			this,
			menu_selector(GameScene::noQiangGouCallBack));
		noItem->setPosition(ccp(size.width/2+290, size.height/2+175));

		CCMenu* pMenu = CCMenu::create(yesItem,noItem, NULL);
		pMenu->setPosition(CCPointZero);
		node->addChild(pMenu,1,1);

		showQiangGouBool= true;
		qiangGouShow = false;
	}
}

void GameScene::yesQiangGouCallBack(CCObject* sender){

	if(!qiangGouShow){
		CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		cocos2d::extension::CCBReader *ccbreaderPay = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
		CCNode  *payNode  = ccbreaderPay->readNodeGraphFromFile("pay/warning_pay.ccbi",this);
		ccbreaderPay->autorelease();
		this->addChild(payNode,101,8512);

		CCSize size =CCDirector::sharedDirector()->getWinSize();
		CCMenuItemImage *yesItem = CCMenuItemImage::create(
			"ccbResources/active/button_yes0.png",
			"ccbResources/active/button_yes1.png",
			this,
			menu_selector(GameScene::yesSecondQiangGouPay));
		yesItem->setPosition(ccp(size.width/2-100, size.height/2-65));

		CCMenuItemImage *noItem = CCMenuItemImage::create(
			"ccbResources/active/button_no0.png",
			"ccbResources/active/button_no1.png",
			this,
			menu_selector(GameScene::noSecondQiangGouPay));
		noItem->setPosition(ccp(size.width/2+100, size.height/2-65));

		CCMenu* pMenu = CCMenu::create(yesItem,noItem, NULL);
		pMenu->setPosition(CCPointZero);
		payNode->addChild(pMenu,1,1);

		qiangGouShow = true;
	}
}

void GameScene::yesSecondQiangGouPay(CCObject* sender){
	qiangGouShow = false;
	
 #if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
	billingPointIndex = 9;//�Ʒѵ�
	SaveBooleanToXML("payResultBool",false);
	SaveBooleanToXML("payGameOver",false);
	//payResultBool = false;;
	//payGameOver = false;
	this->schedule(schedule_selector(GameScene::updateBillingPoint), 0.1f);

	JniMethodInfo minfo;
	CCLog("============"); 
	bool isHave = JniHelper::getStaticMethodInfo(minfo,"quyou/game/tank/billing","show_java_dialog", "(I)I");//"com/game/test/JniFruitHelper","show_java_dialog"
	CCLog("------------"); 
	jint _int=0;
	if (isHave) {
		CCLog("jni-java����ִ�����"); 
		_int = minfo.env->CallStaticIntMethod(minfo.classID, minfo.methodID,billingPointIndex);//
	}else{
		CCLog("jjni:�˺�������"); 
	}
  #else
	this->unschedule(schedule_selector(GameScene::showQiangGou));
	if(this->getChildByTag(8511)){
		this->removeChildByTag(8511,-1);
	}
	this->removeChildByTag(8510,-1);

	if(this->getChildByTag(8512)){
		this->removeChildByTag(8512,-1);
	}

	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	((UIlayer *)scene->getChildByTag(2))->setVisible(true);
	//UIlayer *ui = UIlayer::create(); 
	//scene->addChild(ui,2,2);

	baseFrameFrequency = 0;

	schedule( schedule_selector(GameScene::repositionSprite),0.02f);
	schedule(schedule_selector(GameScene::upDataWorld),0.02f);
	schedule(schedule_selector(GameScene::enemysAIAStar),1.5f);
	showAchieve(2);

	qiangGouSave();

  #endif

}
void GameScene::qiangGouSave(){
	int state = ((tanker*) tileMap->getChildByTag(100))->getState();
	int zOrder =  ((tanker*) tileMap->getChildByTag(100))->getZOrder();

	tileMap->removeChildByTag(100);
	tanker *tankMax =tanker::spriteCreate(4,4,1,1);
	tankMax->setTankerAction();
	tileMap->addChild(tankMax,zOrder,100);
	tankMax->setName("tank");

	tankMax->setState(state);
	if(tankMax!=NULL){
		playBody->SetUserData(tankMax);
	}

	turretId = 4;
	chassisId = 4;
	turretGrade = 1;
	chassisGrade = 1;

	char lable[30];
	UserRecord *urs = new UserRecord("tank.db","shop");
	sprintf(lable,"shopbox%d",4);
	if(urs->readIntFromSQLite(lable)==0){
		turretId = 4;
		urs->saveInt(lable,1);//����ȼ�
	}else{
		turretGrade = urs->readIntFromSQLite(lable);
	}

	sprintf(lable,"shopbox%d",10);
	if(urs->readIntFromSQLite(lable)==0){
		chassisId = 4;
		urs->saveInt(lable,1);//����ȼ�
	}else{
		chassisGrade= urs->readIntFromSQLite(lable);
	}
	urs->saveInt(lable,1);//����ȼ�
	urs->closeDB();
	free(urs);

	UserRecord *ur = new UserRecord("tank.db","tank");
	ur->saveInt("turretId",turretId);
	ur->saveInt("chassisId",chassisId);
	ur->saveInt("turretGrade",turretGrade);
	ur->saveInt("chassisGrade",chassisGrade);
	ur->closeDB();
	free(ur);


	tankerBulletArray->removeAllObjects();
	for(int i=0;i<30;i++){
		Bullet *tankBullet = Bullet::spriteCreate(4,1);
		tankBullet->setBulletAction();
		tankBullet->setPosition(ccp(-40, 0));
		tankBullet->setVisible(false);
		tileMap->addChild(tankBullet,20,110+i); 

		tankBullet->setName("eleball");
		tankerBulletArray->addObject(tankBullet);
	}
}

void GameScene::guangMingQiShiSave(){
	int state = ((tanker*) tileMap->getChildByTag(100))->getState();
	int zOrder =  ((tanker*) tileMap->getChildByTag(100))->getZOrder();

	tileMap->removeChildByTag(100);
	tanker *tankMax =tanker::spriteCreate(5,5,1,1);
	tankMax->setTankerAction();
	tileMap->addChild(tankMax,zOrder,100);
	tankMax->setName("tank");

	tankMax->setState(state);
	if(tankMax!=NULL){
		playBody->SetUserData(tankMax);
	}

	turretId = 5;
	chassisId = 5;
	turretGrade = 1;
	chassisGrade = 1;

	char lable[30];
	UserRecord *urs = new UserRecord("tank.db","shop");
	sprintf(lable,"shopbox%d",5);
	if(urs->readIntFromSQLite(lable)==0){
		turretId = 5;
		urs->saveInt(lable,1);//����ȼ�
	}else{
		turretGrade = urs->readIntFromSQLite(lable);
	}

	sprintf(lable,"shopbox%d",11);
	if(urs->readIntFromSQLite(lable)==0){
		chassisId = 5;
		urs->saveInt(lable,1);//����ȼ�
	}else{
		chassisGrade= urs->readIntFromSQLite(lable);
	}
	urs->saveInt(lable,1);//����ȼ�
	urs->closeDB();
	free(urs);

	UserRecord *ur = new UserRecord("tank.db","tank");
	ur->saveInt("turretId",turretId);
	ur->saveInt("chassisId",chassisId);
	ur->saveInt("turretGrade",turretGrade);
	ur->saveInt("chassisGrade",chassisGrade);
	ur->closeDB();
	free(ur);


	tankerBulletArray->removeAllObjects();
	for(int i=0;i<30;i++){
		Bullet *tankBullet = Bullet::spriteCreate(5,1);
		tankBullet->setBulletAction();
		tankBullet->setPosition(ccp(-40, 0));
		tankBullet->setVisible(false);
		tileMap->addChild(tankBullet,20,110+i); 

		tankBullet->setName("eleball");
		tankerBulletArray->addObject(tankBullet);
	}
}


void GameScene::guangMingQiShi(float time){

	if(!showQiangGouBool){
		CCSize size =CCDirector::sharedDirector()->getWinSize();
		CCNode * node =this->getChildByTag(8510);
		CCMenuItemImage *yesItem = CCMenuItemImage::create(
			"UI/queding1.png",
			"UI/queding2.png",
			this,
			menu_selector(GameScene::yesSecondGuangMingQiShiPay));//yesQiangGouCallBack
		yesItem->setPosition(ccp(size.width/2, size.height/2-150));

		CCMenuItemImage *noItem = CCMenuItemImage::create(
			"UI/guanbi1.png",
			"UI/guanbi2.png",
			this,
			menu_selector(GameScene::noSecondGuangMingQiShiPay));
		noItem->setPosition(ccp(size.width/2+290, size.height/2+175));

		CCMenu* pMenu = CCMenu::create(yesItem,noItem, NULL);
		pMenu->setPosition(CCPointZero);
		node->addChild(pMenu,1,1);

		showQiangGouBool= true;
		qiangGouShow = false;
	}
}

void GameScene::yesSecondGuangMingQiShiPay(CCObject* sender){
	qiangGouShow = false;

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
	billingPointIndex = 14;//�Ʒѵ�
	SaveBooleanToXML("payResultBool",false);
	SaveBooleanToXML("payGameOver",false);
	//payResultBool = false;;
	//payGameOver = false;
	this->schedule(schedule_selector(GameScene::updateBillingPoint), 0.1f);

	JniMethodInfo minfo;
	CCLog("============"); 
	bool isHave = JniHelper::getStaticMethodInfo(minfo,"quyou/game/tank/billing","show_java_dialog", "(I)I");//"com/game/test/JniFruitHelper","show_java_dialog"
	CCLog("------------"); 
	jint _int=0;
	if (isHave) {
		CCLog("jni-java����ִ�����"); 
		_int = minfo.env->CallStaticIntMethod(minfo.classID, minfo.methodID,billingPointIndex);//
	}else{
		CCLog("jjni:�˺�������"); 
	}
  #else
	if(this->getChildByTag(8511)){
		this->removeChildByTag(8511,-1);
	}
	this->removeChildByTag(8510,-1);

	if(this->getChildByTag(8512)){
		this->removeChildByTag(8512,-1);
	}

	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	((UIlayer *)scene->getChildByTag(2))->setVisible(true);
	//UIlayer *ui = UIlayer::create(); 
	//scene->addChild(ui,2,2);

	baseFrameFrequency = 0;

	schedule( schedule_selector(GameScene::repositionSprite),0.02f);
	schedule(schedule_selector(GameScene::upDataWorld),0.02f);
	schedule(schedule_selector(GameScene::enemysAIAStar),1.5f);
	showAchieve(2);
	guangMingQiShiSave();
	//qiangGouSave();

  #endif
}

void GameScene::noSecondGuangMingQiShiPay(CCObject* sender){
	if(this->getChildByTag(8511)){
		this->removeChildByTag(8511,-1);
	}
	this->removeChildByTag(8510,-1);

	if(this->getChildByTag(8512)){
		this->removeChildByTag(8512,-1);
	}

	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	((UIlayer *)scene->getChildByTag(2))->setVisible(true);
	//UIlayer *ui = UIlayer::create(); 
	//scene->addChild(ui,2,2);

	baseFrameFrequency = 0;

	schedule( schedule_selector(GameScene::repositionSprite),0.02f);
	schedule(schedule_selector(GameScene::upDataWorld),0.02f);
	schedule(schedule_selector(GameScene::enemysAIAStar),1.5f);
	showAchieve(2);
	qiangGouShow = false;
}


void GameScene::tianShenXiaFan(float time){
	if(!showQiangGouBool){
		CCSize size =CCDirector::sharedDirector()->getWinSize();
		CCNode * node =this->getChildByTag(8510);
		CCMenuItemImage *yesItem = CCMenuItemImage::create(
			"UI/queding1.png",
			"UI/queding2.png",
			this,
			menu_selector(GameScene::yesSecondtianShenXiaFanPay));//yesQiangGouCallBack
		yesItem->setPosition(ccp(size.width/2, size.height/2-150));

		CCMenuItemImage *noItem = CCMenuItemImage::create(
			"UI/guanbi1.png",
			"UI/guanbi2.png",
			this,
			menu_selector(GameScene::noSecondtianShenXiaFanPay));
		noItem->setPosition(ccp(size.width/2+290, size.height/2+175));

		CCMenu* pMenu = CCMenu::create(yesItem,noItem, NULL);
		pMenu->setPosition(CCPointZero);
		node->addChild(pMenu,1,1);

		showQiangGouBool= true;
		qiangGouShow = false;
	}
}

void GameScene::yesSecondtianShenXiaFanPay(CCObject* sender){
	qiangGouShow = false;

  #if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
	billingPointIndex = 15;//�Ʒѵ�
	SaveBooleanToXML("payResultBool",false);
	SaveBooleanToXML("payGameOver",false);
	//payResultBool = false;;
	//payGameOver = false;
	this->schedule(schedule_selector(GameScene::updateBillingPoint), 0.1f);

	JniMethodInfo minfo;
	CCLog("============"); 
	bool isHave = JniHelper::getStaticMethodInfo(minfo,"quyou/game/tank/billing","show_java_dialog", "(I)I");//"com/game/test/JniFruitHelper","show_java_dialog"
	CCLog("------------"); 
	jint _int=0;
	if (isHave) {
		CCLog("jni-java����ִ�����"); 
		_int = minfo.env->CallStaticIntMethod(minfo.classID, minfo.methodID,billingPointIndex);//
	}else{
		CCLog("jjni:�˺�������"); 
	}
#else
	if(this->getChildByTag(8511)){
		this->removeChildByTag(8511,-1);
	}
	this->removeChildByTag(8510,-1);

	if(this->getChildByTag(8512)){
		this->removeChildByTag(8512,-1);
	}

	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	((UIlayer *)scene->getChildByTag(2))->setVisible(true);
	//UIlayer *ui = UIlayer::create(); 
	//scene->addChild(ui,2,2);

	baseFrameFrequency = 0;

	schedule( schedule_selector(GameScene::repositionSprite),0.02f);
	schedule(schedule_selector(GameScene::upDataWorld),0.02f);
	schedule(schedule_selector(GameScene::enemysAIAStar),1.5f);
	showAchieve(2);
	tianShenXiaFanSave();
	//qiangGouSave();
#endif

}

void GameScene::noSecondtianShenXiaFanPay(CCObject* sender){
	if(this->getChildByTag(8511)){
		this->removeChildByTag(8511,-1);
	}
	this->removeChildByTag(8510,-1);

	if(this->getChildByTag(8512)){
		this->removeChildByTag(8512,-1);
	}

	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	((UIlayer *)scene->getChildByTag(2))->setVisible(true);
	//UIlayer *ui = UIlayer::create(); 
	//scene->addChild(ui,2,2);

	baseFrameFrequency = 0;

	schedule( schedule_selector(GameScene::repositionSprite),0.02f);
	schedule(schedule_selector(GameScene::upDataWorld),0.02f);
	schedule(schedule_selector(GameScene::enemysAIAStar),1.5f);
	showAchieve(2);
	qiangGouShow = false;
}


void GameScene::tianShenXiaFanSave(){
	int state = ((tanker*) tileMap->getChildByTag(100))->getState();
	int zOrder =  ((tanker*) tileMap->getChildByTag(100))->getZOrder();

	tileMap->removeChildByTag(100);
	tanker *tankMax =tanker::spriteCreate(6,6,1,1);
	tankMax->setTankerAction();
	tileMap->addChild(tankMax,zOrder,100);
	tankMax->setName("tank");

	tankMax->setState(state);
	if(tankMax!=NULL){
		playBody->SetUserData(tankMax);
	}

	turretId = 6;
	chassisId = 6;
	turretGrade = 1;
	chassisGrade = 1;

	char lable[30];
	UserRecord *urs = new UserRecord("tank.db","shop");
	sprintf(lable,"shopbox%d",6);
	if(urs->readIntFromSQLite(lable)==0){
		turretId = 6;
		urs->saveInt(lable,1);//����ȼ�
	}else{
		turretGrade = urs->readIntFromSQLite(lable);
	}

	sprintf(lable,"shopbox%d",12);
	if(urs->readIntFromSQLite(lable)==0){
		chassisId = 6;
		urs->saveInt(lable,1);//����ȼ�
	}else{
		chassisGrade= urs->readIntFromSQLite(lable);
	}
	urs->saveInt(lable,1);//����ȼ�
	urs->closeDB();
	free(urs);

	UserRecord *ur = new UserRecord("tank.db","tank");
	ur->saveInt("turretId",turretId);
	ur->saveInt("chassisId",chassisId);
	ur->saveInt("turretGrade",turretGrade);
	ur->saveInt("chassisGrade",chassisGrade);
	ur->closeDB();
	free(ur);


	tankerBulletArray->removeAllObjects();
	for(int i=0;i<30;i++){
		Bullet *tankBullet = Bullet::spriteCreate(6,1);
		tankBullet->setBulletAction();
		tankBullet->setPosition(ccp(-40, 0));
		tankBullet->setVisible(false);
		tileMap->addChild(tankBullet,20,110+i); 

		tankBullet->setName("eleball");
		tankerBulletArray->addObject(tankBullet);
	}
	Bullet *tankBullet=(Bullet*)(tankerBulletArray->objectAtIndex(0));
	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();

	((UIlayer *)scene->getChildByTag(2))->rateOfFire = tankBullet->getRateOfFire();
	CCLOG("000rateOfFire:%f",((UIlayer *)scene->getChildByTag(2))->rateOfFire);

	((UIlayer *)scene->getChildByTag(2))->fireTime.setMillis(((UIlayer *)scene->getChildByTag(2))->rateOfFire*1000);
	((UIlayer *)scene->getChildByTag(2))->fireTime.start();
}


void GameScene::deadTianShenXiaFanSave(){
	UserRecord *gradeUR = new UserRecord("tank.db","tank");
	int turretId = gradeUR->readIntFromSQLite("turretId");//����
	int turretGrade = gradeUR->readIntFromSQLite("turretGrade");//�����ȼ�
	int chassisId = gradeUR->readIntFromSQLite("chassisId");//����
	int chassisGrade = gradeUR->readIntFromSQLite("chassisGrade");//���̵ȼ�

	gradeUR->saveInt("turretId",6);
	gradeUR->saveInt("chassisId",6);

	gradeUR->closeDB();
	free(gradeUR);

	char lable[30];
	UserRecord *ur = new UserRecord("tank.db","shop");
	sprintf(lable,"shopbox%d",6);
	ur->saveInt(lable,1);//����ȼ�
	sprintf(lable,"shopbox%d",12);
	ur->saveInt(lable,1);//����ȼ�
	ur->closeDB();
	free(ur);
}


void GameScene::updateBillingPoint(float time){
  #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
	if(billingPointIndex==14){
		if(LoadBooleanFromXML("payGameOver")){
			this->unschedule(schedule_selector(GameScene::updateBillingPoint));
			if(LoadBooleanFromXML("payResultBool")){//���ѳɹ�

				//unschedule(schedule_selector(GameScene::showQiangGou));
				if(this->getChildByTag(8511)){
					this->removeChildByTag(8511,-1);
				}
				this->removeChildByTag(8510,-1);

				if(this->getChildByTag(8512)){
					this->removeChildByTag(8512,-1);
				}

				//CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
				//UIlayer *ui = UIlayer::create(); 
				//scene->addChild(ui,2,2);
				CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
				((UIlayer *)scene->getChildByTag(2))->setVisible(true);

				baseFrameFrequency = 0;

				schedule( schedule_selector(GameScene::repositionSprite),0.02f);
				schedule(schedule_selector(GameScene::upDataWorld),0.02f);
				schedule(schedule_selector(GameScene::enemysAIAStar),1.5f);
				showAchieve(2);

				guangMingQiShiSave();
			}
		}


	}else if(billingPointIndex==15){
		if(LoadBooleanFromXML("payGameOver")){
			this->unschedule(schedule_selector(GameScene::updateBillingPoint));
			if(LoadBooleanFromXML("payResultBool")){//���ѳɹ�

				if(showDeadTianShenXiafan){
					CCScene* scene = CCDirector::sharedDirector()->getRunningScene();

					((UIlayer *)scene->getChildByTag(2))->removeChildByTag(8511);
					if(!((UIlayer *)scene->getChildByTag(2))->againPayLiftGame){
						((UIlayer *)scene->getChildByTag(2))->failMenu();
					}
					//showPayPoint = false;
					((UIlayer *)scene->getChildByTag(2))->againPayLiftGame = false;
					deadTianShenXiaFanSave();
				}else{
					if(this->getChildByTag(8511)){
						this->removeChildByTag(8511,-1);
					}
					this->removeChildByTag(8510,-1);

					if(this->getChildByTag(8512)){
						this->removeChildByTag(8512,-1);
					}

					//CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
					//UIlayer *ui = UIlayer::create(); 
					//scene->addChild(ui,2,2);
					CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
					((UIlayer *)scene->getChildByTag(2))->setVisible(true);

					baseFrameFrequency = 0;

					schedule( schedule_selector(GameScene::repositionSprite),0.02f);
					schedule(schedule_selector(GameScene::upDataWorld),0.02f);
					schedule(schedule_selector(GameScene::enemysAIAStar),1.5f);
					showAchieve(2);

					tianShenXiaFanSave();

				}
				
			}
		}


	}else if(billingPointIndex==9){
		if(LoadBooleanFromXML("payGameOver")){
			this->unschedule(schedule_selector(GameScene::updateBillingPoint));
			if(LoadBooleanFromXML("payResultBool")){//���ѳɹ�

				unschedule(schedule_selector(GameScene::showQiangGou));
				if(this->getChildByTag(8511)){
					this->removeChildByTag(8511,-1);
				}
				this->removeChildByTag(8510,-1);

				if(this->getChildByTag(8512)){
					this->removeChildByTag(8512,-1);
				}

				//CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
				//UIlayer *ui = UIlayer::create(); 
				//scene->addChild(ui,2,2);
				CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
				((UIlayer *)scene->getChildByTag(2))->setVisible(true);

				baseFrameFrequency = 0;

				schedule( schedule_selector(GameScene::repositionSprite),0.02f);
				schedule(schedule_selector(GameScene::upDataWorld),0.02f);
				schedule(schedule_selector(GameScene::enemysAIAStar),1.5f);
				showAchieve(2);

				qiangGouSave();
			}
		}
	}
#endif

}

void GameScene::noSecondQiangGouPay(CCObject* sender){
	qiangGouShow = false;
	if(this->getChildByTag(8512)){
		this->removeChildByTag(8512,-1);
	}
	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	((UIlayer *)scene->getChildByTag(2))->setVisible(true);
}

void GameScene::noQiangGouCallBack(CCObject* sender){
	unschedule(schedule_selector(GameScene::showQiangGou));
	if(this->getChildByTag(8511)){
		this->removeChildByTag(8511,-1);
	}
	this->removeChildByTag(8510,-1);

	if(this->getChildByTag(8512)){
		this->removeChildByTag(8512,-1);
	}
	
	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	((UIlayer *)scene->getChildByTag(2))->setVisible(true);
	//UIlayer *ui = UIlayer::create(); 
	//scene->addChild(ui,2,2);

	baseFrameFrequency = 0;

	schedule( schedule_selector(GameScene::repositionSprite),0.02f);
	schedule(schedule_selector(GameScene::upDataWorld),0.02f);
	schedule(schedule_selector(GameScene::enemysAIAStar),1.5f);
	showAchieve(2);

}


void GameScene::gameTeach(CCPoint point){
	CCSize size =CCDirector::sharedDirector()->getWinSize();
	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();

	this->setTouchEnabled(true);
	paused = true;
	this->pauseSchedulerAndActions();
	emergeTime.pause();
	gameTime.pause();

	teachIndex=10;
	teachEnd = false;

	UIlayer *ui = ((UIlayer *)scene->getChildByTag(2));
	ui->menuFireStopCallback(ui,CCControlEventTouchDown);

	CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	cocos2d::extension::CCBReader* teachB = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 

	CCNode  *animationsTest  = teachB->readNodeGraphFromFile("teach/instruction_tianshenxiafan1.ccbi",this);//tank11.ccbi
	//�������� 
	//animationsTest->setPosition(ccp(point.x,point.y)); 
	//CCLog("***********************");
	CCPoint  toPoint = getViewpointCenter(ccp(point.x, point.y));//�ƶ���Ļ
	animationsTest->setPosition(ccp(point.x+10,point.y+toPoint.y)); //this->getPosition().y+

	if(animationsTest  != NULL) {
		//this->addChild(animationsTest,102,9003);
		((UIlayer *)scene->getChildByTag(2))->addChild(animationsTest,102,9003);
	}

	if(point.x<size.width/2){
		CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		cocos2d::extension::CCBReader* teachR = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 

		CCNode  *animationsR  = teachR->readNodeGraphFromFile("teach/instruction_tianshenxiafan2_right.ccbi",this);//tank11.ccbi
		//�������� 
		animationsR->setPosition(ccp(0,0)); //-10,-this->getPosition().y

		if(animationsTest!= NULL) {
			((UIlayer *)scene->getChildByTag(2))->addChild(animationsR,101,9002);
		}

		isMoveView  = true;
		CCPoint  toPoint = getViewpointCenter(ccp(point.x+10, point.y));//�ƶ���Ļ

		CCActionInterval* move = CCMoveTo::create(1.0, toPoint);
		CCActionInterval* move_ease_in = CCEaseElasticOut::create (move, 2.0);

		this->runAction(CCSequence::create(move_ease_in,
			CCDelayTime::create(0.5f),
			NULL));

	}else{
		CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		cocos2d::extension::CCBReader* teachL = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 

		CCNode  *animationsL  = teachL->readNodeGraphFromFile("teach/instruction_tianshenxiafan2_left.ccbi",this);//tank11.ccbi
		//�������� 
		animationsL->setPosition(ccp(0,0)); //-10,-this->getPosition().y

		if(animationsTest  != NULL) {     
			((UIlayer *)scene->getChildByTag(2))->addChild(animationsL,101,9002);
		}
		isMoveView  = true;
		CCPoint  toPoint = getViewpointCenter(ccp(point.x+10, point.y));//�ƶ���Ļ

		CCActionInterval* move = CCMoveTo::create(1.0, toPoint);
		CCActionInterval* move_ease_in = CCEaseElasticOut::create (move, 2.0);

		this->runAction(CCSequence::create(move_ease_in,
			CCDelayTime::create(0.5f),
			NULL));

	}
	showProps3(point);
}
void GameScene::teach(){
	this->removeChildByTag(1);
	teachEnd = false;

	teachIndex = 1;

	CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	teachBreader = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
	//��ȡһ��ccbi���ļ�,����һ��ccnodeʵ��
	char tm[50];
	sprintf(tm, "teach/instruction_level%d.ccbi",mapIndex);
	CCNode  *animationsTest  = teachBreader->readNodeGraphFromFile(tm,this);//tank11.ccbi
	//�������� 
	//animationsTest->setPosition(ccp(pobSprite->getPositionX()-5,pobSprite->getPositionY()+60)); 

	if(animationsTest  != NULL) {     
		this->addChild(animationsTest,1,1);
	}
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	if(tileMap->getContentSize().width<size.width){
		animationsTest->setPositionX(-10);
	}
	teachBreader->getAnimationManager()->runAnimationsForSequenceNamed("1st");
	teachBreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(GameScene::goTeachEnd)); 
}
void GameScene::gameTeach3Over(){
	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	paused = false;
	this->resumeSchedulerAndActions();
	teachEnd = false;
	teachIndex = 0;
	this->setTouchEnabled(false);
	emergeTime.start();
	gameTime.start();

	isMoveView  = false;
}
void GameScene::battleStart()//��ʼս��
{
	this->removeChildByTag(1);
	limitCountTime.start();

	if(mapIndex==1){
		teachIndex = 9;
	}else{
		teachIndex = 4;
	}

	if(teachBreader!=NULL){
		teachBreader->autorelease();
	}
	CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	battlestart = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
	//��ȡһ��ccbi���ļ�,����һ��ccnodeʵ��
	char tm[50];
	sprintf(tm, "map/battlestart%d.ccbi",mapIndex); 
	CCNode  *animationsTest  = battlestart->readNodeGraphFromFile(tm,this);//tank11.ccbi
	//�������� 
	//animationsTest->setPosition(ccp(pobSprite->getPositionX()-5,pobSprite->getPositionY()+60)); 

	if(animationsTest  != NULL) {     
		this->addChild(animationsTest,1,1);
	}

	CCSize size = CCDirector::sharedDirector()->getWinSize();
	if(tileMap->getContentSize().width<size.width){
		animationsTest->setPositionX(-10);
	}
	if(sound){
		SimpleAudioEngine::sharedEngine()->playEffect("sound/28.ogg",false);
	}
	battlestart->getAnimationManager()->runAnimationsForSequenceNamed("1st");
	battlestart->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(GameScene::goTeachEnd)); 

}


void GameScene::pausedScene(){
	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	paused = true;
	
	emergeTime.pause();
	gameTime.pause();

}
void GameScene::startScene()//��ʼ
{
	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	paused = false;
	// this->resumeSchedulerAndActions();
	emergeTime.start();
	gameTime.start();
}


bool GameScene::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
	if(teachProps){
		CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
		CCLayer *ui = ((UIlayer *)scene->getChildByTag(2));

		if(ui->getChildByTag(789)){
			ui->removeChildByTag(789,-1);
		}

		paused = false;
		((GameScene *)scene->getChildByTag(1))->resumeSchedulerAndActions();
		ui->resumeSchedulerAndActions();

		gameTime.start();
		((CCMenu*)ui->getChildByTag(1))->setTouchEnabled(true);
		((ImmediateButton*)ui->getChildByTag(100))->setTouchEnabled(true);
		((ImmediateButton*)ui->getChildByTag(101))->setTouchEnabled(true);

		this->setTouchEnabled(false);
		teachProps = false;

		switch(teachPropsIndex){
		case 1:
			{
				if(playBody!=NULL){
					tanker *tank=  ((tanker*)playBody->GetUserData());
					tank->addFullLift();
				}
			}
			break;
		case 2:
			{
				bomOneEnemy();
			}
			break;
		case 3:
			{
				changeTank();
			}
			break;
		case 4:
			{
				if(playBody!=NULL){
					tanker *tank=  ((tanker*)playBody->GetUserData());
					tank->setGod();
				}
			}
			break;
		case 5:
			{
				if(playBody!=NULL){
					tanker *tank=  ((tanker*)playBody->GetUserData());
					tank->setMoveSpeed();
					tankSpeed();
				}
			}
			break;
		case 6:
			{
				tankBulletCrit();
			}
			break;
		case 7:
			{
				setEnemyTankStanding();
			}
			break;
		}

	}

	if(teachIndex==10&&playBody!=NULL){
		CCScene* scene = CCDirector::sharedDirector()->getRunningScene();

		((UIlayer *)scene->getChildByTag(2))->removeChildByTag(9002);
		((UIlayer *)scene->getChildByTag(2))->removeChildByTag(9003);

		isMoveView  = true;
		CCPoint  toPoint = getViewpointCenter(ccp(playBody->GetPosition().x*PTM_RATIO,playBody->GetPosition().y*PTM_RATIO));//�ƶ���Ļ

		CCActionInterval* move = CCMoveTo::create(1.0, toPoint);
		CCActionInterval* move_ease_in = CCEaseElasticOut::create (move, 2.0);
		this->runAction(CCSequence::create(move_ease_in,
			CCDelayTime::create(0.1f),
			CCCallFunc::create(this,callfunc_selector(GameScene::gameTeach3Over)),
			NULL));

		return true;
		//SaveBooleanToXML("teach1",true);
	}
	if(mapIndex==1){
		CCLog("================teachIndex:%d",teachIndex);
		if(teachEnd){
			if(teachIndex==1){
				//teachEnd = false;
				teachBreader->getAnimationManager()->runAnimationsForSequenceNamed("3rd");
				teachBreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(GameScene::goTeachEnd)); 
				teachIndex = 2;
			}else if(teachIndex==2){
				teachEnd = false;
				teachBreader->getAnimationManager()->runAnimationsForSequenceNamed("4th");
				teachBreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(GameScene::goTeachEnd)); 
				teachIndex = 3;
			}else if(teachIndex==3){
				teachEnd = false;
				teachBreader->getAnimationManager()->runAnimationsForSequenceNamed("5th");
				teachBreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(GameScene::goTeachEnd)); 
				//teachBreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(GameScene::battleStart)); 
				teachIndex = 4;
			}else if(teachIndex==4)
			{
				//teachEnd = false;
				teachBreader->getAnimationManager()->runAnimationsForSequenceNamed("7th");
				teachBreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(GameScene::goTeachEnd)); 

				teachIndex = 5;
			}else if(teachIndex==5){
				teachEnd = false;
				teachBreader->getAnimationManager()->runAnimationsForSequenceNamed("8th");
				teachBreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(GameScene::goTeachEnd)); 

				teachIndex = 6;
			} else if(teachIndex==6){
				teachEnd = false;
				teachBreader->getAnimationManager()->runAnimationsForSequenceNamed("9th");
				teachBreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(GameScene::goTeachEnd)); 

				teachIndex = 7;
			} else if(teachIndex==7){
				//teachEnd = false;
				teachBreader->getAnimationManager()->runAnimationsForSequenceNamed("10th");
				teachBreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(GameScene::goTeachEnd)); 

				teachIndex = 8;
			}else if(teachIndex==8){
				teachEnd = false;
				teachBreader->getAnimationManager()->runAnimationsForSequenceNamed("11th");
				teachBreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(GameScene::battleStart)); 

				//teachIndex = 9;
			}else if(teachIndex==9){
				teachEnd = false;
				this->setTouchEnabled(false);
				battlestart->getAnimationManager()->runAnimationsForSequenceNamed("3rd");
				//this->schedule(schedule_selector(GameScene::uiShow),2.5f);
				battlestart->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(GameScene::startGame)); 
			}
		}
	}else if(mapIndex==2){
		if(teachEnd){
			if(teachIndex==1){
				//teachEnd = false;
				teachBreader->getAnimationManager()->runAnimationsForSequenceNamed("3rd");
				teachBreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(GameScene::goTeachEnd)); 
				teachIndex = 2;
			}else if(teachIndex==2){
				teachEnd = false;
				teachBreader->getAnimationManager()->runAnimationsForSequenceNamed("4th");
				teachBreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(GameScene::battleStart)); 
			}

			if(teachIndex==4){
				teachEnd = false;
				battlestart->getAnimationManager()->runAnimationsForSequenceNamed("3rd");
				battlestart->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(GameScene::startGame)); 
				//this->schedule(schedule_selector(GameScene::uiShow),2.5f);
				this->setTouchEnabled(false);
			}
		}
		if(taechBom){//ը����ѧ
			if(teachBomIndex==0){
				bomReader->getAnimationManager()->runAnimationsForSequenceNamed("3rd");
				teachBomIndex = 1;

				CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
				CCLayer *ui = ((UIlayer *)scene->getChildByTag(2));

				ui->getChildByTag(101)->setVisible(true);
				if(ui->getChildByTag(101)){
					((ImmediateButton*)ui->getChildByTag(101))->setTouchEnabled(true);
				}
			}else if(teachBomIndex==1){
				CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
				CCLayer *ui = ((UIlayer *)scene->getChildByTag(2));

				CCPoint startLocation = pTouch->getLocationInView();
				startLocation =  CCDirector::sharedDirector()->convertToGL(startLocation);


				if(PointInRound(startLocation.x,startLocation.y, ui->getChildByTag(101)->getPositionX(), ui->getChildByTag(101)->getPositionY(),35.f)){
					bomReader->getAnimationManager()->runAnimationsForSequenceNamed("5th");
					teachBomIndex = 2;
					paused = false;
					((GameScene *)scene->getChildByTag(1))->resumeSchedulerAndActions();

					gameTime.start();
					((CCMenu*)ui->getChildByTag(1))->setTouchEnabled(true);
					((ImmediateButton*)ui->getChildByTag(100))->setTouchEnabled(true);
					if(ui->getChildByTag(102)){
						((ImmediateButton*)ui->getChildByTag(102))->setTouchEnabled(true);
					}
					bomReader->autorelease();

					this->setTouchEnabled(false);
					bomBeEnemy();
					ui->removeChildByTag(798,-1);

				}
			}

		}
	}else if(mapIndex==3){
		if(teachEnd){
			if(teachIndex==1){
				teachEnd = false;
				teachBreader->getAnimationManager()->runAnimationsForSequenceNamed("3rd");
				teachBreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(GameScene::goTeachEnd)); 
				teachIndex = 2;
			}else if(teachIndex==2){
				teachEnd = false;
				teachBreader->getAnimationManager()->runAnimationsForSequenceNamed("4th");
				teachBreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(GameScene::battleStart)); 
			}

			if(teachIndex==4){
				teachEnd = false;
				battlestart->getAnimationManager()->runAnimationsForSequenceNamed("3rd");
				battlestart->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(GameScene::startGame)); 
				//this->schedule(schedule_selector(GameScene::uiShow),2.5f);
				this->setTouchEnabled(false);
			}
		}
	}else{
		if(teachIndex==4){
			teachEnd = false;
			battlestart->getAnimationManager()->runAnimationsForSequenceNamed("3rd");
			battlestart->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(GameScene::startGame)); 
			//this->schedule(schedule_selector(GameScene::uiShow),2.5f);
			this->setTouchEnabled(false);
		}
	}

	return true;
}
void GameScene::startGame(){
	if(sound){
		SimpleAudioEngine::sharedEngine()->playEffect("sound/29.ogg",false);
	}
	this->removeChildByTag(1);

	CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	cocos2d::extension::CCBReader *startGameCCBR = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
	//��ȡһ��ccbi���ļ�,����һ��ccnodeʵ��

	CCNode  *animationsTest  = startGameCCBR->readNodeGraphFromFile("map/battlestartend.ccbi",this);//tank11.ccbi
	//�������� 
	//animationsTest->setPosition(ccp(pobSprite->getPositionX()-5,pobSprite->getPositionY()+60)); 

	if(animationsTest  != NULL) {     
		this->addChild(animationsTest,1,1);
	}

	this->schedule(schedule_selector(GameScene::uiShow),1.5f);
	teachIndex = 100;
}

void GameScene::goTeachEnd(){
	teachEnd = true;
}
GameScene::~GameScene(void){
	if(world){
		delete world;
		delete m_debugDraw;
		world = NULL;
	}
}
void GameScene::draw()
{
	// Default GL states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
	// Needed states:  GL_VERTEX_ARRAY, 
	// Unneeded states: GL_TEXTURE_2D, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
	/*glDisable(GL_TEXTURE_2D);  
	glDisableClientState(GL_COLOR_ARRAY);  
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);  

	world->DrawDebugData();  

	glEnable(GL_TEXTURE_2D);  
	glEnableClientState(GL_COLOR_ARRAY);  
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);  */

	/*if(drawWorldDebug){
	CCLayer::draw();       
	ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );    //    
	kmGLPushMatrix();       
	world->DrawDebugData();        
	kmGLPopMatrix();
	}*/
}

void GameScene::showAchieve(int index){
	CCSize size =CCDirector::sharedDirector()->getWinSize();
	char pszFileName[30];

	switch(index){
	case 1:
		{
			//CCLog("chassisId:%d",chassisId);
			//CCLog("turretId:%d",turretId);
			/*if (!LoadBooleanFromXML("showAchieve1")){
			UserRecord *achieveUR = new UserRecord("tank.db","achieveRecord");
			char taskname[30];
			sprintf(taskname,"achieveRecord%d", i);
			int achieveRecord=achieveUR->readIntFromSQLite(taskname);
			achieveUR->closeDB();
			free(achieveUR);

			XMLParser *pXmlParser = XMLParser::parseWithFile("achieve/achieve.xml");
			char taskname[30];
			sprintf(taskname,"task_%d", index+1);
			task= (pXmlParser->getString(taskname))->intValue();

			SaveBooleanToXML("showAchieve1", true); 
			}*/   


		}
		break;
	case 2:
		{
			if((chassisId!=1||turretId!=1)&&!LoadBooleanFromXML("showAchieve1")){
				sprintf(pszFileName,"achieve/acv_com%d.ccbi", index);
				CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
				cocos2d::extension::CCBReader *ccbreader = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
				ccbreader->autorelease();
				//��ȡһ��ccbi���ļ�,����һ��ccnodeʵ��
				CCNode  *animationsTest  = ccbreader->readNodeGraphFromFile(pszFileName,this);//tank11.ccbi
				//�������� 
				animationsTest->setPosition(ccp(size.width/2,size.height/2)); 

				//ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("GoUp");
				ccbreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(GameScene::showAchieveEnd)); 

				if(animationsTest!= NULL) {  
					CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
					((UIlayer *)scene->getChildByTag(2))->addChild(animationsTest,1,5000);
					//this->addChild(animationsTest,1,5000);
				}


				UserRecord *achieveUR = new UserRecord("tank.db","achieveRecord");
				char taskname[30];
				sprintf(taskname,"achieveRecord%d", index-1);
				//int achieveRecord=achieveUR->readIntFromSQLite(taskname);
				achieveUR->saveInt(taskname,1);
				achieveUR->closeDB();
				free(achieveUR);

				SaveBooleanToXML("showAchieve1", true); 


			}
		}
		break;
	case 3:
		{
			if(!LoadBooleanFromXML("showAchieve2")){
				sprintf(pszFileName,"achieve/acv_com%d.ccbi", index);
				CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
				cocos2d::extension::CCBReader *ccbreader = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
				ccbreader->autorelease();
				//��ȡһ��ccbi���ļ�,����һ��ccnodeʵ��
				CCNode  *animationsTest  = ccbreader->readNodeGraphFromFile(pszFileName,this);//tank11.ccbi
				//�������� 
				animationsTest->setPosition(ccp(size.width/2,size.height/2)); 

				//ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("GoUp");
				ccbreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(GameScene::showAchieveEnd)); 

				if(animationsTest!= NULL) {     
					// this->addChild(animationsTest,1,5000);
					CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
					((UIlayer *)scene->getChildByTag(2))->addChild(animationsTest,1,5000);
				}

				UserRecord *ur = new UserRecord("tank.db","tank");
				ur->saveInt("enemyNum",enemyNum);
				ur->closeDB();
				free(ur);

				UserRecord *achieveUR = new UserRecord("tank.db","achieveRecord");
				char taskname[30];
				sprintf(taskname,"achieveRecord%d", index-1);
				//int achieveRecord=achieveUR->readIntFromSQLite(taskname);
				achieveUR->saveInt(taskname,20);
				achieveUR->closeDB();
				free(achieveUR);

				SaveBooleanToXML("showAchieve2", true); 
			}
		}
		break;
	case 4:
		{
			if(!LoadBooleanFromXML("showAchieve3")){
				sprintf(pszFileName,"achieve/acv_com%d.ccbi", index);
				CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
				cocos2d::extension::CCBReader *ccbreader = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
				ccbreader->autorelease();
				//��ȡһ��ccbi���ļ�,����һ��ccnodeʵ��
				CCNode  *animationsTest  = ccbreader->readNodeGraphFromFile(pszFileName,this);//tank11.ccbi
				//�������� 
				animationsTest->setPosition(ccp(size.width/2,size.height/2)); 

				//ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("GoUp");
				ccbreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(GameScene::showAchieveEnd)); 

				if(animationsTest!= NULL) {     
					// this->addChild(animationsTest,1,5000);
					CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
					((UIlayer *)scene->getChildByTag(2))->addChild(animationsTest,1,5000);
				}

				UserRecord *ur = new UserRecord("tank.db","tank");
				ur->saveInt("enemyNum",enemyNum);
				ur->closeDB();
				free(ur);

				UserRecord *achieveUR = new UserRecord("tank.db","achieveRecord");
				char taskname[30];
				sprintf(taskname,"achieveRecord%d", index-1);
				//int achieveRecord=achieveUR->readIntFromSQLite(taskname);
				achieveUR->saveInt(taskname,100);
				achieveUR->closeDB();
				free(achieveUR);

				SaveBooleanToXML("showAchieve3", true); 
			}      
		}
		break;
	case 5:
		{
			if(!LoadBooleanFromXML("showAchieve4")){
				sprintf(pszFileName,"achieve/acv_com%d.ccbi", index);
				CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
				cocos2d::extension::CCBReader *ccbreader = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
				ccbreader->autorelease();
				//��ȡһ��ccbi���ļ�,����һ��ccnodeʵ��
				CCNode  *animationsTest  = ccbreader->readNodeGraphFromFile(pszFileName,this);//tank11.ccbi
				//�������� 
				animationsTest->setPosition(ccp(size.width/2,size.height/2)); 

				//ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("GoUp");
				ccbreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(GameScene::showAchieveEnd)); 

				if(animationsTest!= NULL) {     
					// this->addChild(animationsTest,1,5000);
					CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
					((UIlayer *)scene->getChildByTag(2))->addChild(animationsTest,1,5000);
				}


				UserRecord *ur = new UserRecord("tank.db","tank");
				ur->saveInt("enemyNum",enemyNum);
				ur->closeDB();
				free(ur);

				UserRecord *achieveUR = new UserRecord("tank.db","achieveRecord");
				char taskname[30];
				sprintf(taskname,"achieveRecord%d", index-1);
				//int achieveRecord=achieveUR->readIntFromSQLite(taskname);
				achieveUR->saveInt(taskname,1000);
				achieveUR->closeDB();
				free(achieveUR);

				SaveBooleanToXML("showAchieve4", true); 
			}
		}
		break;
	case 8:
		{
			if(!LoadBooleanFromXML("showAchieve7")){
				sprintf(pszFileName,"achieve/acv_com%d.ccbi", index);
				CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
				cocos2d::extension::CCBReader *ccbreader = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
				ccbreader->autorelease();
				//��ȡһ��ccbi���ļ�,����һ��ccnodeʵ��
				CCNode  *animationsTest  = ccbreader->readNodeGraphFromFile(pszFileName,this);//tank11.ccbi
				//�������� 
				animationsTest->setPosition(ccp(size.width/2,size.height/2)); 

				//ccbreader->getAnimationManager()->runAnimationsForSequenceNamed("GoUp");
				ccbreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(GameScene::showAchieveEnd)); 

				if(animationsTest!= NULL) {     
					// this->addChild(animationsTest,1,5000);
					CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
					((UIlayer *)scene->getChildByTag(2))->addChild(animationsTest,1,5000);
				}

				UserRecord *ur = new UserRecord("tank.db","tank");
				ur->saveInt("enemyNum",enemyNum);
				ur->closeDB();
				free(ur);

				UserRecord *achieveUR = new UserRecord("tank.db","achieveRecord");
				char taskname[30];
				sprintf(taskname,"achieveRecord%d", index-1);
				//int achieveRecord=achieveUR->readIntFromSQLite(taskname);
				achieveUR->saveInt(taskname,20);
				achieveUR->closeDB();
				free(achieveUR);

				SaveBooleanToXML("showAchieve7", true); 
			}
		}
		break;
	}
}

void GameScene::showAchieveEnd(){
	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	if(((UIlayer *)scene->getChildByTag(2))->getChildByTag(5000)){
		// ((UIlayer *)scene->getChildByTag(2))->addChild(animationsTest,1,5000);
		((UIlayer *)scene->getChildByTag(2))->removeChildByTag(5000);
	}
}


void GameScene::updateBossMap(){
	GB2ShapeCache::sharedGB2ShapeCache()->addShapesWithFile("image/box.plist");
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	int  mapWidthTileNum  = tileMap->getMapSize().width;//�� Tile����
	int  mapHeightTileNum = tileMap->getMapSize().height;///tile ������

	mapObstruct = new int[mapWidthTileNum*mapHeightTileNum];//��ͼ�赲���� 

	for(int i=0;i<mapWidthTileNum*mapHeightTileNum;i++){
		mapObstruct[i] = 0;
	}

	float  mapHeight = tileMap->getContentSize().height;//
	float  mapWidth = tileMap->getContentSize().width;//

	for(int i=0;i<mapHeightTileNum;i++){
		char layerName[30];
		sprintf(layerName,"block%d",  i+1);//
		CCTMXLayer    *layer = tileMap->layerNamed(layerName);

		for(int drawTileyY=0;drawTileyY<mapHeightTileNum;drawTileyY++)
		{  
			for(int drawTileyX=0;drawTileyX<mapWidthTileNum;drawTileyX++)
			{

				int gid = layer->tileGIDAt(ccp(drawTileyX,drawTileyY));

				switch(gid){
				case 4:
					{
						CCSprite*  sprite =layer->tileAt(ccp(drawTileyX,drawTileyY));
						CCPoint p = sprite->getPosition();

						Box *box = Box::spriteCreate(sprite->displayFrame());
						box->retain();
						box->setAnchorPoint(ccp(0.5,0.5));
						box->setPosition(p);
						box->setBoxPosLayer(ccp(drawTileyX,drawTileyY));

						//box->id = layer->tileGIDAt(ccp(drawTileyX,drawTileyY));
						box->setId(layer->tileGIDAt(ccp(drawTileyX,drawTileyY)));

						box->setName("floor_box_boss");
						box->setLayerName(layer->getLayerName());//����ͼ�������
						//box->lifeNum = 100;


						b2BodyDef bodyDef;
						bodyDef.type = b2_kinematicBody;

						bodyDef.position.Set((p.x+60)/PTM_RATIO, (p.y+60)/PTM_RATIO);
						bodyDef.userData = box;
						b2Body *body = world->CreateBody(&bodyDef);


						// add the fixture definitions to the body
						GB2ShapeCache *sc = GB2ShapeCache::sharedGB2ShapeCache();
						sc->addFixturesToBody(body,"floor_box_boss"); 
						box->setAnchorPoint(sc->anchorPointForShape("floor_box_boss"));

						mapObstruct[drawTileyY*mapWidthTileNum+drawTileyX] = 1; 
					}
					break;
				}
			}
		}

	}
}
/*
��ͼ��ʼ��
*/
void GameScene::updateMap(){
	GB2ShapeCache::sharedGB2ShapeCache()->addShapesWithFile("image/box.plist");
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	int  mapWidthTileNum  = tileMap->getMapSize().width;//�� Tile����
	int  mapHeightTileNum = tileMap->getMapSize().height;///tile �� ����

	mapObstruct = new int[mapWidthTileNum*mapHeightTileNum];//��ͼ�赲���� 

	for(int i=0;i<mapWidthTileNum*mapHeightTileNum;i++){
		mapObstruct[i] = 0;
	}

	float  mapHeight = tileMap->getContentSize().height;//
	float  mapWidth = tileMap->getContentSize().width;//

	for(int i=0;i<mapHeightTileNum+1;i++){
		char layerName[30];
		sprintf(layerName,"block%d",  i-1);//
		CCTMXLayer    *layer = tileMap->layerNamed(i==0?"river":layerName);

		//CCTMXLayer    *layer2 = tileMap->layerNamed("Layer2");
		//tileMap->setAnchorPoint(ccp(0,0));
		//int  tileWidth = tileMap->getTileSize().width;//tile  ��
		//int  tileHeight= tileMap->getTileSize().height;//tile ��

		for(int drawTileyY=0;drawTileyY<mapHeightTileNum;drawTileyY++)
		{  
			for(int drawTileyX=0;drawTileyX<mapWidthTileNum;drawTileyX++)
			{

				int gid = layer->tileGIDAt(ccp(drawTileyX,drawTileyY));
				//CCLog("gid:%d",gid);
				switch(gid){
				case 23://̹�� ,ˮ����ͨ��
				case 24:
				case 25:
				case 26:
				case 27:
				case 28:
				case 29:
				case 30:
				case 31:
				case 32:
				case 33:
				case 34:
				case 35:
				case 36:
				case 37:
				case 38:
				case 39:
				case 40:
					{
						CCSprite*  sprite =layer->tileAt(ccp(drawTileyX,drawTileyY));
						CCPoint p = sprite->getPosition();

						Box *box = Box::spriteCreate(sprite->displayFrame());
						box->retain();
						box->setAnchorPoint(ccp(0.5,0.5));
						box->setPosition(p);
						box->setBoxPosLayer(ccp(drawTileyX,drawTileyY));

						//box->id = layer->tileGIDAt(ccp(drawTileyX,drawTileyY));
						box->setId(layer->tileGIDAt(ccp(drawTileyX,drawTileyY)));

						box->setName("water");
						box->setLayerName(layer->getLayerName());//����ͼ�������
						//box->lifeNum = 100;


						b2BodyDef bodyDef;
						bodyDef.type = b2_kinematicBody;

						bodyDef.position.Set((p.x+32)/PTM_RATIO, (p.y+32)/PTM_RATIO);
						bodyDef.userData = box;
						b2Body *body = world->CreateBody(&bodyDef);


						// add the fixture definitions to the body
						GB2ShapeCache *sc = GB2ShapeCache::sharedGB2ShapeCache();
						sc->addFixturesToBody(body,"water"); 
						box->setAnchorPoint(sc->anchorPointForShape("water"));

						mapObstruct[drawTileyY*mapWidthTileNum+drawTileyX] = 1;
					}
					break;
				case sachet:
				case mound:
				case stone:
				case console:
				case commander:
				case radar:
				case skull:
				case stab:
				case signpost:
				case barrier:
				case metalBox:
				case zsignpost:
					{
						CCSprite*  sprite =layer->tileAt(ccp(drawTileyX,drawTileyY));//Box *box =dynamic_cast<Box *>(layer->tileAt(ccp(drawTileyX,drawTileyY)));//
						CCPoint p = sprite->getPosition();

						Box *box = Box::spriteCreate(sprite->displayFrame());
						box->retain();
						box->setAnchorPoint(ccp(0.5,0.5));
						box->setPosition(p);
						box->setBoxPosLayer(ccp(drawTileyX,drawTileyY));


						//box->id = layer->tileGIDAt(ccp(drawTileyX,drawTileyY));
						box->setId(layer->tileGIDAt(ccp(drawTileyX,drawTileyY)));

						box->setName("Box");
						box->setLayerName(layer->getLayerName());//����ͼ�������
						//box->lifeNum = 100;


						b2BodyDef bodyDef;
						bodyDef.type = b2_kinematicBody;

						bodyDef.position.Set((p.x+32)/PTM_RATIO, (p.y+32)/PTM_RATIO);
						bodyDef.userData = box;
						b2Body *body = world->CreateBody(&bodyDef);


						// add the fixture definitions to the body
						GB2ShapeCache *sc = GB2ShapeCache::sharedGB2ShapeCache();
						sc->addFixturesToBody(body,"box"); 
						box->setAnchorPoint(sc->anchorPointForShape("box"));

						//CCLog("=================(body->GetUserData())X:%d",(int)((Box*)(body->GetUserData()))->getBoxPosLayer().x);
						//CCLog("=================(body->GetUserData())Y:%d",(int)((Box*)(body->GetUserData()))->getBoxPosLayer().y);
						//CCLog("=================getName:%s",((Box*)(body->GetUserData()))->getName());
						//UserDataConsistencyCCSprite* xx=((UserDataConsistencyCCSprite*)(body->GetUserData()));
						//((CCSprite *)body->GetUserData())
						mapObstruct[drawTileyY*mapWidthTileNum+drawTileyX] = 1;

						/* for (int i=0;i<3;i++)//��
						{
						for(int j=0;j<3;j++){//��
						if((drawTileyX-1+j)>=0&&(drawTileyX-1+j)<tileMap->getMapSize().width
						&&(drawTileyY-1+i)>=0&&(drawTileyY-1+i)<tileMap->getMapSize().height){

						CCLog("pos.x-1+j :%f   pos.y-1+i:%f",drawTileyX-1+j,drawTileyY-1+i); 
						int gid = layer->tileGIDAt(ccp(drawTileyX-1+j,drawTileyY-1+i));
						if(gid==ourBase){

						}
						}
						}
						}*/


					}
					break;
				case oilkeg://��Ͱ
					{
						CCSprite*  sprite =layer->tileAt(ccp(drawTileyX,drawTileyY));//Box *box =dynamic_cast<Box *>(layer->tileAt(ccp(drawTileyX,drawTileyY)));//
						CCPoint p = sprite->getPosition();

						Box *box = Box::spriteCreate(sprite->displayFrame());
						box->retain();
						box->setAnchorPoint(ccp(0.5,0.5));
						box->setPosition(p);
						box->setBoxPosLayer(ccp(drawTileyX,drawTileyY));

						//box->id = layer->tileGIDAt(ccp(drawTileyX,drawTileyY));
						box->setId(layer->tileGIDAt(ccp(drawTileyX,drawTileyY)));

						box->setName("oilkeg");
						box->setLayerName(layer->getLayerName());//����ͼ�������
						//box->lifeNum = 100;


						b2BodyDef bodyDef;
						bodyDef.type = b2_kinematicBody;

						bodyDef.position.Set((p.x+32)/PTM_RATIO, (p.y+32)/PTM_RATIO);
						bodyDef.userData = box;
						b2Body *body = world->CreateBody(&bodyDef);


						// add the fixture definitions to the body
						GB2ShapeCache *sc = GB2ShapeCache::sharedGB2ShapeCache();
						sc->addFixturesToBody(body,"oilkeg"); 
						box->setAnchorPoint(sc->anchorPointForShape("oilkeg"));

						mapObstruct[drawTileyY*mapWidthTileNum+drawTileyX] = 1;
					}
					break;
				case stakes://ľ׮
					{
						CCSprite*  sprite =layer->tileAt(ccp(drawTileyX,drawTileyY));//Box *box =dynamic_cast<Box *>(layer->tileAt(ccp(drawTileyX,drawTileyY)));//
						CCPoint p = sprite->getPosition();

						Box *box = Box::spriteCreate(sprite->displayFrame());
						box->retain();
						box->setAnchorPoint(ccp(0.5,0.5));
						box->setPosition(p);
						box->setBoxPosLayer(ccp(drawTileyX,drawTileyY));

						//box->id = layer->tileGIDAt(ccp(drawTileyX,drawTileyY));
						box->setId(layer->tileGIDAt(ccp(drawTileyX,drawTileyY)));

						box->setName("stakes");
						box->setLayerName(layer->getLayerName());//����ͼ�������
						//box->lifeNum = 100;


						b2BodyDef bodyDef;
						bodyDef.type = b2_kinematicBody;

						bodyDef.position.Set((p.x+32)/PTM_RATIO, (p.y+32)/PTM_RATIO);
						bodyDef.userData = box;
						b2Body *body = world->CreateBody(&bodyDef);


						// add the fixture definitions to the body
						GB2ShapeCache *sc = GB2ShapeCache::sharedGB2ShapeCache();
						sc->addFixturesToBody(body,"stakes"); 
						box->setAnchorPoint(sc->anchorPointForShape("stakes"));

						mapObstruct[drawTileyY*mapWidthTileNum+drawTileyX] = 1;
						if(drawTileyY!=0){
							mapObstruct[(drawTileyY-1)*mapWidthTileNum+drawTileyX] = 1;
						}

					}
					break;
				case hstakes://��ľ׮
					{
						CCSprite*  sprite =layer->tileAt(ccp(drawTileyX,drawTileyY));//Box *box =dynamic_cast<Box *>(layer->tileAt(ccp(drawTileyX,drawTileyY)));//
						CCPoint p = sprite->getPosition();

						Box *box = Box::spriteCreate(sprite->displayFrame());
						box->retain();
						box->setAnchorPoint(ccp(0.5,0.5));
						box->setPosition(p);
						box->setBoxPosLayer(ccp(drawTileyX,drawTileyY));

						//box->id = layer->tileGIDAt(ccp(drawTileyX,drawTileyY));
						box->setId(layer->tileGIDAt(ccp(drawTileyX,drawTileyY)));

						box->setName("hstakes");
						box->setLayerName(layer->getLayerName());//����ͼ�������
						//box->lifeNum = 100;


						b2BodyDef bodyDef;
						bodyDef.type = b2_kinematicBody;

						bodyDef.position.Set((p.x+32)/PTM_RATIO, (p.y+32)/PTM_RATIO);
						bodyDef.userData = box;
						b2Body *body = world->CreateBody(&bodyDef);


						// add the fixture definitions to the body
						GB2ShapeCache *sc = GB2ShapeCache::sharedGB2ShapeCache();
						sc->addFixturesToBody(body,"hstakes"); 
						box->setAnchorPoint(sc->anchorPointForShape("hstakes"));

						mapObstruct[drawTileyY*mapWidthTileNum+drawTileyX] = 1;
						if(drawTileyX<mapWidthTileNum){
							mapObstruct[drawTileyY*mapWidthTileNum+drawTileyX+1] = 1;
						}
					}
					break;
				case withered:
					{
						CCSprite*  sprite =layer->tileAt(ccp(drawTileyX,drawTileyY));//Box *box =dynamic_cast<Box *>(layer->tileAt(ccp(drawTileyX,drawTileyY)));//
						CCPoint p = sprite->getPosition();

						Box *box = Box::spriteCreate(sprite->displayFrame());
						box->retain();
						box->setAnchorPoint(ccp(0.5,0.5));
						box->setPosition(p);
						box->setBoxPosLayer(ccp(drawTileyX,drawTileyY));

						//box->id = layer->tileGIDAt(ccp(drawTileyX,drawTileyY));
						box->setId(layer->tileGIDAt(ccp(drawTileyX,drawTileyY)));

						box->setName("withered");
						box->setLayerName(layer->getLayerName());//����ͼ�������
						//box->lifeNum = 100;


						b2BodyDef bodyDef;
						bodyDef.type = b2_kinematicBody;

						bodyDef.position.Set((p.x+32)/PTM_RATIO, (p.y+32)/PTM_RATIO);
						bodyDef.userData = box;
						b2Body *body = world->CreateBody(&bodyDef);


						// add the fixture definitions to the body
						GB2ShapeCache *sc = GB2ShapeCache::sharedGB2ShapeCache();
						sc->addFixturesToBody(body,"withered"); 
						box->setAnchorPoint(sc->anchorPointForShape("withered"));

						mapObstruct[drawTileyY*mapWidthTileNum+drawTileyX] = 1;
						if(drawTileyX<mapWidthTileNum){
							mapObstruct[drawTileyY*mapWidthTileNum+drawTileyX+1] = 1;
						}

					}
					break;
				case generator:
					{
						CCSprite*  sprite =layer->tileAt(ccp(drawTileyX,drawTileyY));//Box *box =dynamic_cast<Box *>(layer->tileAt(ccp(drawTileyX,drawTileyY)));//
						CCPoint p = sprite->getPosition();

						Box *box = Box::spriteCreate(sprite->displayFrame());
						box->retain();
						box->setAnchorPoint(ccp(0.5,0.5));
						box->setPosition(p);
						box->setBoxPosLayer(ccp(drawTileyX,drawTileyY));

						//box->id = layer->tileGIDAt(ccp(drawTileyX,drawTileyY));
						box->setId(layer->tileGIDAt(ccp(drawTileyX,drawTileyY)));

						box->setName("generator");
						box->setLayerName(layer->getLayerName());//����ͼ�������
						//box->lifeNum = 100;


						b2BodyDef bodyDef;
						bodyDef.type = b2_kinematicBody;

						bodyDef.position.Set((p.x+32)/PTM_RATIO, (p.y+32)/PTM_RATIO);
						bodyDef.userData = box;
						b2Body *body = world->CreateBody(&bodyDef);


						// add the fixture definitions to the body
						GB2ShapeCache *sc = GB2ShapeCache::sharedGB2ShapeCache();
						sc->addFixturesToBody(body,"generator"); 
						box->setAnchorPoint(sc->anchorPointForShape("generator"));

						mapObstruct[drawTileyY*mapWidthTileNum+drawTileyX] = 1;
						if(drawTileyX<mapWidthTileNum){
							mapObstruct[drawTileyY*mapWidthTileNum+drawTileyX+1] = 1;
						}
					}
					break;
				case tree:
					{
						CCSprite*  sprite =layer->tileAt(ccp(drawTileyX,drawTileyY));//Box *box =dynamic_cast<Box *>(layer->tileAt(ccp(drawTileyX,drawTileyY)));//
						CCPoint p = sprite->getPosition();

						Box *box = Box::spriteCreate(sprite->displayFrame());
						box->retain();
						box->setAnchorPoint(ccp(0.5,0.5));
						box->setPosition(p);
						box->setBoxPosLayer(ccp(drawTileyX,drawTileyY));

						//box->id = layer->tileGIDAt(ccp(drawTileyX,drawTileyY));
						box->setId(layer->tileGIDAt(ccp(drawTileyX,drawTileyY)));

						box->setName("tree");
						box->setLayerName(layer->getLayerName());//����ͼ�������
						//box->lifeNum = 100;


						b2BodyDef bodyDef;
						bodyDef.type = b2_kinematicBody;

						bodyDef.position.Set((p.x+32)/PTM_RATIO, (p.y+32)/PTM_RATIO);
						bodyDef.userData = box;
						b2Body *body = world->CreateBody(&bodyDef);


						// add the fixture definitions to the body
						GB2ShapeCache *sc = GB2ShapeCache::sharedGB2ShapeCache();
						sc->addFixturesToBody(body,"tree"); 
						box->setAnchorPoint(sc->anchorPointForShape("tree"));  

						mapObstruct[drawTileyY*mapWidthTileNum+drawTileyX] = 1;
						if(drawTileyX<mapWidthTileNum){
							mapObstruct[drawTileyY*mapWidthTileNum+drawTileyX+1] = 1;
						}
					}
					break;
				case enemyBase:
					{
						CCSprite*  sprite =layer->tileAt(ccp(drawTileyX,drawTileyY));//Box *box =dynamic_cast<Box *>(layer->tileAt(ccp(drawTileyX,drawTileyY)));//
						CCPoint p = sprite->getPosition();

						Box *box = Box::spriteCreate(sprite->displayFrame());
						box->retain();
						box->setAnchorPoint(ccp(0.5,0.5));
						box->setPosition(p);
						box->setBoxPosLayer(ccp(drawTileyX,drawTileyY));

						//box->id = layer->tileGIDAt(ccp(drawTileyX,drawTileyY));
						box->setId(layer->tileGIDAt(ccp(drawTileyX,drawTileyY)));

						box->setName("enemyBase");
						box->setLayerName(layer->getLayerName());//����ͼ�������
						//box->lifeNum = 100;


						b2BodyDef bodyDef;
						bodyDef.type = b2_kinematicBody;

						bodyDef.position.Set((p.x+32)/PTM_RATIO, (p.y+32)/PTM_RATIO);
						bodyDef.userData = box;
						b2Body *body = world->CreateBody(&bodyDef);


						// add the fixture definitions to the body
						GB2ShapeCache *sc = GB2ShapeCache::sharedGB2ShapeCache();
						sc->addFixturesToBody(body,"box"); 
						box->setAnchorPoint(sc->anchorPointForShape("box"));

						enemyBaseInfo.layerName=layer->getLayerName();
						enemyBaseInfo.pointBase = ccp(drawTileyX,drawTileyY);
						enemyBaseInfo.show = true;
						mapObstruct[drawTileyY*mapWidthTileNum+drawTileyX] = 1;
					}
					break;
				case ourBase:
					{
						CCSprite*  sprite =layer->tileAt(ccp(drawTileyX,drawTileyY));//Box *box =dynamic_cast<Box *>(layer->tileAt(ccp(drawTileyX,drawTileyY)));//
						CCPoint p = sprite->getPosition();

						Box *box = Box::spriteCreate(sprite->displayFrame());
						box->retain();
						box->setAnchorPoint(ccp(0.5,0.5));
						box->setPosition(p);
						box->setBoxPosLayer(ccp(drawTileyX,drawTileyY));

						//box->id = layer->tileGIDAt(ccp(drawTileyX,drawTileyY));
						box->setId(layer->tileGIDAt(ccp(drawTileyX,drawTileyY)));

						box->setName("ourBase");
						box->setLayerName(layer->getLayerName());//����ͼ�������
						//box->lifeNum = 100;


						b2BodyDef bodyDef;
						bodyDef.type = b2_kinematicBody;

						bodyDef.position.Set((p.x+32)/PTM_RATIO, (p.y+32)/PTM_RATIO);
						bodyDef.userData = box;
						b2Body *body = world->CreateBody(&bodyDef);


						// add the fixture definitions to the body
						GB2ShapeCache *sc = GB2ShapeCache::sharedGB2ShapeCache();
						sc->addFixturesToBody(body,"box"); 
						box->setAnchorPoint(sc->anchorPointForShape("box"));

						ourBaseInfo.layerName=layer->getLayerName();
						ourBaseInfo.pointBase = ccp(drawTileyX,drawTileyY);
						ourBaseInfo.show = true;
						mapObstruct[drawTileyY*mapWidthTileNum+drawTileyX] = 1;



					}
					break;
				case fountain:
					{
						CCSprite*  sprite =layer->tileAt(ccp(drawTileyX,drawTileyY));//Box *box =dynamic_cast<Box *>(layer->tileAt(ccp(drawTileyX,drawTileyY)));//
						CCPoint p = sprite->getPosition();

						Box *box = Box::spriteCreate(sprite->displayFrame());
						box->retain();
						box->setAnchorPoint(ccp(0.5,0.5));
						box->setPosition(p);
						box->setBoxPosLayer(ccp(drawTileyX,drawTileyY));

						//box->id = layer->tileGIDAt(ccp(drawTileyX,drawTileyY));
						box->setId(layer->tileGIDAt(ccp(drawTileyX,drawTileyY)));

						box->setName("fountain");
						box->setLayerName(layer->getLayerName());//����ͼ�������
						//box->lifeNum = 100;


						b2BodyDef bodyDef;
						bodyDef.type = b2_kinematicBody;

						bodyDef.position.Set((p.x+32)/PTM_RATIO, (p.y+32)/PTM_RATIO);
						bodyDef.userData = box;
						b2Body *body = world->CreateBody(&bodyDef);


						// add the fixture definitions to the body
						GB2ShapeCache *sc = GB2ShapeCache::sharedGB2ShapeCache();
						sc->addFixturesToBody(body,"fountain"); 
						box->setAnchorPoint(sc->anchorPointForShape("fountain"));

						mapObstruct[drawTileyY*mapWidthTileNum+drawTileyX] = 1;
						if(drawTileyX<mapWidthTileNum){
							mapObstruct[drawTileyY*mapWidthTileNum+drawTileyX+1] = 1;
						}
					}
					break;
				}
			}
		}
	}


	/* CCPoint pos = ourBaseInfo.pointBase;
	//int num =returnInt(ourBaseInfo.layerName);
	CCLog("ourBaseInfo.layerName :%s   num:%d",ourBaseInfo.layerName,returnInt(ourBaseInfo.layerName));
	CCLog("pos.x :%f   pos.y:%f",pos.x,pos.y);

	for (int i=0;i<3;i++)//��
	{
	for(int j=0;j<3;j++){//��
	if((pos.x-1+j)>=0&&(pos.x-1+j)<tileMap->getMapSize().width
	&&(pos.y-1+i)>=0&&(pos.y-1+i)<tileMap->getMapSize().height){

	CCLog("pos.x-1+j :%f   pos.y-1+i:%f",pos.x-1+j,pos.y-1+i); 

	char lable[20];
	sprintf(lable, "block%d",(int)(pos.y-1+i));
	CCTMXLayer    *layer = tileMap->layerNamed(lable);

	if(!((pos.x-1+j)== pos.x&&(pos.y-1+i)==pos.y)){

	}
	}
	}
	}*/

}
//�������̹���ٶ�
void GameScene::setPlayerSpeed(float x,float y){
	if(playBody!=NULL){
		playBody->SetLinearVelocity(b2Vec2(x/PTM_RATIO,y/PTM_RATIO));
	}
}

//��������������ʼ�ص���ʱ��ᷢ�����¼�
void GameScene:: BeginContact(b2Contact*  contact) {
	b2Body*  bodyA = contact->GetFixtureA()->GetBody(); 
	b2Body*  bodyB = contact->GetFixtureB()->GetBody();

	if(contact->GetFixtureA()->GetFilterData().categoryBits!=0x0001&&contact->GetFixtureB()->GetFilterData().categoryBits!=0x0001){
		//����ӵ���Box
		if((((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="eleball"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="Box")
			||(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="Box"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="eleball")
			||(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="floor_box_boss"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="eleball")
			||(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="eleball"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="floor_box_boss")){

				if(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="eleball"){
					Bullet *bullet =  ((Bullet*)bodyA->GetUserData());
					bullet->setState(Bullet::EXPLODE);
					bodyA->SetLinearVelocity(b2Vec2(0,0));
					Box *box =  ((Box*)bodyB->GetUserData());

					if(bullet->getBullStrenght()>=box->getStrength()){//ǿ�ȱȽ�
						box->beAttack(bullet->getBulletAttack(),this,callfuncN_selector(GameScene::setBoxTileCallBack));
					}else{
						if(sound){
							SimpleAudioEngine::sharedEngine()->playEffect("sound/32.ogg",false);
						}
					}
				}else{
					Bullet *bullet =  ((Bullet*)bodyB->GetUserData());
					bullet->setState(Bullet::EXPLODE);
					bodyB->SetLinearVelocity(b2Vec2(0,0));

					Box *box =  ((Box*)bodyA->GetUserData());
					if(bullet->getBullStrenght()>=box->getStrength()){//ǿ�ȱȽ�
						box->beAttack(bullet->getBulletAttack(),this,callfuncN_selector(GameScene::setBoxTileCallBack));
					}else{
						if(sound){
							SimpleAudioEngine::sharedEngine()->playEffect("sound/32.ogg",false);
						}
					}
				}
				//����ӵ���oilkeg
		}else if((((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="eleball"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="oilkeg")
			||(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="oilkeg"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="eleball")){

				if(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="eleball"){
					Bullet *bullet =  ((Bullet*)bodyA->GetUserData());
					bullet->setState(Bullet::EXPLODE);
					bodyA->SetLinearVelocity(b2Vec2(0,0));

					Box *box =  ((Box*)bodyB->GetUserData());

					if(bullet->getBullStrenght()>=box->getStrength()){//ǿ�ȱȽ�
						box->beAttack(bullet->getBulletAttack(),this,callfuncN_selector(GameScene::setBoxTileCallBack));
					}else{
						if(sound){
							SimpleAudioEngine::sharedEngine()->playEffect("sound/32.ogg",false);
						}
					}

				}else{
					Bullet *bullet =  ((Bullet*)bodyB->GetUserData());
					bullet->setState(Bullet::EXPLODE);
					bodyB->SetLinearVelocity(b2Vec2(0,0));

					Box *box =  ((Box*)bodyA->GetUserData());

					if(bullet->getBullStrenght()>=box->getStrength()){//ǿ�ȱȽ�
						box->beAttack(bullet->getBulletAttack(),this,callfuncN_selector(GameScene::setBoxTileCallBack));
					}else{
						if(sound){
							SimpleAudioEngine::sharedEngine()->playEffect("sound/32.ogg",false);
						}
					}
				}
				//����ӵ���stakes
		}else if((((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="eleball"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="stakes")
			||(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="stakes"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="eleball")){

				if(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="eleball"){
					Bullet *bullet =  ((Bullet*)bodyA->GetUserData());
					bullet->setState(Bullet::EXPLODE);
					bodyA->SetLinearVelocity(b2Vec2(0,0));

					Box *box =  ((Box*)bodyB->GetUserData());

					if(bullet->getBullStrenght()>=box->getStrength()){//ǿ�ȱȽ�
						box->beAttack(bullet->getBulletAttack(),this,callfuncN_selector(GameScene::setBoxTileCallBack));
					}else{
						if(sound){
							SimpleAudioEngine::sharedEngine()->playEffect("sound/32.ogg",false);
						}
					}

				}else{
					Bullet *bullet =  ((Bullet*)bodyB->GetUserData());
					bullet->setState(Bullet::EXPLODE);
					bodyB->SetLinearVelocity(b2Vec2(0,0));

					Box *box =  ((Box*)bodyA->GetUserData());

					if(bullet->getBullStrenght()>=box->getStrength()){//ǿ�ȱȽ�
						box->beAttack(bullet->getBulletAttack(),this,callfuncN_selector(GameScene::setBoxTileCallBack));
					}else{
						if(sound){
							SimpleAudioEngine::sharedEngine()->playEffect("sound/32.ogg",false);
						}
					}
				}
				//����ӵ���withered,generator,hstakes,tree,fountain
		}else if((((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="eleball"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="withered")
			||(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="withered"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="eleball")
			||(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="eleball"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="generator")
			||(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="generator"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="eleball")
			||(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="eleball"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="hstakes")
			||(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="hstakes"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="eleball")
			||(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="eleball"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="tree")
			||(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="tree"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="eleball")
			||(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="eleball"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="fountain")
			||(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="fountain"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="eleball")) {

				if(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="eleball"){
					Bullet *bullet =  ((Bullet*)bodyA->GetUserData());
					bullet->setState(Bullet::EXPLODE);
					bodyA->SetLinearVelocity(b2Vec2(0,0));

					Box *box =  ((Box*)bodyB->GetUserData());

					if(bullet->getBullStrenght()>=box->getStrength()){//ǿ�ȱȽ�
						box->beAttack(bullet->getBulletAttack(),this,callfuncN_selector(GameScene::setBoxTileCallBack));
					}else{
						if(sound){
							SimpleAudioEngine::sharedEngine()->playEffect("sound/32.ogg",false);
						}
					}

				}else{
					Bullet *bullet =  ((Bullet*)bodyB->GetUserData());
					bullet->setState(Bullet::EXPLODE);
					bodyB->SetLinearVelocity(b2Vec2(0,0));

					Box *box =  ((Box*)bodyA->GetUserData());

					if(bullet->getBullStrenght()>=box->getStrength()){//ǿ�ȱȽ�
						box->beAttack(bullet->getBulletAttack(),this,callfuncN_selector(GameScene::setBoxTileCallBack));
					}else{
						if(sound){
							SimpleAudioEngine::sharedEngine()->playEffect("sound/32.ogg",false);
						}
					}
				}
				//����ӵ��������ײ
		}else if((((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="eleball"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="enemy")
			||(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="enemy"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="eleball")){

				if(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="eleball"){
					Bullet *bullet =  ((Bullet*)bodyA->GetUserData());
					bullet->setState(Bullet::EXPLODE);
					bodyA->SetLinearVelocity(b2Vec2(0,0));

					enemyTank *enemy=  ((enemyTank*)bodyB->GetUserData());
					bodyB->SetLinearVelocity(b2Vec2(0,0));
					int attack = bullet->getBulletAttack();

					enemy->beAttack(attack,bullet->getCritRateBool());//bullet->getBulletAttack()

					if(enemy->getLifeNum()<=0){
						bodyB->SetLinearVelocity(b2Vec2(0,0));//�����ٶ�Ϊ0
						((enemyTank*)bodyB->GetUserData())->setShowStand(true);
						if(((enemyTank*)bodyB->GetUserData())->getShow4()){
							showCoins(enemy->getPosition());
						}

						if(sound){
							SimpleAudioEngine::sharedEngine()->playEffect("sound/30.ogg",false);
						}
					}
				}else{
					Bullet *bullet =  ((Bullet*)bodyB->GetUserData());
					bullet->setState(Bullet::EXPLODE);
					bodyB->SetLinearVelocity(b2Vec2(0,0));

					enemyTank *enemy =  ((enemyTank*)bodyA->GetUserData());
					int attack = bullet->getBulletAttack();

					enemy->beAttack(attack,bullet->getCritRateBool());
					if(enemy->getLifeNum()<=0){
						bodyA->SetLinearVelocity(b2Vec2(0,0));//�����ٶ�Ϊ0
						((enemyTank*)bodyA->GetUserData())->setShowStand(true);
						if(((enemyTank*)bodyB->GetUserData())->getShow4()){
							showCoins(enemy->getPosition());
						}
						if(sound){
							SimpleAudioEngine::sharedEngine()->playEffect("sound/30.ogg",false);
						}
					}
				}
				//����ӵ�����˻���
		}else if((((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="eleball"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="enemyBase")
			||(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="enemyBase"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="eleball")){

				if(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="eleball"){
					Bullet *bullet =  ((Bullet*)bodyA->GetUserData());
					bullet->setState(Bullet::EXPLODE);
					bodyA->SetLinearVelocity(b2Vec2(0,0));

					Box *box =  ((Box*)bodyB->GetUserData());
					box->beAttack(1000,this,callfuncN_selector(GameScene::setBoxTileCallBack));
					//ʤ��
					CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
					((UIlayer *)scene->getChildByTag(2))->successMenu();

				}else{
					Bullet *bullet =  ((Bullet*)bodyB->GetUserData());
					bullet->setState(Bullet::EXPLODE);
					bodyB->SetLinearVelocity(b2Vec2(0,0));

					Box *box =  ((Box*)bodyA->GetUserData());
					box->beAttack(1000,this,callfuncN_selector(GameScene::setBoxTileCallBack));
					//ʤ��
					CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
					((UIlayer *)scene->getChildByTag(2))->successMenu();

				}
				//����ӵ�����һ���
		}else if((((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="eleball"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="ourBase")
			||(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="ourBase"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="eleball")){
				if(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="eleball"){
					Bullet *bullet =  ((Bullet*)bodyA->GetUserData());
					bullet->setState(Bullet::EXPLODE);
					bodyA->SetLinearVelocity(b2Vec2(0,0));
				}else{
					Bullet *bullet =  ((Bullet*)bodyB->GetUserData());
					bullet->setState(Bullet::EXPLODE);
					bodyB->SetLinearVelocity(b2Vec2(0,0));
				}
		}

		//�����ӵ���Box floor_box_boss
		if((((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="enemybullt"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="Box")
			||(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="Box"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="enemybullt")
			||(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="floor_box_boss"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="enemybullt")
			||(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="enemybullt"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="floor_box_boss")){

				if(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="enemybullt"){
					enemyBullet *bullet =  ((enemyBullet*)bodyA->GetUserData());
					bullet->setState(enemyBullet::EXPLODE);
					bodyA->SetLinearVelocity(b2Vec2(0,0));

					Box *box =  ((Box*)bodyB->GetUserData());

					//CCLog("bullet->getBullStrenght():%d   ,box->getStrength():%d",bullet->getBullStrenght(),box->getStrength());

					if(bullet->getBullStrenght()>=box->getStrength()){//ǿ�ȱȽ�
						box->beAttack(bullet->getBulletAttack(),this,callfuncN_selector(GameScene::setBoxTileCallBack));
					}

				}else{
					enemyBullet *bullet =  ((enemyBullet*)bodyB->GetUserData());
					bullet->setState(enemyBullet::EXPLODE);
					bodyB->SetLinearVelocity(b2Vec2(0,0));

					Box *box =  ((Box*)bodyA->GetUserData());
					//CCLog("bullet->getBullStrenght():%d   ,box->getStrength():%d",bullet->getBullStrenght(),box->getStrength());

					if(bullet->getBullStrenght()>=box->getStrength()){//ǿ�ȱȽ�
						box->beAttack(bullet->getBulletAttack(),this,callfuncN_selector(GameScene::setBoxTileCallBack));
					}
				}
				//�����ӵ���oilkeg
		}else if((((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="enemybullt"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="oilkeg")
			||(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="oilkeg"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="enemybullt")){

				if(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="enemybullt"){
					enemyBullet *bullet =  ((enemyBullet*)bodyA->GetUserData());
					bullet->setState(enemyBullet::EXPLODE);
					bodyA->SetLinearVelocity(b2Vec2(0,0));

					Box *box =  ((Box*)bodyB->GetUserData());
					if(bullet->getBullStrenght()>=box->getStrength()){//ǿ�ȱȽ�
						box->beAttack(bullet->getBulletAttack(),this,callfuncN_selector(GameScene::setBoxTileCallBack));
					}else{
						if(sound){
							SimpleAudioEngine::sharedEngine()->playEffect("sound/32.ogg",false);
						}
					}

				}else{
					enemyBullet *bullet =  ((enemyBullet*)bodyB->GetUserData());
					bullet->setState(enemyBullet::EXPLODE);
					bodyB->SetLinearVelocity(b2Vec2(0,0));

					Box *box =  ((Box*)bodyA->GetUserData());
					if(bullet->getBullStrenght()>=box->getStrength()){//ǿ�ȱȽ�
						box->beAttack(bullet->getBulletAttack(),this,callfuncN_selector(GameScene::setBoxTileCallBack));
					}
				}
				//�����ӵ���stakes
		}else if((((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="enemybullt"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="stakes")
			||(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="stakes"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="enemybullt")){

				if(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="enemybullt"){
					enemyBullet *bullet =  ((enemyBullet*)bodyA->GetUserData());
					bullet->setState(enemyBullet::EXPLODE);
					bodyA->SetLinearVelocity(b2Vec2(0,0));

					Box *box =  ((Box*)bodyB->GetUserData());
					if(bullet->getBullStrenght()>=box->getStrength()){//ǿ�ȱȽ�
						box->beAttack(bullet->getBulletAttack(),this,callfuncN_selector(GameScene::setBoxTileCallBack));
					}

				}else{
					enemyBullet *bullet =  ((enemyBullet*)bodyB->GetUserData());
					bullet->setState(enemyBullet::EXPLODE);
					bodyB->SetLinearVelocity(b2Vec2(0,0));

					Box *box =  ((Box*)bodyA->GetUserData());
					if(bullet->getBullStrenght()>=box->getStrength()){//ǿ�ȱȽ�
						box->beAttack(bullet->getBulletAttack(),this,callfuncN_selector(GameScene::setBoxTileCallBack));
					}
				}
				//�����ӵ���withered,generator,hstakes,tree,fountain
		}else if((((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="enemybullt"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="withered")
			||(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="withered"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="enemybullt")
			||(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="enemybullt"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="generator")
			||(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="generator"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="enemybullt")
			||(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="enemybullt"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="hstakes")
			||(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="hstakes"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="enemybullt")
			||(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="enemybullt"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="tree")
			||(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="tree"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="enemybullt")
			||(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="enemybullt"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="fountain")
			||(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="fountain"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="enemybullt")) {

				if(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="enemybullt"){
					enemyBullet *bullet =  ((enemyBullet*)bodyA->GetUserData());
					bullet->setState(enemyBullet::EXPLODE);
					bodyA->SetLinearVelocity(b2Vec2(0,0));

					Box *box =  ((Box*)bodyB->GetUserData());
					if(bullet->getBullStrenght()>=box->getStrength()){//ǿ�ȱȽ�
						box->beAttack(bullet->getBulletAttack(),this,callfuncN_selector(GameScene::setBoxTileCallBack));
					}

				}else{
					enemyBullet *bullet =  ((enemyBullet*)bodyB->GetUserData());
					bullet->setState(enemyBullet::EXPLODE);
					bodyB->SetLinearVelocity(b2Vec2(0,0));

					Box *box =  ((Box*)bodyA->GetUserData());
					if(bullet->getBullStrenght()>=box->getStrength()){//ǿ�ȱȽ�
						box->beAttack(bullet->getBulletAttack(),this,callfuncN_selector(GameScene::setBoxTileCallBack));
					}
				}
		  //�����ӵ������̹����ײ
		}else if((((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="enemybullt"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="tank")
			||(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="tank"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="enemybullt")){
				
				if(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="enemybullt"){
					enemyBullet *bullet =  ((enemyBullet*)bodyA->GetUserData());
					bullet->setState(enemyBullet::EXPLODE);
					bodyA->SetLinearVelocity(b2Vec2(0,0));

					playBody->SetLinearVelocity(b2Vec2(0,0));
					if(playBody!=NULL){
						tanker *tank=  ((tanker*)playBody->GetUserData());
						if(!tank->isGod()){
							int attack = bullet->getBulletAttack();

							tank->beAttack(attack,bullet->getCritRateBool());//				
							// CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
							//((UIlayer *)scene->getChildByTag(2))->setTankLift(tank->getLife());
							if(tank->getLife()<120){//�Ʒѵ�
								if (!LoadBooleanFromXML("wudi"))
								{
									CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
									((UIlayer *)scene->getChildByTag(2))->wudiCallback(this);
									SaveBooleanToXML("wudi",true);
								}
							}
							if(tank->getLife()<=0){
								if(sound){
									SimpleAudioEngine::sharedEngine()->playEffect("sound/30.ogg",false);
								}
							}
						}
					}
				}else{
					enemyBullet *bullet =  ((enemyBullet*)bodyB->GetUserData());
					bullet->setState(enemyBullet::EXPLODE);
					bodyB->SetLinearVelocity(b2Vec2(0,0));

					playBody->SetLinearVelocity(b2Vec2(0,0));
					if(playBody!=NULL){
						tanker *tank=  ((tanker*)playBody->GetUserData());
						if(!tank->isGod()){
							int attack = bullet->getBulletAttack();
							tank->beAttack(attack,bullet->getCritRateBool());//

							//CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
							//((UIlayer *)scene->getChildByTag(2))->setTankLift(tank->getLife());
							
							
							if(tank->getLife()<120){//�Ʒѵ�
								if (!LoadBooleanFromXML("wudi"))
								{
									CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
									((UIlayer *)scene->getChildByTag(2))->wudiCallback(this);
									SaveBooleanToXML("wudi",true);
								}
							}
							if(tank->getLife()<=0){
								if(sound){
									SimpleAudioEngine::sharedEngine()->playEffect("sound/30.ogg",false);
								}
							}
						}
					}
				}
				//�����ӵ�����һ�����ײ
		}else if((((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="enemybullt"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="ourBase")
			||(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="ourBase"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="enemybullt")){
				ourBaseIsHurt = true;//�ܵ��˺�

				if(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="enemybullt"){
					enemyBullet *bullet =  ((enemyBullet*)bodyA->GetUserData());
					bullet->setState(enemyBullet::EXPLODE);
					bodyA->SetLinearVelocity(b2Vec2(0,0));

					Box *box =  ((Box*)bodyB->GetUserData());
					box->beAttack(bullet->getBulletAttack(),this,callfuncN_selector(GameScene::setBoxTileCallBack));
					//ʧ��
					// CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
					//((UIlayer *)scene->getChildByTag(2))->failMenu();
				}else{
					enemyBullet *bullet =  ((enemyBullet*)bodyB->GetUserData());
					bullet->setState(enemyBullet::EXPLODE);
					bodyB->SetLinearVelocity(b2Vec2(0,0));

					Box *box =  ((Box*)bodyA->GetUserData());
					box->beAttack(bullet->getBulletAttack(),this,callfuncN_selector(GameScene::setBoxTileCallBack));
					//ʧ��
					// CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
					// ((UIlayer *)scene->getChildByTag(2))->failMenu();
				}
				//�����ӵ�����˻�����ײ
		}else if((((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="enemybullt"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="enemyBase")
			||(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="enemyBase"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="enemybullt")){
				if(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="enemybullt"){
					enemyBullet *bullet =  ((enemyBullet*)bodyA->GetUserData());
					bullet->setState(enemyBullet::EXPLODE);
					bodyA->SetLinearVelocity(b2Vec2(0,0));
				}else{
					enemyBullet *bullet =  ((enemyBullet*)bodyB->GetUserData());
					bullet->setState(enemyBullet::EXPLODE);
					bodyB->SetLinearVelocity(b2Vec2(0,0));
				}
				//�����ӵ�������ӵ�
		}else if((((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="enemybullt"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="eleball")
			||(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="eleball"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="enemybullt")){
				if(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="enemybullt"){
					enemyBullet *enemybullet =  ((enemyBullet*)bodyA->GetUserData());
					if(enemybullet->getState()!=enemyBullet::EXPLODE){
						enemybullet->setState(enemyBullet::EXPLODE);
						bodyA->SetLinearVelocity(b2Vec2(0,0));
					}

					Bullet *bullet =  ((Bullet*)bodyB->GetUserData());
					if(bullet->getState()!=Bullet::EXPLODE){
						bullet->setState(Bullet::EXPLODE);
						bodyB->SetLinearVelocity(b2Vec2(0,0));
					}
				}else{
					enemyBullet *enemybullet =  ((enemyBullet*)bodyB->GetUserData());
					if(enemybullet->getState()!=enemyBullet::EXPLODE){
						enemybullet->setState(enemyBullet::EXPLODE);
						bodyB->SetLinearVelocity(b2Vec2(0,0));
					}

					Bullet *bullet =  ((Bullet*)bodyA->GetUserData());
					if(bullet->getState()!=Bullet::EXPLODE){
						bullet->setState(Bullet::EXPLODE);
						bodyA->SetLinearVelocity(b2Vec2(0,0));
					}
				}
		}

		//����̹�������̹����ײ
		if((((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="enemy"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="tank")
			||(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="tank"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="enemy")){

				if(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="enemy"){

					enemyTank *enemy=  ((enemyTank*)bodyA->GetUserData());
					if(enemy->getKing()==9){
						bodyB->SetLinearVelocity(b2Vec2(0,0));
						enemy->beAttack(1000,true);

						playBody->SetLinearVelocity(b2Vec2(0,0));
						if(playBody!=NULL){
							tanker *tank=  ((tanker*)playBody->GetUserData());
							tank->beAttack(1000,true);//bullet->getBulletAttack()

							//CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
							//((UIlayer *)scene->getChildByTag(2))->setTankLift(tank->getLife());
						}
					}else if(enemy->getKing()==1){//��һ��̹��
						tanker   *tank=  ((tanker*)bodyB->GetUserData());
						CCPoint  enemyPos = tilePosFromLocation(enemy->getPosition());
						CCPoint  tankPos = tilePosFromLocation(tank->getPosition());

						switch(enemy->getState()){
						case  1://��
							{
								if((enemyPos.y-1)==tankPos.y){
									if(playBody!=NULL){
										int attack = enemy->getBulletAttack()*random(8,12)/10;
										tank->beAttack(attack,false);//
										//CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
										//((UIlayer *)scene->getChildByTag(2))->setTankLift(tank->getLife());
										if(tank->getLife()<=0){
											if(sound){
												SimpleAudioEngine::sharedEngine()->playEffect("sound/30.ogg",false);
											}
										}
									}
								}
							}
							break;
						case 2://��
							{
								if((enemyPos.x-1)==tankPos.x){
									if(playBody!=NULL){
										int attack = enemy->getBulletAttack()*random(8,12)/10;
										tank->beAttack(attack,false);//
										//CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
										//((UIlayer *)scene->getChildByTag(2))->setTankLift(tank->getLife());
										if(tank->getLife()<=0){
											if(sound){
												SimpleAudioEngine::sharedEngine()->playEffect("sound/30.ogg",false);
											}
										}
									}
								}
							}
							break;
						case 3://��
							{
								if((enemyPos.y+1)==tankPos.y){
									if(playBody!=NULL){
										int attack = enemy->getBulletAttack()*random(8,12)/10;
										tank->beAttack(attack,false);//
										//CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
										//((UIlayer *)scene->getChildByTag(2))->setTankLift(tank->getLife());
										if(tank->getLife()<=0){
											if(sound){
												SimpleAudioEngine::sharedEngine()->playEffect("sound/30.ogg",false);
											}
										}
									}
								}
							}
							break;
						case 4://��
							{
								if((enemyPos.x+1)==tankPos.x){
									if(playBody!=NULL){
										int attack = enemy->getBulletAttack()*random(8,12)/10;
										tank->beAttack(attack,false);//

										//CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
										//((UIlayer *)scene->getChildByTag(2))->setTankLift(tank->getLife());
										if(tank->getLife()<=0){
											if(sound){
												SimpleAudioEngine::sharedEngine()->playEffect("sound/30.ogg",false);
											}
										}
									}
								}
							}
							break;
						}
					}else{
						bodyA->SetLinearVelocity(b2Vec2(0,0));
						if(((enemyTank*)bodyA->GetUserData())->isChangeDir()){
							enemyAIChangeDirection(bodyA);
						}
					}
				}else{
					enemyTank *enemy=  ((enemyTank*)bodyB->GetUserData());
					if(enemy->getKing()==9){
						bodyB->SetLinearVelocity(b2Vec2(0,0));
						enemy->beAttack(1000,true);

						if(playBody!=NULL){
							playBody->SetLinearVelocity(b2Vec2(0,0));
							tanker *tank=  ((tanker*)playBody->GetUserData());
							tank->beAttack(1000,true);//bullet->getBulletAttack()

							// CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
							//((UIlayer *)scene->getChildByTag(2))->setTankLift(tank->getLife());
						}
					}else if(enemy->getKing()==1){

						tanker   *tank=  ((tanker*)bodyB->GetUserData());
						CCPoint  enemyPos = tilePosFromLocation(enemy->getPosition());
						CCPoint  tankPos = tilePosFromLocation(tank->getPosition());

						switch(enemy->getState()){
						case  1://��
							{
								if((enemyPos.y-1)==tankPos.y){
									if(playBody!=NULL){
										int attack = enemy->getBulletAttack()*random(8,12)/10;
										tank->beAttack(attack,false);//

										// CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
										//((UIlayer *)scene->getChildByTag(2))->setTankLift(tank->getLife());

										if(tank->getLife()<=0){
											if(sound){
												SimpleAudioEngine::sharedEngine()->playEffect("sound/30.ogg",false);
											}
										}
									}
								}
							}
							break;
						case 2://��
							{
								if((enemyPos.x-1)==tankPos.x){
									if(playBody!=NULL){
										int attack = enemy->getBulletAttack()*random(8,12)/10;
										tank->beAttack(attack,false);//

										// CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
										// ((UIlayer *)scene->getChildByTag(2))->setTankLift(tank->getLife());

										if(tank->getLife()<=0){
											if(sound){
												SimpleAudioEngine::sharedEngine()->playEffect("sound/30.ogg",false);
											}
										}
									}
								}
							}
							break;
						case 3://��
							{
								if((enemyPos.y+1)==tankPos.y){
									if(playBody!=NULL){
										int attack = enemy->getBulletAttack()*random(8,12)/10;
										tank->beAttack(attack,false);//

										//CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
										//((UIlayer *)scene->getChildByTag(2))->setTankLift(tank->getLife());

										if(tank->getLife()<=0){
											if(sound){
												SimpleAudioEngine::sharedEngine()->playEffect("sound/30.ogg",false);
											}
										}
									}

								}
							}
							break;
						case 4://��
							{
								if((enemyPos.x+1)==tankPos.x){
									if(playBody!=NULL){
										int attack = enemy->getBulletAttack()*random(8,12)/10;
										tank->beAttack(attack,false);//

										// CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
										// ((UIlayer *)scene->getChildByTag(2))->setTankLift(tank->getLife());

										if(tank->getLife()<=0){
											if(sound){
												SimpleAudioEngine::sharedEngine()->playEffect("sound/30.ogg",false);
											}
										}
									}
								}
							}
							break;
						}
					}else{
						bodyB->SetLinearVelocity(b2Vec2(0,0));
						if(((enemyTank*)bodyB->GetUserData())->isChangeDir()){
							enemyAIChangeDirection(bodyB);
						}
					}
				}
				//���̹����ײ�����
		}else if((((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="bfitem_box"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="tank")
			||(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="tank"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="bfitem_box")){
				contact->SetEnabled(false);

				if(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="bfitem_box"){

					Props *props =  ((Props*)bodyA->GetUserData());
					if(props->getIndex()==1){//��Ѫ

						if(LoadBooleanFromXML("propsTeach1")){
							if(playBody!=NULL){
								tanker *tank=  ((tanker*)playBody->GetUserData());
								tank->addFullLift();
							}
						}else{
							propsTeach(1);
							SaveBooleanToXML("propsTeach1",true);
						}


					}else if(props->getIndex()==2){//��������һ������
						if(LoadBooleanFromXML("propsTeach2")){
							bomOneEnemy();
						}else{
							propsTeach(2);
							SaveBooleanToXML("propsTeach2",true);
						}

					}else if(props->getIndex()==3){//�������е���
						if(LoadBooleanFromXML("propsTeach3")){
							changeTank();
						}else{
							propsTeach(3);
							SaveBooleanToXML("propsTeach3",true);
						}

					}else if(props->getIndex()==4){//�޵�״̬
						if(LoadBooleanFromXML("propsTeach4")){
							if(playBody!=NULL){
								tanker *tank=  ((tanker*)playBody->GetUserData());
								tank->setGod();
							}
						}else{
							propsTeach(4);
							SaveBooleanToXML("propsTeach4",true);
						}							
					}else if(props->getIndex()==5){
						if(LoadBooleanFromXML("propsTeach5")){
							if(playBody!=NULL){
								tanker *tank=  ((tanker*)playBody->GetUserData());
								tank->setMoveSpeed();
								tankSpeed();
							}
						}else{
							propsTeach(5);
							SaveBooleanToXML("propsTeach5",true);

						}
					}else if(props->getIndex()==6){
						if(LoadBooleanFromXML("propsTeach6")){
							tankBulletCrit();
						}else{
							propsTeach(6);
							SaveBooleanToXML("propsTeach6",true);

						}

					}else if(props->getIndex()==7){
						if(LoadBooleanFromXML("propsTeach7")){
							setEnemyTankStanding();
						}else{
							propsTeach(7);
							SaveBooleanToXML("propsTeach7",true);

						}
					}
					CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
					((UIlayer *)scene->getChildByTag(2))->showTableProps(props->getIndex());
					((UIlayer *)scene->getChildByTag(2))->playEffect(18);
					//props->setDestroy(true);
					props->setExplode();
				}else {
					Props *props =  ((Props*)bodyB->GetUserData());

					if(props->getIndex()==1){//��Ѫ

						if(LoadBooleanFromXML("propsTeach1")){
							if(playBody!=NULL){
								tanker *tank=  ((tanker*)playBody->GetUserData());
								tank->addFullLift();
							}
						}else{
							propsTeach(1);
							SaveBooleanToXML("propsTeach1",true);
						}


					}else if(props->getIndex()==2){//��������һ������
						if(LoadBooleanFromXML("propsTeach2")){
							bomOneEnemy();
						}else{
							propsTeach(2);
							SaveBooleanToXML("propsTeach2",true);
						}

					}else if(props->getIndex()==3){//�������е���
						if(LoadBooleanFromXML("propsTeach3")){
							changeTank();
						}else{
							propsTeach(3);
							SaveBooleanToXML("propsTeach3",true);
						}

					}else if(props->getIndex()==4){//�޵�״̬
						if(LoadBooleanFromXML("propsTeach4")){
							if(playBody!=NULL){
								tanker *tank=  ((tanker*)playBody->GetUserData());
								tank->setGod();
							}
						}else{
							propsTeach(4);
							SaveBooleanToXML("propsTeach4",true);
						}							
					}else if(props->getIndex()==5){
						if(LoadBooleanFromXML("propsTeach5")){
							if(playBody!=NULL){
								tanker *tank=  ((tanker*)playBody->GetUserData());
								tank->setMoveSpeed();
								tankSpeed();
							}
						}else{
							propsTeach(5);
							SaveBooleanToXML("propsTeach5",true);

						}
					}else if(props->getIndex()==6){
						if(LoadBooleanFromXML("propsTeach6")){
							tankBulletCrit();
						}else{
							propsTeach(6);
							SaveBooleanToXML("propsTeach6",true);

						}

					}else if(props->getIndex()==7){
						if(LoadBooleanFromXML("propsTeach7")){
							setEnemyTankStanding();
						}else{
							propsTeach(7);
							SaveBooleanToXML("propsTeach7",true);

						}
					}
					//props->setDestroy(true);
					CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
					((UIlayer *)scene->getChildByTag(2))->showTableProps(props->getIndex());
					((UIlayer *)scene->getChildByTag(2))->playEffect(18);
					props->setExplode();
				}
		}

		//CCLOG("(UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName():%s",((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName());
		//CCLOG("(UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName():%s",((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName());
		//�����������ײ
		if((((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="enemy"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="landMine")
			||(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="landMine"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="enemy")){

				if(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="landMine"){

					landMine *landmine =  ((landMine*)bodyA->GetUserData());
					landmine->setState(landMine::EXPLODE);

					enemyTank *enemy=  ((enemyTank*)bodyB->GetUserData());
					bodyB->SetLinearVelocity(b2Vec2(0,0));
					enemy->beAttack(landmine->getLandMineAttack(),true);

					landmineEnemy++;
				}else{				    
					landMine *landmine =  ((landMine*)bodyB->GetUserData());
					landmine->setState(landMine::EXPLODE);

					enemyTank *enemy=  ((enemyTank*)bodyA->GetUserData());
					bodyB->SetLinearVelocity(b2Vec2(0,0));
					enemy->beAttack(landmine->getLandMineAttack(),true);

					landmineEnemy++;
				}
		}

	}else{//����ӵ�,�����ӵ���ǽ����ײ
		if(contact->GetFixtureA()->GetFilterData().categoryBits!=0x0001){
			if(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="eleball"){
				((Bullet*)bodyA->GetUserData())->setState(Bullet::EXPLODE);
			}else if(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="enemybullt"){
				((enemyBullet*)bodyA->GetUserData())->setState(enemyBullet::EXPLODE);
			}
		}else if(contact->GetFixtureB()->GetFilterData().categoryBits!=0x0001){
			if(((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="eleball"){
				((Bullet*)bodyB->GetUserData())->setState(Bullet::EXPLODE);
			}else if(((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="enemybullt"){
				((enemyBullet*)bodyB->GetUserData())->setState(enemyBullet::EXPLODE);
			}
		}
	}
}

void GameScene::showCoins(cocos2d::CCPoint point){
	//CCPoint p= ccp(_point.x*60,60.0*(tileMap->getMapSize().height-_point.y)-25);
	CCNodeLoaderLibrary *backloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	cocos2d::extension::CCBReader *backBreader= new cocos2d::extension::CCBReader(backloaderlibrary); 
	backBreader->autorelease(); 
	CCNode  *coinsNode  = backBreader->readNodeGraphFromFile("enemy/coins.ccbi",this);
	coinsNode->setPosition(point);

	tileMap->addChild(coinsNode,20);
	coinsNode->runAction(CCSequence::create(CCDelayTime::create(2.1f),CCCallFuncN::create(this,callfuncN_selector(GameScene::callBackshowCoins)),NULL));
}
void GameScene::callBackshowCoins(CCNode* sender){
	tileMap->removeChild(sender,-1);
}
void GameScene::propsTeach(int index){
	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	this->setTouchEnabled(true);

	teachPropsIndex = index;

	paused = true;
	UIlayer *ui = ((UIlayer *)scene->getChildByTag(2));
	ui->menuFireStopCallback(ui,CCControlEventTouchDown);

	this->pauseSchedulerAndActions();

	gameTime.pause();
	((CCMenu*)ui->getChildByTag(1))->setTouchEnabled(false);

	ui->pauseSchedulerAndActions();

	((ImmediateButton*)ui->getChildByTag(100))->setTouchEnabled(false);
	if(ui->getChildByTag(101)){
		((ImmediateButton*)ui->getChildByTag(101))->setTouchEnabled(false);
	}
	if(ui->getChildByTag(102)){
		((ImmediateButton*)ui->getChildByTag(102))->setTouchEnabled(false);
	}


	//CCLayer *ui = ((UIlayer *)scene->getChildByTag(2));

	char name[50];
	CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary(); 

	//����һ���Զ��ͷŵ�CCB��ȡ�ࣨCCBReader�� 
	cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary); 
	ccbReader->autorelease(); 

	sprintf(name,"teach/instruction_bfitem%d.ccbi",  index);//
	//��ȡccbi�ļ���ȡ���ڵ� 
	CCNode * node = ccbReader->readNodeGraphFromFile(name, this); 

	ccbReader->getAnimationManager()->runAnimationsForSequenceNamed("1st");//
	ccbReader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(GameScene::propsTeachEnd)); 

	if(node != NULL) {
		ui->addChild(node,1,789);
	}
}

void GameScene::propsTeachEnd(){
	teachProps = true;

}


//���¼�����ײ���֮�󣬵�����ײ���֮ǰ����
void GameScene:: PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {
	b2Body*  bodyA = contact->GetFixtureA()->GetBody(); 
	b2Body*  bodyB = contact->GetFixtureB()->GetBody();

	if(contact->GetFixtureA()->GetFilterData().categoryBits!=0x0001&&contact->GetFixtureB()->GetFilterData().categoryBits!=0x0001){
		//�����ӵ�
		if((((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="bfitem_box"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="tank")
			||(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="tank"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="bfitem_box")){
				contact->SetEnabled(false);
		}

		if((((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="enemy"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()!="eleball")
			||(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()!="eleball"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="enemy")){
				//����̹�������̹��
				if(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="enemy"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="enemy"){

					if(((enemyTank*)bodyA->GetUserData())->isChangeDir()){
						bodyA->SetLinearVelocity(b2Vec2(0,0));
						enemyAIChangeDirection(bodyA);
						((enemyTank*)bodyA->GetUserData())->setTrackState(false);
					}else{
						if(((enemyTank*)bodyB->GetUserData())->getState()%2!=((enemyTank*)bodyA->GetUserData())->getState()%2){
							bodyA->SetLinearVelocity(b2Vec2(0,0));
						}
						((enemyTank*)bodyA->GetUserData())->setTrackState(false);
					}
					if(((enemyTank*)bodyB->GetUserData())->isChangeDir()){
						bodyB->SetLinearVelocity(b2Vec2(0,0));
						enemyAIChangeDirection(bodyB);
						((enemyTank*)bodyB->GetUserData())->setTrackState(false);
					}else{
						if(((enemyTank*)bodyB->GetUserData())->getState()%2!=((enemyTank*)bodyA->GetUserData())->getState()%2){
							bodyB->SetLinearVelocity(b2Vec2(0,0));
						}
						((enemyTank*)bodyB->GetUserData())->setTrackState(false);
					}
					//����̹�������̹��
				}else if((((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="enemy"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="tank")||
					(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="tank"&&((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="enemy")){

						if(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="enemy"){
							enemyTank *enemy=  ((enemyTank*)bodyA->GetUserData());
							if(enemy->getKing()!=1){
								bodyA->SetLinearVelocity(b2Vec2(0,0));
								if(((enemyTank*)bodyA->GetUserData())->isChangeDir()){
									enemyAIChangeDirection(bodyA);
								}

								tanker   *tank=  ((tanker*)bodyB->GetUserData());
								CCPoint  enemyPos = tilePosFromLocation(enemy->getPosition());
								CCPoint  tankPos = tilePosFromLocation(tank->getPosition());

								switch(tank->getState()){
								case 1://���� 
									{
										//CCLOG("bodyB=========-=-=-=-=-=-=-=-=-=-=-=-:%f-=:%f",enemyPos.y,tankPos.y);
										if(enemyPos.y==(tankPos.y-1)){
											bodyB->SetLinearVelocity(b2Vec2(0,0));

										}
									}
									break;
								case 2://����
									{
										if(enemyPos.y==(tankPos.y+1)){
											bodyB->SetLinearVelocity(b2Vec2(0,0));

										}
									}
									break;
								case 3://��
									{
										if(enemyPos.x==(tankPos.x-1)){
											bodyB->SetLinearVelocity(b2Vec2(0,0));
										}
									}
									break;
								case 4://��
									{
										if(enemyPos.x==(tankPos.x+1)){
											bodyB->SetLinearVelocity(b2Vec2(0,0));
										}
									}
									break;
								}

							}else{//���һ��̹����ײ
								tanker   *tank=  ((tanker*)bodyB->GetUserData());
								CCPoint  enemyPos = tilePosFromLocation(enemy->getPosition());
								CCPoint  tankPos = tilePosFromLocation(tank->getPosition());

								switch(tank->getState()){
								case 1://���� 
									{
										//CCLOG("bodyB=========-=-=-=-=-=-=-=-=-=-=-=-:%f-=:%f",enemyPos.y,tankPos.y);
										if(enemyPos.y==(tankPos.y-1)){
											bodyB->SetLinearVelocity(b2Vec2(0,0));

										}
									}
									break;
								case 2://����
									{
										if(enemyPos.y==(tankPos.y+1)){
											bodyB->SetLinearVelocity(b2Vec2(0,0));

										}
									}
									break;
								case 3://��
									{
										if(enemyPos.x==(tankPos.x-1)){
											bodyB->SetLinearVelocity(b2Vec2(0,0));
										}
									}
									break;
								case 4://��
									{
										if(enemyPos.x==(tankPos.x+1)){
											bodyB->SetLinearVelocity(b2Vec2(0,0));
										}
									}
									break;
								}

								if(enemy->openFire()){
									switch(enemy->getState()){
									case  1://��
										{
											if((enemyPos.y-1)==tankPos.y){
												if(playBody!=NULL){
													int attack = enemy->getBulletAttack()*random(8,12)/10;
													tank->beAttack(attack,false);//

													// CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
													//((UIlayer *)scene->getChildByTag(2))->setTankLift(tank->getLife());

													if(tank->getLife()<=0){
														if(sound){
															SimpleAudioEngine::sharedEngine()->playEffect("sound/30.ogg",false);
														}
													}
												}
											}
										}
										break;
									case 2://��
										{
											if((enemyPos.x-1)==tankPos.x){
												if(playBody!=NULL){
													int attack = enemy->getBulletAttack()*random(8,12)/10;
													tank->beAttack(attack,false);//

													// CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
													// ((UIlayer *)scene->getChildByTag(2))->setTankLift(tank->getLife());

													if(tank->getLife()<=0){
														if(sound){
															SimpleAudioEngine::sharedEngine()->playEffect("sound/30.ogg",false);
														}
													}
												}
											}
										}
										break;
									case 3://��
										{
											if((enemyPos.y+1)==tankPos.y){
												if(playBody!=NULL){
													int attack = enemy->getBulletAttack()*random(8,12)/10;
													tank->beAttack(attack,false);//

													//CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
													//((UIlayer *)scene->getChildByTag(2))->setTankLift(tank->getLife());

													if(tank->getLife()<=0){
														if(sound){
															SimpleAudioEngine::sharedEngine()->playEffect("sound/30.ogg",false);
														}
													}
												}

											}
										}
										break;
									case 4://��
										{
											if((enemyPos.x+1)==tankPos.x){
												if(playBody!=NULL){
													int attack = enemy->getBulletAttack()*random(8,12)/10;
													tank->beAttack(attack,false);//

													// CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
													// ((UIlayer *)scene->getChildByTag(2))->setTankLift(tank->getLife());

													if(tank->getLife()<=0){
														if(sound){
															SimpleAudioEngine::sharedEngine()->playEffect("sound/30.ogg",false);
														}
													}
												}
											}
										}
										break;
									}

								}
							}
							enemy->setTrackState(false);
							//bodyB->SetLinearVelocity(b2Vec2(0,0));
						}else{
							enemyTank *enemy=  ((enemyTank*)bodyB->GetUserData());
							if(enemy->getKing()!=1){
								bodyB->SetLinearVelocity(b2Vec2(0,0));
								if(((enemyTank*)bodyB->GetUserData())->isChangeDir()){
									enemyAIChangeDirection(bodyB);
								}

								tanker *tank=  ((tanker*)bodyA->GetUserData());
								CCPoint  enemyPos = tilePosFromLocation(enemy->getPosition());
								CCPoint  tankPos = tilePosFromLocation(tank->getPosition());

								switch(tank->getState()){
								case 1://���� 
									{
										//CCLOG("bodyB=========-=-=-=-=-=-=-=-=-=-=-=-:%f-=:%f",enemyPos.y,tankPos.y);
										if(enemyPos.y==(tankPos.y-1)){
											bodyB->SetLinearVelocity(b2Vec2(0,0));
										}
									}
									break;
								case 2://����
									{
										if(enemyPos.y==(tankPos.y+1)){
											bodyB->SetLinearVelocity(b2Vec2(0,0));
										}
									}
									break;
								case 3://��
									{
										if(enemyPos.x==(tankPos.x-1)){
											bodyB->SetLinearVelocity(b2Vec2(0,0));
										}
									}
									break;
								case 4://��
									{
										if(enemyPos.x==(tankPos.x+1)){
											bodyB->SetLinearVelocity(b2Vec2(0,0));
										}
									}
									break;
								}
							}else{
								tanker   *tank=  ((tanker*)bodyA->GetUserData());
								CCPoint  enemyPos = tilePosFromLocation(enemy->getPosition());
								CCPoint  tankPos = tilePosFromLocation(tank->getPosition());

								switch(tank->getState()){
								case 1://���� 
									{
										//CCLOG("bodyB=========-=-=-=-=-=-=-=-=-=-=-=-:%f-=:%f",enemyPos.y,tankPos.y);
										if(enemyPos.y==(tankPos.y-1)){
											bodyB->SetLinearVelocity(b2Vec2(0,0));
										}
									}
									break;
								case 2://����
									{
										if(enemyPos.y==(tankPos.y+1)){
											bodyB->SetLinearVelocity(b2Vec2(0,0));
										}
									}
									break;
								case 3://��
									{
										if(enemyPos.x==(tankPos.x-1)){
											bodyB->SetLinearVelocity(b2Vec2(0,0));
										}
									}
									break;
								case 4://��
									{
										if(enemyPos.x==(tankPos.x+1)){
											bodyB->SetLinearVelocity(b2Vec2(0,0));
										}
									}
									break;
								}

								if(enemy->openFire()){
									switch(enemy->getState()){
									case  1://��
										{
											if((enemyPos.y-1)==tankPos.y){
												if(playBody!=NULL){
													int attack = enemy->getBulletAttack()*random(8,12)/10;
													tank->beAttack(attack,false);//
													//CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
													//((UIlayer *)scene->getChildByTag(2))->setTankLift(tank->getLife());
													if(tank->getLife()<=0){
														if(sound){
															SimpleAudioEngine::sharedEngine()->playEffect("sound/30.ogg",false);
														}
													}
												}
											}
										}
										break;
									case 2://��
										{
											if((enemyPos.x-1)==tankPos.x){
												if(playBody!=NULL){
													int attack = enemy->getBulletAttack()*random(8,12)/10;
													tank->beAttack(attack,false);//
													//CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
													//((UIlayer *)scene->getChildByTag(2))->setTankLift(tank->getLife());
													if(tank->getLife()<=0){
														if(sound){
															SimpleAudioEngine::sharedEngine()->playEffect("sound/30.ogg",false);
														}
													}
												}
											}
										}
										break;
									case 3://��
										{
											if((enemyPos.y+1)==tankPos.y){
												if(playBody!=NULL){
													int attack = enemy->getBulletAttack()*random(8,12)/10;
													tank->beAttack(attack,false);//
													//CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
													//((UIlayer *)scene->getChildByTag(2))->setTankLift(tank->getLife());
													if(tank->getLife()<=0){
														if(sound){
															SimpleAudioEngine::sharedEngine()->playEffect("sound/30.ogg",false);
														}
													}
												}
											}
										}
										break;
									case 4://��
										{
											if((enemyPos.x+1)==tankPos.x){
												if(playBody!=NULL){
													int attack = enemy->getBulletAttack()*random(8,12)/10;
													tank->beAttack(attack,false);//

													//CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
													//((UIlayer *)scene->getChildByTag(2))->setTankLift(tank->getLife());
													if(tank->getLife()<=0){
														if(sound){
															SimpleAudioEngine::sharedEngine()->playEffect("sound/30.ogg",false);
														}
													}
												}
											}
										}
										break;
									}
								}
								//bodyB->SetLinearVelocity(b2Vec2(0,0));
								//bodyA->SetLinearVelocity(b2Vec2(0,0));
							}
							enemy->setTrackState(false);
						}
						//����̹�������
				}else
				{
					if(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="enemy"){
						if(((enemyTank*)bodyA->GetUserData())->isChangeDir()){
							enemyAIChangeDirection(bodyA);
						}
					}else{
						if(((enemyTank*)bodyB->GetUserData())->isChangeDir()){
							enemyAIChangeDirection(bodyB);

						}
					}
				}
		}
	}else{
		if(contact->GetFixtureA()->GetFilterData().categoryBits!=0x0001){
			if(((UserDataConsistencyCCSprite*)bodyA->GetUserData())->getName()=="enemy"){
				if(((enemyTank*)bodyA->GetUserData())->isChangeDir()){
					enemyAIChangeDirection(bodyA);
				}
			}
		}else if(contact->GetFixtureB()->GetFilterData().categoryBits!=0x0001){
			if(((UserDataConsistencyCCSprite*)bodyB->GetUserData())->getName()=="enemy"){
				if(((enemyTank*)bodyB->GetUserData())->isChangeDir()){
					enemyAIChangeDirection(bodyB);
				}
			}
		}
	}

}
//����õ���ײ���������ʱ�򣬴��¼�������
void GameScene:: PostSolve(b2Contact* contact, const b2Manifold* oldManifold){
	b2Body*  bodyA = contact->GetFixtureA()->GetBody(); 
	b2Body*  bodyB = contact->GetFixtureB()->GetBody();

}

//�����������������ص���ʱ��ᷢ�����¼�
void GameScene::EndContact(b2Contact* contact) {
	b2Body*  bodyA = contact->GetFixtureA()->GetBody(); 
	b2Body*  bodyB = contact->GetFixtureB()->GetBody();


}

void GameScene::setBoxTileCallBack(CCNode* sender){
	CCTMXLayer  *layer;
	Box* box = (Box*)sender;

	if(box->getId()==stone){
		layer= tileMap->layerNamed(box->getLayerName());
		layer->setTileGID(53,box->boxPosLayer);
	}else if(box->getId()==sachet){
		layer= tileMap->layerNamed(box->getLayerName());
		layer->setTileGID(51,box->boxPosLayer);
	}else if(box->getId()==mound){
		layer= tileMap->layerNamed(box->getLayerName());
		layer->setTileGID(52,box->boxPosLayer);
	}else if(box->getId()==oilkeg){
		layer= tileMap->layerNamed(box->getLayerName());
		layer->setTileGID(54,box->boxPosLayer);
	}else if(box->getId()==console){
		layer= tileMap->layerNamed(box->getLayerName());
		layer->setTileGID(55,box->boxPosLayer);
	}else if(box->getId()==commander){
		layer= tileMap->layerNamed(box->getLayerName());
		layer->setTileGID(56,box->boxPosLayer);
	}else if(box->getId()==radar){
		layer= tileMap->layerNamed(box->getLayerName());
		layer->setTileGID(57,box->boxPosLayer);
	}else if(box->getId()==withered){
		layer= tileMap->layerNamed(box->getLayerName());
		layer->setTileGID(63,box->boxPosLayer);
		layer->setTileGID(64,ccp((box->boxPosLayer).x+1,(box->boxPosLayer).y));
	}else if(box->getId()==tree){
		layer= tileMap->layerNamed(box->getLayerName());
		layer->setTileGID(73,box->boxPosLayer);
		layer->setTileGID(74,ccp((box->boxPosLayer).x+1,(box->boxPosLayer).y));
	}else if(box->getId()==fountain){
		layer= tileMap->layerNamed(box->getLayerName());
		layer->setTileGID(83,box->boxPosLayer);
		layer->setTileGID(84,ccp((box->boxPosLayer).x+1,(box->boxPosLayer).y));
	}

	if(box->getId()!=ourBase){
		createBoxExplode(box);
	}

	if(box->getDestroy()){//����
		int  mapWidthTileNum  = tileMap->getMapSize().width;//�� Tile����
		int  mapHeightTileNum = tileMap->getMapSize().height;///tile �� ����

		if(box->getId()==ourBase){//�ҷ����ر��ݻ�
			isMoveView  = true;
			CCPoint  toPoint = getViewpointCenter(ccp(box->getPosition().x, box->getPosition().y));//�ƶ���Ļ

		  // CCPoint  toPoint = getViewpointCenter(ccp(400, 240));//�ƶ���Ļ

			CCActionInterval* move = CCMoveTo::create(1.0, toPoint);
			CCActionInterval* move_ease_in = CCEaseElasticOut::create (move, 2.0);

			this->runAction(CCSequence::create(move_ease_in,
				CCDelayTime::create(0.5f),
				NULL));
			//��ͣ
			CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
			paused = true;
			//this->pauseSchedulerAndActions();

			UIlayer *ui = ((UIlayer *)scene->getChildByTag(2));
			ui->menuFireStopCallback(ui,CCControlEventTouchDown);

			gameTime.pause();
			((CCMenu*)ui->getChildByTag(1))->setTouchEnabled(false);
			((ImmediateButton*)ui->getChildByTag(100))->setTouchEnabled(false);
			if(ui->getChildByTag(101)){
				((ImmediateButton*)ui->getChildByTag(101))->setTouchEnabled(false);
			}
			if(ui->getChildByTag(102)){
				((ImmediateButton*)ui->getChildByTag(102))->setTouchEnabled(false);
			}

			createOurBaseExplode(box);
		}

		if((int)((box->boxPosLayer).y*mapWidthTileNum+(box->boxPosLayer).x)<=mapWidthTileNum*mapHeightTileNum){
			mapObstruct[(int)((box->boxPosLayer).y*mapWidthTileNum+(box->boxPosLayer).x)] = 0;
		}

		proposShowBool = true;
		propsPoint = box->boxPosLayer;


		//drawTileyY*mapWidthTileNum+drawTileyX
	}
}

void GameScene::showProps(CCPoint point){
	if(randomBool(20)){//&&!world->IsLocked()  20
		propsNum++;
		int _index = random(1,7);
		while (_index==3){
			_index = random(1,7);
		}

		CCPoint  _point = point;
		CCPoint p= ccp(_point.x*60,60.0*(tileMap->getMapSize().height-_point.y)-25);

		Props *props = Props::spriteCreate(_index);
		props->setPoropsAction();
		props->retain();
		props->setAnchorPoint(ccp(0.5,0.5));
		props->setPosition(p);
		props->setBoxPosLayer(_point);

		// box->id = layer->tileGIDAt(point);
		props->setName("bfitem_box");
		//props->setLayerName(layer->getLayerName());//����ͼ�������

		tileMap->addChild(props,5,650+propsNum);

		b2BodyDef bodyDef;
		bodyDef.type = b2_kinematicBody;

		bodyDef.position.Set((p.x+32)/PTM_RATIO, (p.y)/PTM_RATIO);
		bodyDef.userData = props;
		b2Body *body = world->CreateBody(&bodyDef);

		GB2ShapeCache *sc = GB2ShapeCache::sharedGB2ShapeCache();
		sc->addFixturesToBody(body,"bfitem_box"); 
		props->setAnchorPoint(sc->anchorPointForShape("bfitem_box"));
	}
}
void GameScene::showProps3(cocos2d::CCPoint point){
	propsNum++;
	int _index = 3;

	CCPoint  _point = point;
	CCPoint p= ccp(_point.x,_point.y);

	Props *props = Props::spriteCreate(_index);
	props->setPoropsAction();
	props->retain();
	props->setAnchorPoint(ccp(0.5,0.5));
	props->setPosition(p);
	props->setBoxPosLayer(_point);
	props->pausedTime();

	// box->id = layer->tileGIDAt(point);
	props->setName("bfitem_box");
	//props->setLayerName(layer->getLayerName());//����ͼ�������

	tileMap->addChild(props,5,650+propsNum);

	b2BodyDef bodyDef;
	bodyDef.type = b2_kinematicBody;

	bodyDef.position.Set((p.x)/PTM_RATIO, (p.y)/PTM_RATIO);
	bodyDef.userData = props;
	b2Body *body = world->CreateBody(&bodyDef);

	GB2ShapeCache *sc = GB2ShapeCache::sharedGB2ShapeCache();
	sc->addFixturesToBody(body,"bfitem_box"); 
	props->setAnchorPoint(sc->anchorPointForShape("bfitem_box"));
}


void GameScene::createOurBaseExplode(Box* box){
	CCPoint pot = box->getBoxPosLayer();//λ��
	//CCLog("========================pot.x:%d",(int)pot.x);
	//CCLog("========================pot.y:%d",(int)pot.y);

	//��ը����
	CCNodeLoaderLibrary *godnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	cocos2d::extension::CCBReader  *explodeBreader = new cocos2d::extension::CCBReader(godnodeloaderlibrary); 
	explodeBreader->autorelease();
	CCNode  *animationsExplode  = explodeBreader->readNodeGraphFromFile("effect_explode_boss.ccbi",this);

	animationsExplode->setPosition(ccp(pot.x*60+30.0,60.0*(tileMap->getMapSize().height-pot.y)-30));

	if(animationsExplode  != NULL){     
		tileMap->addChild(animationsExplode,20);
	}
	explodeBreader->getAnimationManager()->runAnimationsForSequenceNamed("1st"); //Flying HoldUp  runAnimationsForSequenceNamed
	animationsExplode->runAction(CCSequence::create(CCDelayTime::create(3.1f),CCCallFuncN::create(this,callfuncN_selector(GameScene::callBackremoveOurBaseExplode)),NULL));
}

void GameScene::callBackremoveOurBaseExplode(CCNode* sender){
	tileMap->removeChild(sender,-1);

	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	((UIlayer *)scene->getChildByTag(2))->failMenu();
}

void GameScene::createBoxExplode(Box* box){
	CCPoint pot = box->getBoxPosLayer();//λ��
	//CCLog("========================pot.x:%d",(int)pot.x);
	//CCLog("========================pot.y:%d",(int)pot.y);

	//��ը����
	CCNodeLoaderLibrary *godnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	cocos2d::extension::CCBReader  *explodeBreader = new cocos2d::extension::CCBReader(godnodeloaderlibrary); 
	explodeBreader->autorelease();
	CCNode  *animationsExplode  = explodeBreader->readNodeGraphFromFile("effect_explode6060.ccbi",this);

	animationsExplode->setPosition(ccp(pot.x*60+30.0,60.0*(tileMap->getMapSize().height-pot.y)-30));

	if(animationsExplode  != NULL){     
		tileMap->addChild(animationsExplode,10);
	}

	/* CCArray* allSeq = explodeBreader->getAnimationManager()->getSequences(); 
	CCLog("============================") ;
	for (int i=0; i<(allSeq->count()); i++)
	{
	//��ȡ��ÿһ��Seq 
	CCBSequence* everySeq = (CCBSequence*)allSeq->objectAtIndex(i);
	//��ȡÿ�����ж������ڡ������Լ�id 
	// everySeq->getDuration(); 
	CCLog("===========%s",everySeq->getName()) ; 
	CCLog("===========%d",everySeq->getSequenceId()) ;
	} */
	explodeBreader->getAnimationManager()->runAnimationsForSequenceNamed("effect_explode"); //Flying HoldUp  runAnimationsForSequenceNamed
	animationsExplode->runAction(CCSequence::create(CCDelayTime::create(6.0f),CCCallFuncN::create(this,callfuncN_selector(GameScene::callBackremoveBoxExplode)),NULL));

}
void GameScene::callBackremoveBoxExplode(CCNode* sender){
	tileMap->removeChild(sender,-1);
	//CCLog("===========callBackremoveBoxExplode") ;
}

void GameScene::createLandMing(){
	CCPoint  point = ccp((playBody->GetPosition().x*PTM_RATIO-20),(playBody->GetPosition().y*PTM_RATIO));

	landMine *mine = landMine::spriteCreate(landmineIndex);
	//mine->setAnchorPoint(ccp(0.5,0.5));
	mine->setPosition(point);
	tileMap->addChild(mine,1,3000);
	mine->setName("landMine");


	b2BodyDef bodyDef;
	bodyDef.type = b2_kinematicBody;
	//bodyDef.fixedRotation = true;//��ֹ������ת

	bodyDef.position.Set((point.x)/PTM_RATIO, (point.y)/PTM_RATIO);
	bodyDef.userData = mine;
	b2Body *body = world->CreateBody(&bodyDef);

	GB2ShapeCache *sc = GB2ShapeCache::sharedGB2ShapeCache();
	sc->addFixturesToBody(body, "landmine"); //player
	mine->setAnchorPoint(sc->anchorPointForShape("landmine"));

}

void GameScene::changeTank(){

	if(playBody!=NULL&&!changeTankBool){
		int state = ((tanker*) tileMap->getChildByTag(100))->getState();
		int zOrder =  ((tanker*) tileMap->getChildByTag(100))->getZOrder();
		// int liftNum = ((tanker*) tileMap->getChildByTag(100))->getLife();
		//  tanker* tank = (tanker*)tileMap->getChildByTag(10010);
		tanker *tankMax =tanker::spriteCreate(6,6,3,3);
		tankMax->setTankerAction();
		tileMap->addChild(tankMax,zOrder,10010);
		tankMax->setName("tank");


		tankMax->setState(state);
		//  tank->lifeNum = liftNum;

		tileMap->getChildByTag(100)->setVisible(false);

		if(tankMax!=NULL){
			playBody->SetUserData(tankMax);
		}
		//���
		//this->scheduleOnce(schedule_selector(GameScene::changeBack),10.0f);

		Bullet *tankBullet=(Bullet*)(tankerBulletArray6->objectAtIndex(0));
		CCScene* scene = CCDirector::sharedDirector()->getRunningScene();

		float _rateOfFire = tankBullet->getRateOfFire();
		((UIlayer *)scene->getChildByTag(2))->rateOfFire = _rateOfFire;

		CCLog("changeTank:%f",_rateOfFire);
		((UIlayer *)scene->getChildByTag(2))->fireTime.setMillis(_rateOfFire*1000);
		((UIlayer *)scene->getChildByTag(2))->fireTime.start();

		changeTankBool = true;
		if(((UIlayer *)scene->getChildByTag(2))->isFireBool){//������
			((UIlayer *)scene->getChildByTag(2))->menuFireEventCallback(this,CCControlEventTouchDown);
		}
	}
}


void GameScene::changeBack(float time)//���
{
	changeTankBool = false;

	if(playBody!=NULL &&tileMap->getChildByTag(10010)){
		int state = ((tanker*) tileMap->getChildByTag(10010))->getState();
		int zOrder =  ((tanker*) tileMap->getChildByTag(10010))->getZOrder();
		tanker* tank = (tanker*)tileMap->getChildByTag(100);
		tank->setState(state);
		tank->setZOrder(zOrder);

		tileMap->removeChildByTag(10010,-1);

		if(tank!=NULL){
			playBody->SetUserData(tank);
			tank->setVisible(true);
		}


		Bullet *tankBullet=(Bullet*)(tankerBulletArray->objectAtIndex(0));
		CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
		float _rateOfFire = tankBullet->getRateOfFire();
		((UIlayer *)scene->getChildByTag(2))->rateOfFire = _rateOfFire;
		((UIlayer *)scene->getChildByTag(2))->fireTime.setMillis(_rateOfFire*1000);
		((UIlayer *)scene->getChildByTag(2))->fireTime.start();

		if(((UIlayer *)scene->getChildByTag(2))->isFireBool){//������
			((UIlayer *)scene->getChildByTag(2))->menuFireEventCallback(this,CCControlEventTouchDown);
		}
	}
}

void GameScene:: bomBeEnemy(){
	/*CCObject* _object = NULL; 
	CCARRAY_FOREACH(enemyTankArray, _object) {

	//int king = ((enemyKingNumDictElement *)_object)->King;
	//int num =  ((enemyKingNumDictElement *)_object)->Num;
	//(( enemyTank*)((CCArray*)_object)->objectAtIndex(0))->getKing();
	for(int i=0;i<((CCArray*)_object)->count();i++){

	if(((enemyTank*)(((CCArray*)_object)->objectAtIndex(i)))->getShow()){
	((enemyTank*)(((CCArray*)_object)->objectAtIndex(i)))->setState(enemyTank::BOMBING);
	//((enemyTank*)(((CCArray*)_object)->objectAtIndex(i)))->beAttack(1000);

	}
	}
	//CCLog("----------------king:%d",(( enemyTank*)((CCArray*)_object)->objectAtIndex(0))->getKing());
	}*/
	CCLog("----------------this->getPosition().x:%f    this->getPosition().y:%f",this->getPosition().x,this->getPosition().y);
	if(enemys.size()>0){
		vector<b2Body *>::iterator pos2;
		for(pos2 = enemys.begin(); pos2 != enemys.end();) {//++pos2
			b2Body *body = *pos2;
			if (body->GetUserData() != NULL){
				if(((enemyTank*)body->GetUserData())->getShow()){
					((enemyTank*)body->GetUserData())->setState(enemyTank::BOMBING);
				}

				body->SetLinearVelocity(b2Vec2(0/PTM_RATIO,0/PTM_RATIO));
				pos2++;
			}else{
				pos2++;
			}
		}
	}
	this->scheduleOnce(schedule_selector(GameScene::showExplodeAll),0.5f);
}

void GameScene::showExplodeAll(float time){
	CCSize size = CCDirector::sharedDirector()->getWinSize();

	if(sound){
		SimpleAudioEngine::sharedEngine()->playEffect("sound/33.ogg",false);
	}

	CCShake  *shake = CCShake::create(1.5,5.0);//����
	this->runAction(shake);

	CCNodeLoaderLibrary *godnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	cocos2d::extension::CCBReader  *explodeBreader = new cocos2d::extension::CCBReader(godnodeloaderlibrary); 
	explodeBreader->autorelease();
	CCNode  *animationsExplode  = explodeBreader->readNodeGraphFromFile("effect_explode_allscreen.ccbi",this);

	animationsExplode->setPosition(ccp(abs(this->getPosition().x),abs(this->getPosition().y)));
	if(animationsExplode  != NULL){     
		this->addChild(animationsExplode,1670);
	}
	animationsExplode->runAction(CCSequence::create(CCDelayTime::create(3.0f),CCCallFuncN::create(this,callfuncN_selector(GameScene::callBackremoveBoxExplode)),NULL));
}

void GameScene::callBackremoveExplodeAll(CCNode* sender){

	this->removeChild(sender,-1);
}
void GameScene::bomOneEnemy(){
	/*	CCObject* _object = NULL; 
	CCARRAY_FOREACH(enemyTankArray, _object) {
	//int king = ((enemyKingNumDictElement *)_object)->King;
	//int num =  ((enemyKingNumDictElement *)_object)->Num;
	//(( enemyTank*)((CCArray*)_object)->objectAtIndex(0))->getKing();
	for(int i=0;i<((CCArray*)_object)->count();i++){
	if(((enemyTank*)(((CCArray*)_object)->objectAtIndex(i)))->getShow()){
	((enemyTank*)(((CCArray*)_object)->objectAtIndex(i)))->setState(enemyTank::BOMBING);

	enemyTank* _enemyTank = ((enemyTank*)(((CCArray*)_object)->objectAtIndex(i)));

	isMoveView  = true;
	CCPoint  toPoint = getViewpointCenter(ccp(_enemyTank->getPosition().x, _enemyTank->getPosition().y));//�ƶ���Ļ


	CCActionInterval* move = CCMoveTo::create(1.0, toPoint);
	CCActionInterval* move_ease_in = CCEaseElasticOut::create (move, 2.0);


	this->runAction(CCSequence::create(move_ease_in,
	CCDelayTime::create(0.5f),
	CCCallFunc::create(this,callfunc_selector(GameScene::bomOneEnemyBack)),
	NULL));
	return;
	}
	}
	}*/
	if(enemys.size()>0){
		vector<b2Body *>::iterator pos2;
		for(pos2 = enemys.begin(); pos2 != enemys.end();) {//++pos2
			b2Body *body = *pos2;
			if (body->GetUserData() != NULL){
				if(((enemyTank*)body->GetUserData())->getShow()){
					body->SetLinearVelocity(b2Vec2(0/PTM_RATIO,0/PTM_RATIO));

					enemyTank* _enemyTank = (enemyTank*)body->GetUserData();
					_enemyTank->setState(enemyTank::BOMBING);

					isMoveView  = true;
					CCPoint  toPoint = getViewpointCenter(ccp(_enemyTank->getPosition().x, _enemyTank->getPosition().y));//�ƶ���Ļ

					CCActionInterval* move = CCMoveTo::create(1.0, toPoint);
					CCActionInterval* move_ease_in = CCEaseElasticOut::create (move, 2.0);


					this->runAction(CCSequence::create(move_ease_in,
						CCDelayTime::create(0.5f),
						CCCallFunc::create(this,callfunc_selector(GameScene::bomOneEnemyBack)),
						NULL));

					return;
				}
				pos2++;
			}else{
				pos2++;
			}
		}
	}
}

void GameScene::bomOneEnemyBack(){
	CCPoint  backPoint = getViewpointCenter(ccp(playBody->GetPosition().x*PTM_RATIO,playBody->GetPosition().y*PTM_RATIO));//�ƶ���Ļ
	CCActionInterval* move_ease_out =  CCEaseElasticOut::create (CCMoveTo::create(1.0,backPoint), 2.0);

	this->runAction(CCSequence::create(
		move_ease_out,
		CCCallFunc::create(this,callfunc_selector(GameScene::bomOneEnemyEnd)),
		NULL));

}

void GameScene::bomOneEnemyEnd(){
	isMoveView = false;
}
void GameScene::setEnemyTankStanding(){
	if(enemys.size()>0){
		vector<b2Body *>::iterator pos2;
		for(pos2 = enemys.begin(); pos2 != enemys.end();) {//++pos2
			b2Body *body = *pos2;
			if (body->GetUserData() != NULL){
				body->SetLinearVelocity(b2Vec2(0/PTM_RATIO,0/PTM_RATIO));
				((enemyTank*)body->GetUserData())->setShowStand(true);
				pos2++;
			}else{
				pos2++;
			}
		}
		schedule(schedule_selector(GameScene::upDataTankStanding),10.1f);
		isEnemyTankStanding = true;
	}
}

void GameScene::upDataTankStanding(float dt){
	isEnemyTankStanding = false;
	unschedule(schedule_selector(GameScene::upDataTankStanding));
	if(enemys.size()>0){
		vector<b2Body *>::iterator pos2;
		for(pos2 = enemys.begin(); pos2 != enemys.end();){//++pos2
			b2Body *body = *pos2;
			if (body->GetUserData() != NULL){
				enemyAIChangeDirection(body);
				((enemyTank*)body->GetUserData())->setShowStand(false);
				pos2++;
			}else{
				pos2++;
			}
		}
	}
}

void GameScene::tankSpeed(){
	if(playBody!=NULL){
		tanker* tank = ((tanker*)playBody->GetUserData());

		int dirct = tank->getState();//����

		if(dirct==tank->moveDown||dirct==tank->holdDown){
			setPlayerSpeed(0,-tank->getMoveSpeed());
		}else  if(dirct==tank->moveUp||dirct==tank->holdUp){
			setPlayerSpeed(0,tank->getMoveSpeed());
		}else if(dirct==tank->moveRight||dirct==tank->holdRight){
			setPlayerSpeed(tank->getMoveSpeed(),0);
		}else if(dirct==tank->moveLeft||dirct==tank->holdLeft){
			setPlayerSpeed(-tank->getMoveSpeed(),0); 
		}
	}
}




void GameScene::tankBulletCrit(){//
	CCObject* _object = NULL;

	CCARRAY_FOREACH(changeTankBool?tankerBulletArray6:tankerBulletArray, _object) {
		Bullet *tankBullet = (Bullet*)_object;
		tankBullet->setUpCrit();
	}
	if(playBody!=NULL){
		tanker *tank=  ((tanker*)playBody->GetUserData());
		tank->setCritical();
		tankSpeed();
	}
}



void GameScene::upDataWorld(float dt)
{
	//It is recommended that a fixed time step is used with Box2D for stability
	//of the simulation, however, we are using a variable time step here.
	//You need to make an informed choice, the following URL is useful

	int velocityIterations = 8;
	int positionIterations = 1;

	if(paused){
		dt=0;
		return;
	}
	// Instruct the world to perform a single step of simulation. It is
	// generally best to keep the time step and iterations fixed.
	world->Step(dt, velocityIterations, positionIterations);

	//Iterate over the bodies in the physics world
	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		if (b->GetUserData() != NULL) {
			//Synchronize the AtlasSprites position and rotation with the corresponding body
			UserDataConsistencyCCSprite* myActor=((UserDataConsistencyCCSprite*)(b->GetUserData()));
			myActor->setPosition( CCPointMake( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO) );
			myActor->setRotation( -1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));

			if(((UserDataConsistencyCCSprite*)b->GetUserData())->getName()=="enemy"){
				if(((enemyTank*)b->GetUserData())->getState()==enemyTank::BOMATTACK){
					b->SetLinearVelocity(b2Vec2(0,0));
				}
			}

			if(((UserDataConsistencyCCSprite*)b->GetUserData())->getDestroy()){
				//if(((UserDataConsistencyCCSprite*)b->GetUserData())->getName()!="tank"){
				toDestroy.push_back(b);
				/*}else{//����̹��
				if(tankCont>0){

				playBody_x;
				tankCont-=1;
				}else{

				}
				}*/
			}

		}	
	}
	cleanDestroy();
	enemysEmerge();
	//���λ��

	if(playBody!=NULL &&!isMoveView){
		setViewpointCenter(ccp(playBody->GetPosition().x*PTM_RATIO,playBody->GetPosition().y*PTM_RATIO));
	}
	if(playBody==NULL){
		if(tankCont>0){
			if(!isMoveView){//�ƶ���Ļ
				isMoveView  = true;
				CCPoint  toPoint = getViewpointCenter(ccp(playBody_x, playBody_y));
				CCActionInterval* move = CCMoveTo::create(1.2, toPoint);
				CCActionInterval* move_ease_in = CCEaseElasticOut::create (move, 2.5);
				this->runAction(CCSequence::create(move_ease_in,
					CCCallFunc::create(this,callfunc_selector(GameScene::endMoveView)),
					NULL));
			}
		}else{//����,��ʾ�Ʒѵ�
			if(mapIndex<40){
				if(!showPayPoint){
					CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
						if(!(((UIlayer *)scene->getChildByTag(2))->getChildByTag(8510))){
							showPayPoint = true;
							//CCSize size = CCDirector::sharedDirector()->getWinSize();
							/*CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
							cocos2d::extension::CCBReader *ccbreaderPay = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
							CCNode  *payNode  = ccbreaderPay->readNodeGraphFromFile("pay/warning_lifebuy_fuhuo.ccbi",this);
							ccbreaderPay->autorelease();

							((UIlayer *)scene->getChildByTag(2))->addChild(payNode,100,8510);
							ccbreaderPay->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(GameScene::showPayEnd)); 
							
							*/
							ysePay(this);
						}
					
					
				}
			}else{
				CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
				if(mapIndex>=40){
					((UIlayer *)scene->getChildByTag(2))->successMenu();
				}else{
					((UIlayer *)scene->getChildByTag(2))->failMenu();
				}
			}
		}
	}

	enemysAIupDate();

	//changeTank();//��̹��
	if(tankCont<5 && mapIndex<40){
		if(regainLifeTime.paused())
		{
		  regainLifeTime.setMillis(15*60*1000);
		  regainLifeTime.start();
		}
		//CCLog("===================yyyyyy:%ld",regainLifeTime.getSurplusMillis());
		if(regainLifeTime.getSurplusMillis()<=0){
			tankCont++;
			CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
			((UIlayer *)scene->getChildByTag(2))->setTankCount(tankCont);//����̹������

			UserRecord *tankUR = new UserRecord("tank.db","tank");
			tankUR->saveInt("tankCont",tankCont);
			tankUR->closeDB();
			free(tankUR);

			if(tankCont<=5){
				regainLifeTime.setSurplusMillis(15*60*1000);
				regainLifeTime.start();
			}
		}
	}else{
		if(!regainLifeTime.paused())
		{
			regainLifeTime.pause();
		}
	}

	if(proposShowBool){
		showProps(propsPoint);
		proposShowBool = false;
	}
}


void GameScene::showPayEnd(){
	CCSize size =CCDirector::sharedDirector()->getWinSize();
	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	CCNode * node = ((UIlayer *)scene->getChildByTag(2))->getChildByTag(8510);
	//CCNode * node =this->getChildByTag(8510);

	CCMenuItemImage *yesItem = CCMenuItemImage::create(
		"ccbResources/active/button_yes0.png",
		"ccbResources/active/button_yes1.png",
		this,
		menu_selector(GameScene::ysePay));// yseSecondPay
	yesItem->setPosition(ccp(size.width/2-120, size.height/2-70));

	CCMenuItemImage *noItem = CCMenuItemImage::create(
		"ccbResources/active/button_no0.png",
		"ccbResources/active/button_no1.png",
		this,
		menu_selector(GameScene::noPay));
	noItem->setPosition(ccp(size.width/2+120, size.height/2-70));

	CCMenu* pMenu = CCMenu::create(yesItem,noItem, NULL);
	pMenu->setPosition(CCPointZero);
	node->addChild(pMenu,1,1);

}

void GameScene::yseSecondPay(CCObject* pSender){
	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	
	if(!((UIlayer *)scene->getChildByTag(2))->secondPay){
		CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		cocos2d::extension::CCBReader *ccbreaderPay = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
		CCNode  *payNode  = ccbreaderPay->readNodeGraphFromFile("pay/warning_pay.ccbi",this);
		ccbreaderPay->autorelease();

		((UIlayer *)scene->getChildByTag(2))->addChild(payNode,100,7510);

		CCSize size =CCDirector::sharedDirector()->getWinSize();
		CCMenuItemImage *yesItem = CCMenuItemImage::create(
			"ccbResources/active/button_yes0.png",
			"ccbResources/active/button_yes1.png",
			this,
			menu_selector(GameScene::ysePay));
		yesItem->setPosition(ccp(size.width/2-100, size.height/2-65));

		CCMenuItemImage *noItem = CCMenuItemImage::create(
			"ccbResources/active/button_no0.png",
			"ccbResources/active/button_no1.png",
			this,
			menu_selector(GameScene::noSecondPay));
		noItem->setPosition(ccp(size.width/2+100, size.height/2-65));

		CCMenu* pMenu = CCMenu::create(yesItem,noItem, NULL);
		pMenu->setPosition(CCPointZero);
		payNode->addChild(pMenu,1,1);

		((UIlayer *)scene->getChildByTag(2))->secondPay = true;
	}


}
 void GameScene::noSecondPay(CCObject* pSender){

	 CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	 ((UIlayer *)scene->getChildByTag(2))->secondPay = false;
	 ((UIlayer *)scene->getChildByTag(2))->removeChildByTag(7510,-1);
 }
void GameScene::ysePay(CCObject* pSender){
	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	((UIlayer *)scene->getChildByTag(2))->secondPay = false;
	//((UIlayer *)scene->getChildByTag(2))->removeChildByTag(7510,-1);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  

	//CCScene* scene = CCDirector::sharedDirector()->getRunningScene();

	((UIlayer *)scene->getChildByTag(2))->billingPointIndex = 5;//�Ʒѵ�
	SaveBooleanToXML("payResultBool",false);
	SaveBooleanToXML("payGameOver",false);
	//payResultBool = false;;
	//payGameOver = false;
	((UIlayer *)scene->getChildByTag(2))->schedule(schedule_selector(UIlayer::updateBillingPoint), 0.1f);

	JniMethodInfo minfo;
	CCLog("============"); 
	bool isHave = JniHelper::getStaticMethodInfo(minfo,"quyou/game/tank/billing","show_java_dialog", "(I)I");//"com/game/test/JniFruitHelper","show_java_dialog"
	CCLog("------------"); 
	jint _int=0;
	if (isHave) {
		CCLog("jni-java����ִ�����"); 
		_int = minfo.env->CallStaticIntMethod(minfo.classID, minfo.methodID,((UIlayer *)scene->getChildByTag(2))->billingPointIndex );//
	}else{
		CCLog("jjni:�˺�������"); 
	}
  #else

	//((UIlayer *)scene->getChildByTag(2))->removeChildByTag(8510,-1);
	//this->removeChildByTag(8510,-1);
	//SaveBooleanToXML("pay0",true);
	tankCont = 1;

	UserRecord *gradeUR = new UserRecord("tank.db","tank");
	//int  tankCont = gradeUR->readIntFromSQLite("tankCont");
	gradeUR->saveInt("tankCont",tankCont);
	gradeUR->closeDB();
	free(gradeUR);

	showPayPoint = false;
	if(((UIlayer *)scene->getChildByTag(2))->againPayLiftGame){
	  ((UIlayer *)scene->getChildByTag(2))->againPayLiftGame = false;
	  CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	  CCScene *pScene = GameScene::scene();
	  CCDirector::sharedDirector()->replaceScene(pScene);
	}
  #endif
}
void GameScene::noPay(CCObject* pSender){
	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	((UIlayer *)scene->getChildByTag(2))->removeChildByTag(8510,-1);

	/*if(!((UIlayer *)scene->getChildByTag(2))->againPayLiftGame){
		((UIlayer *)scene->getChildByTag(2))->failMenu();
	}
	//showPayPoint = false;
	((UIlayer *)scene->getChildByTag(2))->againPayLiftGame = false;*/

	char lable[30];
	UserRecord *urs = new UserRecord("tank.db","shop");
	sprintf(lable,"shopbox%d",6);
	int shopbox6= urs->readIntFromSQLite(lable);
	sprintf(lable,"shopbox%d",12);
	int shopbox12 = urs->readIntFromSQLite(lable);

	if(shopbox6==0||shopbox12==0){
		CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
		//UIlayer *ui = UIlayer::create(); 
		//scene->addChild(ui,2,2);
		//ui->setVisible(false);
		/*CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		cocos2d::extension::CCBReader *ccbreaderPay = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
		CCNode  *payNode  = ccbreaderPay->readNodeGraphFromFile("pay/scene_buy_tianshenxiafan.ccbi",this);
		ccbreaderPay->autorelease();
		//this->setPosition(ccp());
		((UIlayer *)scene->getChildByTag(2))->addChild(payNode,100,8511);*/
		showQiangGouBool = false;
		//tianShenXiaFan();
		yesShowtianShenXiaFan(this);
	}else{
		//CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
		if(!((UIlayer *)scene->getChildByTag(2))->againPayLiftGame){
			((UIlayer *)scene->getChildByTag(2))->failMenu();
		}
		//showPayPoint = false;
		((UIlayer *)scene->getChildByTag(2))->againPayLiftGame = false;
	}

	
}

void GameScene::tianShenXiaFan(){
	if(!showQiangGouBool){
		CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
		CCSize size =CCDirector::sharedDirector()->getWinSize();
		CCNode * node =((UIlayer *)scene->getChildByTag(2))->getChildByTag(8511);
		CCMenuItemImage *yesItem = CCMenuItemImage::create(
			"UI/queding1.png",
			"UI/queding2.png",
			this,
			menu_selector(GameScene::yesShowtianShenXiaFan));//yesQiangGouCallBack
		yesItem->setPosition(ccp(size.width/2, size.height/2-150));

		CCMenuItemImage *noItem = CCMenuItemImage::create(
			"UI/guanbi1.png",
			"UI/guanbi2.png",
			this,
			menu_selector(GameScene::noShowtianShenXiaFan));
		noItem->setPosition(ccp(size.width/2+290, size.height/2+175));

		CCMenu* pMenu = CCMenu::create(yesItem,noItem, NULL);
		pMenu->setPosition(CCPointZero);
		node->addChild(pMenu,1,1);

		showQiangGouBool= true;
		qiangGouShow = false;
	}
}

void GameScene::yesShowtianShenXiaFan(CCObject* sender){

	showDeadTianShenXiafan = true;
	qiangGouShow = false;

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
	billingPointIndex = 15;//�Ʒѵ�
	SaveBooleanToXML("payResultBool",false);
	SaveBooleanToXML("payGameOver",false);
	//payResultBool = false;;
	//payGameOver = false;
	this->schedule(schedule_selector(GameScene::updateBillingPoint), 0.1f);

	JniMethodInfo minfo;
	CCLog("============"); 
	bool isHave = JniHelper::getStaticMethodInfo(minfo,"quyou/game/tank/billing","show_java_dialog", "(I)I");//"com/game/test/JniFruitHelper","show_java_dialog"
	CCLog("------------"); 
	jint _int=0;
	if (isHave) {
		CCLog("jni-java����ִ�����"); 
		_int = minfo.env->CallStaticIntMethod(minfo.classID, minfo.methodID,billingPointIndex);//
	}else{
		CCLog("jjni:�˺�������"); 
	}
#else
	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();

	((UIlayer *)scene->getChildByTag(2))->removeChildByTag(8511);
	if(!((UIlayer *)scene->getChildByTag(2))->againPayLiftGame){
		((UIlayer *)scene->getChildByTag(2))->failMenu();
	}
	//showPayPoint = false;
	((UIlayer *)scene->getChildByTag(2))->againPayLiftGame = false;
	deadTianShenXiaFanSave();
#endif


}

void GameScene::noShowtianShenXiaFan(CCObject* sender){
	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	if(((UIlayer *)scene->getChildByTag(2))->getChildByTag(8511)){
		((UIlayer *)scene->getChildByTag(2))->removeChildByTag(8511);
	}

	
	if(!((UIlayer *)scene->getChildByTag(2))->againPayLiftGame){
		((UIlayer *)scene->getChildByTag(2))->failMenu();
	}
	//showPayPoint = false;
	((UIlayer *)scene->getChildByTag(2))->againPayLiftGame = false;

}

void GameScene::endMoveView(){
	tanker *tank =tanker::spriteCreate(chassisId,turretId,chassisGrade,turretGrade);
	tank->setTankerAction();
	tank->setPosition(ccp(playBody_x, playBody_y));
	tileMap->addChild(tank,1,100);
	tank->setName("tank");


	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	Bullet *tankBullet=(Bullet*)(tankerBulletArray->objectAtIndex(0));

	float _rateOfFire = tankBullet->getRateOfFire();
	((UIlayer *)scene->getChildByTag(2))->rateOfFire = _rateOfFire;
	((UIlayer *)scene->getChildByTag(2))->fireTime.setMillis(_rateOfFire*1000);
	((UIlayer *)scene->getChildByTag(2))->fireTime.start();

	if(changeTankBool){
		changeTankBool = false;
		this->unschedule(schedule_selector(GameScene::changeBack));
	}

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.fixedRotation = true;//��ֹ������ת

	bodyDef.position.Set(playBody_x/PTM_RATIO, playBody_y/PTM_RATIO);
	bodyDef.userData = tank;
	playBody = world->CreateBody(&bodyDef);

	GB2ShapeCache *sc = GB2ShapeCache::sharedGB2ShapeCache();
	sc->addFixturesToBody(playBody, "player"); //player
	tank->setAnchorPoint(sc->anchorPointForShape("player"));
	//tankCont-=1;
	//((UIlayer *)scene->getChildByTag(2))->setTankCount(tankCont);//����̹������

	UserRecord *tankUR = new UserRecord("tank.db","tank");
	tankUR->saveInt("tankCont",tankCont);
	tankUR->closeDB();
	free(tankUR);

	if(tankCont<5){
		if(regainLifeTime.getSurplusMillis()<=0){
			regainLifeTime.setMillis(6*60*1000);
		}
		regainLifeTime.start();
	}

	isMoveView = false;
}
void GameScene::cleanDestroy(){
	//destroy��ʧ
	if(enemys.size()>0){
		vector<b2Body *>::iterator pos2;
		for(pos2 = enemys.begin(); pos2 != enemys.end();) {//++pos2
			b2Body *body = *pos2;
			if (body->GetUserData() != NULL){
				if (((enemyTank *)body->GetUserData())->getDestroy())
				{
					tileMap->removeChildByTag(((enemyTank *)body->GetUserData())->getTag(),-1);//�ӵ�ͼ���Ƴ�
					pos2=enemys.erase(pos2);
				}else{
					pos2++;
				}				
			}else{
				pos2++;
			}
		}
	}

	if(toDestroy.size()>0){
		// b2Vec2 playerPosition = playBody->GetPosition(); 

		vector<b2Body *>::iterator pos2;
		for(pos2 = toDestroy.begin(); pos2 != toDestroy.end();) {//++pos2
			b2Body *body = *pos2;

			if (body->GetUserData() != NULL){
				if(!world->IsLocked()){
					if(((UserDataConsistencyCCSprite *)body->GetUserData())->getDestroy()){
						// b2Filter r = body->GetFixtureList()->GetFilterData();
						CCTMXLayer  *layer;
						if(((UserDataConsistencyCCSprite *)body->GetUserData())->getName()=="Box"
							||((UserDataConsistencyCCSprite *)body->GetUserData())->getName()=="oilkeg"
							||((UserDataConsistencyCCSprite *)body->GetUserData())->getName()=="stakes"){

								layer= tileMap->layerNamed(((UserDataConsistencyCCSprite *)body->GetUserData())->getLayerName());
								layer->removeChild(layer->tileAt(((Box *)body->GetUserData())->getBoxPosLayer()),true);

						}else if(((UserDataConsistencyCCSprite *)body->GetUserData())->getName()=="withered"
							||((UserDataConsistencyCCSprite *)body->GetUserData())->getName()=="generator"
							||((UserDataConsistencyCCSprite *)body->GetUserData())->getName()=="hstakes"
							||((UserDataConsistencyCCSprite *)body->GetUserData())->getName()=="tree"
							||((UserDataConsistencyCCSprite *)body->GetUserData())->getName()=="fountain"){
								layer= tileMap->layerNamed(((UserDataConsistencyCCSprite *)body->GetUserData())->getLayerName());

								CCPoint point = ((Box *)body->GetUserData())->getBoxPosLayer();
								layer->removeChild(layer->tileAt(point),true);
								layer->removeChild(layer->tileAt(ccp(point.x+1,point.y)),true);

						}else if(((UserDataConsistencyCCSprite *)body->GetUserData())->getName()=="eleball"){//����ӵ�
							CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
							((UIlayer *)scene->getChildByTag(2))->playEffect(11);

						}else if(((UserDataConsistencyCCSprite *)body->GetUserData())->getName()=="enemy"){//����̹��
							//����̹�˼���
							countEnemy_death++;
							enemyNum++;
							CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
							((UIlayer *)scene->getChildByTag(2))->enemyCountNum--;
							((UIlayer *)scene->getChildByTag(2))->setEnemyCount(((UIlayer *)scene->getChildByTag(2))->enemyCountNum);


							if(enemyNum==1&&!LoadBooleanFromXML("changTank")){//��ѧ,��ʾ��̹��
								gameTeach(((UserDataConsistencyCCSprite *)body->GetUserData())->getPosition());
								SaveBooleanToXML("changTank",true);
							}else if(enemyNum==20){
								showAchieve(3);
							}else if(enemyNum==100){
								showAchieve(4);
							}else if(enemyNum==1000){
								showAchieve(5);
							}else if(enemyNum==2000){
								showAchieve(8);
							}

						}else if(((UserDataConsistencyCCSprite *)body->GetUserData())->getName()=="enemybullt"){//����̹���ӵ�
							tileMap->removeChildByTag(((UserDataConsistencyCCSprite *)body->GetUserData())->getTag(),-1);
							CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
							((UIlayer *)scene->getChildByTag(2))->playEffect(11);
						}else if(((UserDataConsistencyCCSprite *)body->GetUserData())->getName()=="tank"){//���̹��

							if(((UserDataConsistencyCCSprite *)body->GetUserData())->getTag()!=100){
								tileMap->removeChildByTag(100,-1);
							}
							tileMap->removeChildByTag(((UserDataConsistencyCCSprite *)body->GetUserData())->getTag(),-1);

							playBody=NULL;
							playerDeath = true;

							if(mapIndex<40){
								CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
								tankCont-=1;
								((UIlayer *)scene->getChildByTag(2))->setTankCount(tankCont);//����̹������
								UserRecord *tankUR = new UserRecord("tank.db","tank");
								tankUR->saveInt("tankCont",tankCont);
								tankUR->closeDB();
								free(tankUR);
								/*if(regainLifeTime.getSurplusMillis()<=0){
									regainLifeTime.setMillis(15*60*1000);
									regainLifeTime.start();
								}*/
							}
							

						}else if(((UserDataConsistencyCCSprite *)body->GetUserData())->getName()=="enemyBase"){
							layer= tileMap->layerNamed(((UserDataConsistencyCCSprite *)body->GetUserData())->getLayerName());
							layer->removeChild(layer->tileAt(((Box *)body->GetUserData())->getBoxPosLayer()),true);
							enemyBaseInfo.show = false;
						}else if(((UserDataConsistencyCCSprite *)body->GetUserData())->getName()=="ourBase"){
							layer= tileMap->layerNamed(((UserDataConsistencyCCSprite *)body->GetUserData())->getLayerName());
							layer->removeChild(layer->tileAt(((Box *)body->GetUserData())->getBoxPosLayer()),true);
							ourBaseInfo.show = false;
						}else if(((UserDataConsistencyCCSprite *)body->GetUserData())->getName()=="bfitem_box"){
							tileMap->removeChildByTag(((UserDataConsistencyCCSprite *)body->GetUserData())->getTag(),-1);
						}

						pos2=toDestroy.erase(pos2);
						world->DestroyBody(body);
						body = NULL;
					}else{
						pos2++;
					}
				}     
			}else{
				pos2++;
			}
		}
		toDestroy.clear();//�Ƴ���������������
	}
}

void GameScene::enemysAIupDate(){
	if(enemys.size()>0&&!isEnemyTankStanding&&playBody!=NULL){
		vector<b2Body *>::iterator pos2;
		for(pos2 = enemys.begin(); pos2 != enemys.end();) {//++pos2
			b2Body *body = *pos2;
			enemyTank* enemy= (enemyTank*)body->GetUserData();

			if (body->GetUserData() != NULL&&enemy->getLifeNum()>0){

				if(enemy->getTrackState()){//A*״̬
					//CCLog("mmmmmmmmmmmmmmmmmmmmmmmmmmmm");
					CCArray *path = enemy->getTrackPath();//·��
					// CCLog("nnnnnnnnnnnnnnnnnnnnnnnnnnenemy->getTrackPathStep():%d",enemy->getTrackPathStep());

					if(path!=NULL&&enemy->getTrackPathStep()!=-1){
						// CCLog("path:%d",path->count());
						// CCLog("path:%d",enemy->getTrackPathStep());
						AstarItem* stepPos= (AstarItem *)path->objectAtIndex(enemy->getTrackPathStep());
						AstarItem* endPos= (AstarItem *)path->objectAtIndex(path->count()-1);//(path->lastObject());
						/* CCLog("vvvvvvvvvvvvvvvvvvvtilePosFromLocation(enemy->getPosition()).x:%f   tilePosFromLocation(enemy->getPosition()).y:%f",
						tilePosFromLocation(enemy->getPosition()).x,tilePosFromLocation(enemy->getPosition()).y);*/
						//  CCLog("vvvvvvvvvvvvvvvvvvvendPos->getcol():%d  endPos->getrow():%d",endPos->getcol(),endPos->getrow());
						if(tilePosFromLocation(enemy->getPosition())==ccp(endPos->getcol(),endPos->getrow())){
							// CCLog("qqqqqqqqqqqqqqqqqqqqqqq");
							enemy->setTrackState(false);
							//return;
						}
						// CCLog("11111111111111111111111111111");
						CCPoint currentPos= enemy->getPosition();//tilePosFromLocation(��ǰ����Tile

						//CCLog("-----ncol:%d    nrow:%d",stepPos->getcol(),(stepPos->getrow()));//(int)tileMap->getMapSize().height-
						// CCLog("-----currentPos.x:%f    currentPos.y:%f",currentPos.x,currentPos.y);
						// CCLog("-----tileMap->getMapSize().height-stepPos->getrow():%d",(int)tileMap->getMapSize().height-stepPos->getrow());

						float _width = tileMap->getTileSize().width;//���
						// CCLog("_width:%f",_width);
						float _halfWidth = _width/2;

						if(abs(((stepPos->getcol())*_width+_halfWidth)-currentPos.x)<(enemy->getSpeed()/PTM_RATIO)
							&&abs((((int)tileMap->getMapSize().height-stepPos->getrow())*_width-_halfWidth)-currentPos.y)<(enemy->getSpeed()/PTM_RATIO)){
								// CCLog("  zzz  =%f",((((int)tileMap->getMapSize().height-stepPos->getrow())*_width-_halfWidth)-currentPos.y));
								//  CCLog("=====   =x:%f    y:%f",tilePosFromLocation(enemy->getPosition()).x,tilePosFromLocation(enemy->getPosition()).y);
								if(tilePosFromLocation(enemy->getPosition())==ccp(stepPos->getcol(),stepPos->getrow())){
									enemy->setTrackPathStep(enemy->getTrackPathStep()+1);
									AstarItem *nextStepPos = (AstarItem *)path->objectAtIndex(enemy->getTrackPathStep());

									//CCLog("-----nextStepPos.x:%d    nextStepPos.y:%d",nextStepPos->getcol(),nextStepPos->getrow());

									if(nextStepPos->getcol()>stepPos->getcol()){//��,�����ƶ�
										enemy->setState(enemyTank::moveRight);
										body->SetLinearVelocity(b2Vec2(enemy->getSpeed()/PTM_RATIO,0));
									}else if(nextStepPos->getcol()<stepPos->getcol()){//�����ƶ�
										enemy->setState(enemyTank::moveLeft);
										body->SetLinearVelocity(b2Vec2(-enemy->getSpeed()/PTM_RATIO,0));
									}else if(nextStepPos->getrow()>stepPos->getrow()){//��
										enemy->setState(enemyTank::moveDown);
										body->SetLinearVelocity(b2Vec2(0/PTM_RATIO,-enemy->getSpeed()/PTM_RATIO));
									}else if(nextStepPos->getrow()<stepPos->getrow()){//��
										enemy->setState(enemyTank::moveUp);
										body->SetLinearVelocity(b2Vec2(0/PTM_RATIO,(enemy->getSpeed()/PTM_RATIO)));
									}
								}else{//
									switch (enemy->getState())
									{
									case 1://��
										{
											// body->SetLinearVelocity(b2Vec2(0/PTM_RATIO,enemy->getSpeed()/PTM_RATIO));
											//CCLog("ccccc=%f",((((int)tileMap->getMapSize().height-stepPos->getrow())*_width-_halfWidth)-currentPos.y));
											body->SetLinearVelocity(b2Vec2(0/PTM_RATIO,enemy->getSpeed()/PTM_RATIO-((((int)tileMap->getMapSize().height-stepPos->getrow())*_width-_halfWidth)-currentPos.y)));
										}
										break;
									case 2://��
										{

										}
										break;
									case 3://��
										{

										}
										break;
									case 4://��
										{

										}
										break;
									}
								}
								// body->set


								// CCLog("nextStepPos col:%d    nrow:%d",nextStepPos->getcol(),(nextStepPos->getrow()));

								/*if(((nextStepPos->getcol())*60.0+30.0)-currentPos.x>5.0f){//�����ƶ�
								// CCLog("---------------------------moveRight");
								enemy->setState(enemyTank::moveRight);
								body->SetLinearVelocity(b2Vec2(enemy->getSpeed()/PTM_RATIO,0));
								}else if(currentPos.x-((nextStepPos->getcol())*60.0+30.0)>5.0f){//�����ƶ�
								// CCLog("--------------------------moveLeft");
								enemy->setState(enemyTank::moveLeft);
								body->SetLinearVelocity(b2Vec2(-enemy->getSpeed()/PTM_RATIO,0));
								}else if((((int)tileMap->getMapSize().height-nextStepPos->getrow())*60.0-30.0)-currentPos.y>5.0f){//����
								// CCLog("--------------------------moveDown");
								enemy->setState(enemyTank::moveDown);
								body->SetLinearVelocity(b2Vec2(0/PTM_RATIO,-enemy->getSpeed()/PTM_RATIO));
								}else if(currentPos.y-(((int)tileMap->getMapSize().height-nextStepPos->getrow())*60.0-30.0)<5.0f){//����
								// CCLog("---------------------------moveUp");
								enemy->setState(enemyTank::moveUp);
								body->SetLinearVelocity(b2Vec2(0/PTM_RATIO,enemy->getSpeed()/PTM_RATIO));
								}*/
						}else{//û���м�����
							// CCLog("=======stepPos col:%d    stepPos:%d",stepPos->getcol(),(stepPos->getrow()));

							//	CCLog("22222222222222222222222222222222222");
							int next = 0;
							if((enemy->getTrackPathStep()+1)<path->count()){
								next = enemy->getTrackPathStep()+1;
							}else{
								next = path->count()-1;
							}

							// AstarItem *nextStepPos = (AstarItem *)path->objectAtIndex(next);

							if(currentPos.x-((stepPos->getcol())*_width+_halfWidth)>=(enemy->getSpeed()/PTM_RATIO)){//�����ƶ�
								//CCLog("==========================moveLeft");
								//if(nextStepPos->getcol()!=enemyTank::moveRight){

								enemy->setState(enemyTank::moveLeft);
								body->SetLinearVelocity(b2Vec2(-enemy->getSpeed()/PTM_RATIO,0));
								// }
							}else if(((stepPos->getcol())*_width+_halfWidth)-currentPos.x>=(enemy->getSpeed()/PTM_RATIO)){//�����ƶ�		
								//CCLog("==========================moveRight");
								// if(nextStepPos->getcol()!=enemyTank::moveLeft){

								enemy->setState(enemyTank::moveRight);
								body->SetLinearVelocity(b2Vec2(enemy->getSpeed()/PTM_RATIO,0));
								// }
							}else if(currentPos.y-(((int)tileMap->getMapSize().height-stepPos->getrow())*_width-_halfWidth)>=(enemy->getSpeed()/PTM_RATIO)){////����	
								//CCLog("==========================moveDown");
								// if(nextStepPos->getrow()!=enemyTank::moveUp){

								enemy->setState(enemyTank::moveDown);
								body->SetLinearVelocity(b2Vec2(0/PTM_RATIO,-enemy->getSpeed()/PTM_RATIO));
								//}
							}else if((((int)tileMap->getMapSize().height-stepPos->getrow())*_width-_halfWidth)-currentPos.y>=(enemy->getSpeed()/PTM_RATIO)){//����
								//CCLog("==========================moveUp");
								// if(nextStepPos->getrow()!=enemyTank::moveDown){

								enemy->setState(enemyTank::moveUp);
								body->SetLinearVelocity(b2Vec2(0/PTM_RATIO,enemy->getSpeed()/PTM_RATIO));
								// }
							}
						}	
					}
				}else{//��������
					if(abs(body->GetLinearVelocity().x)< ((enemyTank*)body->GetUserData())->getSpeed()/PTM_RATIO
						&&abs(body->GetLinearVelocity().y)<((enemyTank*)body->GetUserData())->getSpeed()/PTM_RATIO){
							if(((enemyTank*)body->GetUserData())->isChangeDir()){
								enemyAIChangeDirection(body);
							}
					}
					CCPoint enemyPos= tilePosFromLocation(((enemyTank*)body->GetUserData())->getPosition());
					CCPoint tankPos= tilePosFromLocation(((tanker*)playBody->GetUserData())->getPosition());

					// CCLog("enemyPos.x:%f  ,%f",enemyPos.x,enemyPos.y);
					// CCLog("tankPos.x:%f   ,tankPos.y:%f",tankPos.x,tankPos.y);

					if(enemyPos.x == tankPos.x||enemyPos.y==tankPos.y){
						if(enemyPos.x == tankPos.x){
							int  mapWidthTileNum  = tileMap->getMapSize().width;//�� Tile����
							int  mapHeightTileNum = tileMap->getMapSize().height;///tile �� ����
							//CCLog("enemyPos.y:%f",enemyPos.y);
							// CCLog("tankPos.y:%f",tankPos.y);

							if(enemyPos.y>tankPos.y){
								bool isSee = true;//�Ƿ��ܿ���
								for(int i= (int)tankPos.y;i<(int)enemyPos.y;i++){
									//CCLog("s: %d=%d",(int)(enemyPos.x+i*mapWidthTileNum),mapObstruct[(int)(enemyPos.x+i*mapWidthTileNum)]);
									if(mapObstruct[(int)(tankPos.x+i*mapWidthTileNum)]!=0){
										isSee = false;
										break;
									}
								}
								if(isSee){
									enemy->setState(enemyTank::moveUp);//�����ƶ�
									body->SetLinearVelocity(b2Vec2(0/PTM_RATIO,enemy->getSpeed()/PTM_RATIO)); 
								}

							}else if(enemyPos.y<tankPos.y){
								bool isSee = true;//�Ƿ��ܿ���
								for(int i= (int)enemyPos.y;i<(int)tankPos.y;i++){
									//   CCLog("i: %d=%d",(int)(enemyPos.x+i*mapWidthTileNum),mapObstruct[(int)(enemyPos.x+i*mapWidthTileNum)]);
									if(mapObstruct[(int)(enemyPos.x+i*mapWidthTileNum)]!=0){
										isSee = false;
										break;
									}
								}
								if(isSee){
									enemy->setState(enemyTank::moveDown);
									body->SetLinearVelocity(b2Vec2(0/PTM_RATIO,-enemy->getSpeed()/PTM_RATIO));
								}
							}

						}else if(enemyPos.y==tankPos.y){
							int  mapWidthTileNum  = tileMap->getMapSize().width;//�� Tile����
							int  mapHeightTileNum = tileMap->getMapSize().height;///tile �� ����

							if(enemyPos.x>tankPos.x){
								bool isSee = true;//�Ƿ��ܿ���

								for(int i=(int)(tankPos.x+tankPos.y*mapWidthTileNum);i<(int)(enemyPos.x+tankPos.y*mapWidthTileNum);i++){
									//  CCLog("z: %d=%d",i,mapObstruct[i]);
									if(mapObstruct[i]!=0){
										isSee = false;
										break;
									}
								}
								if(isSee){
									enemy->setState(enemyTank::moveLeft);//����
									body->SetLinearVelocity(b2Vec2(-enemy->getSpeed()/PTM_RATIO,0));
								}
							}else if(enemyPos.x<tankPos.x){
								bool isSee = true;//�Ƿ��ܿ���
								// CCLog("enemyPos.x:%f",enemyPos.x);
								//CCLog("tankPos.x:%f",tankPos.x);
								for(int i= (int)(enemyPos.x+tankPos.y*mapWidthTileNum);i<(int)(tankPos.x+tankPos.y*mapWidthTileNum);i++){
									//CCLog("i: %d=%d",i,mapObstruct[i]);
									if(mapObstruct[i]!=0){
										isSee = false;
										break;
									}
								}
								if(isSee){
									enemy->setState(enemyTank::moveRight);//����
									body->SetLinearVelocity(b2Vec2(enemy->getSpeed()/PTM_RATIO,0));
								}
							}
						}
					}
				}
				//CCLog("******************************");
				if(enemy->openFire()&&enemy->getKing()!=1&&enemy->getKing()!=9){//����,��һ��̹�˲������ӵ�
					//CCArray *_enemyBulletArray =(CCArray*)eneymTankBullArray->objectAtIndex(enemy->getBulletArrayIndex());
					//enemyBullet *_enemyBullt =(enemyBullet *)eneymTankBullArray->objectAtIndex(enemy->getBulletArrayIndex());
					//CCPoint  snap[10] ={ccp(0,0),ccp(0,0),ccp(0,0),ccp(0,0),ccp(0,0),ccp(0,0),ccp(0,0),ccp(0,0),ccp(0,0),ccp(0,0)};

					int bulletNum = 1;
					if(enemy->getKing()==6||enemy->getKing()==7||enemy->getKing()==8||enemy->getKing()==13||enemy->getKing()==11){
						bulletNum = 2;
					}
					int bulletIndex = 0;


					CCArray  *_array = (CCArray*)eneymTankBullArray->objectAtIndex(enemy->getBulletArrayIndex());
					CCObject *_object = NULL; 
					int index = 0;
					CCARRAY_FOREACH(_array, _object) {
						enemyBullet *_enemyBullt =(enemyBullet*) _object;
						if(_enemyBullt->bulletState==_enemyBullt->END&&!tileMap->getChildByTag(enemy->getBulletArrayIndex()*40+2000+index)){
							b2BodyDef bodyBulletDef;
							bodyBulletDef.type =b2_dynamicBody ;//

							_enemyBullt->setPosition(ccp(0, 0));
							_enemyBullt->setTag(enemy->getBulletArrayIndex()*40+2000+index);
							CCLog("=====_enemyBullt");
							tileMap->addChild(_enemyBullt,100);
							CCLog("----_enemyBullt");

							int dirct = enemy->getState();//����
							if(dirct==enemy->moveDown){
								if(enemy->getKing()==6||enemy->getKing()==7||enemy->getKing()==8||enemy->getKing()==13||enemy->getKing()==11){
									float dex = 0;
									float dey = 0;
									float distance = 20.0f;
									int   king = 0;
									if(enemy->getKing()==6){
										dey=0;
										dex =0;
									}else if(enemy->getKing()==7){
										dey=0;
										dex =0;
									}else if(enemy->getKing()==8||enemy->getKing()==11){
										dey=0;
										dex =0;
									}else if(enemy->getKing()==13){
										dey=-90.0f;
										dex =0;
										distance = bulletIndex==0?-15.0f:18.0f;
										king = 1;
									}
									bodyBulletDef.position.Set((enemy->getPositionX()+(bulletIndex+king)*distance-10)/PTM_RATIO, (enemy->getPositionY())/PTM_RATIO);
								}else{
									float dex = 0;
									float dey = 0;

									if(enemy->getKing()==2){
										dey=0;
										dex =0;
									}else if(enemy->getKing()==3){
										dey=0;
										dex =0;
									}else if(enemy->getKing()==4){
										dey=0;
										dex =0;
									}else if(enemy->getKing()==5){
										dey=0;
										dex =0;
									}else if(enemy->getKing()==10){
										dey=0;
										dex =0;
									}else if(enemy->getKing()==12){
										dey=-80.0f;
										dex =0;
									}else if(enemy->getKing()==14){
										dey=-80.0f;
										dex =8.0f;
									}

									bodyBulletDef.position.Set((enemy->getPositionX()+dex)/PTM_RATIO, (enemy->getPositionY()+dey)/PTM_RATIO);
								}
							}else if(dirct==enemy->moveUp){
								if(enemy->getKing()==6||enemy->getKing()==7||enemy->getKing()==8||enemy->getKing()==13||enemy->getKing()==11){
									float dex = 0;
									float dey = 0;

									float distance = 20.0f;
									int   king = 0;

									if(enemy->getKing()==6){
										dey=32.0f;
										dex =0;
									}else if(enemy->getKing()==7){
										dey=32.0f;
										dex =0;
									}else if(enemy->getKing()==8||enemy->getKing()==11){
										dey=32.0f;
										dex =0;
									}else if(enemy->getKing()==13){
										dey=90.0f;
										dex =0;

										distance = bulletIndex==0?-15.0f:18.0f;
										king = 1;
									}
									bodyBulletDef.position.Set((enemy->getPositionX()+(bulletIndex+king)*distance-10+dex)/PTM_RATIO, (enemy->getPositionY()+dey)/PTM_RATIO);
								}else{
									float dex = 0;
									float dey = 0;
									if(enemy->getKing()==2){
										dey=32.0f;
										dex =0;
									}else if(enemy->getKing()==3){
										dey=32.0f;
										dex =0;
									}else if(enemy->getKing()==4){
										dey=32.0f;
										dex =0;
									}else if(enemy->getKing()==5){
										dey=32.0f;
										dex =0;
									}else if(enemy->getKing()==10){
										dey=35.0f;
										dex =0;
									}else if(enemy->getKing()==12){
										dey=110.0f;
										dex =0;
									}else if(enemy->getKing()==14){
										dey=100.0f;
										dex =0;
									}
									bodyBulletDef.position.Set((enemy->getPositionX()+dex)/PTM_RATIO, (enemy->getPositionY()+dey)/PTM_RATIO);
								}

							}else if(dirct==enemy->moveRight){
								if(enemy->getKing()==6||enemy->getKing()==7||enemy->getKing()==8||enemy->getKing()==13||enemy->getKing()==11){
									float dex = 0;
									float dey = 0;

									float distance = 20.0f;
									int   king = 0;
									if(enemy->getKing()==6){
										dey=10.0f;
										dex =32.0f;
									}else if(enemy->getKing()==7){
										dey=16.0f;
										dex =32.0f;
									}else if(enemy->getKing()==8||enemy->getKing()==11){
										dey=16.0f;
										dex =32.0f;
									}else if(enemy->getKing()==13){
										dey=20.0f;
										dex =90.0f;

										distance = bulletIndex==0?-20.0f:8.0f;
										king = 1;
									}
									bodyBulletDef.position.Set((enemy->getPositionX()+dex)/PTM_RATIO, (enemy->getPositionY()+(bulletIndex+king)*distance-10+dey)/PTM_RATIO);
								}else{
									float dex = 0;
									float dey = 0;
									if(enemy->getKing()==2){
										dey=16.0f;
										dex =32.0f;
									}else if(enemy->getKing()==3){
										dey=16.0f;
										dex = 32.0f;
									}else if(enemy->getKing()==4){
										dey=16.0f;
										dex = 32.0f;
									}else if(enemy->getKing()==5){
										dey=16.0f;
										dex = 32.0f;
									}else if(enemy->getKing()==10){
										dey=12.0f;
										dex = 32.0f;
									}else if(enemy->getKing()==12){
										
										dey=12.0f;
										dex =90.0f;
									}else if(enemy->getKing()==14){
										dey=15.0f;
										dex =90.0f;
									}
									bodyBulletDef.position.Set((enemy->getPositionX()+dex)/PTM_RATIO, (enemy->getPositionY()+dey)/PTM_RATIO);
								}

							}else if(dirct==enemy->moveLeft){
								if(enemy->getKing()==6||enemy->getKing()==7||enemy->getKing()==8||enemy->getKing()==13||enemy->getKing()==11){
									float dex = 0;
									float dey = 0;

									float distance = 20.0f;
									int   king = 0;

									if(enemy->getKing()==6){
										dey=10.0f;
										dex = -32.0f;
									}else if(enemy->getKing()==7){
										dey=16.0f;
										dex = -32.0f;
									}else if(enemy->getKing()==8||enemy->getKing()==11){
										dey=16.0f;
										dex = -32.0f;
									}else if(enemy->getKing()==13){
										dey=12.0f;
										dex = -90.0f;

										distance = bulletIndex==0?-15.0f:12.0f;
										king = 1;
									}
									bodyBulletDef.position.Set((enemy->getPositionX()+dex)/PTM_RATIO,  (enemy->getPositionY()+(bulletIndex+king)*distance-10+dey)/PTM_RATIO);
								}else{
									float dex = 0;
									float dey = 0;
									if(enemy->getKing()==2){
										dey=16.0f;
										dex = -32.0f;
									}else if(enemy->getKing()==3){
										dey=16.0f;
										dex = -32.0f;
									}else if(enemy->getKing()==4){
										dey=16.0f;
										dex = -32.0f;
									}else if(enemy->getKing()==5){
										dey=16.0f;
										dex = -32.0f;
									}else if(enemy->getKing()==10){
										dey=12.0f;
										dex = -32.0f;
									}else if(enemy->getKing()==12){
										dey=12.0f;
										dex = -90.0f;
									}else if(enemy->getKing()==14){
										dey=5.0f;
										dex = -90.0f;
									}

									bodyBulletDef.position.Set((enemy->getPositionX()+dex)/PTM_RATIO, (enemy->getPositionY()+dey)/PTM_RATIO);
								}

							}
							bodyBulletDef.userData = _enemyBullt;
							b2Body *body  = world->CreateBody(&bodyBulletDef);

							char enemyN[30];
							sprintf(enemyN,"enemybullet_%d", enemy->getKing()==11?8:enemy->getKing());

							GB2ShapeCache *sfc = GB2ShapeCache::sharedGB2ShapeCache();
							sfc->addFixturesToBody(body, enemyN); //
							_enemyBullt->setAnchorPoint(sfc->anchorPointForShape(enemyN));

							if(dirct==enemy->moveDown){//�ӵ��ٶ�
								_enemyBullt->setState(_enemyBullt->moveDown);
								body->SetLinearVelocity(b2Vec2(0/PTM_RATIO,-(_enemyBullt->getBulletSpeed()/PTM_RATIO)));
							}else if(dirct==enemy->moveUp){
								_enemyBullt->setState(_enemyBullt->moveUp);
								body->SetLinearVelocity(b2Vec2(0/PTM_RATIO,_enemyBullt->getBulletSpeed()/PTM_RATIO));
							}else if(dirct==enemy->moveRight){
								_enemyBullt->setState(_enemyBullt->moveRight);
								body->SetLinearVelocity(b2Vec2(_enemyBullt->getBulletSpeed()/PTM_RATIO,0/PTM_RATIO));
							}else if(dirct==enemy->moveLeft){
								_enemyBullt->setState(_enemyBullt->moveLeft);
								body->SetLinearVelocity(b2Vec2(-_enemyBullt->getBulletSpeed()/PTM_RATIO,0/PTM_RATIO));
							}
							//CCLog("x:%f,y:%f",body->GetLinearVelocity().x,body->GetLinearVelocity().y);
							bulletIndex++;
							if(bulletIndex>=bulletNum){
								break;
							}

						}
						index++;
					}
					//enemyBullet *enemybullt = enemyBullet::spriteCreate(enemy);
					//enemybullt->setBulletAction();pornhub
					//tileMap->addChild(enemybullt,1);

				}
				//CCLog("KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKk");
			}
			pos2++;
		}
	}
	//CCLog("zzzzzzzzzzzzzzzzzzzzzzzzzzz");
}
void GameScene::enemysAIAStar(float dt){
//	CCLog("enemysAIAStar======%d",enemys.size());
	if(enemys.size()>0&&!isEnemyTankStanding&&playBody!=NULL){
		vector<b2Body *>::iterator pos2;
		for(pos2 = enemys.begin(); pos2 != enemys.end();) {//++pos2
			b2Body *body = *pos2;
			if (body->GetUserData() != NULL){
				enemyTank* enemy= (enemyTank*)body->GetUserData();
				//�ж��Ƿ��ڼ�ⷶΧ
				CCPoint enemyTilePot = tilePosFromLocation(enemy->getPosition());
				CCPoint playerTilePot = tilePosFromLocation(((tanker*)playBody->GetUserData())->getPosition());
				//CCLog("------------enemy->getState():%d",enemy->getState());
				if(enemy->getState()<enemyTank::explode){
					//CCLog("------------enemyX:%f   enemyY:%f",enemyTilePot.x,enemyTilePot.y);
					//CCLog("------------playerX:%f  playerY:%f",playerTilePot.x,playerTilePot.y);

					if(enemy->isTrackRadius(enemyTilePot,playerTilePot)){//�ж��Ƿ����
						//CCLog("------------enemyX:%f   enemyY:%f",enemy->getPosition().x,enemy->getPosition().y);
						//CCLog("============enemyX:%f   enemyY:%f",tilePosFromLocation(enemy->getPosition()).x,tilePosFromLocation(enemy->getPosition()).y);
						//CCLog("------------tanker:%f   tanker:%f",((tanker*)playBody->GetUserData())->getPosition().x,((tanker*)playBody->GetUserData())->getPosition().y);
						//CCLog("============tanker:%f   tanker:%f",tilePosFromLocation(((tanker*)playBody->GetUserData())->getPosition()).x,tilePosFromLocation(((tanker*)playBody->GetUserData())->getPosition()).y);					
						CCArray *path=myastar->findPath((int)enemyTilePot.x,(int)enemyTilePot.y,(int)playerTilePot.x,(int)playerTilePot.y,mapObstruct,//(int)aimpoint.x,(int)aimpoint.y
							(int)tileMap->getMapSize().width,(int)tileMap->getMapSize().height);
						enemy->setTrackPath(path);
						if(path!=NULL){
							CCObject* _object = NULL; 
							CCARRAY_FOREACH(path, _object) {
								int ncol = ((AstarItem *)_object)->getcol();
								int nrow = ((AstarItem *)_object)->getrow();
								CCLog("ncol:%d    nrow:%d",ncol,nrow);
							}
						}else{
							enemy->setTrackState(false);
							// CCLog("============tankerhhhhhhhhhhhh");
						}
					}
				}
			}
			pos2++;
		}
	}
}
void GameScene::enemyAIChangeDirection(b2Body* enemyBody){
	if(!isEnemyTankStanding){
		enemyTank* enemy=(enemyTank*)enemyBody->GetUserData();
		int state = enemy->getTankState();
		//CCLog("=================enemy->getKing():%d",enemy->getKing());
		if(!enemy->getTrackState()){
			//srand((unsigned)time(0));
			while(true){
				int rand = random(1,4);
				//CCLog("------state:%d----------rand:%d",state,rand);
				if(rand!=state){
					state = rand;

					switch(state){
					case enemyTank::moveUp:
						{
							enemy->setState(enemyTank::moveUp);
							enemyBody->SetLinearVelocity(b2Vec2(0/PTM_RATIO,enemy->getSpeed()/PTM_RATIO));
						}
						break;
					case enemyTank::moveDown:
						{
							enemy->setState(enemyTank::moveDown);
							enemyBody->SetLinearVelocity(b2Vec2(0/PTM_RATIO,-enemy->getSpeed()/PTM_RATIO));
						}
						break;
					case enemyTank::moveLeft:
						{
							enemy->setState(enemyTank::moveLeft);
							enemyBody->SetLinearVelocity(b2Vec2(-enemy->getSpeed()/PTM_RATIO,0));
						}
						break;
					case enemyTank::moveRight:
						{
							enemy->setState(enemyTank::moveRight);
							enemyBody->SetLinearVelocity(b2Vec2(enemy->getSpeed()/PTM_RATIO,0));
						}
						break;
					}
					break;
				}
			}
		}
		//CCLog("--------------------enemy->getKing():%d",enemy->getKing());
	}
}

//��̹�˱���������
void GameScene::setViewpointCenter(cocos2d::CCPoint position)
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	//_tileMap = (CCTMXTiledMap *)this->getChildByTag(0);s
	int dey = 60;
	int x = MAX(position.x, winSize.width / 2);
	int y = MAX(position.y, winSize.height / 2-dey);
	x = MIN(x, (tileMap->getMapSize().width * tileMap->getTileSize().width) - winSize.width / 2);
	y = MIN(y, (tileMap->getMapSize().height * tileMap->getTileSize().height) - (winSize.height / 2)-dey);
	CCPoint actualPosition = ccp(x, y);

	// CCPoint centerOfView = ccp(winSize.width / 2, winSize.height / 2);
	CCPoint centerOfView = ccp(MIN(winSize.width / 2,tileMap->getContentSize().width/2),MIN(winSize.height/2-dey,tileMap->getContentSize().height/2));

	CCPoint viewPoint = ccpSub(centerOfView, actualPosition);
	this->setPosition(viewPoint);
} 

CCPoint GameScene::getViewpointCenter(cocos2d::CCPoint position){
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	//_tileMap = (CCTMXTiledMap *)this->getChildByTag(0);

	int x = MAX(position.x, winSize.width / 2);
	int y = MAX(position.y, winSize.height / 2);
	x = MIN(x, (tileMap->getMapSize().width * tileMap->getTileSize().width) - winSize.width / 2);
	y = MIN(y, (tileMap->getMapSize().height * tileMap->getTileSize().height) - winSize.height / 2);
	CCPoint actualPosition = ccp(x, y);

	//CCPoint centerOfView = ccp(winSize.width / 2, winSize.height / 2);
	CCPoint centerOfView = ccp(MIN(winSize.width / 2,tileMap->getContentSize().width/2),MIN(winSize.height/2,tileMap->getContentSize().height/2));
	CCPoint viewPoint = ccpSub(centerOfView, actualPosition);

	return viewPoint;
}
/*
��ͼ������
*/
CCPoint GameScene::tilePosFromLocation(CCPoint loction)
{
	CCPoint pos = ccpSub(loction,tileMap->getPosition());
	pos.x = (int)(pos.x / tileMap->getTileSize().width);
	pos.y = (int)((tileMap->getMapSize().height*tileMap->getTileSize().height - pos.y)/tileMap->getTileSize().height);

	return pos;
}

void GameScene::repositionSprite(float dt)
{
	CCArray *tankArray = CCArray::create();

	int  mapHeightTileNum = tileMap->getMapSize().height+4;///tile �� ����
	// CCLOG("mapHeightTileNum:%d",mapHeightTileNum);
	float _width = tileMap->getTileSize().width;//���

	float _halfWidth = _width/2;

	vector<b2Body *>::iterator pos2;
	for(pos2 = enemys.begin(); pos2 != enemys.end();) {//++pos2
		b2Body *body = *pos2;
		if (body->GetUserData() != NULL){
			enemyTank* enemy= (enemyTank*)body->GetUserData();
			CCPoint enemyp = (tileMap->getChildByTag(enemy->getTag()))->getPosition();
			enemyp = CC_POINT_POINTS_TO_PIXELS(enemyp);
			int enemyZ = mapHeightTileNum -  ((enemyp.y-_halfWidth) / _width);//17
			enemyZ = max(enemyZ,4);//

			//CCLog("newZ:%d ,king:%d",enemyZ,enemy->getKing());
			//enemy->setVertexZ(enemyZ);
			//enemy->getZOrder();
			//tileMap->reorderChild(tileMap->getChildByTag(enemy->getTag()), enemyZ);
			enemy->setZOrder(enemyZ);

			CCObject *_object;
			int _index = 0;
			bool isInsert = false;
			CCARRAY_FOREACH(tankArray, _object) {
				UserDataConsistencyCCSprite *udcc = (UserDataConsistencyCCSprite*)_object;
				//CCLog("udcc->getZOrder():%d",udcc->getZOrder());

				if(udcc->getZOrder()>enemy->getZOrder()){
					tankArray->insertObject(enemy,_index);
					isInsert = true;
					break;
				}else if(udcc->getZOrder()==enemy->getZOrder()){
					if(CC_POINT_POINTS_TO_PIXELS(udcc->getPosition()).y<enemyp.y){
						tankArray->insertObject(enemy,_index);
						isInsert = true;
						break;
					}
				}
				_index++;
			}

			if(!isInsert){
				tankArray->addObject(enemy);
			}

		}
		pos2++;
	}

	if(tileMap->getChildByTag(100)){
		CCPoint p = (tileMap->getChildByTag(100))->getPosition();
		p = CC_POINT_POINTS_TO_PIXELS(p);

		int newZ = (int)(mapHeightTileNum - ((p.y-_halfWidth) / _width));//17
		newZ = max(newZ,4);
		//newZ = min(newZ,19);
		// CCLog("newZ:%d",newZ);//

		tanker *_tank = (tanker *)tileMap->getChildByTag(100);
		// tileMap->reorderChild(tileMap->getChildByTag(100), newZ);
		_tank->setZOrder(newZ);

		CCObject *_object;
		int _index = 0;
		bool isInsert = false;
		CCARRAY_FOREACH(tankArray, _object) {
			UserDataConsistencyCCSprite *udcc = (UserDataConsistencyCCSprite*)_object;
			// CCLog("udcc->getZOrder():%d",udcc->getZOrder());

			if(udcc->getZOrder()>_tank->getZOrder()){
				tankArray->insertObject(_tank,_index);
				isInsert = true;
				break;
			}else if(udcc->getZOrder()==_tank->getZOrder()){
				// CCLog("CC_POINT_POINTS_TO_PIXELS(udcc->getPosition()).y :%f,p.y:%f",CC_POINT_POINTS_TO_PIXELS(udcc->getPosition()).y,p.y);
				if(CC_POINT_POINTS_TO_PIXELS(udcc->getPosition()).y<p.y){
					tankArray->insertObject(_tank,_index);
					isInsert = true;
					break;
				}
			}
			_index++;
		}
		if(!isInsert){
			tankArray->addObject(_tank);
		}

	}


	CCObject *_objectTank;

	CCARRAY_FOREACH(tankArray, _objectTank) {
		UserDataConsistencyCCSprite *udcc = (UserDataConsistencyCCSprite*)_objectTank;

		tileMap->reorderChild(udcc, udcc->getZOrder());

	}


	//���ض���
	if(baseFrameFrequency>4){
		baseFrameFrequency=0;
	}else{
		baseFrameFrequency++;
	}

	if(enemyBaseInfo.show){
		CCTMXLayer    *layer = tileMap->layerNamed(enemyBaseInfo.layerName);
		layer->setTileGID(75+baseFrameFrequency,enemyBaseInfo.pointBase);
	}
	if(ourBaseInfo.show){
		CCTMXLayer    *layer = tileMap->layerNamed(ourBaseInfo.layerName);
		// layer->setTileGID(85+baseFrameFrequency,ourBaseInfo.pointBase);
	}

	tankArray->removeAllObjects();
}

void GameScene::ourBaseWall(){

	CCPoint pos = ourBaseInfo.pointBase;
	//int num =returnInt(ourBaseInfo.layerName);
	CCLog("ourBaseInfo.layerName :%s   num:%d",ourBaseInfo.layerName,returnInt(ourBaseInfo.layerName));
	CCLog("pos.x :%f   pos.y:%f",pos.x,pos.y);

	for (int i=0;i<3;i++)//��
	{
		for(int j=0;j<3;j++){//��
			if((pos.x-1+j)>=0&&(pos.x-1+j)<tileMap->getMapSize().width
				&&(pos.y-1+i)>=0&&(pos.y-1+i)<tileMap->getMapSize().height){

					CCLog("pos.x-1+j :%f   pos.y-1+i:%f",pos.x-1+j,pos.y-1+i); 

					char lable[20];
					sprintf(lable, "block%d",(int)(pos.y-1+i));
					CCTMXLayer    *layer = tileMap->layerNamed(lable);

					if(!((pos.x-1+j)== pos.x&&(pos.y-1+i)==pos.y)){
						mapObstruct[(int)((pos.y-1+i)*tileMap->getMapSize().width)+(int)(pos.x-1+j)] = 1;
						layer->setTileGID(zsignpost,ccp(pos.x-1+j,pos.y-1+i));

						CCSprite*  sprite =layer->tileAt(ccp(pos.x-1+j,pos.y-1+i));//Box *box =dynamic_cast<Box *>(layer->tileAt(ccp(drawTileyX,drawTileyY)));//
						CCPoint p = sprite->getPosition();

						Box *box = Box::spriteCreate(sprite->displayFrame());
						box->retain();
						box->setAnchorPoint(ccp(0.5,0.5));
						box->setPosition(p);
						box->setBoxPosLayer(ccp(pos.x-1+j,pos.y-1+i));


						//box->id = layer->tileGIDAt(ccp(drawTileyX,drawTileyY));
						box->setId(layer->tileGIDAt(ccp(pos.x-1+j,pos.y-1+i)));

						box->setName("Box");
						box->setLayerName(layer->getLayerName());//����ͼ�������
						//box->lifeNum = 100;


						b2BodyDef bodyDef;
						bodyDef.type = b2_kinematicBody;

						bodyDef.position.Set((p.x+32)/PTM_RATIO, (p.y+32)/PTM_RATIO);
						bodyDef.userData = box;
						b2Body *body = world->CreateBody(&bodyDef);


						// add the fixture definitions to the body
						GB2ShapeCache *sc = GB2ShapeCache::sharedGB2ShapeCache();
						sc->addFixturesToBody(body,"box"); 
						box->setAnchorPoint(sc->anchorPointForShape("box"));
					}

			}
		}
	}

	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		if (b->GetUserData() != NULL) {
			//Synchronize the AtlasSprites position and rotation with the corresponding body
			UserDataConsistencyCCSprite* myActor=((UserDataConsistencyCCSprite*)(b->GetUserData()));
			myActor->setPosition( CCPointMake( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO) );
			myActor->setRotation( -1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));

			if(myActor->getName()=="Box"){
				Box* box = (Box*)myActor;
				CCPoint point = box->getBoxPosLayer();

				for (int i=0;i<3;i++)//��
				{
					for(int j=0;j<3;j++){//��
						if((pos.x-1+j)>=0&&(pos.x-1+j)<tileMap->getMapSize().width
							&&(pos.y-1+i)>=0&&(pos.y-1+i)<tileMap->getMapSize().height){
								if((pos.x-1+j)==point.x&&(pos.y-1+i)==point.y){
									box->setStrength(15);
								}
						}
					}
				}

			}
		}
	}

}


enemyKingNumDictElement::enemyKingNumDictElement(){

}
enemyKingNumDictElement::~enemyKingNumDictElement(){

}
void enemyKingNumDictElement::myInit(int k,int n,int index)//
{
	this->King = k;
	this->Num = n;
	this->index = index;
	//enemytank = enemyTank::spriteCreate(King);
	//enemytank->setTankerAction();
}
enemyKingNumDictElement* enemyKingNumDictElement::enemyKingNumDictElementCreate(int k,int n,int index){
	enemyKingNumDictElement *pobSprite = new enemyKingNumDictElement();       
	if (pobSprite)//��ע1     
	{        
		pobSprite->myInit(k,n,index);
		pobSprite->autorelease();        
		return pobSprite;     
	}     
	CC_SAFE_DELETE(pobSprite);     
	return NULL;
}

UIlayer::UIlayer(){ 

}
UIlayer::~UIlayer(){  

}

bool UIlayer::init() 
{     
	bool bRet = false;
	do {
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		CCScene* scene = CCDirector::sharedDirector()->getRunningScene();

		UserRecord *gradeUR = new UserRecord("tank.db","tank");
		bombNum = gradeUR->readIntFromSQLite("bomb");
		landmineNum = gradeUR->readIntFromSQLite("landmine");
		// if(mapIndex<=3){
		// ((GameScene *)scene->getChildByTag(1))->tankCont = 99;
		// }else{

		//  }
		int tankNumCount = gradeUR->readIntFromSQLite("tankCont");
		gradeUR->closeDB();
		free(gradeUR);

		if(mapIndex>=40){
			((GameScene*)scene->getChildByTag(1))->tankCont =0;

			CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
			UserRecord *tankUR = new UserRecord("tank.db","tank");
			int cont = tankUR->readIntFromSQLite("tankCont");
			tankUR->saveInt("tankCont",cont-1);
			tankUR->closeDB();
			free(tankUR);
		}else{
			((GameScene*)scene->getChildByTag(1))->tankCont = tankNumCount;
		}

		CCSprite *handleBack=CCSprite::create("UI/handleBack.png");
		handleBack->setPosition(ccp(100,100));
		this->addChild(handleBack,1,1000);

	    CCSize frameSize =	CCEGLView::sharedOpenGLView()->getFrameSize();  
		
		if((frameSize.height/frameSize.width)>(size.height/size.width)){
			handleBack->setScaleX(1.0+((frameSize.height/frameSize.width)-(size.height/size.width)));//
		}

	

		CCSprite *handlePoint=CCSprite::create("UI/handlePoint.png");
		handlePoint->setPosition(ccp(50,50));
		handlePoint->setVisible(false);
		this->addChild(handlePoint,1,1001);

		CCSprite *statusBar=CCSprite::create("UI/statusBar.png");
		statusBar->setPosition(ccp(size.width-220,size.height - 30));
		this->addChild(statusBar,1,1002);


		char tm[50];
		sprintf(tm, "%d%d:%d%d",0/10,0%10,0/10,0%10); //ʱ��
		CCLabelAtlas* gameTimeLable=CCLabelAtlas::create(tm, "UI/num.png",  18, 18, '0');
		gameTimeLable->setAnchorPoint(ccp(0,0));
		gameTimeLable->setPosition(ccp(70,15));
		statusBar->addChild(gameTimeLable,1,1);

		int _tankComt =((GameScene *)scene->getChildByTag(1))->tankCont;

		sprintf(tm, "%d%d",_tankComt/10,_tankComt%10); //̹������
		CCLabelAtlas* tankNum=CCLabelAtlas::create(tm, "UI/num.png",  18, 18, '0');
		tankNum->setAnchorPoint(ccp(0,0));
		tankNum->setPosition(ccp(382,15));//240
		statusBar->addChild(tankNum,1,2);

		sprintf(tm, "%d%d",100/10,100%10); //Ѫ��
		CCLabelAtlas* lifeNum=CCLabelAtlas::create(tm, "UI/num.png",  18, 18, '0');
		lifeNum->setAnchorPoint(ccp(0,0));
		lifeNum->setPosition(ccp(240,15));//382
		statusBar->addChild(lifeNum,1,3);

		CCMenuItemImage *pFireItem = CCMenuItemImage::create(
			"UI/pauseNormal.png",
			"UI/pauseSelect.png",
			this,
			menu_selector(UIlayer::pauseMenuCallback));
		pFireItem->setPosition(ccp(30, size.height-30));
		CCMenu* pMenu = CCMenu::create(pFireItem, NULL);
		pMenu->setPosition(CCPointZero);
		this->addChild(pMenu,1,1);


		XMLParser *pXmlParser = XMLParser::parseWithFile("blocks.xml");

		char enemyN[30];
		sprintf(enemyN,"block%d", mapIndex);
		CCString  *pValue3 = pXmlParser->getString(enemyN);
		int tuhaojin = pValue3->intValue();//ͬʱ���ֵĵ�������


		CCMenuItemImage *tuhaojinItem = CCMenuItemImage::create(
			"UI/button_tuhaojin.png",
			"UI/button_tuhaojin2.png",
			this,
			menu_selector(UIlayer::tuhaojinCallback));
		tuhaojinItem->setPosition(ccp(240, 60));

		CCMenu* tMenu = CCMenu::create(tuhaojinItem, NULL);
		tMenu->setPosition(CCPointZero);
		this->addChild(tMenu,1,371);

		if(tuhaojin==0){
			tMenu->setVisible(false);
		}

		CCMenuItemImage *wudiItem = CCMenuItemImage::create(
			"UI/button_wudi.png",
			"UI/button_wudi2.png",
			this,
			menu_selector(UIlayer::wudiCallback));
		wudiItem->setPosition(ccp(230, 150));

		CCMenu* wMenu = CCMenu::create(wudiItem, NULL);
		wMenu->setPosition(CCPointZero);
		this->addChild(wMenu,1,771);
		if(mapIndex>=40){
			wMenu->setVisible(false);
		}
		


		CCSprite *numBack = CCSprite::create("UI/numBack.png");
		numBack->setPosition(ccp(120, size.height-30));
		this->addChild(numBack,1);

		sprintf(tm, "%d",(mapIndex/10)+1); //
		CCLabelAtlas* _sceneNum=CCLabelAtlas::create(tm, "UI/num.png",  18, 18, '0');
		_sceneNum->setAnchorPoint(ccp(0.5,0.5));
		_sceneNum->setPosition(ccp(60,25));
		numBack->addChild(_sceneNum,1,1);

		sprintf(tm, "%d",mapIndex%10); //
		CCLabelAtlas* _mapNum=CCLabelAtlas::create(tm, "UI/num.png",  18, 18, '0');
		_mapNum->setAnchorPoint(ccp(0.5,0.5));
		_mapNum->setPosition(ccp(90,25));
		numBack->addChild(_mapNum,1,2);

		Bullet *tankBullet=(Bullet*)(((GameScene *)scene->getChildByTag(1))->tankerBulletArray)->objectAtIndex(0);
		rateOfFire = tankBullet->getRateOfFire();
		CCLOG("000rateOfFire:%f",rateOfFire);
		fireTime.setMillis(rateOfFire*1000);
		fireTime.start();
		CCLOG("000fireTime.getSurplusMillis():%d",fireTime.getSurplusMillis());
		//������ȴ��ť
		// ImmediateButton  *fireSkillButton = ImmediateButton::createSkillButton(tankBullet->getRateOfFire(), "UI/stencilL.png", "UI/fireNormal.png", this,callfuncO_selector(UIlayer::menuFireCallback));
		// SkillButton *fireSkillButton=SkillButton::createSkillButton(tankBullet->getRateOfFire(),"UI/stencilL.png","UI/fireNormal.png", "UI/fireSelect.png");
		/* ����״̬�µİ�ťͼƬ */  
		CCScale9Sprite* btnNormal = CCScale9Sprite::create("UI/fireNormal.png");  
		/* ���״̬�µİ�ťͼƬ */  
		CCScale9Sprite* btnDown = CCScale9Sprite::create("UI/fireSelect.png");  

		CCControlButton  *fireSkillButton = CCControlButton::create(btnNormal);
		fireSkillButton->setPreferredSize(CCSizeMake(114,114)); //���ô�С   

		// fireSkillButton->setBackgroundSpriteForState(btnDown, CCControlStateSelected); 
		fireSkillButton->setBackgroundSpriteForState(btnDown,CCControlStateHighlighted);  

		fireSkillButton->setPosition(ccp(size.width-60, 60));
		this->addChild(fireSkillButton,2,100);
		//���ð��»ص��¼�   
		fireSkillButton->addTargetWithActionForControlEvents(this,cccontrol_selector(UIlayer::menuFireEventCallback),  CCControlEventTouchDown); //����
		fireSkillButton->addTargetWithActionForControlEvents(this,cccontrol_selector(UIlayer::menuFireStopCallback),  CCControlEventTouchUpInside); //�����ڲ�̧��
		fireSkillButton->addTargetWithActionForControlEvents(this,cccontrol_selector(UIlayer::menuFireStopCallback),  CCControlEventTouchDragExit);//�϶��뿪���ڲ�  
		//fireSkillButton->addTargetWithActionForControlEvents(this,cccontrol_selector(UIlayer::menuFireStopCallback),  CCControlEventTouchDragEnter);//�϶��������ڲ�    
		//fireSkillButton->setTouchEnabled(false);


		//ը����ȴ��ť
		//ImmediateButton  *bombSkillButton = ImmediateButton::createSkillButton(tankBullet->getRateOfFire(), "UI/stencilS.png", "UI/bombNormal.png", this,callfuncO_selector(UIlayer::menuBombCallback));
		CCScale9Sprite* bombNormal = CCScale9Sprite::create("UI/bombNormal.png");  
		/* ���״̬�µİ�ťͼƬ */  
		CCScale9Sprite* bombDown = CCScale9Sprite::create("UI/bombSelect.png");  

		CCControlButton  *bombSkillButton = CCControlButton::create(bombNormal);
		bombSkillButton->setPreferredSize(CCSizeMake(70,70)); //���ô�С   
		bombSkillButton->setBackgroundSpriteForState(bombDown,CCControlStateHighlighted);  

		bombSkillButton->setPosition(ccp(size.width-60, 150));
		this->addChild(bombSkillButton,2,101);

		bombSkillButton->addTargetWithActionForControlEvents(this,cccontrol_selector(UIlayer::menuBombEventCallback),  CCControlEventTouchDown); //����

		sprintf(tm, "%d",bombNum); 
		CCLabelAtlas* _bombNum=CCLabelAtlas::create(tm, "UI/num.png",  18, 18, '0');
		_bombNum->setAnchorPoint(ccp(0.5,0.5));
		_bombNum->setPosition(ccp(35,35));
		bombSkillButton->addChild(_bombNum,1,1);
		/*if(bombNum==0){
			bombSkillButton->setTouchEnabled(false);
		}*/

		if(mapIndex>2){
			bombSkillButton->setVisible(true);
		}else{
			bombSkillButton->setVisible(false);
		}

		//������ȴ��ť
		if(mapIndex>1){
			// ImmediateButton  *landmineSkillButton = ImmediateButton::createSkillButton(tankBullet->getRateOfFire(), "UI/stencilS.png", "UI/landmineNormal.png", this,callfuncO_selector(UIlayer::menulandmineCallback));
			CCScale9Sprite* landmineNormal = CCScale9Sprite::create("UI/landmineNormal.png");  
			/* ���״̬�µİ�ťͼƬ */  
			CCScale9Sprite* landmineDown = CCScale9Sprite::create("UI/landmineSelect.png");  

			CCControlButton  *landmineSkillButton = CCControlButton::create(landmineNormal);
			landmineSkillButton->setPreferredSize(CCSizeMake(66,66)); //���ô�С   
			landmineSkillButton->setBackgroundSpriteForState(landmineDown,CCControlStateHighlighted);  


			landmineSkillButton->setPosition(ccp(size.width-150, 60));
			this->addChild(landmineSkillButton,2,102);
			landmineSkillButton->addTargetWithActionForControlEvents(this,cccontrol_selector(UIlayer::menulandmineEventCallback),  CCControlEventTouchDown); //����

			sprintf(tm, "%d",landmineNum); 
			CCLabelAtlas* _landmineNum=CCLabelAtlas::create(tm, "UI/num.png",  18, 18, '0');
			_landmineNum->setAnchorPoint(ccp(0.5,0.5));
			_landmineNum->setPosition(ccp(33,33));
			landmineSkillButton->addChild(_landmineNum,1,1);

			/*if(landmineNum==0){
				landmineSkillButton->setTouchEnabled(false);
			}*/
		}
		gameTime.setMillis(0);
		gameTime.start();
		schedule(schedule_selector(UIlayer::showGameTime),0.5f);

		failMenuBool = true;
		showTablePropsIndex = 0;
		showReturnMainMenu = false;
		isFireBool = false;
		gameOver = false;

		secondPay = false;
		againPayLiftGame = false;

		enemyCountNum = ((GameScene *)scene->getChildByTag(1))->enemysCount;
		setEnemyCount(enemyCountNum);


		tanker* tankPlayer= (tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData());
		// setTankLift(tankPlayer->getMAXLift());

		this->setTouchEnabled(true); 

		if(LoadBooleanFromXML("moveControlHandle")){
			setAccelerometerEnabled(true);//��������Ӧ   
		}

		SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("sound/1.ogg");
		SimpleAudioEngine::sharedEngine()->preloadEffect("sound/10.ogg");//��ͨ��ť��
		SimpleAudioEngine::sharedEngine()->preloadEffect("sound/161.ogg");//ʤ��
		SimpleAudioEngine::sharedEngine()->preloadEffect("sound/162.ogg");//ʤ��
		SimpleAudioEngine::sharedEngine()->preloadEffect("sound/163.ogg");//ʤ��
		SimpleAudioEngine::sharedEngine()->preloadEffect("sound/17.ogg");//ʧ��
		SimpleAudioEngine::sharedEngine()->preloadEffect("sound/18.ogg");//ʰȡ����

		SimpleAudioEngine::sharedEngine()->preloadEffect("sound/11.ogg");//�ڵ���ը
		SimpleAudioEngine::sharedEngine()->preloadEffect("sound/9.ogg");//���ŵ���
		SimpleAudioEngine::sharedEngine()->preloadEffect("sound/12.ogg");//��ը


		SimpleAudioEngine::sharedEngine()->preloadEffect("sound/3.ogg");//1�Ż���
		SimpleAudioEngine::sharedEngine()->preloadEffect("sound/4.ogg");//2�Ż���
		SimpleAudioEngine::sharedEngine()->preloadEffect("sound/5.ogg");//3�Ż���
		SimpleAudioEngine::sharedEngine()->preloadEffect("sound/6.ogg");//4�Ż���
		SimpleAudioEngine::sharedEngine()->preloadEffect("sound/7.ogg");//5�Ż���
		SimpleAudioEngine::sharedEngine()->preloadEffect("sound/8.ogg");//6�Ż���



		SimpleAudioEngine::sharedEngine()->preloadEffect("sound/27.ogg");//mission_judge.ccbi������������ʱ���߶���ô��
		if(sound){
			if(!SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()){
				SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sound/1.ogg",true);
			}
		}

	}while (0);    
	return true; 

}
void UIlayer::registerWithTouchDispatcher(void)
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, true);
} 

void  UIlayer::playEffect(int index){
	if (sound)
	{
		char  effectName[50];
		sprintf(effectName, "sound/%d.ogg",index); 
		SimpleAudioEngine::sharedEngine()->playEffect(effectName,false);
	}
}
void  UIlayer ::showTableProps(int index){
	CCLOG("showTableProps-----------------===================showTablePropsIndex:%d",showTablePropsIndex);
	showTablePropsIndex++;
	CCSize size= CCDirector::sharedDirector()->getWinSize();

	CCNodeLoaderLibrary *showTableloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	cocos2d::extension::CCBReader *showTableBreader= new cocos2d::extension::CCBReader(showTableloaderlibrary); 
	showTableBreader->autorelease();
	char table[50];
	sprintf(table,"map/text_bfitem%d.ccbi",index);
	CCNode  *showTableNode  = showTableBreader->readNodeGraphFromFile(table,this);

	showTableNode->setPosition(ccp(0,0-50*showTablePropsIndex));
	if(showTableNode!= NULL) {
		this->addChild(showTableNode,12);
	}

	//showTableBreader->getAnimationManager()->runAnimationsForSequenceNamed("1st");
	//showTableBreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(GameScene::showTableEnd)); 

	showTableNode->runAction(CCSequence::create(CCDelayTime::create(3.0f),
		CCCallFuncN::create(this,callfuncN_selector( UIlayer::showTableEnd)),NULL));
}


void  UIlayer::showTableEnd(CCNode* sender){
	//CCLOG("showTableEnd-----------------===================showTablePropsIndex:%d",showTablePropsIndex);
	this->removeChild(sender,-1);

	if(showTablePropsIndex>0){
		showTablePropsIndex--;
	}
}
bool UIlayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	startPoint = pTouch->getLocationInView();
	startPoint =  CCDirector::sharedDirector()->convertToGL(startPoint);//ת��GL

	if(PointInRound(startPoint.x,startPoint.y,(this->getChildByTag(1000))->getPositionX(),(this->getChildByTag(1000))->getPositionY(),100)){//80
		this->getChildByTag(1001)->setVisible(true);
		this->getChildByTag(1001)->setPosition(startPoint);
	}else{
		this->getChildByTag(1001)->setVisible(false);
		return true;
	}

	//��
	if(PointInRound(startPoint.x,startPoint.y,(this->getChildByTag(1000))->getPositionX()-65,(this->getChildByTag(1000))->getPositionY(),30)){
		//CCLog("===========left");
		CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
		if(((GameScene *)scene->getChildByTag(1))->playBody!=NULL){

			tanker* tankPlayer= (tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData());

			((GameScene *)scene->getChildByTag(1))->setPlayerSpeed(-tankPlayer->getMoveSpeed(),0);
			tankPlayer->setState(tanker::moveLeft);
		}
		//��
	}else if(PointInRound(startPoint.x,startPoint.y,(this->getChildByTag(1000))->getPositionX()+65,(this->getChildByTag(1000))->getPositionY(),30)){
		//CCLog("===========right");
		CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
		if(((GameScene *)scene->getChildByTag(1))->playBody!=NULL){
			tanker* tankPlayer= (tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData());

			((GameScene *)scene->getChildByTag(1))->setPlayerSpeed(tankPlayer->getMoveSpeed(),0);
			tankPlayer->setState(tanker::moveRight);
		}
		//��
	}else if(PointInRound(startPoint.x,startPoint.y,(this->getChildByTag(1000))->getPositionX(),(this->getChildByTag(1000))->getPositionY()-65,30)){
		//CCLog("===========down");
		CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
		if(((GameScene *)scene->getChildByTag(1))->playBody!=NULL){
			tanker* tankPlayer= (tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData());

			((GameScene *)scene->getChildByTag(1))->setPlayerSpeed(0,-tankPlayer->getMoveSpeed());
			tankPlayer->setState(tanker::moveDown);
		}
		//��
	}else if(PointInRound(startPoint.x,startPoint.y,(this->getChildByTag(1000))->getPositionX(),(this->getChildByTag(1000))->getPositionY()+65,30)){

		CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
		if(((GameScene *)scene->getChildByTag(1))->playBody!=NULL){
			tanker* tankPlayer= (tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData());

			((GameScene *)scene->getChildByTag(1))->setPlayerSpeed(0,tankPlayer->getMoveSpeed());
			tankPlayer->setState(tanker::moveUp);
		}

		//CCLog("===========up");
	}
	//this->getChildByTag(1000)->setPosition(startPoint);
	//this->getChildByTag(1000)->setVisible(true);
	//this->getChildByTag(1001)->setPosition(startPoint);
	//this->getChildByTag(1001)->setVisible(true);
	return true;
} 



void UIlayer::showGameTime(float  t){

	int  second= (int)(gameTime.getMillis()/1000);
	//CCLog("===========second:%d",second);

	char tm[50];
	sprintf(tm, "%d%d:%d%d",(second/60)/10,(second/60)%10,(second%60)/10,(second%60)%10); 
	CCLabelAtlas* gameTimeLable = (CCLabelAtlas*)(this->getChildByTag(1002))->getChildByTag(1);
	gameTimeLable->setString(tm);

}

void UIlayer::wudiCallback(CCObject* pSender){

	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	paused = true;
	((GameScene *)scene->getChildByTag(1))->pauseSchedulerAndActions();
	menuFireStopCallback(this,CCControlEventTouchDown);

	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCNode *node = (CCNode*)this->getChildByTag(2055);

	gameTime.pause();
	((CCMenu*)this->getChildByTag(1))->setTouchEnabled(false);
	((ImmediateButton*)this->getChildByTag(100))->setTouchEnabled(false);
	if(this->getChildByTag(101)){
		((ImmediateButton*)this->getChildByTag(101))->setTouchEnabled(false);
	}
	if(this->getChildByTag(102)){
		((ImmediateButton*)this->getChildByTag(102))->setTouchEnabled(false);
	}
	((CCMenu*)this->getChildByTag(371))->setEnabled(false);
	((CCMenu*)this->getChildByTag(771))->setEnabled(false);

	yesPayWuDiSecondButton(this);
	/*CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	cocos2d::extension::CCBReader *ccbreader = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
	ccbreader->autorelease();
	//��ȡһ��ccbi���ļ�,����һ��ccnodeʵ��
	CCNode  *animationsTest  = ccbreader->readNodeGraphFromFile("pay/scene_buy_wudi.ccbi",this);//

	if(animationsTest!= NULL){
		this->addChild(animationsTest,10,8791);
	}
	ccbreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(UIlayer::showPayWuDiBotton)); 
	*/
}

 void UIlayer::showPayWuDiBotton(){
	 CCSize size =CCDirector::sharedDirector()->getWinSize();
	 CCNode * node =this->getChildByTag(8791);
	 CCMenuItemImage *yesItem = CCMenuItemImage::create(
		 "UI/queding1.png",
		 "UI/queding2.png",
		 this,
		 menu_selector(UIlayer::yesPayWuDiSecondButton));//yesPayWuDiButton
	 yesItem->setPosition(ccp(size.width/2+120, size.height/2-45));

	 CCMenuItemImage *noItem = CCMenuItemImage::create(
		 "UI/guanbi1.png",
		 "UI/guanbi2.png",
		 this,
		 menu_selector(UIlayer::noPayWuDiButton));
	 noItem->setPosition(ccp(size.width/2+200, size.height/2+120));

	 CCMenu* pMenu = CCMenu::create(yesItem,noItem, NULL);
	 pMenu->setPosition(CCPointZero);
	 node->addChild(pMenu,1,1);
 }

 void UIlayer::yesPayWuDiButton(CCObject* pSender){
	 if(!secondPay){
		 CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		 cocos2d::extension::CCBReader *ccbreaderPay = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
		 CCNode  *payNode  = ccbreaderPay->readNodeGraphFromFile("pay/warning_pay.ccbi",this);
		 ccbreaderPay->autorelease();

		 this->addChild(payNode,100,7510);

		 CCSize size =CCDirector::sharedDirector()->getWinSize();
		 CCMenuItemImage *yesItem = CCMenuItemImage::create(
			 "ccbResources/active/button_yes0.png",
			 "ccbResources/active/button_yes1.png",
			 this,
			 menu_selector(UIlayer::yesPayWuDiSecondButton));
		 yesItem->setPosition(ccp(size.width/2-100, size.height/2-65));

		 CCMenuItemImage *noItem = CCMenuItemImage::create(
			 "ccbResources/active/button_no0.png",
			 "ccbResources/active/button_no1.png",
			 this,
			 menu_selector(UIlayer::noPayWuDiSecondButton));
		 noItem->setPosition(ccp(size.width/2+100, size.height/2-65));

		 CCMenu* pMenu = CCMenu::create(yesItem,noItem, NULL);
		 pMenu->setPosition(CCPointZero);
		 payNode->addChild(pMenu,1,1);

		 secondPay = true;
	 }


 }

 void UIlayer::noPayWuDiButton(CCObject* pSender){
	 ((CCMenu*)this->getChildByTag(371))->setEnabled(true);
	 ((CCMenu*)this->getChildByTag(771))->setEnabled(true);
	 this->removeChildByTag(8791);
	 CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	 paused = false;
	 ((GameScene *)scene->getChildByTag(1))->resumeSchedulerAndActions();

	 gameTime.start();
	 ((CCMenu*)this->getChildByTag(1))->setTouchEnabled(true);
	 ((ImmediateButton*)this->getChildByTag(100))->setTouchEnabled(true);

	 if(this->getChildByTag(102)){
		 ((ImmediateButton*)this->getChildByTag(102))->setTouchEnabled(true);
	 }
	 if(this->getChildByTag(101)){
		 ((ImmediateButton*)this->getChildByTag(101))->setTouchEnabled(true);
	 }
 }

 void UIlayer::yesPayWuDiSecondButton(CCObject* pSender){
	 secondPay = false;
	// this->removeChildByTag(7510,-1);
	 ((CCMenu*)this->getChildByTag(371))->setEnabled(true);
	 ((CCMenu*)this->getChildByTag(771))->setEnabled(true);
  #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
	 billingPointIndex = 10;//�Ʒѵ�
	 SaveBooleanToXML("payResultBool",false);
	 SaveBooleanToXML("payGameOver",false);
	 //payResultBool = false;;
	 //payGameOver = false;
	 this->schedule(schedule_selector(UIlayer::updateBillingPoint), 0.1f);

	 JniMethodInfo minfo;
	 CCLog("============"); 
	 bool isHave = JniHelper::getStaticMethodInfo(minfo,"quyou/game/tank/billing","show_java_dialog", "(I)I");//"com/game/test/JniFruitHelper","show_java_dialog"
	 CCLog("------------"); 
	 jint _int=0;
	 if (isHave) {
		 CCLog("jni-java����ִ�����"); 
		 _int = minfo.env->CallStaticIntMethod(minfo.classID, minfo.methodID,billingPointIndex);//
	 }else{
		 CCLog("jjni:�˺�������"); 
	 }
  #else
	 ((CCMenu*)this->getChildByTag(771))->setVisible(false);
	 CCSize size = CCDirector::sharedDirector()->getWinSize();
	 this->removeChildByTag(8791);

	 CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	 paused = false;
	 ((GameScene *)scene->getChildByTag(1))->resumeSchedulerAndActions();

	 gameTime.start();
	 ((CCMenu*)this->getChildByTag(1))->setTouchEnabled(true);
	 ((ImmediateButton*)this->getChildByTag(100))->setTouchEnabled(true);

	 if(this->getChildByTag(102)){
		 ((ImmediateButton*)this->getChildByTag(102))->setTouchEnabled(true);
	 }
	 if(this->getChildByTag(101)){
		 ((ImmediateButton*)this->getChildByTag(101))->setTouchEnabled(true);
	 }
	
	tanker *tank=  (tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData());
	tank->setPayGod();
  #endif

 }
 void UIlayer::noPayWuDiSecondButton(CCObject* pSender){
	 this->removeChildByTag(7510,-1);
	 secondPay = false;

 }
void UIlayer::setTankCount(int num){
	CCLog("num:%d",num);
	char tm[50];
	sprintf(tm, "%d%d",num/10,num%10); 
	CCLabelAtlas* tankNum = (CCLabelAtlas*)(this->getChildByTag(1002))->getChildByTag(2);
	tankNum->setString(tm);
	
	CCLog("num:%s",tankNum->getString());
}

void UIlayer::setTankLift(int lift){


	/*char tm[50];
	sprintf(tm, "%d%d",lift/10,lift%10); //Ѫ��
	CCLabelAtlas* tankNum = (CCLabelAtlas*)(this->getChildByTag(1002))->getChildByTag(3);
	tankNum->setString(tm);*/
}
void UIlayer::setEnemyCount(int num){
	char tm[50];
	sprintf(tm, "%d%d",num/10,num%10); //Ѫ��
	CCLabelAtlas* tankNum = (CCLabelAtlas*)(this->getChildByTag(1002))->getChildByTag(3);
	tankNum->setString(tm);
}
void UIlayer::keyBackClicked(){
	pauseMenuCallback(this);
}

void UIlayer::tuhaojinCallback(CCObject* pSender){
	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	paused = true;
	((GameScene *)scene->getChildByTag(1))->pauseSchedulerAndActions();
	menuFireStopCallback(this,CCControlEventTouchDown);

	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCNode *node = (CCNode*)this->getChildByTag(2055);

	gameTime.pause();
	((CCMenu*)this->getChildByTag(1))->setTouchEnabled(false);
	((ImmediateButton*)this->getChildByTag(100))->setTouchEnabled(false);
	if(this->getChildByTag(101)){
		((ImmediateButton*)this->getChildByTag(101))->setTouchEnabled(false);
	}
	if(this->getChildByTag(102)){
		((ImmediateButton*)this->getChildByTag(102))->setTouchEnabled(false);
	}
	((CCMenu*)this->getChildByTag(371))->setEnabled(false);
	
	((CCMenu*)this->getChildByTag(771))->setEnabled(false);

	/*CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	cocos2d::extension::CCBReader *ccbreader = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
	ccbreader->autorelease();
	//��ȡһ��ccbi���ļ�,����һ��ccnodeʵ��
	CCNode  *animationsTest  = ccbreader->readNodeGraphFromFile("UI/scene_goldenblock.ccbi",this);//

	if(animationsTest!= NULL){
		this->addChild(animationsTest,10,8791);
	}
	ccbreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(UIlayer::showPay)); 
	*/
	ysePay(this);
}

void UIlayer::showPay(){

	CCSize size =CCDirector::sharedDirector()->getWinSize();
	CCNode * node =this->getChildByTag(8791);
	CCMenuItemImage *yesItem = CCMenuItemImage::create(
		"UI/queding1.png",
		"UI/queding2.png",
		this,
		menu_selector(UIlayer::ysePay));//onShowPaytuhaojinSecondClicked
	yesItem->setPosition(ccp(size.width/2+120, size.height/2-45));

	CCMenuItemImage *noItem = CCMenuItemImage::create(
		"UI/guanbi1.png",
		"UI/guanbi2.png",
		this,
		menu_selector(UIlayer::noPaytuhaojin));
	noItem->setPosition(ccp(size.width/2+200, size.height/2+120));

	CCMenu* pMenu = CCMenu::create(yesItem,noItem, NULL);
	pMenu->setPosition(CCPointZero);
	node->addChild(pMenu,1,1);
}
void UIlayer::onShowPaytuhaojinSecondClicked(CCObject *pSender){

	if(!secondPay){
		CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		cocos2d::extension::CCBReader *ccbreaderPay = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
		CCNode  *payNode  = ccbreaderPay->readNodeGraphFromFile("pay/warning_pay.ccbi",this);
		ccbreaderPay->autorelease();

		this->addChild(payNode,100,7510);

		CCSize size =CCDirector::sharedDirector()->getWinSize();
		CCMenuItemImage *yesItem = CCMenuItemImage::create(
			"ccbResources/active/button_yes0.png",
			"ccbResources/active/button_yes1.png",
			this,
			menu_selector(UIlayer::ysePay));
		yesItem->setPosition(ccp(size.width/2-100, size.height/2-65));

		CCMenuItemImage *noItem = CCMenuItemImage::create(
			"ccbResources/active/button_no0.png",
			"ccbResources/active/button_no1.png",
			this,
			menu_selector(UIlayer::noShowPaytuhaojinSecond));
		noItem->setPosition(ccp(size.width/2+100, size.height/2-65));

		CCMenu* pMenu = CCMenu::create(yesItem,noItem, NULL);
		pMenu->setPosition(CCPointZero);
		payNode->addChild(pMenu,1,1);

		secondPay = true;
	}
}

void UIlayer::noShowPaytuhaojinSecond(CCObject *pSender){
	this->removeChildByTag(7510,-1);
	secondPay = false;
}

void UIlayer::noPaytuhaojin(CCObject* pSender){

	((CCMenu*)this->getChildByTag(371))->setEnabled(true);
	((CCMenu*)this->getChildByTag(771))->setEnabled(true);
	this->removeChildByTag(8791);
	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	paused = false;
	((GameScene *)scene->getChildByTag(1))->resumeSchedulerAndActions();

	gameTime.start();
	((CCMenu*)this->getChildByTag(1))->setTouchEnabled(true);
	((ImmediateButton*)this->getChildByTag(100))->setTouchEnabled(true);

	if(landmineNum!=0&&this->getChildByTag(102)){
		((ImmediateButton*)this->getChildByTag(102))->setTouchEnabled(true);
	}
	if(bombNum!=0&&this->getChildByTag(101)){
		((ImmediateButton*)this->getChildByTag(101))->setTouchEnabled(true);
	}
}


void UIlayer::ysePay(CCObject* pSender){
 secondPay = false;
//this->removeChildByTag(7510,-1);
((CCMenu*)this->getChildByTag(371))->setEnabled(true);
((CCMenu*)this->getChildByTag(771))->setEnabled(true);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
	billingPointIndex = 6;//�Ʒѵ�
	SaveBooleanToXML("payResultBool",false);
	SaveBooleanToXML("payGameOver",false);
	//payResultBool = false;;
	//payGameOver = false;
	this->schedule(schedule_selector(UIlayer::updateBillingPoint), 0.1f);

	JniMethodInfo minfo;
	CCLog("============"); 
	bool isHave = JniHelper::getStaticMethodInfo(minfo,"quyou/game/tank/billing","show_java_dialog", "(I)I");//"com/game/test/JniFruitHelper","show_java_dialog"
	CCLog("------------"); 
	jint _int=0;
	if (isHave) {
		CCLog("jni-java����ִ�����"); 
		_int = minfo.env->CallStaticIntMethod(minfo.classID, minfo.methodID,billingPointIndex);//
	}else{
		CCLog("jjni:�˺�������"); 
	}
#else

	CCSize size = CCDirector::sharedDirector()->getWinSize();
	this->getChildByTag(371)->setVisible(false);
	//---------------------------------����ɹ�-------------------------
	CCNodeLoaderLibrary *burSuccessloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	cocos2d::extension::CCBReader *buySuccessBreader= new cocos2d::extension::CCBReader(burSuccessloaderlibrary); 
	CCNode  *buySccessNode  = buySuccessBreader->readNodeGraphFromFile("shop/UI_box_goumaichenggong.ccbi",this);

	buySccessNode->setPosition(ccp(size.width/2,size.height/2));

	if(buySccessNode  != NULL) {
		this->addChild(buySccessNode,12,20130);
	}
	buySuccessBreader->getAnimationManager()->runAnimationsForSequenceNamed("1st");
	buySuccessBreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(UIlayer::showBuyEnd)); 
	//-----------------------------------------------------------------------------------

	//this->removeChildByTag(8791);
	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	((GameScene *)scene->getChildByTag(1))->ourBaseWall();

	paused = false;
	((GameScene *)scene->getChildByTag(1))->resumeSchedulerAndActions();

	gameTime.start();
	((CCMenu*)this->getChildByTag(1))->setTouchEnabled(true);
	((ImmediateButton*)this->getChildByTag(100))->setTouchEnabled(true);

	if(landmineNum!=0&&this->getChildByTag(102)){
		((ImmediateButton*)this->getChildByTag(102))->setTouchEnabled(true);
	}
	if(bombNum!=0&&this->getChildByTag(101)){
		((ImmediateButton*)this->getChildByTag(101))->setTouchEnabled(true);
	}
#endif
}

extern "C"  {
	
	void Java_quyou_game_tank_billing_success10()
	{
		SaveBooleanToXML("payGameOver",true);
		SaveBooleanToXML("payResultBool",true);
		CCLog("10************ssssssssssssss");
	}

	void Java_quyou_game_tank_billing_fail10()
	{
		SaveBooleanToXML("payGameOver",true);
		SaveBooleanToXML("payResultBool",false);
		CCLog("10************ffffffffffffff");
	}

	void Java_quyou_game_tank_billing_success11()
	{
		SaveBooleanToXML("payGameOver",true);
		SaveBooleanToXML("payResultBool",true);
		CCLog("11************ssssssssssssss");
	}

	void Java_quyou_game_tank_billing_fail11()
	{
		SaveBooleanToXML("payGameOver",true);
		SaveBooleanToXML("payResultBool",false);
		CCLog("11************ffffffffffffff");
	}
	void Java_quyou_game_tank_billing_success12()
	{
		SaveBooleanToXML("payGameOver",true);
		SaveBooleanToXML("payResultBool",true);
		CCLog("12************ssssssssssssss");
	}

	void Java_quyou_game_tank_billing_fail12()
	{
		SaveBooleanToXML("payGameOver",true);
		SaveBooleanToXML("payResultBool",false);
		CCLog("12************ffffffffffffff");
	}
	void Java_quyou_game_tank_billing_success13()
	{
		SaveBooleanToXML("payGameOver",true);
		SaveBooleanToXML("payResultBool",true);
		CCLog("13************ssssssssssssss");
	}

	void Java_quyou_game_tank_billing_fail13()
	{
		SaveBooleanToXML("payGameOver",true);
		SaveBooleanToXML("payResultBool",false);
		CCLog("13************ffffffffffffff");
	}
	void Java_quyou_game_tank_billing_success14()
	{
		SaveBooleanToXML("payGameOver",true);
		SaveBooleanToXML("payResultBool",true);
		CCLog("14************ssssssssssssss");
	}

	void Java_quyou_game_tank_billing_fail14()
	{
		SaveBooleanToXML("payGameOver",true);
		SaveBooleanToXML("payResultBool",false);
		CCLog("14************ffffffffffffff");
	}
	void Java_quyou_game_tank_billing_success16()
	{

		SaveBooleanToXML("payGameOver",true);
		SaveBooleanToXML("payResultBool",true);
		CCLog("8888************ssssssssssssss");
	}

	void Java_quyou_game_tank_billing_fail16()
	{
		SaveBooleanToXML("payGameOver",true);
		SaveBooleanToXML("payResultBool",false);
		CCLog("8888************ffffffffffffff");
	}
}


void UIlayer::updateBillingPoint(float time){
	 #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
	switch (billingPointIndex)
	{
	case 16://��Ϸ����
		{
			if(LoadBooleanFromXML("payGameOver")){
				this->unschedule(schedule_selector(UIlayer::updateBillingPoint));
				if(LoadBooleanFromXML("payResultBool")){//���ѳɹ�
					
					UserRecord *gradeUR = new UserRecord("tank.db","tank");
					int gold = gradeUR->readIntFromSQLite("gold");
					int landmine = gradeUR->readIntFromSQLite("landmine");//����
					int bomb = gradeUR->readIntFromSQLite("bomb");//ը��
					int goldNum = 666;

					gradeUR->saveInt("gold",goldNum+gold);
					gradeUR->saveInt("landmine",landmine+10);
					gradeUR->saveInt("bomb",bomb+5);

					gradeUR->closeDB();
					free(gradeUR);

					mapIndex+=1;
					this->removeChildByTag(8511,-1);
					SaveBooleanToXML("pay0",true);
					CCScene *pScene = GameScene::scene();
					CCDirector::sharedDirector()->replaceScene(pScene);

				}else{
					if(mapIndex<43){
						if(mapIndex%10==9){
							mapIndex+=2;
						}else{
							mapIndex++;
						}
					}
					if(mapIndex%4==0||mapIndex%8==0){//ǿ�ƽ����̵�

						gameSceneMagazine = false;
						//����һ���Զ��ͷŵ�CCNode���ؿ� 
						CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary(); 
						ccNodeLoaderLibrary->registerCCNodeLoader("magazine_frame", magazine_frameCocosBuilderLayerLoader::loader());//

						//����һ���Զ��ͷŵ�CCB��ȡ�ࣨCCBReader�� 
						cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary); 
						ccbReader->autorelease(); 

						//��ȡccbi�ļ���ȡ���ڵ� 
						CCNode * node = ccbReader->readNodeGraphFromFile("shop/magazine_frame.ccbi", this); 

						CCArray* allSeq = ccbReader->getAnimationManager()->getSequences(); 
						CCLog("magazine_frameCocosBuilderLayer============================");

						for (int i=0; i<allSeq->count(); i++)
						{     
							//��ȡ��ÿһ��Seq 
							CCBSequence* everySeq = (CCBSequence*)allSeq->objectAtIndex(i);
							//��ȡÿ�����ж������ڡ������Լ�id 
							//everySeq->getDuration(); 
							CCLog("magazine_frameCocosBuilderLayer===========%s",everySeq->getName()) ; 
							CCLog("magazine_frameCocosBuilderLayer===========%d",everySeq->getSequenceId()) ;
						} 

						CCScene * scene = CCScene::create();
						if(node != NULL) {
							scene->addChild(node);
						}
						scene->setUserObject(ccbReader->getAnimationManager());
						CCDirector::sharedDirector()->replaceScene(scene);

					}else{
						playEffect(10);
						CCScene *pScene = GameScene::scene();
						CCDirector::sharedDirector()->replaceScene(pScene);
					}
				}
			}
		}
		break;
	case 5:
		{
			if(LoadBooleanFromXML("payGameOver")){
				
				if(LoadBooleanFromXML("payResultBool")){//���ѳɹ�
					this->unschedule(schedule_selector(UIlayer::updateBillingPoint));

					CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
					//this->removeChildByTag(8510,-1);
					//this->removeChildByTag(8510,-1);
					//SaveBooleanToXML("pay0",true);

					((GameScene*)scene->getChildByTag(1))->tankCont = 1;

				
					this->setTankCount(((GameScene*)scene->getChildByTag(1))->tankCont);

					UserRecord *gradeUR = new UserRecord("tank.db","tank");
					//int  tankCont = gradeUR->readIntFromSQLite("tankCont");
					gradeUR->saveInt("tankCont",((GameScene*)scene->getChildByTag(1))->tankCont);
					gradeUR->closeDB();
					free(gradeUR);
					CCLog("zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz");
					((GameScene*)scene->getChildByTag(1))->showPayPoint = false;

					if(((UIlayer *)scene->getChildByTag(2))->againPayLiftGame){
						((UIlayer *)scene->getChildByTag(2))->againPayLiftGame = false;
						CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
						CCScene *pScene = GameScene::scene();
						CCDirector::sharedDirector()->replaceScene(pScene);
						
					}
				}
			}
		}
		break;
	case 6://������
		{
			if(LoadBooleanFromXML("payGameOver")){
				this->unschedule(schedule_selector(UIlayer::updateBillingPoint));
				if(LoadBooleanFromXML("payResultBool")){//���ѳɹ�

					secondPay = false;
					this->removeChildByTag(7510,-1);

					CCSize size = CCDirector::sharedDirector()->getWinSize();
					this->getChildByTag(371)->setVisible(false);
					//---------------------------------����ɹ�-------------------------
					CCNodeLoaderLibrary *burSuccessloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
					cocos2d::extension::CCBReader *buySuccessBreader= new cocos2d::extension::CCBReader(burSuccessloaderlibrary); 
					CCNode  *buySccessNode  = buySuccessBreader->readNodeGraphFromFile("shop/UI_box_goumaichenggong.ccbi",this);

					buySccessNode->setPosition(ccp(size.width/2,size.height/2));

					if(buySccessNode  != NULL) {
						this->addChild(buySccessNode,12,20130);
					}
					buySuccessBreader->getAnimationManager()->runAnimationsForSequenceNamed("1st");
					buySuccessBreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(UIlayer::showBuyEnd)); 
					//-----------------------------------------------------------------------------------

					//this->removeChildByTag(8791);
					CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
					((GameScene *)scene->getChildByTag(1))->ourBaseWall();

					paused = false;
					((GameScene *)scene->getChildByTag(1))->resumeSchedulerAndActions();

					gameTime.start();
					((CCMenu*)this->getChildByTag(1))->setTouchEnabled(true);
					((ImmediateButton*)this->getChildByTag(100))->setTouchEnabled(true);

					if(landmineNum!=0&&this->getChildByTag(102)){
						((ImmediateButton*)this->getChildByTag(102))->setTouchEnabled(true);
					}
					if(bombNum!=0&&this->getChildByTag(101)){
						((ImmediateButton*)this->getChildByTag(101))->setTouchEnabled(true);
					}
				}
			}
		}
		break;
	case 12:
		{
			if(LoadBooleanFromXML("payGameOver")){
				this->unschedule(schedule_selector(UIlayer::updateBillingPoint));
				if(LoadBooleanFromXML("payResultBool")){
					CCSize size = CCDirector::sharedDirector()->getWinSize();
					//this->removeChildByTag(8791);
					CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
					paused = false;
					((GameScene *)scene->getChildByTag(1))->resumeSchedulerAndActions();

					gameTime.start();
					((CCMenu*)this->getChildByTag(1))->setTouchEnabled(true);
					((ImmediateButton*)this->getChildByTag(100))->setTouchEnabled(true);

					if(this->getChildByTag(102)){
						((ImmediateButton*)this->getChildByTag(102))->setTouchEnabled(true);
					}
					if(this->getChildByTag(101)){
						((ImmediateButton*)this->getChildByTag(101))->setTouchEnabled(true);
					}

					UserRecord *ur = new UserRecord("tank.db","tank");
					int bomb = ur->readIntFromSQLite("bomb");
					ur->saveInt("bomb",2);
					ur->closeDB();
					free(ur);

					char tm[50];
					sprintf(tm, "%d",2); 
					bombNum = 2;
					CCLabelAtlas* _landmineNum =(CCLabelAtlas*) ((ImmediateButton*)this->getChildByTag(101))->getChildByTag(1);
					_landmineNum->setString(tm);
					/*UserRecord *ur = new UserRecord("tank.db","tank");
					int bomb = ur->readIntFromSQLite("bomb");
					ur->saveInt("bomb",2);
					ur->closeDB();
					free(ur);

					char tm[50];
					sprintf(tm, "%d",2); 
					bombNum = 2;
					CCLabelAtlas* _landmineNum =(CCLabelAtlas*) ((ImmediateButton*)this->getChildByTag(101))->getChildByTag(1);
					_landmineNum->setString(tm);*/
				}
			}
		}
		break;
	case 13:
		{
			if(LoadBooleanFromXML("payGameOver")){
				this->unschedule(schedule_selector(UIlayer::updateBillingPoint));
				if(LoadBooleanFromXML("payResultBool")){

					CCSize size = CCDirector::sharedDirector()->getWinSize();
					this->removeChildByTag(8791);

					CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
					paused = false;
					((GameScene *)scene->getChildByTag(1))->resumeSchedulerAndActions();

					gameTime.start();
					((CCMenu*)this->getChildByTag(1))->setTouchEnabled(true);
					((ImmediateButton*)this->getChildByTag(100))->setTouchEnabled(true);

					if(this->getChildByTag(102)){
						((ImmediateButton*)this->getChildByTag(102))->setTouchEnabled(true);
					}
					if(this->getChildByTag(101)){
						((ImmediateButton*)this->getChildByTag(101))->setTouchEnabled(true);
					}

					UserRecord *ur = new UserRecord("tank.db","tank");
					int bomb = ur->readIntFromSQLite("landmine");
					ur->saveInt("landmine",4);
					ur->closeDB();
					free(ur);

					char tm[50];
					sprintf(tm, "%d",4); 
					landmineNum = 4;
					CCLabelAtlas* _landmineNum =(CCLabelAtlas*) ((ImmediateButton*)this->getChildByTag(102))->getChildByTag(1);
					_landmineNum->setString(tm);
					secondPay = false;
					this->removeChildByTag(8791);
				}
			}
		}
		break;
	case 10:
		{
			if(LoadBooleanFromXML("payGameOver")){
				this->unschedule(schedule_selector(UIlayer::updateBillingPoint));
				if(LoadBooleanFromXML("payResultBool")){
					((CCMenu*)this->getChildByTag(771))->setVisible(false);

					CCSize size = CCDirector::sharedDirector()->getWinSize();
					this->removeChildByTag(8791);

					CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
					paused = false;
					((GameScene *)scene->getChildByTag(1))->resumeSchedulerAndActions();

					gameTime.start();
					((CCMenu*)this->getChildByTag(1))->setTouchEnabled(true);
					((ImmediateButton*)this->getChildByTag(100))->setTouchEnabled(true);

					if(this->getChildByTag(102)){
						((ImmediateButton*)this->getChildByTag(102))->setTouchEnabled(true);
					}
					if(this->getChildByTag(101)){
						((ImmediateButton*)this->getChildByTag(101))->setTouchEnabled(true);
					}

					tanker *tank=  (tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData());
					tank->setPayGod();

					//---------------------------------����ɹ�-------------------------
					CCNodeLoaderLibrary *burSuccessloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
					cocos2d::extension::CCBReader *buySuccessBreader= new cocos2d::extension::CCBReader(burSuccessloaderlibrary); 
					CCNode  *buySccessNode  = buySuccessBreader->readNodeGraphFromFile("shop/UI_box_goumaichenggong.ccbi",this);

					buySccessNode->setPosition(ccp(size.width/2,size.height/2));

					if(buySccessNode  != NULL) {
						this->addChild(buySccessNode,12,20130);
					}
					buySuccessBreader->getAnimationManager()->runAnimationsForSequenceNamed("1st");
					buySuccessBreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(UIlayer::showBuyEnd)); 
					//-----------------------------------------------------------------------------------
				}
			}
		}
		break;

	}
	 #endif
}


void UIlayer::showBuyEnd(){

	this->removeChildByTag(20130,-1);
}
void UIlayer::noPay(CCObject* pSender){
	this->removeChildByTag(8791);
	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	paused = false;
	((GameScene *)scene->getChildByTag(1))->resumeSchedulerAndActions();

	gameTime.start();
	((CCMenu*)this->getChildByTag(1))->setTouchEnabled(true);
	((ImmediateButton*)this->getChildByTag(100))->setTouchEnabled(true);

	if(landmineNum!=0&&this->getChildByTag(102)){
		((ImmediateButton*)this->getChildByTag(102))->setTouchEnabled(true);
	}
	if(bombNum!=0&&this->getChildByTag(101)){
		((ImmediateButton*)this->getChildByTag(101))->setTouchEnabled(true);
	}

}

void UIlayer::pauseMenuCallback(CCObject* pSender){
	//CCDirector::sharedDirector()->end();
	paused = true;
	playEffect(10);
	CCNodeLoaderLibrary *pauseloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	cocos2d::extension::CCBReader *pauseBreader = new cocos2d::extension::CCBReader(pauseloaderlibrary); 

	CCNode  *animationsPause  = pauseBreader->readNodeGraphFromFile("UI/scene_pause.ccbi",this);

	if(animationsPause  != NULL) {     
		this->addChild(animationsPause,2,2055);
	}
	pauseBreader->getAnimationManager()->runAnimationsForSequenceNamed("1st");
	pauseBreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(UIlayer::pauseAddMenu)); 

	//showShopTeach();
	//successMenu();
	//failMenu();
}

void UIlayer::pauseAddMenu(){
	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	
	((GameScene *)scene->getChildByTag(1))->pauseSchedulerAndActions();
	menuFireStopCallback(this,CCControlEventTouchDown);

	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCNode *node = (CCNode*)this->getChildByTag(2055);

	gameTime.pause();
	((CCMenu*)this->getChildByTag(1))->setTouchEnabled(false);
	((CCMenu*)this->getChildByTag(371))->setTouchEnabled(false);
	// ((CCMenu*)this->getChildByTag(771))->setEnabled(false);
	((ImmediateButton*)this->getChildByTag(100))->setTouchEnabled(false);
	if(this->getChildByTag(101)){
		((ImmediateButton*)this->getChildByTag(101))->setTouchEnabled(false);
	}
	if(this->getChildByTag(102)){
		((ImmediateButton*)this->getChildByTag(102))->setTouchEnabled(false);
	}

	CCMenuItemImage *goOnItem = CCMenuItemImage::create(
		"UI/goOnNormal.png",
		"UI/goOnSelect.png",
		this,
		menu_selector(UIlayer::goOnCallback));
	goOnItem->setPosition(ccp(size.width/2, size.height/2+90));

	CCMenuItemImage *mainMenuItem = CCMenuItemImage::create(
		"UI/mainMenuNormal.png",
		"UI/mainMenuSelect.png",
		this,
		menu_selector(UIlayer::mainMenuCallback));
	mainMenuItem->setPosition(ccp(size.width/2, size.height/2));

	CCMenuItemImage *setItem = CCMenuItemImage::create(
		"UI/voice1.png",
		"UI/voice2.png",
		this,
		menu_selector(UIlayer::setCallback));
	setItem->setTag(2);
	setItem->setPosition(ccp(size.width/2, size.height/2-90));

	if(sound){
		setItem->unselected();
	}else{
		setItem->selected();
	}

	CCMenu* pMenu = CCMenu::create(goOnItem, mainMenuItem,setItem,NULL);
	pMenu->setPosition(CCPointZero);
	node->addChild(pMenu,1,1);
}

void UIlayer:: goOnCallback(CCObject* pSender){

	this->removeChildByTag(2055);
	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	paused = false;
	((GameScene *)scene->getChildByTag(1))->resumeSchedulerAndActions();

	gameTime.start();
	((CCMenu*)this->getChildByTag(1))->setTouchEnabled(true);
	((CCMenu*)this->getChildByTag(371))->setTouchEnabled(true);
	 ((CCMenu*)this->getChildByTag(771))->setEnabled(true);
	((ImmediateButton*)this->getChildByTag(100))->setTouchEnabled(true);

	if(this->getChildByTag(102)){
		((ImmediateButton*)this->getChildByTag(102))->setTouchEnabled(true);
	}
	if(this->getChildByTag(101)){
		((ImmediateButton*)this->getChildByTag(101))->setTouchEnabled(true);
	}
}


void UIlayer:: successMenu(){
	if(!gameOver){
		CCScene* scene = CCDirector::sharedDirector()->getRunningScene();

		((CCMenu*)this->getChildByTag(1))->setTouchEnabled(false);
		((CCMenu*)this->getChildByTag(371))->setTouchEnabled(false);

		((ImmediateButton*)this->getChildByTag(100))->setTouchEnabled(false);
		if(this->getChildByTag(101)){
			((ImmediateButton*)this->getChildByTag(101))->setTouchEnabled(false);
		}

		if(this->getChildByTag(102)){
			((ImmediateButton*)this->getChildByTag(102))->setTouchEnabled(false);
		}

		this->scheduleOnce(schedule_selector(UIlayer::showSuccessMenu),2.0f);
		gameOver = true;
	}
}
void UIlayer::showSuccessMenu(float time){
	if(failMenuBool){
		this->unschedule(schedule_selector(UIlayer::menuFireCallback));
		CCScene* scene = CCDirector::sharedDirector()->getRunningScene();

		menuFireStopCallback(this,CCControlEventTouchDown);
		paused = true;
		((GameScene *)scene->getChildByTag(1))->pauseSchedulerAndActions();
		gameTime.pause();

		CCSize size = CCDirector::sharedDirector()->getWinSize();
		CCNodeLoaderLibrary *pauseloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		cocos2d::extension::CCBReader *sucBreader = new cocos2d::extension::CCBReader(pauseloaderlibrary); 

		if(((GameScene *)scene->getChildByTag(1))->playerDeath) {
			passLevel = 1;
		}else{
			passLevel = 2;
		}

		task_Bool = false;
		//-------------------------------����--------------------------------------
		switch (mapIndex)
		{
		case 1:{   
			//����ȫ������
			passLevel +=1;
			task_Bool = true;
			   }
			   break;
		case 2:{//ָ�Ӳ����ܵ��κ��˺�
			if(!((GameScene *)scene->getChildByTag(1))->ourBaseIsHurt){
				passLevel +=1;
				task_Bool = true;
			}
			   }
			   break;
		case 3:
		case 11:
		case 13:
		case 21:
		case 23:
		case 31:
		case 33:

			{//��Ҫ�����˻���
				if(((GameScene *)scene->getChildByTag(1))->playBody!=NULL){
					tanker *tank=  (tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData());
					if(tank->lifeNum>100){
						passLevel +=1;
						task_Bool = true;
					}
				}
			}
			break;
		case 4:
		case 8:
		case 12:
		case 14:
		case 18:
		case 22:
		case 24:
		case 28:
		case 32:
		case 34:
		case 38:
			{//ָ�Ӳ����ܵ��κ��˺�
				if(!((GameScene *)scene->getChildByTag(1))->ourBaseIsHurt){
					passLevel +=1;
					task_Bool = true;
				}
			}
			break;
		case 5:
			{   //2������ȡ��ʤ��
				int  second= (int)(gameTime.getMillis()/1000);
				if(second<120){
					passLevel +=1;
					task_Bool = true;
				}
			}
			break;
		case 6:

			{   //ʹ�÷�ŭ֮����ɱ���
				if(((GameScene *)scene->getChildByTag(1))->playBody!=NULL){
					tanker *tank=  (tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData());
					if(tank->turretKing==2){
						passLevel +=1;
						task_Bool = true;
					}
				}
			}
			break;
		case 7:
			{
				if(((GameScene *)scene->getChildByTag(1))->landmineEnemy>=3){
					passLevel +=1;
					task_Bool = true;
				}
			}
			break;
		case 15:
		case 25:
		case 9:
		case 19:
		case 39:
			{
				//3������ȡ��ʤ��
				int  second= (int)(gameTime.getMillis()/1000);
				if(second<180){
					passLevel +=1;
					task_Bool = true;
				}
			}
			break;
		case 16:
		case 0:
		case 26:
			{//ʹ�ù�����ũ��ɱ���
				if(((GameScene *)scene->getChildByTag(1))->playBody!=NULL){
					tanker *tank=  (tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData());
					if(tank->turretKing==3){
						passLevel +=1;
						task_Bool = true;
					}
				}
			}
			break;
		case 17:
		case 27:
			{//ʹ�õ���ը��10���о�
				if(((GameScene *)scene->getChildByTag(1))->landmineEnemy>=10){
					passLevel +=1;
					task_Bool = true;
				}
			}
			break;
		case 29:
		case 10:
			{
				//ʹ��ս��������ɱ���
				if(((GameScene *)scene->getChildByTag(1))->playBody!=NULL){
					tanker *tank=  (tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData());
					if(tank->turretKing==4){
						passLevel +=1;
						task_Bool = true;
					}
				}
			}
			break;
		case 35:
			{
				//5������ȡ��ʤ��
				int  second= (int)(gameTime.getMillis()/1000);
				if(second<300){
					passLevel +=1;
					task_Bool = true;
				}
			}
			break;
		case 36:
		case 20:
			{
				//ʹ�ù���֮����ɱ���
				if(((GameScene *)scene->getChildByTag(1))->playBody!=NULL){
					tanker *tank=  (tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData());
					if(tank->turretKing==5){
						passLevel +=1;
						task_Bool = true;
					}
				}
			}
			break;
		case 37:
			{
				//ʹ�õ���ը��10���о�
				if(((GameScene *)scene->getChildByTag(1))->landmineEnemy>=20){
					passLevel +=1;
					task_Bool = true;
				}
			}
			break;
		  case 30:
			{
			//ʹ������ƻ���ɱ���
			if(((GameScene *)scene->getChildByTag(1))->playBody!=NULL){
			tanker *tank=  (tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData());
			if(tank->turretKing==6){
			  passLevel +=1;
			  task_Bool = true;
			}
			}
			}
			break;
		}

		playEffect(160+passLevel);

		char tm[50];
		sprintf(tm, "UI/scene_win_%dstars.ccbi",passLevel); 

		CCNode  *animationsSuccess = sucBreader->readNodeGraphFromFile(tm,this);
		animationsSuccess->setAnchorPoint(ccp(0.5f,0.5f));
		animationsSuccess->setPosition(ccp(0,0));

		if(animationsSuccess  != NULL) {     
			this->addChild(animationsSuccess,2,2066);
		}
		addMenuBool = true;
		sucBreader->getAnimationManager()->runAnimationsForSequenceNamed("1st");
		sucBreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(UIlayer::successAddMenu)); 
	}

}
void UIlayer::failMenu(){
	if(!gameOver){
		CCScene* scene = CCDirector::sharedDirector()->getRunningScene();

		((CCMenu*)this->getChildByTag(1))->setTouchEnabled(false);
		((CCMenu*)this->getChildByTag(371))->setTouchEnabled(false);
		((ImmediateButton*)this->getChildByTag(100))->setTouchEnabled(false);
		if(this->getChildByTag(101)){
			((ImmediateButton*)this->getChildByTag(101))->setTouchEnabled(false);
		}

		if(this->getChildByTag(102)){
			((ImmediateButton*)this->getChildByTag(102))->setTouchEnabled(false);
		}

		this->scheduleOnce(schedule_selector(UIlayer::showFailMenu),1.0f);
		gameOver = true;

	}
}

void UIlayer::showFailMenu(float time){
	if(!this->getChildByTag(2066)){

		CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
		paused = true;
		((GameScene *)scene->getChildByTag(1))->pauseSchedulerAndActions();
		gameTime.pause();
		menuFireStopCallback(this,CCControlEventTouchDown);
		this->unschedule(schedule_selector(UIlayer::menuFireCallback));
		playEffect(17);

		((CCMenu*)this->getChildByTag(1))->setTouchEnabled(false);

		CCSize size = CCDirector::sharedDirector()->getWinSize();
		CCNodeLoaderLibrary *pauseloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		cocos2d::extension::CCBReader *sucBreader = new cocos2d::extension::CCBReader(pauseloaderlibrary); 


		((GameScene *)scene->getChildByTag(1))->showPayPoint = false;

		char tm[50];
		if(((GameScene *)scene->getChildByTag(1))->tankCont>0){
			sprintf(tm, "UI/scene_lose%d.ccbi",1); 
		}else{
			sprintf(tm, "UI/scene_lose%d.ccbi",2); 
		}

		CCNode  *animationsSuccess = sucBreader->readNodeGraphFromFile(tm,this);

		animationsSuccess->setAnchorPoint(ccp(0.5f,0.5f));
		animationsSuccess->setPosition(ccp(0,0));

		if(animationsSuccess  != NULL) {     
			this->addChild(animationsSuccess,2,2066);
		}

		sucBreader->getAnimationManager()->runAnimationsForSequenceNamed("1st");
		sucBreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(UIlayer::failAddMenu)); 
	}

}


void UIlayer::successAddMenu(){
	if(addMenuBool){
		addMenuBool = false;

		CCSize size = CCDirector::sharedDirector()->getWinSize();
		char tm[50];
		//��ʾʱ��
		int  second= (int)(gameTime.getMillis()/1000);
		//CCLog("===========second:%d",second);
		sprintf(tm, "%d%d:%d%d",(second/60)/10,(second/60)%10,(second%60)/10,(second%60)%10); 

		CCLabelAtlas* gameTimeLable=CCLabelAtlas::create(tm, "UI/num.png",  18, 18, '0');
		gameTimeLable->setAnchorPoint(ccp(0,0));
		gameTimeLable->setPosition(ccp(220,93));
		(this->getChildByTag(2066))->addChild(gameTimeLable,1,20);

		CCScene* scene = CCDirector::sharedDirector()->getRunningScene();

		int _num = 0;
		int _gold =0 ;

	  if(mapIndex<40){
		CCObject* _object = NULL; 
		CCARRAY_FOREACH(((GameScene *)scene->getChildByTag(1))->enemy_King_Num, _object) {
			int king = ((enemyKingNumDictElement *)_object)->King;
			int num =  ((enemyKingNumDictElement *)_object)->Num;
			//CCLOG("--------------------king:%d",king);
			//CCLOG("--------------------num:%d",num);

			_num+=num;//����̹������
			_gold = num;//��ý��
		}
	  }else{
		  _num = ((GameScene *)scene->getChildByTag(1))->countEnemy_death;
		  _gold = (int)(_num*1.2f);
	  }
		

		char lable[50];
		UserRecord *gradeUR = new UserRecord("tank.db","tank");
		int gold = gradeUR->readIntFromSQLite("gold");
		//gradeUR->saveInt("gold",gold+_gold);//������

		int passCheckNum=gradeUR->readIntFromSQLite("passCheckNum");

		gradeUR->saveInt("enemyNum",((GameScene *)scene->getChildByTag(1))->enemyNum);

		if(mapIndex%10!=9){
			sprintf(lable,"passCheck%d",mapIndex+1);
			gradeUR->saveInt(lable,1);//�ؿ��Ƿ���
		}else{//��������
			// if(mapIndex<10){
			CCLog("(mapIndex/10)*10=%d",(mapIndex/10)*10);
			sprintf(lable,"passCheck%d",(mapIndex/10)*10);
		    gradeUR->saveInt(lable,1);//�׹ؿ���
			sprintf(lable,"passCheck%d",mapIndex+2);
			gradeUR->saveInt(lable,1);

			if(mapIndex==19){
				sprintf(lable,"passCheck%d",40);
				gradeUR->saveInt(lable,1);
			}else if(mapIndex==19){
				sprintf(lable,"passCheck%d",41);
				gradeUR->saveInt(lable,1);
			}else if(mapIndex==29){
				sprintf(lable,"passCheck%d",42);
				gradeUR->saveInt(lable,1);
			}
			//}
		}

		if(mapIndex>=passCheckNum&&mapIndex!=40&&mapIndex!=41&&mapIndex!=42){//������
			gradeUR->saveInt("passCheckNum",mapIndex+1);
			CCLog("1passCheckNum:%d",passCheckNum);
		}
		
		sprintf(lable,"passCheckIndex%d",mapIndex);
		int passCheckIndex =  gradeUR->readIntFromSQLite(lable);

		if(passCheckIndex<passLevel){
			sprintf(lable,"star/state%d.xml",mapIndex);
			XMLParser *pXmlParser = XMLParser::parseWithFile(lable);

			int _gold_num = 0;
			for(int i =passCheckIndex;i<passLevel;i++ ){
				sprintf(lable,"star%d",passCheckIndex+1);
				CCLog("lable:%s",lable);
				CCLog("(pXmlParser->getString(lable))->intValue():%d",(pXmlParser->getString(lable))->intValue());
				_gold_num += (pXmlParser->getString(lable))->intValue();
			}
			_gold+=_gold_num;
			sprintf(lable,"passCheckIndex%d",mapIndex);
			gradeUR->saveInt(lable,passLevel);//�ؿ���
		}

		gradeUR->saveInt("gold",gold+_gold);//������
		CCLog("--------------%d",gradeUR->readIntFromSQLite("tankCont"));

		if(mapIndex>40){
			regainLifeTime.setMillis(15*60*1000);
			regainLifeTime.start();
		}

		gradeUR->closeDB();
		free(gradeUR);

		sprintf(tm, "%d",_num); 
		CCLabelAtlas* enemyNumLable=CCLabelAtlas::create(tm, "UI/num.png",  18, 18, '0');
		enemyNumLable->setAnchorPoint(ccp(0,0));
		enemyNumLable->setPosition(ccp(415,93));
		(this->getChildByTag(2066))->addChild(enemyNumLable,1,21);

		sprintf(tm, "%d",_gold); 
		CCLabelAtlas* getGoldLable=CCLabelAtlas::create(tm, "UI/num.png",  18, 18, '0');
		getGoldLable->setAnchorPoint(ccp(0,0));
		getGoldLable->setPosition(ccp(580,93));
		(this->getChildByTag(2066))->addChild(getGoldLable,1,22);

		//addChild();
		CCMenuItemImage *mainMenuItem = CCMenuItemImage::create(
			"UI/button_menu_0.png",
			"UI/button_menu_1.png",
			this,
			menu_selector(UIlayer::mainMenuCallback));
		mainMenuItem->setPosition(ccp(size.width/2-225,50));

		CCMenuItemImage *shopItem = CCMenuItemImage::create(
			"UI/zhonggongchang1.png",
			"UI/zhonggongchang2.png",
			this,
			menu_selector(UIlayer::shopCallback));
		shopItem->setPosition(ccp(size.width/2-75,50));

		CCMenuItemImage *nextItem = CCMenuItemImage::create(
			"UI/button_nextlv_0.png",
			"UI/button_nextlv_1.png",
			this,
			menu_selector(UIlayer::nextCallback));
		nextItem->setPosition(ccp(size.width/2+225,50));

		CCMenuItemImage *againItem = CCMenuItemImage::create(
			"UI/button_restart_0.png",
			"UI/button_restart_1.png",
			this,
			menu_selector(UIlayer::againCallback));
		againItem->setPosition(ccp(size.width/2+75,50));


		if(mapIndex<40&&mapIndex!=0&&mapIndex!=10&&mapIndex!=20&&mapIndex!=30){
		  CCMenu* pMenu = CCMenu::create(mainMenuItem,shopItem,nextItem,againItem,NULL);

		  pMenu->setPosition(CCPointZero);
		  (this->getChildByTag(2066))->addChild(pMenu,1,1);
		}else{
			CCMenu* pMenu = CCMenu::create(mainMenuItem,shopItem,againItem,NULL);

			pMenu->setPosition(CCPointZero);
			(this->getChildByTag(2066))->addChild(pMenu,1,1);
		}

		//----------------------------�ɾ�-------------------------------------
		if(!LoadBooleanFromXML("showAchieve2")){
			UserRecord *achieveUR = new UserRecord("tank.db","achieveRecord");
			char taskname[30];
			sprintf(taskname,"achieveRecord%d", 2);
			achieveUR->saveInt(taskname,((GameScene *)scene->getChildByTag(1))->enemyNum);

			sprintf(taskname,"achieveRecord%d", 3);
			achieveUR->saveInt(taskname,((GameScene *)scene->getChildByTag(1))->enemyNum);

			sprintf(taskname,"achieveRecord%d", 4);
			achieveUR->saveInt(taskname,((GameScene *)scene->getChildByTag(1))->enemyNum);

			sprintf(taskname,"achieveRecord%d", 7);
			achieveUR->saveInt(taskname,((GameScene *)scene->getChildByTag(1))->enemyNum);

			achieveUR->closeDB();
			free(achieveUR);
		}else if(!LoadBooleanFromXML("showAchieve3")){
			UserRecord *achieveUR = new UserRecord("tank.db","achieveRecord");
			char taskname[30];

			sprintf(taskname,"achieveRecord%d", 3);
			achieveUR->saveInt(taskname,((GameScene *)scene->getChildByTag(1))->enemyNum);

			sprintf(taskname,"achieveRecord%d", 4);
			achieveUR->saveInt(taskname,((GameScene *)scene->getChildByTag(1))->enemyNum);

			sprintf(taskname,"achieveRecord%d", 7);
			achieveUR->saveInt(taskname,((GameScene *)scene->getChildByTag(1))->enemyNum);
			achieveUR->closeDB();
			free(achieveUR);
		}else if(!LoadBooleanFromXML("showAchieve4")){
			UserRecord *achieveUR = new UserRecord("tank.db","achieveRecord");
			char taskname[30];
			sprintf(taskname,"achieveRecord%d", 4);
			achieveUR->saveInt(taskname,((GameScene *)scene->getChildByTag(1))->enemyNum);

			sprintf(taskname,"achieveRecord%d", 7);
			achieveUR->saveInt(taskname,((GameScene *)scene->getChildByTag(1))->enemyNum);
			achieveUR->closeDB();
			free(achieveUR);

		}else if(!LoadBooleanFromXML("showAchieve7")){
			UserRecord *achieveUR = new UserRecord("tank.db","achieveRecord");
			char taskname[30];
			sprintf(taskname,"achieveRecord%d", 7);
			achieveUR->saveInt(taskname,((GameScene *)scene->getChildByTag(1))->enemyNum);
			achieveUR->closeDB();
			free(achieveUR);

		}

		//----------------------��ʾ������ʾ------------------------------------
		if(mapIndex==9){
			char lable[50];
			sprintf(lable,"shopBoxLock%d",2);
			SaveBooleanToXML(lable,true);
			sprintf(lable,"shopBoxLock%d",8);
			SaveBooleanToXML(lable,true);

			CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary(); 

			//����һ���Զ��ͷŵ�CCB��ȡ�ࣨCCBReader�� 
			cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary); 
			ccbReader->autorelease(); 
			CCNode * node = ccbReader->readNodeGraphFromFile("UI/unlock_9.ccbi", this); 
			this->addChild(node,101);

		}else if(mapIndex==19){
			char lable[50];
			sprintf(lable,"shopBoxLock%d",3);
			SaveBooleanToXML(lable,true);
			sprintf(lable,"shopBoxLock%d",9);
			SaveBooleanToXML(lable,true);


			CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary(); 

			//����һ���Զ��ͷŵ�CCB��ȡ�ࣨCCBReader�� 
			cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary); 
			ccbReader->autorelease(); 
			CCNode * node = ccbReader->readNodeGraphFromFile("UI/unlock_19.ccbi", this); 
			this->addChild(node,101);

		}else if(mapIndex==29){
			char lable[50];
			sprintf(lable,"shopBoxLock%d",4);
			SaveBooleanToXML(lable,true);
			sprintf(lable,"shopBoxLock%d",10);
			SaveBooleanToXML(lable,true);

			CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary(); 
			//����һ���Զ��ͷŵ�CCB��ȡ�ࣨCCBReader�� 
			cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary); 
			ccbReader->autorelease(); 
			CCNode * node = ccbReader->readNodeGraphFromFile("UI/unlock_29.ccbi", this); 
			this->addChild(node,101);
		}else if(mapIndex==39){
			char lable[50];
			sprintf(lable,"shopBoxLock%d",5);
			SaveBooleanToXML(lable,true);
			sprintf(lable,"shopBoxLock%d",11);
			SaveBooleanToXML(lable,true);


			CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary(); 

			//����һ���Զ��ͷŵ�CCB��ȡ�ࣨCCBReader�� 
			cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary); 
			ccbReader->autorelease(); 
			CCNode * node = ccbReader->readNodeGraphFromFile("UI/unlock_39.ccbi", this); 
			this->addChild(node,101);
		}




		//---------��ʾ�������--------------------------------------------------
		CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary(); 

		//����һ���Զ��ͷŵ�CCB��ȡ�ࣨCCBReader�� 
		cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary); 
		ccbReader->autorelease(); 

		//��ȡccbi�ļ���ȡ���ڵ�
		CCNode * node = ccbReader->readNodeGraphFromFile("UI/mission_judge.ccbi", this); 
		if(task_Bool){
			ccbReader->getAnimationManager()->runAnimationsForSequenceNamed("1st");
			playEffect(27);
		}else{
			ccbReader->getAnimationManager()->runAnimationsForSequenceNamed("2nd");
			playEffect(27);
		}
		node->setPosition(ccp(size.width/2,size.height/2));
		if (NULL!=node) {
			this->addChild(node,100,1900);
		}


		//����һ���Զ��ͷŵ�CCNode���ؿ� 
		CCNodeLoaderLibrary * ccmissionLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary(); 

		//����һ���Զ��ͷŵ�CCB��ȡ�ࣨCCBReader�� 
		cocos2d::extension::CCBReader * ccmissionReader = new cocos2d::extension::CCBReader(ccmissionLoaderLibrary); 
		ccmissionReader->autorelease(); 
		//��ȡccbi�ļ���ȡ���ڵ�
		if(mapIndex==0||mapIndex==20||mapIndex==30||mapIndex==40||mapIndex==41||mapIndex==42){
			sprintf(lable,"menu/finish%d.ccbi",mapIndex);
		}else{
			sprintf(lable,"menu/mission%d_text.ccbi",mapIndex);
		}

		CCNode * ccmissionnode = ccmissionReader->readNodeGraphFromFile(lable, this); 
		ccmissionnode->setPosition(ccp(size.width/2-50,size.height/2-130));
		if (NULL!=node) {
			this->addChild(ccmissionnode,100,2900);
		}
	}
}

void UIlayer::failAddMenu(){
	if(failMenuBool){

		failMenuBool = false;
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		//addChild();
		CCMenuItemImage *mainMenuItem = CCMenuItemImage::create(
			"UI/button_menu_0.png",
			"UI/button_menu_1.png",
			this,
			menu_selector(UIlayer::mainMenuCallback));
		mainMenuItem->setPosition(ccp(size.width/2-140,100));


		CCMenuItemImage *shopItem = CCMenuItemImage::create(
			"UI/zhonggongchang1.png",
			"UI/zhonggongchang2.png",
			this,
			menu_selector(UIlayer::shopCallback));
		shopItem->setPosition(ccp(size.width/2,100));

		CCMenuItemImage *againItem = CCMenuItemImage::create(
			"UI/button_restart_0.png",
			"UI/button_restart_1.png",
			this,
			menu_selector(UIlayer::againCallback));
		againItem->setPosition(ccp(size.width/2+140,100));


		CCMenu* pMenu = CCMenu::create(mainMenuItem,shopItem,NULL);
		CCScene* scene = CCDirector::sharedDirector()->getRunningScene();

		//if(((GameScene *)scene->getChildByTag(1))->tankCont>0){
		   pMenu->addChild(againItem,1,2);
		//}

		pMenu->setPosition(CCPointZero);
		(this->getChildByTag(2066))->addChild(pMenu,3,1255);


		UserRecord *ur = new UserRecord("tank.db","tank");
		ur->saveInt("enemyNum",((GameScene *)scene->getChildByTag(1))->enemyNum);
		ur->closeDB();
		free(ur);

		if(!LoadBooleanFromXML("showAchieve3")){
			UserRecord *achieveUR = new UserRecord("tank.db","achieveRecord");
			char taskname[30];
			sprintf(taskname,"achieveRecord%d", 3);
			achieveUR->saveInt(taskname,((GameScene *)scene->getChildByTag(1))->enemyNum);

			sprintf(taskname,"achieveRecord%d", 4);
			achieveUR->saveInt(taskname,((GameScene *)scene->getChildByTag(1))->enemyNum);

			sprintf(taskname,"achieveRecord%d", 5);
			achieveUR->saveInt(taskname,((GameScene *)scene->getChildByTag(1))->enemyNum);
			achieveUR->closeDB();
			free(achieveUR);
		}else if(!LoadBooleanFromXML("showAchieve4")){
			UserRecord *achieveUR = new UserRecord("tank.db","achieveRecord");
			char taskname[30];

			sprintf(taskname,"achieveRecord%d", 4);
			achieveUR->saveInt(taskname,((GameScene *)scene->getChildByTag(1))->enemyNum);

			sprintf(taskname,"achieveRecord%d", 5);
			achieveUR->saveInt(taskname,((GameScene *)scene->getChildByTag(1))->enemyNum);
			achieveUR->closeDB();
			free(achieveUR);
		}else if(!LoadBooleanFromXML("showAchieve5")){
			UserRecord *achieveUR = new UserRecord("tank.db","achieveRecord");
			char taskname[30];
			sprintf(taskname,"achieveRecord%d", 5);
			achieveUR->saveInt(taskname,((GameScene *)scene->getChildByTag(1))->enemyNum);
			achieveUR->closeDB();
			free(achieveUR);

		}
	}
}

void UIlayer::nextCallback(CCObject* pSender){
	//CCLog("mapIndex:%d",mapIndex);
	if(mapIndex==1&&!LoadBooleanFromXML("teachShop")){
		showShopTeach();
	}else if(LoadBooleanFromXML("pay0")){//mapIndex==5&&!
		/*CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		cocos2d::extension::CCBReader *ccbreaderPay = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
		CCNode  *payNode  = ccbreaderPay->readNodeGraphFromFile("pay/JHJM.ccbi",this);
		ccbreaderPay->autorelease();

		this->addChild(payNode,100,8511);
		ccbreaderPay->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(UIlayer::showPay_0)); 
		*/
		ysePay_0(this);
	}else{
		if(mapIndex<43){
			if(mapIndex%10==9){
				mapIndex+=2;
			}else{
				mapIndex++;
			}
		}
		if(mapIndex%4==0||mapIndex%8==0){//ǿ�ƽ����̵�

			gameSceneMagazine = false;
			//����һ���Զ��ͷŵ�CCNode���ؿ� 
			CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary(); 
			ccNodeLoaderLibrary->registerCCNodeLoader("magazine_frame", magazine_frameCocosBuilderLayerLoader::loader());//

			//����һ���Զ��ͷŵ�CCB��ȡ�ࣨCCBReader�� 
			cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary); 
			ccbReader->autorelease(); 

			//��ȡccbi�ļ���ȡ���ڵ� 
			CCNode * node = ccbReader->readNodeGraphFromFile("shop/magazine_frame.ccbi", this); 

			CCArray* allSeq = ccbReader->getAnimationManager()->getSequences(); 
			CCLog("magazine_frameCocosBuilderLayer============================");

			for (int i=0; i<allSeq->count(); i++)
			{     
				//��ȡ��ÿһ��Seq 
				CCBSequence* everySeq = (CCBSequence*)allSeq->objectAtIndex(i);
				//��ȡÿ�����ж������ڡ������Լ�id 
				//everySeq->getDuration(); 
				CCLog("magazine_frameCocosBuilderLayer===========%s",everySeq->getName()) ; 
				CCLog("magazine_frameCocosBuilderLayer===========%d",everySeq->getSequenceId()) ;
			} 

			CCScene * scene = CCScene::create();
			if(node != NULL) {
				scene->addChild(node);
			}
			scene->setUserObject(ccbReader->getAnimationManager());
			CCDirector::sharedDirector()->replaceScene(scene);

		}else{
			playEffect(10);
			CCScene *pScene = GameScene::scene();
			CCDirector::sharedDirector()->replaceScene(pScene);
		}
		
	}
}
void UIlayer::showPay_0(){
	CCSize size =CCDirector::sharedDirector()->getWinSize();

	CCNode * node =this->getChildByTag(8511);

	CCMenuItemImage *yesItem = CCMenuItemImage::create(
		"ccbResources/JHJM/jlan.png",
		"ccbResources/JHJM/jlan.png",
		this,
		menu_selector(UIlayer::ysePay_0));//onShowPay_0SecondClicked
	yesItem->setPosition(ccp(size.width/2, size.height/2-200));//ccp(size.width/2-120, size.height/2-70)

	CCMenuItemImage *noItem = CCMenuItemImage::create(
		"UI/guanbi1.png",
		"UI/guanbi2.png",
		this,
		menu_selector(UIlayer::noPay_0));
	noItem->setPosition(ccp(size.width/2+380, size.height/2+210));

	CCMenu* pMenu = CCMenu::create(yesItem, noItem,NULL);//
	pMenu->setPosition(CCPointZero);
	node->addChild(pMenu,1,1);

}

void UIlayer::onShowPay_0SecondClicked(CCObject *pSender){
	if(!secondPay){
		CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		cocos2d::extension::CCBReader *ccbreaderPay = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
		CCNode  *payNode  = ccbreaderPay->readNodeGraphFromFile("pay/warning_pay.ccbi",this);
		ccbreaderPay->autorelease();

		this->addChild(payNode,100,7510);

		CCSize size =CCDirector::sharedDirector()->getWinSize();
		CCMenuItemImage *yesItem = CCMenuItemImage::create(
			"ccbResources/active/button_yes0.png",
			"ccbResources/active/button_yes1.png",
			this,
			menu_selector(UIlayer::ysePay_0));
		yesItem->setPosition(ccp(size.width/2-100, size.height/2-65));

		CCMenuItemImage *noItem = CCMenuItemImage::create(
			"ccbResources/active/button_no0.png",
			"ccbResources/active/button_no1.png",
			this,
			menu_selector(UIlayer::onNoPay_0SecondClicked));
		noItem->setPosition(ccp(size.width/2+100, size.height/2-65));

		CCMenu* pMenu = CCMenu::create(yesItem,noItem, NULL);
		pMenu->setPosition(CCPointZero);
		payNode->addChild(pMenu,1,1);

		secondPay = true;
	}


}
 void UIlayer::onNoPay_0SecondClicked(CCObject *pSender){
	 this->removeChildByTag(7510,-1);
	 secondPay = false;
 }
void UIlayer::ysePay_0(CCObject* pSender){
	//this->removeChildByTag(7510,-1);
	secondPay = false;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
	billingPointIndex = 16;//�Ʒѵ�
	SaveBooleanToXML("payResultBool",false);
	SaveBooleanToXML("payGameOver",false);
	//payResultBool = false;;
	//payGameOver = false;
	this->schedule(schedule_selector(UIlayer::updateBillingPoint), 0.1f);

	JniMethodInfo minfo;
	CCLog("============"); 
	bool isHave = JniHelper::getStaticMethodInfo(minfo,"quyou/game/tank/billing","show_java_dialog", "(I)I");//"com/game/test/JniFruitHelper","show_java_dialog"
	CCLog("------------"); 
	jint _int=0;
	if (isHave) {
		CCLog("jni-java����ִ�����"); 
		_int = minfo.env->CallStaticIntMethod(minfo.classID, minfo.methodID,billingPointIndex);//
	}else{
		CCLog("jjni:�˺�������"); 
	}
#else
	UserRecord *gradeUR = new UserRecord("tank.db","tank");
	int gold = gradeUR->readIntFromSQLite("gold");
	int landmine = gradeUR->readIntFromSQLite("landmine");//����
	int bomb = gradeUR->readIntFromSQLite("bomb");//ը��
	int goldNum = 666;

	gradeUR->saveInt("gold",goldNum+gold);
	gradeUR->saveInt("landmine",landmine+10);
	gradeUR->saveInt("bomb",bomb+5);

	gradeUR->closeDB();
	free(gradeUR);

	mapIndex+=1;
	this->removeChildByTag(8511,-1);
	SaveBooleanToXML("pay0",true);
	CCScene *pScene = GameScene::scene();
	CCDirector::sharedDirector()->replaceScene(pScene);
#endif

	
}
void UIlayer::noPay_0(CCObject* pSender){
	this->removeChildByTag(8511,-1);
}
void UIlayer::showShopTeach(){
	//����һ���Զ��ͷŵ�CCNode���ؿ� 
	CCNodeLoaderLibrary * _ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary(); 

	//����һ���Զ��ͷŵ�CCB��ȡ�ࣨCCBReader�� 
	cocos2d::extension::CCBReader * _ccbReader = new cocos2d::extension::CCBReader(_ccNodeLoaderLibrary); 
	_ccbReader->autorelease(); 

	CCNode * _node = _ccbReader->readNodeGraphFromFile("teach/MainMenuScene_1.ccbi", this); 


	//����һ���Զ��ͷŵ�CCNode���ؿ� 
	CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary(); 
	ccNodeLoaderLibrary->registerCCNodeLoader("instruction_201", shopTeach_1Loader::loader());//

	//����һ���Զ��ͷŵ�CCB��ȡ�ࣨCCBReader�� 
	cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary); 
	ccbReader->autorelease(); 

	//��ȡccbi�ļ���ȡ���ڵ� 
	CCNode * node = ccbReader->readNodeGraphFromFile("teach/instruction_201.ccbi", this); 

	CCScene *scene = CCScene::create();
	scene->addChild(_node);
	scene->addChild(node);


	CCDirector::sharedDirector()->replaceScene(scene);
}
void UIlayer::shopCallback(CCObject* pSender){
	playEffect(10);
	//gameScene = false;
	CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary(); 
	ccNodeLoaderLibrary->registerCCNodeLoader("CCLayer_shop", ShopCocosBuilderLayerLoader::loader());//

	//����һ���Զ��ͷŵ�CCB��ȡ�ࣨCCBReader�� 
	cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary); 
	ccbReader->autorelease(); 

	//��ȡccbi�ļ���ȡ���ڵ� 
	CCNode * node = ccbReader->readNodeGraphFromFile("shop/scene_shop.ccbi", this); 

	CCArray* allSeq = ccbReader->getAnimationManager()->getSequences(); 
	CCLog("MenuCocosBuilderLayer============================%d",gameScene);

	for (int i=0; i<allSeq->count(); i++)
	{     
		//��ȡ��ÿһ��Seq 
		CCBSequence* everySeq = (CCBSequence*)allSeq->objectAtIndex(i);
		//��ȡÿ�����ж������ڡ������Լ�id 
		//everySeq->getDuration(); 
		CCLog("MenuCocosBuilderLayer===========%s",everySeq->getName()) ; 
		CCLog("MenuCocosBuilderLayer===========%d",everySeq->getSequenceId()) ;
	} 
	
	CCScene * scene = CCScene::create();

	CCNodeLoaderLibrary *backloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	cocos2d::extension::CCBReader *backBreader= new cocos2d::extension::CCBReader(backloaderlibrary); 
	backBreader->autorelease(); 
	CCNode  *backNode  = backBreader->readNodeGraphFromFile("shop/scene_shop_background.ccbi",this);
	backNode->setPosition(ccp(0,0));
	scene->addChild(backNode,0,12300);
	if(node != NULL) {
		scene->addChild(node);
	}
	//CCDirector::sharedDirector()->replaceScene(scene);
	CCDirector::sharedDirector()->pushScene(scene);
	
	
}
void UIlayer::againCallback(CCObject* pSender){
	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	if(((GameScene *)scene->getChildByTag(1))->tankCont>0){
		CCScene *pScene = GameScene::scene();
		CCDirector::sharedDirector()->replaceScene(pScene);
	}else{//���� �Ʒѵ�
		//ysePay(this);
		//((GameScene *)scene->getChildByTag(1))->showPayEnd();
		if(!againPayLiftGame){
			/*CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
			cocos2d::extension::CCBReader *ccbreaderPay = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
			CCNode  *payNode  = ccbreaderPay->readNodeGraphFromFile("pay/warning_lifebuy.ccbi",this);
			ccbreaderPay->autorelease();

			CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
			((UIlayer *)scene->getChildByTag(2))->addChild(payNode,100,8510);

			//this->addChild(payNode,100,8510);
			ccbreaderPay->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(GameScene::showPayEnd)); 
			*/
			ysePay(this);
			againPayLiftGame = true;

		}
		
	}

}

void UIlayer:: mainMenuCallback(CCObject* pSender){
	playEffect(10);

	/*UserRecord *tankUR = new UserRecord("tank.db","tank");
	CCLog("------------%d",tankUR->readIntFromSQLite("tankCont"));
	tankUR->closeDB();
	free(tankUR);*/

	if(!showReturnMainMenu){
		showReturnMainMenu = true;
		//	((CCMenu *)this->getChildByTag(1))->setTouchEnabled(false);

		CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		cocos2d::extension::CCBReader *ccbreader = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
		ccbreader->autorelease();
		//��ȡһ��ccbi���ļ�,����һ��ccnodeʵ��
		CCNode  *animationsTest  = ccbreader->readNodeGraphFromFile("UI/warning_backtomenu.ccbi",this);//

		if(animationsTest!= NULL){
			this->addChild(animationsTest,1000,8790);
		}
		ccbreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(UIlayer::showBackMenu)); 
	}


}
void UIlayer::showBackMenu(){
	CCSize size =CCDirector::sharedDirector()->getWinSize();

	CCNode * node =this->getChildByTag(8790);


	CCMenuItemImage *yesItem = CCMenuItemImage::create(
		"ccbResources/active/button_yes0.png",
		"ccbResources/active/button_yes1.png",
		this,
		menu_selector(UIlayer::yse));
	yesItem->setPosition(ccp(size.width/2-80, size.height/2-20));

	CCMenuItemImage *noItem = CCMenuItemImage::create(
		"ccbResources/active/button_no0.png",
		"ccbResources/active/button_no1.png",
		this,
		menu_selector(UIlayer::no));
	noItem->setPosition(ccp(size.width/2+80, size.height/2-20));

	CCMenu* pMenu = CCMenu::create(yesItem,noItem, NULL);
	pMenu->setPosition(CCPointZero);
	node->addChild(pMenu,1,1);

}

void UIlayer::yse(CCObject* pSender){
	showReturnMainMenu = false;
	this->removeChildByTag(8790);

	CCScene *scene = CCScene::create();
	MenuLayer *node = MenuLayer::create();
	scene->addChild(node);
	CCDirector::sharedDirector()->replaceScene(scene);
}

void UIlayer::no(CCObject* pSender){
	showReturnMainMenu = false;
	//((CCMenu *)this->getChildByTag(4567))->setTouchEnabled(true);
	this->removeChildByTag(8790);
}

void UIlayer:: setCallback(CCObject* pSender){
	
	if(LoadBooleanFromXML("sound")){
		SaveBooleanToXML("sound",false);
		((CCMenuItemImage*)(this->getChildByTag(2055)->getChildByTag(1))->getChildByTag(2))->selected();
		sound = LoadBooleanFromXML("sound");

		if(SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()){
			SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
		}

	}else{
		SaveBooleanToXML("sound",true);
		
		((CCMenuItemImage*)(this->getChildByTag(2055)->getChildByTag(1))->getChildByTag(2))->unselected();
		sound = LoadBooleanFromXML("sound");

		if(sound){
			if(!SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()){
				SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sound/1.ogg",true);
			}
		}
	}
	playEffect(10);
	/*((CCNode*)this->getChildByTag(2055))->setVisible(false);

	CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary(); 
	ccNodeLoaderLibrary->registerCCNodeLoader("scene_option", optionLoader::loader());//
	//ccNodeLoaderLibrary->registerCCNodeLoader("scene_achievement", optionLoader::loader());//

	//����һ���Զ��ͷŵ�CCB��ȡ�ࣨCCBReader�� 
	cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary); 
	ccbReader->autorelease(); 
	//��ȡccbi�ļ���ȡ���ڵ� 
	CCNode * node = ccbReader->readNodeGraphFromFile("scene_option.ccbi", this); //scene_option.ccbi

	CCArray* allSeq = ccbReader->getAnimationManager()->getSequences(); 
	CCLog("MenuCocosBuilderLayer============================") ;
	for (int i=0; i<allSeq->count(); i++)
	{     
		//��ȡ��ÿһ��Seq 
		CCBSequence* everySeq = (CCBSequence*)allSeq->objectAtIndex(i);
		//��ȡÿ�����ж������ڡ������Լ�id 
		// everySeq->getDuration(); 
		CCLog("MenuCocosBuilderLayer===========%s",everySeq->getName()) ; 
		CCLog("MenuCocosBuilderLayer===========%d",everySeq->getSequenceId()) ;

	} 
	// CCScene * scene = CCScene::create();

	CCSize size = CCDirector::sharedDirector()->getWinSize();
	//CCSprite *back = CCSprite::create("ccbResources/back.png");
	//back->setPosition(ccp(size.width/2,size.height/2));
	//scene->addChild(back);
	if(node != NULL) {
		// scene->addChild(node);
		this->addChild(node,10,986);
	}

	CCSpriteFrameCache *UI_mission=CCSpriteFrameCache::sharedSpriteFrameCache();
	UI_mission->addSpriteFramesWithFile("ccbResources/UI_mission.plist");
	CCMenuItemSprite *backMenu= CCMenuItemSprite::create(CCSprite::createWithSpriteFrame(UI_mission->spriteFrameByName("button_back.png")),CCSprite::createWithSpriteFrame(UI_mission->spriteFrameByName("button_back_1.png"))
		,this,menu_selector(UIlayer::menuCloseCallback));
	backMenu->setPosition(ccp(size.width-80,30));

	CCMenu* menu = CCMenu::create(backMenu,NULL);
	menu->setPosition(CCPointZero);
	node->addChild(menu, 101,201);*/

	//CCLog("hhhhhhhhhhhhhhhhhhhhhh");
	//((MenuLayer*)this->getParent())->setTouchEnabled(false);

	// CCDirector::sharedDirector()->pushScene(scene);
	//Button_act->setTouchEnabled(false);
	//act1->setEnabled(false);
	//act2->setEnabled(false);
	//act3->setEnabled(false);
	//act4->setEnabled(false);
}
void UIlayer::menuCloseCallback(CCObject* pSender){
	((CCNode*)this->getChildByTag(2055))->setVisible(true);
	this->removeChildByTag(986);
}
void UIlayer::menulandmineEventCallback(CCObject *sender, CCControlEvent event){

	menulandmineCallback(sender);
}
void UIlayer::menulandmineCallback(CCObject* pSender){


	if (landmineNum>0)
	{
		playEffect(9);
		landmineNum--;

		char tm[50];
		sprintf(tm, "%d",landmineNum); 

		CCLabelAtlas* _landmineNum =(CCLabelAtlas*) ((ImmediateButton*)pSender)->getChildByTag(1);
		_landmineNum->setString(tm);

		/*if (landmineNum==0)
		{
			((ImmediateButton*)pSender)->setTouchEnabled(false);
		}*/

		UserRecord *gradeUR = new UserRecord("tank.db","tank");
		gradeUR->saveInt("landmine",landmineNum);
		gradeUR->closeDB();
		free(gradeUR);

		CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
		((GameScene *)scene->getChildByTag(1))->createLandMing();//��ʼ����
	}else{
		payLandmine();
	}
}


void UIlayer::payLandmine(){
	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	paused = true;
	((GameScene *)scene->getChildByTag(1))->pauseSchedulerAndActions();
	menuFireStopCallback(this,CCControlEventTouchDown);

	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCNode *node = (CCNode*)this->getChildByTag(2055);

	gameTime.pause();
	((CCMenu*)this->getChildByTag(1))->setTouchEnabled(false);
	((ImmediateButton*)this->getChildByTag(100))->setTouchEnabled(false);
	if(this->getChildByTag(101)){
		((ImmediateButton*)this->getChildByTag(101))->setTouchEnabled(false);
	}
	if(this->getChildByTag(102)){
		((ImmediateButton*)this->getChildByTag(102))->setTouchEnabled(false);
	}
	((CCMenu*)this->getChildByTag(371))->setEnabled(false);

	CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	cocos2d::extension::CCBReader *ccbreader = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
	ccbreader->autorelease();
	//��ȡһ��ccbi���ļ�,����һ��ccnodeʵ��
	CCNode  *animationsTest  = ccbreader->readNodeGraphFromFile("pay/scene_buy_mine.ccbi",this);//

	if(animationsTest!= NULL){
		this->addChild(animationsTest,10,8791);
	}
	ccbreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(UIlayer::showPayLandmineButton)); 


}
 void UIlayer::showPayLandmineButton(){
	 CCSize size =CCDirector::sharedDirector()->getWinSize();
	 CCNode * node =this->getChildByTag(8791);
	 CCMenuItemImage *yesItem = CCMenuItemImage::create(
		 "UI/queding1.png",
		 "UI/queding2.png",
		 this,
		 menu_selector(UIlayer::yesPayLandmineSecondButton));//yesPayLandmineButton
	 yesItem->setPosition(ccp(size.width/2, size.height/2-45));

	 CCMenuItemImage *noItem = CCMenuItemImage::create(
		 "UI/guanbi1.png",
		 "UI/guanbi2.png",
		 this,
		 menu_selector(UIlayer::noPayLandmineButton));
	 noItem->setPosition(ccp(size.width/2+240, size.height/2+120));

	 CCMenu* pMenu = CCMenu::create(yesItem,noItem, NULL);
	 pMenu->setPosition(CCPointZero);
	 node->addChild(pMenu,1,1);
 }

 void UIlayer::yesPayLandmineButton(CCObject* pSender){
	 if(!secondPay){
		 CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		 cocos2d::extension::CCBReader *ccbreaderPay = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
		 CCNode  *payNode  = ccbreaderPay->readNodeGraphFromFile("pay/warning_pay.ccbi",this);
		 ccbreaderPay->autorelease();

		 this->addChild(payNode,100,7510);

		 CCSize size =CCDirector::sharedDirector()->getWinSize();
		 CCMenuItemImage *yesItem = CCMenuItemImage::create(
			 "ccbResources/active/button_yes0.png",
			 "ccbResources/active/button_yes1.png",
			 this,
			 menu_selector(UIlayer::yesPayLandmineSecondButton));
		 yesItem->setPosition(ccp(size.width/2-100, size.height/2-65));

		 CCMenuItemImage *noItem = CCMenuItemImage::create(
			 "ccbResources/active/button_no0.png",
			 "ccbResources/active/button_no1.png",
			 this,
			 menu_selector(UIlayer::noPayLandmineSecondButton));
		 noItem->setPosition(ccp(size.width/2+100, size.height/2-65));

		 CCMenu* pMenu = CCMenu::create(yesItem,noItem, NULL);
		 pMenu->setPosition(CCPointZero);
		 payNode->addChild(pMenu,1,1);

		 secondPay = true;
	 }

 }


 void UIlayer::noPayLandmineButton(CCObject* pSender){
	 
	 ((CCMenu*)this->getChildByTag(371))->setEnabled(true);

	 this->removeChildByTag(8791);
	 CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	 paused = false;
	 ((GameScene *)scene->getChildByTag(1))->resumeSchedulerAndActions();

	 gameTime.start();
	 ((CCMenu*)this->getChildByTag(1))->setTouchEnabled(true);
	 ((ImmediateButton*)this->getChildByTag(100))->setTouchEnabled(true);

	 if(this->getChildByTag(102)){
		 ((ImmediateButton*)this->getChildByTag(102))->setTouchEnabled(true);
	 }
	 if(this->getChildByTag(101)){
		 ((ImmediateButton*)this->getChildByTag(101))->setTouchEnabled(true);
	 }
 }

 void UIlayer::yesPayLandmineSecondButton(CCObject* pSender){
	 secondPay = false;
	 //this->removeChildByTag(7510,-1);
	 ((CCMenu*)this->getChildByTag(371))->setEnabled(true);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
	 billingPointIndex = 13;//�Ʒѵ�
	 SaveBooleanToXML("payResultBool",false);
	 SaveBooleanToXML("payGameOver",false);
	 //payResultBool = false;;
	 //payGameOver = false;
	 this->schedule(schedule_selector(UIlayer::updateBillingPoint), 0.1f);

	 JniMethodInfo minfo;
	 CCLog("============"); 
	 bool isHave = JniHelper::getStaticMethodInfo(minfo,"quyou/game/tank/billing","show_java_dialog", "(I)I");//"com/game/test/JniFruitHelper","show_java_dialog"
	 CCLog("------------"); 
	 jint _int=0;
	 if (isHave) {
		 CCLog("jni-java����ִ�����"); 
		 _int = minfo.env->CallStaticIntMethod(minfo.classID, minfo.methodID,billingPointIndex);//
	 }else{
		 CCLog("jjni:�˺�������"); 
	 }
#else
	 CCSize size = CCDirector::sharedDirector()->getWinSize();
	 this->removeChildByTag(8791);

	  CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	 paused = false;
	 ((GameScene *)scene->getChildByTag(1))->resumeSchedulerAndActions();

	 gameTime.start();
	 ((CCMenu*)this->getChildByTag(1))->setTouchEnabled(true);
	 ((ImmediateButton*)this->getChildByTag(100))->setTouchEnabled(true);

	 if(this->getChildByTag(102)){
		 ((ImmediateButton*)this->getChildByTag(102))->setTouchEnabled(true);
	 }
	 if(this->getChildByTag(101)){
		 ((ImmediateButton*)this->getChildByTag(101))->setTouchEnabled(true);
	 }

	 UserRecord *ur = new UserRecord("tank.db","tank");
	 int bomb = ur->readIntFromSQLite("landmine");
	 ur->saveInt("landmine",4);
	 ur->closeDB();
	 free(ur);

	 char tm[50];
	 sprintf(tm, "%d",4); 
	 landmineNum = 4;
	 CCLabelAtlas* _landmineNum =(CCLabelAtlas*) ((ImmediateButton*)this->getChildByTag(102))->getChildByTag(1);
	 _landmineNum->setString(tm);
  #endif


 }

 void UIlayer::noPayLandmineSecondButton(CCObject* pSender){
	 this->removeChildByTag(7510,-1);
	 secondPay = false;
 }


void UIlayer::menuBombEventCallback(cocos2d::CCObject *sender,  CCControlEvent event){
	menuBombCallback(sender);
}
void UIlayer::menuBombCallback(CCObject* pSender){
	if (bombNum>0)
	{
		playEffect(12);
		bombNum--;

		char tm[50];
		sprintf(tm, "%d",bombNum); 

		CCLabelAtlas* _bombNum =(CCLabelAtlas*) ((ImmediateButton*)pSender)->getChildByTag(1);
		_bombNum->setString(tm);

		/*if (bombNum==0)
		{
			((ImmediateButton*)pSender)->setTouchEnabled(false);
		}*/

		CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
		((GameScene *)scene->getChildByTag(1))->bomBeEnemy();//���˱�ը

		UserRecord *gradeUR = new UserRecord("tank.db","tank");
		gradeUR->saveInt("bomb",bombNum);
		gradeUR->closeDB();
		free(gradeUR);
	}else{
		 payBomb();
	}
}

void UIlayer::payBomb(){

	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	paused = true;
	((GameScene *)scene->getChildByTag(1))->pauseSchedulerAndActions();
	menuFireStopCallback(this,CCControlEventTouchDown);

	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCNode *node = (CCNode*)this->getChildByTag(2055);

	gameTime.pause();
	((CCMenu*)this->getChildByTag(1))->setTouchEnabled(false);
	((ImmediateButton*)this->getChildByTag(100))->setTouchEnabled(false);
	if(this->getChildByTag(101)){
		((ImmediateButton*)this->getChildByTag(101))->setTouchEnabled(false);
	}
	if(this->getChildByTag(102)){
		((ImmediateButton*)this->getChildByTag(102))->setTouchEnabled(false);
	}
	((CCMenu*)this->getChildByTag(371))->setEnabled(false);

	/*CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	cocos2d::extension::CCBReader *ccbreader = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
	ccbreader->autorelease();
	//��ȡһ��ccbi���ļ�,����һ��ccnodeʵ��
	CCNode  *animationsTest  = ccbreader->readNodeGraphFromFile("pay/scene_buy_bomb.ccbi",this);//

	if(animationsTest!= NULL){
		this->addChild(animationsTest,10,8791);
	}
	ccbreader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(UIlayer::showPayBombButton)); */
	yesPayBomSecondButton(this);
}

void UIlayer::showPayBombButton(){

	CCSize size =CCDirector::sharedDirector()->getWinSize();
	CCNode * node =this->getChildByTag(8791);
	CCMenuItemImage *yesItem = CCMenuItemImage::create(
		"UI/queding1.png",
		"UI/queding2.png",
		this,
		menu_selector(UIlayer::yesPayBomSecondButton));// yesPayBomButton
	yesItem->setPosition(ccp(size.width/2, size.height/2-45));

	CCMenuItemImage *noItem = CCMenuItemImage::create(
		"UI/guanbi1.png",
		"UI/guanbi2.png",
		this,
		menu_selector(UIlayer::noPayBomButton));
	noItem->setPosition(ccp(size.width/2+240, size.height/2+120));

	CCMenu* pMenu = CCMenu::create(yesItem,noItem, NULL);
	pMenu->setPosition(CCPointZero);
	node->addChild(pMenu,1,1);

}

void UIlayer::yesPayBomButton(CCObject* pSender){
	if(!secondPay){
		CCNodeLoaderLibrary *ccnodeloaderlibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		cocos2d::extension::CCBReader *ccbreaderPay = new cocos2d::extension::CCBReader(ccnodeloaderlibrary); 
		CCNode  *payNode  = ccbreaderPay->readNodeGraphFromFile("pay/warning_pay.ccbi",this);
		ccbreaderPay->autorelease();

		this->addChild(payNode,100,7510);

		CCSize size =CCDirector::sharedDirector()->getWinSize();
		CCMenuItemImage *yesItem = CCMenuItemImage::create(
			"ccbResources/active/button_yes0.png",
			"ccbResources/active/button_yes1.png",
			this,
			menu_selector(UIlayer::yesPayBomSecondButton));
		yesItem->setPosition(ccp(size.width/2-100, size.height/2-65));

		CCMenuItemImage *noItem = CCMenuItemImage::create(
			"ccbResources/active/button_no0.png",
			"ccbResources/active/button_no1.png",
			this,
			menu_selector(UIlayer::noPayBomSecondButton));
		noItem->setPosition(ccp(size.width/2+100, size.height/2-65));

		CCMenu* pMenu = CCMenu::create(yesItem,noItem, NULL);
		pMenu->setPosition(CCPointZero);
		payNode->addChild(pMenu,1,1);

		secondPay = true;
	}


}

void UIlayer::yesPayBomSecondButton(CCObject* pSender){

	secondPay = false;
	//this->removeChildByTag(7510,-1);
	((CCMenu*)this->getChildByTag(371))->setEnabled(true);

  #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)  
	billingPointIndex = 12;//�Ʒѵ�
	SaveBooleanToXML("payResultBool",false);
	SaveBooleanToXML("payGameOver",false);
	//payResultBool = false;;
	//payGameOver = false;
	this->schedule(schedule_selector(UIlayer::updateBillingPoint), 0.1f);

	JniMethodInfo minfo;
	CCLog("============"); 
	bool isHave = JniHelper::getStaticMethodInfo(minfo,"quyou/game/tank/billing","show_java_dialog", "(I)I");//"com/game/test/JniFruitHelper","show_java_dialog"
	CCLog("------------"); 
	jint _int=0;
	if (isHave) {
		CCLog("jni-java����ִ�����"); 
		_int = minfo.env->CallStaticIntMethod(minfo.classID, minfo.methodID,billingPointIndex);//
	}else{
		CCLog("jjni:�˺�������"); 
	}
 #else
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	//this->removeChildByTag(8791);
	 CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	paused = false;
	((GameScene *)scene->getChildByTag(1))->resumeSchedulerAndActions();

	gameTime.start();
	((CCMenu*)this->getChildByTag(1))->setTouchEnabled(true);
	((ImmediateButton*)this->getChildByTag(100))->setTouchEnabled(true);

	if(this->getChildByTag(102)){
		((ImmediateButton*)this->getChildByTag(102))->setTouchEnabled(true);
	}
	if(this->getChildByTag(101)){
		((ImmediateButton*)this->getChildByTag(101))->setTouchEnabled(true);
	}

	UserRecord *ur = new UserRecord("tank.db","tank");
	int bomb = ur->readIntFromSQLite("bomb");
	ur->saveInt("bomb",2);
	ur->closeDB();
	free(ur);

	char tm[50];
	sprintf(tm, "%d",2); 
	bombNum = 2;
	CCLabelAtlas* _landmineNum =(CCLabelAtlas*) ((ImmediateButton*)this->getChildByTag(101))->getChildByTag(1);
	_landmineNum->setString(tm);
  #endif
}
void UIlayer::noPayBomSecondButton(CCObject* pSender){
	this->removeChildByTag(7510,-1);
	secondPay = false;
}

void UIlayer::noPayBomButton(CCObject* pSender){
	((CCMenu*)this->getChildByTag(371))->setEnabled(true);

	this->removeChildByTag(8791);
	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	paused = false;
	((GameScene *)scene->getChildByTag(1))->resumeSchedulerAndActions();

	gameTime.start();
	((CCMenu*)this->getChildByTag(1))->setTouchEnabled(true);
	((ImmediateButton*)this->getChildByTag(100))->setTouchEnabled(true);

	if(this->getChildByTag(102)){
		((ImmediateButton*)this->getChildByTag(102))->setTouchEnabled(true);
	}
	if(this->getChildByTag(101)){
		((ImmediateButton*)this->getChildByTag(101))->setTouchEnabled(true);
	}
}

void UIlayer::menuFireEventCallback(cocos2d::CCObject *sender,  CCControlEvent event){

	float delayTime = 0.001f;
	if(fireTime.getSurplusMillis()>0){
		delayTime = fireTime.getSurplusMillis()/1000.0f;
	}
	//CCLOG("222fireTime.getSurplusMillis():%d",fireTime.getSurplusMillis());
	//CCLOG("rateOfFire:%f",rateOfFire);
	this->schedule(schedule_selector(UIlayer::menuFireCallback),rateOfFire,kCCRepeatForever,delayTime);
	isFireBool = true;
	//menuFireCallback(sender);
}
void UIlayer::menuFireStopCallback(cocos2d::CCObject *sender,  CCControlEvent event){
	this->unschedule(schedule_selector(UIlayer::menuFireCallback));
	isFireBool = false;
}
void UIlayer::menuFireCallback(float t){
	fireTime.reset();
	fireTime.start();

	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	if(((GameScene *)scene->getChildByTag(1))->playBody!=NULL){

		tanker* tank = ((tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData()));
		tank->openFire();

		playEffect(tank->turretKing+2);
		/* vector<b2Body *> _vector= ((GameScene *)scene->getChildByTag(1))->tankerBulletArray;
		vector<b2Body *>::iterator pos2;
		for(pos2 = _vector.begin(); pos2 != _vector.end();) {
		b2Body *body = *pos2;
		if (body->GetUserData() != NULL){
		Bullet *tankBullet = ((Bullet*)body->GetUserData());
		if(tankBullet->bulletState==tankBullet->END){
		tankBullet->setState(tankBullet->FLYING);

		body->SetLinearVelocity(b2Vec2(0/PTM_RATIO,200/PTM_RATIO));
		break;
		} 
		}
		pos2++;
		}*/
		bool _changeTankBool = ((GameScene *)scene->getChildByTag(1))->changeTankBool;

		CCArray * _array= _changeTankBool?((GameScene *)scene->getChildByTag(1))->tankerBulletArray6:((GameScene *)scene->getChildByTag(1))->tankerBulletArray;
		CCObject* _object = NULL; 

		int _turretId=_changeTankBool?6:((GameScene *)scene->getChildByTag(1))->turretId;

		int bulletNum = 1;

		if(_turretId==2||_turretId==3){
			bulletNum = 2;
		}

		int index = 0;//������

		CCARRAY_FOREACH(_array, _object) {
			Bullet *tankBullet = (Bullet*)_object;
			if(tankBullet->bulletState==tankBullet->END){
				int dirct = tank->getState();//����
				tankBullet->setVisible(true);
				if(dirct==tank->moveRight||dirct==tank->holdRight||dirct==tank->moveLeft||dirct==tank->holdLeft){
					tankBullet->setZOrder(max(4,(int)tank->getVertexZ()));
				}else{
					tankBullet->setZOrder(20);
				}
				// ((GameScene *)scene->getChildByTag(1))->tileMap->addChild(tankBullet,tank->getVertexZ());
				//tileMap->addChild(tankBullet,20,110+i); 
				/*int downPoint[6]={0,0,0,0,0,0};
				int upPoint[6]={0,0,0,0,0,0};
				int rightPoint[6]={0,0,0,0,0,0};
				int leftPoint[6]={0,0,0,0,0,0};*/

				b2BodyDef bodyBulletDef;
				bodyBulletDef.type =b2_dynamicBody ;//

				if(dirct==tank->moveDown||dirct==tank->holdDown){
					int dex = 0;
					int dey = 0;

					if(_turretId==2||_turretId==3){
						dex =(index==0?-8:8);
					}else if(_turretId==5){
						dey = -80;

					}else if(_turretId==6){
						dex = random(-12,12);
					}

					bodyBulletDef.position.Set((tank->getPositionX()+dex)/PTM_RATIO, (tank->getPositionY()+dey)/PTM_RATIO);
				}else if(dirct==tank->moveUp||dirct==tank->holdUp){
					int dex = 0;
					int dey = 0;

					if(_turretId==2||_turretId==3){
						dex =(index==0?-8:8); 
					}else if(_turretId==5){
						dey = 80;

					}else if(_turretId==6){
						dex = random(-12,12);
					}
					bodyBulletDef.position.Set((tank->getPositionX()+dex)/PTM_RATIO, (tank->getPositionY()+40+dey)/PTM_RATIO);
				}else if(dirct==tank->moveRight||dirct==tank->holdRight){
					int dex = 0;
					int dey = 0;

					if(_turretId==2||_turretId==3){
						dey =(index==0?-8:8); 

					}else if(_turretId==4){
						dey = 6;

					}else if(_turretId==5){
						dex = 50;
					}else if(_turretId==6){
						dey = random(-12,12);
					}

					bodyBulletDef.position.Set((tank->getPositionX()+35+dex)/PTM_RATIO, (tank->getPositionY()+5+dey)/PTM_RATIO);
				}else if(dirct==tank->moveLeft||dirct==tank->holdLeft){
					int dex = 0;
					int dey = 0;
					if(_turretId==2||_turretId==3){
						dey =(index==0?-8:8); 
					}else if(_turretId==4){
						dey = 6;

					}else if(_turretId==5){

						dex = -50;
					}else if(_turretId==6){

						dey = random(-12,12);
					}
					bodyBulletDef.position.Set((tank->getPositionX()-35+dex)/PTM_RATIO, (tank->getPositionY()+5+dey)/PTM_RATIO);
				}

				bodyBulletDef.userData = tankBullet;

				b2Body *body  = ((GameScene *)scene->getChildByTag(1))->world->CreateBody(&bodyBulletDef);

				int _dex = _turretId;

				if(_turretId==5){
					if(dirct==tank->moveDown||dirct==tank->holdDown||dirct==tank->moveUp||dirct==tank->holdUp){
						_dex = 51;
					}
				}

				char  lable[30];
				sprintf(lable, "eleball%d",_dex); 

				GB2ShapeCache *sfc = GB2ShapeCache::sharedGB2ShapeCache();
				sfc->addFixturesToBody(body, lable);// 
				tankBullet->setAnchorPoint(sfc->anchorPointForShape(lable));
				//tankBullet->setState(tankBullet->FLYING);

				CCLog("---------------------------getBulletSpeed:%d",tankBullet->getBulletSpeed());
				if(dirct==tank->moveDown||dirct==tank->holdDown){//�ӵ��ٶ�
					tankBullet->setState(tankBullet->moveDown);
					body->SetLinearVelocity(b2Vec2(0/PTM_RATIO,-tankBullet->getBulletSpeed()/PTM_RATIO));
				}else if(dirct==tank->moveUp||dirct==tank->holdUp){
					tankBullet->setState(tankBullet->moveUp);
					body->SetLinearVelocity(b2Vec2(0/PTM_RATIO,tankBullet->getBulletSpeed()/PTM_RATIO));
				}else if(dirct==tank->moveRight||dirct==tank->holdRight){
					tankBullet->setState(tankBullet->moveRight);
					body->SetLinearVelocity(b2Vec2(tankBullet->getBulletSpeed()/PTM_RATIO,0/PTM_RATIO));
				}else if(dirct==tank->moveLeft||dirct==tank->holdLeft){
					tankBullet->setState(tankBullet->moveLeft);
					body->SetLinearVelocity(b2Vec2(-tankBullet->getBulletSpeed()/PTM_RATIO,0/PTM_RATIO));
				}


				index++;
				if(index>=bulletNum){
					break;
				}

			}
		}
	}
}
void UIlayer::didAccelerate( CCAcceleration* pAccelerationValue ){
	/*
	//���Ƽ��ٵ�����ֵԽС�������׸ı䷽��     
	float deceleration = 0.5f;    
	//���ټƵ�����ֵԽ�����ǶԼ��ټƵ������Խ����     
	float sensitivity = 10.0;    
	//����ƶ�����     
	float maxVelocity = 100.0f;    

	didAcceleratePos.x = didAcceleratePos.x*deceleration + pAccelerationValue->x*sensitivity; 
	didAcceleratePos.y = didAcceleratePos.y*deceleration + pAccelerationValue->y*sensitivity; 

	if (didAcceleratePos.x>=maxVelocity)    
	{    


	didAcceleratePos.x = maxVelocity;    
	}else if(didAcceleratePos.x <=-maxVelocity)    
	{    


	didAcceleratePos.x = -maxVelocity;    
	}*/   
	//���ټƵ�����ֵԽ�����ǶԼ��ټƵ������Խ����     
	float sensitivity = 10.0;  
	//���Ƽ��ٵ�����ֵԽС�������׸ı䷽��     
	float deceleration = 2.5f;  

	CCLog("=======================pAccelerationValue->y:%f",(float)pAccelerationValue->y);
	CCLog("=======================pAccelerationValue->x:%f",(float)pAccelerationValue->x);

	float dirX = (float)pAccelerationValue->x*sensitivity;
	float dirY = (float)pAccelerationValue->y*sensitivity;
	CCLog("=======================dirX:%f",dirX);
	CCLog("=======================dirY:%f",dirY);

	if(abs(dirX)>abs(dirY)){
		if(dirX >0){
			if(abs(dirX)>deceleration){
				CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
				if(((GameScene *)scene->getChildByTag(1))->playBody!=NULL){
					tanker* tankPlayer= (tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData());

					((GameScene *)scene->getChildByTag(1))->setPlayerSpeed(tankPlayer->getMoveSpeed(),0);
					tankPlayer->setState(tanker::moveRight);
				}
			}else{
				// CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
				//((tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData()))->setState(tanker::holdRight);//
				// ((GameScene *)scene->getChildByTag(1))->setPlayerSpeed(0,0);
			}
		}else if(dirX <0){
			if(abs(dirX)>deceleration){
				CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
				if(((GameScene *)scene->getChildByTag(1))->playBody!=NULL){
					tanker* tankPlayer= (tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData());

					((GameScene *)scene->getChildByTag(1))->setPlayerSpeed(-tankPlayer->getMoveSpeed(),0);
					tankPlayer->setState(tanker::moveLeft);
				}
			}else{
				// CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
				// ((tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData()))->setState(tanker::holdLeft);//
				//  ((GameScene *)scene->getChildByTag(1))->setPlayerSpeed(0,0);
			}
		}

	}else{
		if(dirY>0){
			if(abs(dirY)>deceleration){
				CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
				if(((GameScene *)scene->getChildByTag(1))->playBody!=NULL){
					tanker* tankPlayer= (tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData());
					((GameScene *)scene->getChildByTag(1))->setPlayerSpeed(0,tankPlayer->getMoveSpeed());
					tankPlayer->setState(tanker::moveUp);
				}
			}else{
				// CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
				//((tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData()))->setState(tanker::holdUp);//
				//  ((GameScene *)scene->getChildByTag(1))->setPlayerSpeed(0,0);
			}
		}else if(dirY<0){
			if(abs(dirY)>deceleration){
				CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
				if(((GameScene *)scene->getChildByTag(1))->playBody!=NULL){
					tanker* tankPlayer= (tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData());

					((GameScene *)scene->getChildByTag(1))->setPlayerSpeed(0,-tankPlayer->getMoveSpeed());
					tankPlayer->setState(tanker::moveDown);
				}
			}else{
				// CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
				// ((tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData()))->setState(tanker::holdDown);//
				// ((GameScene *)scene->getChildByTag(1))->setPlayerSpeed(0,0);
			}
		}
	}

}
void UIlayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){

	startPoint = pTouch->getLocationInView();
	startPoint =  CCDirector::sharedDirector()->convertToGL(startPoint);//ת��GL
	if(PointInRound(startPoint.x,startPoint.y,(this->getChildByTag(1000))->getPositionX(),(this->getChildByTag(1000))->getPositionY(),80)){
		this->getChildByTag(1001)->setVisible(true);
		this->getChildByTag(1001)->setPosition(startPoint);
	}else{
		/*if((this->getChildByTag(1001))->isVisible()){//�ɼ�
		//CCPoint endLocation = pTouch->getPreviousLocationInView();
		//CCPoint endPoint =  CCDirector::sharedDirector()->convertToGL(endLocation);
		CCPoint   point = this->getChildByTag(1000)->getPosition();

		if((startPoint.y-point.y)>=abs(startPoint.x-point.x)){//��
		CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
		tanker* tankPlayer= (tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData());
		((GameScene *)scene->getChildByTag(1))->setPlayerSpeed(0,tankPlayer->getMoveSpeed());
		tankPlayer->setState(tanker::moveUp)

		}else if(abs(startPoint.y-point.y)>abs(startPoint.x-point.x)){//��
		CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
		tanker* tankPlayer= (tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData());

		((GameScene *)scene->getChildByTag(1))->setPlayerSpeed(0,-tankPlayer->getMoveSpeed());
		tankPlayer->setState(tanker::moveDown);

		}else if((startPoint.x-point.x)>=0&&(abs(startPoint.y-point.y)<abs(startPoint.x-point.x))){//��
		CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
		tanker* tankPlayer= (tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData());

		((GameScene *)scene->getChildByTag(1))->setPlayerSpeed(tankPlayer->getMoveSpeed(),0);
		tankPlayer->setState(tanker::moveRight);

		}else if((startPoint.x-point.x)<0&&(abs(startPoint.y-point.y)<abs(startPoint.x-point.x))){//��

		CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
		tanker* tankPlayer= (tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData());

		((GameScene *)scene->getChildByTag(1))->setPlayerSpeed(-tankPlayer->getMoveSpeed(),0);
		tankPlayer->setState(tanker::moveLeft);
		}

		}*/
		//touchEnd();
		//return;
	}
	if((this->getChildByTag(1001))->isVisible()){//�ɼ�
		//CCPoint endLocation = pTouch->getPreviousLocationInView();
		//CCPoint endPoint =  CCDirector::sharedDirector()->convertToGL(endLocation);
		CCPoint   point = this->getChildByTag(1000)->getPosition();

		float angleRadians = atanf((startPoint.y-point.y) /(startPoint.x-point.x));
		float angleDegrees =angleRadians;//-1*CC_RADIANS_TO_DEGREES(angleRadians);

		if(startPoint.x-point.x>=0){
			this->getChildByTag(1001)->setPosition(ccp(point.x+cos(angleDegrees)*50.0f,point.y+sin(angleDegrees)*60.0f));
		}else{
			this->getChildByTag(1001)->setPosition(ccp(point.x-cos(angleDegrees)*50.0f,point.y-sin(angleDegrees)*60.0f));
		}

		if((startPoint.y-point.y)>=abs(startPoint.x-point.x)){//��
			CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
			if(((GameScene *)scene->getChildByTag(1))->playBody!=NULL){
				tanker* tankPlayer= (tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData());
				if(tankPlayer->getLife()>0){
					((GameScene *)scene->getChildByTag(1))->setPlayerSpeed(0,tankPlayer->getMoveSpeed());
					tankPlayer->setState(tanker::moveUp);
				}
			}
		}else if(abs(startPoint.y-point.y)>abs(startPoint.x-point.x)){//��
			CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
			if(((GameScene *)scene->getChildByTag(1))->playBody!=NULL){
				tanker* tankPlayer= (tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData());
				if(tankPlayer->getLife()>0){
					((GameScene *)scene->getChildByTag(1))->setPlayerSpeed(0,-tankPlayer->getMoveSpeed());
					tankPlayer->setState(tanker::moveDown);
				}
			}
		}else if((startPoint.x-point.x)>=0&&(abs(startPoint.y-point.y)<abs(startPoint.x-point.x))){//��
			CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
			if(((GameScene *)scene->getChildByTag(1))->playBody!=NULL){
				tanker* tankPlayer= (tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData());
				if(tankPlayer->getLife()>0){
					((GameScene *)scene->getChildByTag(1))->setPlayerSpeed(tankPlayer->getMoveSpeed(),0);
					tankPlayer->setState(tanker::moveRight);
				}
			}
		}else if((startPoint.x-point.x)<0&&(abs(startPoint.y-point.y)<abs(startPoint.x-point.x))){//��

			CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
			if(((GameScene *)scene->getChildByTag(1))->playBody!=NULL){
				tanker* tankPlayer= (tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData());
				if(tankPlayer->getLife()>0){
					((GameScene *)scene->getChildByTag(1))->setPlayerSpeed(-tankPlayer->getMoveSpeed(),0);
					tankPlayer->setState(tanker::moveLeft);
				}
			}
		}

	}
	//��
	/*	if(PointInRound(startPoint.x,startPoint.y,(this->getChildByTag(1000))->getPositionX()-20,(this->getChildByTag(1000))->getPositionY(),20)){
	//CCLog("===========left");
	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	tanker* tankPlayer= (tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData());

	((GameScene *)scene->getChildByTag(1))->setPlayerSpeed(-tankPlayer->getMoveSpeed(),0);
	tankPlayer->setState(tanker::moveLeft);
	//��
	}else if(PointInRound(startPoint.x,startPoint.y,(this->getChildByTag(1000))->getPositionX()+20,(this->getChildByTag(1000))->getPositionY(),20)){
	//CCLog("===========right");
	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	tanker* tankPlayer= (tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData());

	((GameScene *)scene->getChildByTag(1))->setPlayerSpeed(tankPlayer->getMoveSpeed(),0);
	tankPlayer->setState(tanker::moveRight);
	//��
	}else if(PointInRound(startPoint.x,startPoint.y,(this->getChildByTag(1000))->getPositionX(),(this->getChildByTag(1000))->getPositionY()-20,20)){
	//CCLog("===========down");
	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	tanker* tankPlayer= (tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData());

	((GameScene *)scene->getChildByTag(1))->setPlayerSpeed(0,-tankPlayer->getMoveSpeed());
	tankPlayer->setState(tanker::moveDown);
	//��
	}else if(PointInRound(startPoint.x,startPoint.y,(this->getChildByTag(1000))->getPositionX(),(this->getChildByTag(1000))->getPositionY()+20,20)){
	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	tanker* tankPlayer= (tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData());
	((GameScene *)scene->getChildByTag(1))->setPlayerSpeed(0,tankPlayer->getMoveSpeed());
	tankPlayer->setState(tanker::moveUp);

	//CCLog("===========up");
	}*/



	/*
	CCPoint endLocation = pTouch->getPreviousLocationInView();
	endPoint =  CCDirector::sharedDirector()->convertToGL(endLocation);



	if(PointInRound(endPoint.x,endPoint.y,startPoint.x,startPoint.y,45)){
	// CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	this->getChildByTag(1001)->setPosition(endPoint);
	//((GameScene *)scene->getChildByTag(1))->setPlayerSpeed(0,0);
	// return;

	// this->getChildByTag(1001)->setPosition(startPoint);
	this->getChildByTag(1001)->setVisible(true);
	}else{
	return;
	}

	//�������
	CCPoint distant =startPoint-endPoint;

	// float len = sqrtf((distant.x*distant.x)+(distant.y*distant.y));
	// CCLog("----------------------------distant.x:%f",distant.x);
	// CCLog("----------------------------distant.y:%f",distant.y);

	//CCPoint playerPos = (this->getChildByTag(100))->getPosition();
	// b2Vec2 playerVet = playBody->GetPosition();
	// CCPoint playerPos = ccp(playerVet.x*PTM_RATIO,playerVet.y*PTM_RATIO);

	if(abs(distant.x)>abs(distant.y)){//����		
	if(distant.x>0){//����
	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	tanker* tankPlayer= (tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData());

	((GameScene *)scene->getChildByTag(1))->setPlayerSpeed(-tankPlayer->getMoveSpeed(),0);
	tankPlayer->setState(tanker::moveLeft);
	}else{//����
	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	tanker* tankPlayer= (tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData());

	((GameScene *)scene->getChildByTag(1))->setPlayerSpeed(tankPlayer->getMoveSpeed(),0);
	tankPlayer->setState(tanker::moveRight);
	}
	}else{//����

	if(distant.y>0){//����
	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	tanker* tankPlayer= (tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData());

	((GameScene *)scene->getChildByTag(1))->setPlayerSpeed(0,-tankPlayer->getMoveSpeed());
	tankPlayer->setState(tanker::moveDown);
	}else{//����
	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	tanker* tankPlayer= (tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData());
	((GameScene *)scene->getChildByTag(1))->setPlayerSpeed(0,tankPlayer->getMoveSpeed());
	tankPlayer->setState(tanker::moveUp);
	}
	}*/
}
void UIlayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
	touchEnd();
}

void  UIlayer::touchEnd(){

	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	if(((GameScene *)scene->getChildByTag(1))->playBody!=NULL){
		((GameScene *)scene->getChildByTag(1))->setPlayerSpeed(0,0);

		startPoint = ccp(0,0);
		//this->getChildByTag(1000)->setVisible(false);
		this->getChildByTag(1001)->setVisible(false);

		int tankState = ((tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData()))->getState();

		if(tankState==tanker::moveUp){
			((tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData()))->setState(tanker::holdUp);//
		}else if(tankState==tanker::moveDown){
			((tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData()))->setState(tanker::holdDown);
		}else if(tankState==tanker::moveLeft){
			((tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData()))->setState(tanker::holdLeft);
		}else if(tankState==tanker::moveRight){
			((tanker*)(((GameScene *)scene->getChildByTag(1))->playBody->GetUserData()))->setState(tanker::holdRight);
		}
	}
}