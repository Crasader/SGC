#ifndef __SWORD_INFORMATION_CARD_H__
#define __SWORD_INFORMATION_CARD_H__
#include "cocos2d.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
USING_NS_CC_EXT;
#include"SwordBride.h"
#include "SwordEquipScene.h"

using namespace cocos2d::ui;
using namespace cocostudio;
USING_NS_CC;
class SwordInformationCard :public CCNode
{
public:
	static SwordInformationCard* create(SwordBride* sword,int num);
	virtual bool init(SwordBride* sword,int num);
	void onClickChange(Ref*, TouchEventType type);
	void onClickEquip(Ref*, TouchEventType type);
	void ShowSwordsList();
	~SwordInformationCard();

	void setNumInTeam(int num);
	
	int getNumInTeam();



	SwordBride* getSwordBride();
	void setSwordBirde(SwordBride* bride);
protected:
	SwordBride* m_swordBride;
	bool m_isChangeBtnClicked;
	bool m_isEquipBtnClicked;

	int m_numInTeam;




};


#endif