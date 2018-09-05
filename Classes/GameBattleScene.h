#ifndef __GAME_BATTLE_SCENE_H__
#define __GAME_BATTLE_SCENE_H__


#include "cocos2d.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"

USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace cocostudio;
USING_NS_CC;

#include "BattleMap.h"

struct AttackAction
{
	int attackSwordID;
	int attackedSwordID;
	int attackValue;
	bool isBrideAttack;

	bool isKillSword;

	//是否需要增加技能，是否有群体攻击？
};

class GameBattleScene :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene(BattlePoint* p);
	virtual bool init(BattlePoint* p);


	static GameBattleScene* create(BattlePoint* p);

	std::vector<SwordBride*> createBrideSequence(std::vector<SwordBride*>allBrides);   //按照速度由大到小进行排序的剑姬序列

	std::vector<AttackAction> createAttackSequence(std::vector<SwordBride*> bridesSequence,std::vector<SwordBride*>playerBrides,std::vector<SwordBride*>enemyBrides);
	//三个参数，第一个为以速度排列的所有角色，第二个为用户剑姬列表，第三个为敌人列表，返回战斗的攻击序列

	void doAttackAction(std::vector<SwordBride*> bridesSequence, std::vector<SwordBride*>playerBrides, std::vector<SwordBride*>enemyBrides,
		std::vector<Sprite*>bridesSpite,std::vector<Sprite*>enemySprite,int count);   //根据攻击序列展现攻击动作


	void playAction(float delta);  //目前未用
	
	int getAttackValue(SwordBride* attackBride, SwordBride* attackedBride);  //根据攻击者和被攻击者的数据得到攻击数值

	void showContinueDlg(float dt);   //在第一轮攻击完成后显示对话框，是否继续

	void onYesBtnClicked(Ref* ref,TouchEventType type);
	void onNoBtnClicked(Ref* ref,TouchEventType type);

	void enterSecondBattle(std::vector<SwordBride*> playerBrides, std::vector<SwordBride*>enemyBrides,
		std::vector<Sprite*>bridesSpite, std::vector<Sprite*>enemySprite);
	//如果选择继续则进行第二轮战斗

	void ShowBattleResult();   //战斗完成后显示战斗总结

	std::vector<Sprite*> m_bridesSpriteVec;
	std::vector<Sprite*>m_enemiesSpriteVec;


private:
	std::vector<SwordBride*>m_battleEnemies;          //地图敌人的vec
	std::vector<SwordBride*>m_battleBrides;			//地图玩家剑姬的vec				
	int m_battleWeather;

	std::vector<SwordBride*>m_battleTeam1;   //编队1的剑姬，目前战斗只上编队1

	std::vector<SwordBride*>m_allSwords;   //敌人+玩家
	std::vector<SwordBride*>m_atkSequence;  //攻击顺序的剑姬序列

	std::vector<AttackAction>m_attackActionVec;



};




#endif