#include "SwordInformationCard.h"
#include "SwordBride.h"
#include "SwordEquipScene.h"

USING_NS_CC;
SwordInformationCard* SwordInformationCard::create(SwordBride* sword,int num)
{
	SwordInformationCard* card = new SwordInformationCard();
	card->init(sword,num);
	return card;

}
bool SwordInformationCard::init(SwordBride* sword,int num)
{
	this->m_isChangeBtnClicked = false;

	Size size = Director::getInstance()->getWinSize();
	this->setContentSize(Size(400, 200));
	this->setCascadeOpacityEnabled(true);

	

	if (sword != NULL)
	{
		m_swordBride = sword;

		m_numInTeam = num;
		auto UI = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("TeamFormationScene/TeamFormationScene_1.ExportJson");

		UI->setPosition(Point(0, 0));
		Size visibleSize = Director::getInstance()->getVisibleSize();

#if 0
		auto btn_change = (Button*)Helper::seekWidgetByName(UI, "Btn_change");
		btn_change->addTouchEventListener(btn_change, toucheventselector(SwordInformationCard::onClickChange));

		auto btn_equip = (Button*)Helper::seekWidgetByName(UI, "Btn_equip");
		btn_equip->addTouchEventListener(btn_equip, toucheventselector(SwordInformationCard::onClickEquip));
#endif // 0   //按钮的点击事件放在TeamFormationScene实现




		char* m_value = new char[10];
		auto label_Atk = (TextAtlas*)Helper::seekWidgetByName(UI, "Value_attack");             //cocosStudio中的LabelAtlas用TextAtlas才可以
		sprintf(m_value, "%d", (int)m_swordBride->getBrideAtk());
		label_Atk->setStringValue(m_value);

		auto label_Def = (TextAtlas*)Helper::seekWidgetByName(UI, "Value_Defence");
		sprintf(m_value, "%d", m_swordBride->getBrideDef());
		label_Def->setStringValue(m_value);

		auto label_HP = (TextAtlas*)Helper::seekWidgetByName(UI, "Value_HP");
		sprintf(m_value, "%d", m_swordBride->getBrideHp());
		label_HP->setStringValue(m_value);

		auto label_MP = (TextAtlas*)Helper::seekWidgetByName(UI, "Value_MP");
		sprintf(m_value, "%d", m_swordBride->getBrideMp());
		label_MP->setStringValue(m_value);


		addChild(UI, 0, 1);
	}
	else
	{

	}





	return true;
}

SwordInformationCard::~SwordInformationCard()
{
//析构函数需要写释放资源的代码
}




void SwordInformationCard::setSwordBirde(SwordBride* bride)
{
	m_swordBride = bride;
}
SwordBride* SwordInformationCard::getSwordBride()
{
	return m_swordBride;
}


void SwordInformationCard::setNumInTeam(int num)
{
	m_numInTeam = num;
}
int SwordInformationCard::getNumInTeam()
{
	return m_numInTeam;
}