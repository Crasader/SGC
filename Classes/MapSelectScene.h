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

	void onBeginBattleBtnClicked(Ref* ref,TouchEventType type);    //��ʼս����ťʵ�֣�����ս������


	void setCurrentPoint(BattlePoint* point);
	BattlePoint* getCurrentPoint();

	BattleMap* readMapJson(int mapID);


public:
	Sprite* m_curPlayerSp;                     //��ǰ��ҵľ��飬��ʾĿǰ����һ��ս��������
	
protected:
	BattleMap* m_curBattleMap;		//��ǰѡ�еĵ�ͼ�Ļ�����Ϣ��

	
	BattlePoint* m_curPoint;    //��¼��ǰ�ߵ�����һ���ڵ�

	Point StartP;
	Point FinishP;
};

#endif