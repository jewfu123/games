#ifndef _ACHIEVECOCOSBUILDERLAYER_H_
#define _ACHIEVECOCOSBUILDERLAYER_H_

#include "cocos2d.h"
#include "cocos-ext.h"

class AchieveCocosBuilder: public cocos2d::CCLayer
, public cocos2d::extension::CCBSelectorResolver
, public cocos2d::extension::CCBMemberVariableAssigner
, public cocos2d::extension::CCNodeLoaderListener,

public cocos2d::extension::CCTableViewDataSource, public cocos2d::extension::CCTableViewDelegate

{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(AchieveCocosBuilder, create);
	AchieveCocosBuilder(void);
	~AchieveCocosBuilder(void);

		  //菜单事件绑定
     virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
     //控制按钮绑定
     virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);
	 //成员变量绑定
     virtual bool onAssignCCBMemberVariable(cocos2d::CCObject * pTarget, const char * pMemberVariableName, cocos2d::CCNode * pNode);
		//属性绑定
      virtual bool onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue);
	 //初始化
     virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);

	  void onBackClicked(CCObject * pSender);

	  //返回，退出
	  virtual void  keyBackClicked();

	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}
    virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
    virtual cocos2d::CCSize cellSizeForTable(cocos2d::extension::CCTableView *table);
	//生成cell
    virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);

private:
	cocos2d::CCSprite *gold_achievement;
	int  achieveRecord[10];//任务完成数量
	int  task[10];//要完成的任务数
	int  task_gold[10];//完成任务后金币
	int  isRecordGold[10];//是否接受金币
};

#endif