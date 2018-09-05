#include "GameData.h"

static GameData* gamedata = NULL;

GameData::GameData()
{
	m_userDic = CCDictionary::create();
	m_userDic->retain();
	m_level = UserDefault::sharedUserDefault()->getIntegerForKey("level", 1);
	m_exp = UserDefault::sharedUserDefault()->getIntegerForKey("exp", 1);
	UserDefault::sharedUserDefault()->flush();

}

GameData::~GameData()
{

}


GameData* GameData::shareGameData()
{
	if (!gamedata)
	{
		gamedata = new GameData();
	}
	return gamedata;
}
std::vector<SwordBride*> GameData::getUserSwordBrides()
{
	return m_userSwordBrides;
}
std::vector<PlayerItem*> GameData::getUserItems()
{
	return m_userPlayerItems;
}
bool GameData::addSwordBride(SwordBride* bride)
{
	int flag = false;
	for (int i =0 ; i < m_userSwordBrides.size(); i++)
	{
		SwordBride* curBride = (SwordBride*)m_userSwordBrides.at(i);
		if (curBride->getBrideID() == bride->getBrideID())
		{
			flag = true;
			break;
		}
	}
	if (flag)
	{
		return false;
	}
	else
	{
		m_userSwordBrides.push_back(bride);
		return true;
	}
}
bool GameData::deleteBride(SwordBride* bride)
{
	int flag = false;
	std::vector<SwordBride*>::iterator iter;
	for (iter = m_userSwordBrides.begin(); iter != m_userSwordBrides.end();)
	{
		///if (iter->getBrideID()== bride->getBrideID())         //É¾³ý¹¦ÄÜ´ýÍêÉÆ
		{
		}
	}


	return true;

}

void GameData::setCurrentSwordTeam(std::vector<SwordBride*> swordTeam)
{
	m_currentSwordTeam = swordTeam;
}
std::vector<SwordBride*> GameData::getCurrentSwordTeam()
{
	return m_currentSwordTeam;
}