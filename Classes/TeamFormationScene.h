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

	SwordEquipScene* createSwordEquipScene(SwordBride* bride);    //��������װ�����溯��
	LayerColor* createSwordList(std::vector<SwordBride*> swordBrideVec);   //���������б�����������������
	void onSwordEquipBtnClick(Ref* ref, TouchEventType type);
	void onSwordChangeBtnClick(Ref* ref, TouchEventType type);
	void onSwordListItemClick(Ref* ref, TouchEventType type);
	void onSwordListConfirmClick(Ref* ref, TouchEventType type);

	void onTeam1BtnClick(Ref* ref, TouchEventType type);   //��ʾ1-4��ӵĽ���
	void onTeam2BtnClick(Ref* ref, TouchEventType type);
	void onTeam3BtnClick(Ref* ref, TouchEventType type);
	void onTeam4BtnClick(Ref* ref, TouchEventType type);

	void onCloseBtnClick(Ref* ref,TouchEventType type);

	void refreshInformationCard(SwordBride* sword, int i);       //ˢ�µ�i��λ�õĽ������ݣ���������Ϊ����Ľ�����

	void setChangeBtnClickNum(int num);      //��¼�ĸ������ĸ��İ�ť������
	void setEquipBtnClickNum(int num);		//��¼�ĸ�������װ����ť������			
	int getChangeBtnClickNum();
	int getEquipBtnClickNum();

	void setChangeBtnClickStatus(bool status);     //�����Ƿ��н����ĸ��İ�ť������
	void setEquipBtnClickStatus(bool status);		//�����Ƿ��н�����װ����ť������
	bool getChangeBtnClickStatus();
	bool getEquipBtnClickStatus();

	bool onTouchBegan(Touch *touch, Event *event);
	void onTouchMoved(Touch *touch, Event *event);
	void onTouchEnded(Touch *touch, Event *event);
	SwordInformationCard* getSwordCards();    //���������ÿ�ɾ����

	int getTouchCardNum();
	void setTouchCardNum(int i);

	Point getPreviousTouchCardPoint();
	void setPreviousTouchCardPoint(Point p);

	std::vector<SwordBride*> getSwordBeChosenVec();

	void setCurTeamNum(int num);
	int getCurTeamNum();

public:
	SwordBride* m_curTeamSwords[4][6];  //�洢��ǰ�ı�ӣ������4�ӣ�ÿ��6��
private:
	SwordInformationCard* m_teamSwordCard[6];
	bool m_isChangeBtnClicked;       //�Ƿ��и��İ�ť�����£������һ�����£��������ܰ���
	bool m_isEquipBtnClicked;		//�Ƿ���װ����ť�����£������һ�����£��������ܰ���
	int m_ChangeBtnClickNum;		//��һ���ĸ��İ�ť�����£����û�У���-1��ʾ
	int m_EquipBtnClickNum;			//��һ����װ����ť�����£����û�У���-1��ʾ

	int m_touchCardNum;

	std::vector <SwordBride*>m_SwordCanBeChosen;    //�ɸ����Ľ����б�ÿ���û���ǰӵ�е����н����б�



	Point previousLoactionOfTouchedCard;   //��һ�������µĿ��Ƶ�λ�ã���������λ�ý���

	int m_curTeamNum;   //��ǰ��ʾ������һ�����
};


#endif