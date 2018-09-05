#include "ItemInfoScene.h"
#include "BackPackScene.h"
USING_NS_CC;


Scene* ItemInfoScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	//
	// 'layer' is an autorelease object
	auto layer = ItemInfoScene::create();


	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool ItemInfoScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	this->setContentSize(Size(500, 500));
	this->ignoreAnchorPointForPosition(false);
	this->setAnchorPoint(ccp(0.5f,0.5f));

	Button* confirmBtn = Button::create("ItemScene/confirm.png");
	Button* addBtn = Button::create("ItemScene/zengjia.png");
	Button* decreaseBtn = Button::create("ItemScene/jianshao.png");

	confirmBtn->setPosition(Point(325, 100));
	addBtn->setPosition(Point(300,300));
	decreaseBtn->setPosition(Point(350,300));

	selectedNum = 0;

#if 0
	numAtlas = LabelAtlas::create("0123456789", "number2.png", 46, 80, '0');
	char* str_num = new char[2];
	sprintf(str_num, "%d", selectedNum);
	numAtlas->setString(str_num);
	numAtlas->setPosition(Point(325, 150));   //物品数量艺术字的实现  
#endif // 0


	confirmBtn->addTouchEventListener(this, toucheventselector(ItemInfoScene::onConfirmClick));
	addBtn->addTouchEventListener(this, toucheventselector(ItemInfoScene::onAddBtnClick));
	decreaseBtn->addTouchEventListener(this,toucheventselector(ItemInfoScene::onDecreaseBtnClick));

	this->addChild(addBtn, 1);
	this->addChild(decreaseBtn, 1);
	this->addChild(confirmBtn, 1);
}

void ItemInfoScene::setPlayerItem(PlayerItem* myItem)
{
	m_playerItem = myItem;
	Size winSize = Director::getInstance()->getWinSize();
	int ID = myItem->getItemID();
	ImageView* bigview = ImageView::create("ItemScene/bigView.png");    //根据物品的ID，获取详细图标图片并显示
	bigview->setAnchorPoint(ccp(0.5f, 0.5f));   
	bigview->setPosition(Point(140, 200));

	this->addChild(bigview, 1, 1);
}

void ItemInfoScene::onConfirmClick(Ref*, TouchEventType type)
{
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:

		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
	{
		BackPackScene* packScene = (BackPackScene*)this->getParent();
		if (selectedNum > 0)
		{
			PlayerItem* item = m_playerItem;
			item->setItemNum(selectedNum);
		}  //当点击确认后，如果选中的物品数量大于0，放入选中物品集合中
		this->removeFromParentAndCleanup(true);
		break;
	}
	case TouchEventType::TOUCH_EVENT_CANCELED:
		break;

	}
}
void ItemInfoScene::onAddBtnClick(Ref*, TouchEventType type)
{
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:

		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
		if (selectedNum<m_playerItem->getItemNum())
		{
			//物品数量加1，并显示
			selectedNum++;
			char* str_num = new char[2];
			sprintf(str_num, "%d", selectedNum);
			numAtlas->setString(str_num);
		}
		break;
	case TouchEventType::TOUCH_EVENT_CANCELED:
		break;

	}
}

void ItemInfoScene::onDecreaseBtnClick(Ref*, TouchEventType type)
{
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
		if (selectedNum>0)
		{
			//物品数量减1，并显示
			selectedNum--;
			char* str_num = new char[2];
			sprintf(str_num, "%d", selectedNum);
			numAtlas->setString(str_num);
		}
		break;
	case TouchEventType::TOUCH_EVENT_CANCELED:
		break;

	}
}