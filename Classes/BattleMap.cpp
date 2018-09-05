#include "BattleMap.h"

USING_NS_CC;


BattlePoint::BattlePoint()
{
	m_isBattleDone = false;    //初始默认未完成战斗
	m_isClocked = true;			//初始应为解锁状态
	m_isDrawed = false;			//初始未完成绘制

	m_nextPointVec.clear();
	m_nextPoints_ID.clear();
	m_enemySwords.clear();


}

BattlePoint::~BattlePoint()
{
	m_nextPointVec.clear();
	m_nextPoints_ID.clear();
}



BattleMap::BattleMap(int mapID)
{


	m_MapPointVec.clear();


	m_MapID = mapID;
#if 0
	if (m_MapID == 1)
	{
		for (int i = 0; i < 6; i++)
		{
			BattlePoint* battleP = new BattlePoint();
			battleP->m_curPoint = Point(50 + i * 150, 500);
			m_MapPointVec.push_back(battleP);
		}
		for (int i = 0; i < 5; i++)
		{
			BattlePoint* p1 = (BattlePoint*)m_MapPointVec.at(i);
			BattlePoint* p2 = (BattlePoint*)m_MapPointVec.at(i + 1);
			p1->m_nextPointVec.push_back(p2);

		}
	}
	BattlePoint* battleP[7];
	if (m_MapID == 2)
	{
		for (int i = 0; i < 7; i++)
		{
			battleP[i] = new BattlePoint();
		}
		battleP[0]->m_curPoint = Point(50, 500);
		battleP[1]->m_curPoint = Point(200, 700);
		battleP[2]->m_curPoint = Point(200, 300);
		battleP[3]->m_curPoint = Point(350, 500);
		battleP[4]->m_curPoint = Point(500, 700);
		battleP[5]->m_curPoint = Point(500, 300);
		battleP[6]->m_curPoint = Point(650, 500);

		battleP[0]->m_nextPointVec.push_back(battleP[1]);
		battleP[0]->m_nextPointVec.push_back(battleP[2]);
		battleP[1]->m_nextPointVec.push_back(battleP[3]);
		battleP[2]->m_nextPointVec.push_back(battleP[3]);
		battleP[3]->m_nextPointVec.push_back(battleP[4]);
		battleP[3]->m_nextPointVec.push_back(battleP[5]);
		battleP[4]->m_nextPointVec.push_back(battleP[6]);
		battleP[5]->m_nextPointVec.push_back(battleP[6]);

		for (int i = 0; i < 7; i++)
		{
			m_MapPointVec.push_back(battleP[i]);
		}

	}
#endif // 0


	//以上均为测试数据

	


}

BattleMap::~BattleMap()
{
	m_MapPointVec.clear();
}