#ifndef __TEAM_FORMATION_SCENE_H__
#define __TEAM_FORMATION_SCENE_H__
#include "cocos2d.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
USING_NS_CC_EXT;
#include "SwordInformationCard.h"

#include "SwordInformationCard.h"
#include "SwordBride.h"



using namespace cocos2d::ui;
using namespace cocostudio;
USING_NS_CC;
class TeamFormationScene:public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(TeamFormationScene);

	SwordEquipScene* createSwordEquipScene(SwordBride* bride);    //创建剑姬装备界面函数
	LayerColor* createSwordList(std::vector<SwordBride*> swordBrideVec);   //创建剑姬列表函数，用来更换剑姬
	void onSwordEquipBtnClick(Ref* ref, TouchEventType type);
	void onSwordChangeBtnClick(Ref* ref, TouchEventType type);
	void onSwordListItemClick(Ref* ref, TouchEventType type);
	void onSwordListConfirmClick(Ref* ref, TouchEventType type);

	void onTeam1BtnClick(Ref* ref, TouchEventType type);   //显示1-4编队的剑姬
	void onTeam2BtnClick(Ref* ref, TouchEventType type);
	void onTeam3BtnClick(Ref* ref, TouchEventType type);
	void onTeam4BtnClick(Ref* ref, TouchEventType type);

	void onCloseBtnClick(Ref* ref,TouchEventType type);

	void refreshInformationCard(SwordBride* sword, int i);       //刷新第i个位置的剑姬数据，数据内容为传入的剑姬类

	void setChangeBtnClickNum(int num);      //记录哪个剑姬的更改按钮被按下
	void setEquipBtnClickNum(int num);		//记录哪个剑姬的装备按钮被拿下			
	int getChangeBtnClickNum();
	int getEquipBtnClickNum();

	void setChangeBtnClickStatus(bool status);     //设置是否有剑姬的更改按钮被按下
	void setEquipBtnClickStatus(bool status);		//设置是否有剑姬的装备按钮被按下
	bool getChangeBtnClickStatus();
	bool getEquipBtnClickStatus();

	bool onTouchBegan(Touch *touch, Event *event);
	void onTouchMoved(Touch *touch, Event *event);
	void onTouchEnded(Touch *touch, Event *event);
	SwordInformationCard* getSwordCards();    //函数无作用可删除？

	int getTouchCardNum();
	void setTouchCardNum(int i);

	Point getPreviousTouchCardPoint();
	void setPreviousTouchCardPoint(Point p);

	std::vector<SwordBride*> getSwordBeChosenVec();

	void setCurTeamNum(int num);
	int getCurTeamNum();

public:
	SwordBride* m_curTeamSwords[4][6];  //存储当前的编队，编队有4队，每队6个
private:
	SwordInformationCard* m_teamSwordCard[6];
	bool m_isChangeBtnClicked;       //是否有更改按钮被按下，如果有一个按下，其他不能按下
	bool m_isEquipBtnClicked;		//是否有装备按钮被按下，如果有一个按下，其他不能按下
	int m_ChangeBtnClickNum;		//哪一个的更改按钮被按下，如果没有，用-1表示
	int m_EquipBtnClickNum;			//哪一个的装备按钮被按下，如果没有，用-1表示

	int m_touchCardNum;

	std::vector <SwordBride*>m_SwordCanBeChosen;    //可更换的剑姬列表，每个用户当前拥有的所有剑姬列表



	Point previousLoactionOfTouchedCard;   //上一个被按下的卡牌的位置，用来进行位置交换

	int m_curTeamNum;   //当前显示的是哪一个编队
};


#endif