
#include "SwordBride.h"
#include "SwordEquipScene.h"
#include "TeamFormationScene.h"


USING_NS_CC;

SwordEquipScene* SwordEquipScene::create(SwordBride* bride)
{
	SwordEquipScene* scene = new SwordEquipScene();
	if(scene->init(bride))
	return scene;
}

bool SwordEquipScene::init(SwordBride* bride)
{
	//////////////////////////////
	// 1. super init first

	m_swordBride = bride;
	Size size = Director::getInstance()->getWinSize();

	auto UI = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("TeamFormationScene/SwordEquipScene/SwordEquipScene_1.ExportJson");
	UI->setPosition(Point(0, 0));

	auto bride_img = (ImageView*)Helper::seekWidgetByName(UI, "Bride_Image");
	bride_img->loadTexture("TeamFormationScene/SwordEquipScene/SwordImage.jpg");

	auto confirm_btn = (Button*)Helper::seekWidgetByName(UI, "button_confirm");
	confirm_btn->addTouchEventListener(confirm_btn, toucheventselector(SwordEquipScene::onConfirmBtnClick));

	auto cancel_btn = (Button*)Helper::seekWidgetByName(UI, "button_cancel");


	auto atk_text = (TextAtlas*)Helper::seekWidgetByName(UI, "value_atk");
	auto def_text = (TextAtlas*)Helper::seekWidgetByName(UI, "value_def");
	auto hp_text = (TextAtlas*)Helper::seekWidgetByName(UI, "value_HP");
	auto mp_text = (TextAtlas*)Helper::seekWidgetByName(UI, "Value_MP");

	char* m_value = new char[10];
;
	sprintf(m_value, "%d", (int)m_swordBride->getBrideAtk());
	atk_text->setStringValue(m_value);

	sprintf(m_value, "%d", (int)m_swordBride->getBrideDef());
	def_text->setStringValue(m_value);

	sprintf(m_value, "%d", (int)m_swordBride->getBrideHp());
	hp_text->setStringValue(m_value);

	sprintf(m_value, "%d", (int)m_swordBride->getBrideMp());
	mp_text->setStringValue(m_value);

	this->addChild(UI);



	return true;
}

SwordEquipScene::~SwordEquipScene()
{

}

void SwordEquipScene::onConfirmBtnClick(Ref* ref, TouchEventType type)
{
	Button* confirmBtn = dynamic_cast<Button*>(ref);
	TeamFormationScene* formationScene = (TeamFormationScene*)(confirmBtn->getParent()->getParent()->getParent());
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
	{
		confirmBtn->getParent()->getParent()->removeAllChildrenWithCleanup(true);
		formationScene->setEquipBtnClickStatus(false);
	}
	default:
		break;
	}
}