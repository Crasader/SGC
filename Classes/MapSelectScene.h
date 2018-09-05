#ifndef __MAP_SELECT_SCENE_H__
#define __MAP_SELECT_SCENE_H__

#include "cocos2d.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
USING_NS_CC_EXT;

#include "MapSelectScene.h"
#include "BattleMap.h"
using namespace cocos2d::ui;
using namespace cocostudio;
USING_NS_CC;

class MapSelectScene:public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(MapSelectScene);

	void setBattleMap(int map_id);

	void onRunBtnClicked(Ref* ref,TouchEventType type);
	void onReturnBtnClicked(Ref* ref,TouchEventType type);

	void onBeginBattleBtnClicked(Ref* ref,TouchEventType type);    //开始战斗按钮实现，进入战斗界面


	void setCurrentPoint(BattlePoint* point);
	BattlePoint* getCurrentPoint();

	BattleMap* readMapJson(int mapID);


public:
	Sprite* m_curPlayerSp;                     //当前玩家的精灵，显示目前在哪一个战斗场景中
	
protected:
	BattleMap* m_curBattleMap;		//当前选中的地图的基本信息类

	
	BattlePoint* m_curPoint;    //记录当前走到了哪一个节点

	Point StartP;
	Point FinishP;
};

#endif