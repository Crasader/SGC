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

	//�Ƿ���Ҫ���Ӽ��ܣ��Ƿ���Ⱥ�幥����
};

class GameBattleScene :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene(BattlePoint* p);
	virtual bool init(BattlePoint* p);


	static GameBattleScene* create(BattlePoint* p);

	std::vector<SwordBride*> createBrideSequence(std::vector<SwordBride*>allBrides);   //�����ٶ��ɴ�С��������Ľ�������

	std::vector<AttackAction> createAttackSequence(std::vector<SwordBride*> bridesSequence,std::vector<SwordBride*>playerBrides,std::vector<SwordBride*>enemyBrides);
	//������������һ��Ϊ���ٶ����е����н�ɫ���ڶ���Ϊ�û������б�������Ϊ�����б�����ս���Ĺ�������

	void doAttackAction(std::vector<SwordBride*> bridesSequence, std::vector<SwordBride*>playerBrides, std::vector<SwordBride*>enemyBrides,
		std::vector<Sprite*>bridesSpite,std::vector<Sprite*>enemySprite,int count);   //���ݹ�������չ�ֹ�������


	void playAction(float delta);  //Ŀǰδ��
	
	int getAttackValue(SwordBride* attackBride, SwordBride* attackedBride);  //���ݹ����ߺͱ������ߵ����ݵõ�������ֵ

	void showContinueDlg(float dt);   //�ڵ�һ�ֹ�����ɺ���ʾ�Ի����Ƿ����

	void onYesBtnClicked(Ref* ref,TouchEventType type);
	void onNoBtnClicked(Ref* ref,TouchEventType type);

	void enterSecondBattle(std::vector<SwordBride*> playerBrides, std::vector<SwordBride*>enemyBrides,
		std::vector<Sprite*>bridesSpite, std::vector<Sprite*>enemySprite);
	//���ѡ���������еڶ���ս��

	void ShowBattleResult();   //ս����ɺ���ʾս���ܽ�

	std::vector<Sprite*> m_bridesSpriteVec;
	std::vector<Sprite*>m_enemiesSpriteVec;


private:
	std::vector<SwordBride*>m_battleEnemies;          //��ͼ���˵�vec
	std::vector<SwordBride*>m_battleBrides;			//��ͼ��ҽ�����vec				
	int m_battleWeather;

	std::vector<SwordBride*>m_battleTeam1;   //���1�Ľ�����Ŀǰս��ֻ�ϱ��1

	std::vector<SwordBride*>m_allSwords;   //����+���
	std::vector<SwordBride*>m_atkSequence;  //����˳��Ľ�������

	std::vector<AttackAction>m_attackActionVec;



};




#endif