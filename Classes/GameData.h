#ifndef __SGC__GameData__
#define __SGC__GameData__

#include "cocos2d.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
USING_NS_CC_EXT;
#include "SwordBride.h"
#include "PlayerItem.h"

using namespace cocos2d::ui;
using namespace cocostudio;
USING_NS_CC;

class GameData
{
public:
	GameData();
	~GameData();
	void readJson();
	void saveDefault();
	std::vector<SwordBride*> getUserSwordBrides();
	bool addSwordBride(SwordBride* bride);
	bool deleteBride(SwordBride* bride);
	std::vector<PlayerItem*> getUserItems();
	bool addItem(PlayerItem* item);
	bool deleteItem(PlayerItem* item);
	int* getUserResources();
	static GameData* shareGameData();

	void setCurrentSwordTeam(std::vector<SwordBride*>SwordTeam);

	std::vector<SwordBride*> getCurrentSwordTeam();



private:
	std::vector<SwordBride*>m_userSwordBrides;
	std::vector<PlayerItem*>m_userPlayerItems;
	int m_userResources[4];
	CCDictionary* m_userDic;
	int m_level;
	int m_exp;
	std::vector<SwordBride*> m_currentSwordTeam;

};



#endif