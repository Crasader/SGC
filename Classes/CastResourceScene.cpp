#include "CastResourceScene.h"
#include "CastSwordScene.h"
#include "BackPackScene.h"



USING_NS_CC;

Scene* CastResourceScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	//
	// 'layer' is an autorelease object
	auto layer = CastResourceScene::create();


	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool CastResourceScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!LayerColor::init())
	{
		return false;
	}
	Size winsize = Director::getInstance()->getWinSize();
	this->changeWidthAndHeight(winsize.width*0.6372,winsize.height*0.88667);
	this->setColor(Color3B(255,215,0));
	Size contentSize = this->getContentSize();

	auto UI = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("CastswordScene/CastSourceScene_1.ExportJson");

	UI->setPosition(Point(0, 0));
	Size visibleSize = Director::getInstance()->getVisibleSize();
	this->addChild(UI);
	this->sp_blueprint = (Sprite*)Helper::seekWidgetByName(UI,"blueprint");
	this->btn_quick = (Button*)Helper::seekWidgetByName(UI, "btn_quick");
	this->btn_start = (Button*)Helper::seekWidgetByName(UI,"btn_start");
	auto img_stone = (Sprite*)Helper::seekWidgetByName(UI, "img_stone");
	auto img_iron = (Sprite*)Helper::seekWidgetByName(UI, "img_iron");
	auto img_jade = (Sprite*)Helper::seekWidgetByName(UI, "img_jade");
	auto img_soul = (Sprite*)Helper::seekWidgetByName(UI, "img_soul");
	text_stone = (Text*)Helper::seekWidgetByName(UI, "label_stone");

	text_iron = (Text*)Helper::seekWidgetByName(UI, "label_iron");
	text_jade = (Text*)Helper::seekWidgetByName(UI, "label_jade");
	text_soul = (Text*)Helper::seekWidgetByName(UI, "label_soul");
	//加入材料图标
	//从cocostudio中加入的label需使用text来进行操作，否则会报错，应为版本bug

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			Point beginPos = this->convertToNodeSpace(img_stone->getPosition());
			numBtn[i][j] = new ResourceNumBtn("scale.png",i,j);
			numBtn[i][j]->setPosition(beginPos.x+50+120*j,beginPos.y-50-130*i);   //按钮的坐标位置有问题，从cocos提取的坐标有误？
			this->addChild(numBtn[i][j],1);
		}
		
	}
	//加入数量选择图标  带完善

	isquick = false;   //默认不进行快速建造
	m_StoneNum = 0;
	m_IronNum = 0;
	m_JadeNum = 0;
	m_SoulNum = 0;    // 初始化各个资源的数量均为0

	btn_quick->addTouchEventListener(this, toucheventselector(CastResourceScene::onClickQuick));
	btn_start->addTouchEventListener(this,toucheventselector(CastResourceScene::onClickStart));
	return true;
}

void CastResourceScene::onClickQuick(Ref*, TouchEventType type)
{
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
	{
		if (isquick == false)
		{
			isquick = true;
			btn_quick->setColor(Color3B(0, 245, 255));
		}
		else
		{
			isquick = false;
			btn_quick->setColor(Color3B(255, 255, 255));
		}
		BackPackScene* myPack = BackPackScene::create();
		myPack->setPosition(Point(0, 0));
		this->addChild(myPack, 5, 10);
		break;
	}

	}
}

void CastResourceScene::onClickStart(Ref*, TouchEventType type)
{
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
		auto parent = (CastSwordScene*)(this->getParent()->getParent());
		getResourceNum();
		parent->setGrooveToDefaultAndUpdate(m_StoneNum,m_IronNum,m_JadeNum,m_SoulNum);
		this->removeFromParentAndCleanup(true);
		break;

	}
}
void CastResourceScene::getResourceNum()
{
	m_StoneNum = numBtn[0][0]->getNum()*1000+numBtn[0][1]->getNum()*100+numBtn[0][2]->getNum()*10+numBtn[0][3]->getNum();
	m_IronNum = numBtn[1][0]->getNum() * 1000 + numBtn[1][1]->getNum() * 100 + numBtn[1][2]->getNum() * 10 + numBtn[1][3]->getNum();
	m_JadeNum = numBtn[2][0]->getNum() * 1000 + numBtn[2][1]->getNum() * 100 + numBtn[2][2]->getNum() * 10 + numBtn[2][3]->getNum();
	m_SoulNum = numBtn[3][0]->getNum() * 1000 + numBtn[3][1]->getNum() * 100 + numBtn[3][2]->getNum() * 10 + numBtn[3][3]->getNum();
}

void CastResourceScene::setSelectedItem(std::vector<PlayerItem*>items)
{
	m_selectedItems.clear();
	m_selectedItems = items;
}

