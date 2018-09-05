#ifndef __SGC__BattleMap__
#define __SGC__BattleMap__

#include "cocos2d.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
USING_NS_CC_EXT;
#include "SwordBride.h"
#include "PlayerItem.h"
#include "BattleMap.h"
#include "SwordBride.h"

using namespace cocos2d::ui;
using namespace cocostudio;
USING_NS_CC;

class BattlePoint
{
public:
	BattlePoint();
	~BattlePoint();

public:
	int m_ID;   //  该场景在本地图中的ID号，唯一标识
	Point m_curPoint;   //地图中小场景的显示位置，需要哪些数据来构造场景，待确认
	bool m_isBattleDone;   //是否已经完成战斗
	bool m_isClocked;		//该场景是否解锁
	bool m_isDrawed;     //是否在地图中被绘制
	std::vector<BattlePoint*> m_nextPointVec;    //下一个场景的集合，有可能有多个场景可以进入


	std::vector<int>m_nextPoints_ID;   //下一个场景的ID号，可直接存储ID号不存储对象，更合理

	std::vector<SwordBride*>m_enemySwords;   //场景中的敌人配置
	int m_weather;   //场景中的天气类型
	

};






class BattleMap
{
public:
	BattleMap(int mapID);
	~BattleMap();

public:
	std::vector<BattlePoint*> m_MapPointVec;
	int m_MapID;


};


#endif