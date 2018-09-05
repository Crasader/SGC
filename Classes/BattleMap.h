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
	int m_ID;   //  �ó����ڱ���ͼ�е�ID�ţ�Ψһ��ʶ
	Point m_curPoint;   //��ͼ��С��������ʾλ�ã���Ҫ��Щ���������쳡������ȷ��
	bool m_isBattleDone;   //�Ƿ��Ѿ����ս��
	bool m_isClocked;		//�ó����Ƿ����
	bool m_isDrawed;     //�Ƿ��ڵ�ͼ�б�����
	std::vector<BattlePoint*> m_nextPointVec;    //��һ�������ļ��ϣ��п����ж���������Խ���


	std::vector<int>m_nextPoints_ID;   //��һ��������ID�ţ���ֱ�Ӵ洢ID�Ų��洢���󣬸�����

	std::vector<SwordBride*>m_enemySwords;   //�����еĵ�������
	int m_weather;   //�����е���������
	

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