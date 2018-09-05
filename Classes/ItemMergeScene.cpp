#include "ItemMergeScene.h"
#include "BackPackScene.h"
#include "PropertyBtn.h"
USING_NS_CC;

Scene* ItemMergeScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	// 'layer' is an autorelease object
	auto layer = ItemMergeScene::create();


	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool ItemMergeScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	Size winSize = Director::getInstance()->getWinSize();
	this->setContentSize(Size(850,600));

	Button* btn_confirm = Button::create("ItemMergeScene/ComfirmBtn.png");
	btn_confirm->setPosition(Point(700,100));
	btn_confirm->addTouchEventListener(this, toucheventselector(ItemMergeScene::onConfirmClick));
	this->addChild(btn_confirm,1);

	for (int i = 0; i < 4; i++)
	{
		 m_ItemIsSelected[i] = false;
		 m_btnItemSelectBox[i] = Button::create("ItemMergeScene/ItemSelectBox.png");
		 m_btnItemSelectBox[i]->setPosition(Point(87+150*i,500));
		 this->addChild(m_btnItemSelectBox[i],1);
	}
	m_btnItemSelectBox[0]->addTouchEventListener(this, toucheventselector(ItemMergeScene::onItemSelect1Click));
	m_btnItemSelectBox[1]->addTouchEventListener(this, toucheventselector(ItemMergeScene::onItemSelect2Click));
	m_btnItemSelectBox[2]->addTouchEventListener(this, toucheventselector(ItemMergeScene::onItemSelect3Click));
	m_btnItemSelectBox[3]->addTouchEventListener(this, toucheventselector(ItemMergeScene::onItemSelect4Click));




	 propetyListView = ListView::create();     //显示属性的列表

	propetyListView->setDirection(SCROLLVIEW_DIR_VERTICAL);
	propetyListView->setTouchEnabled(true);
	propetyListView->setBounceEnabled(true);
	propetyListView->setBackGroundImage("ItemMergeScene/green_edit.png");
	propetyListView->setBackGroundImageScale9Enabled(true);
	propetyListView->setSize(Size(300, 200));
	propetyListView->setPosition(Point(200,100));
//	propetyListView->addEventListenerListView(this, listvieweventselector(LayoutTest::selectedItemEvent));





	this->addChild(propetyListView);

	return true;
	
	
}
void ItemMergeScene::onConfirmClick(Ref* ref, TouchEventType type)
{
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
		DoMergeItem();
		break;
	}
}
void ItemMergeScene::onItemSelect1Click(Ref* ref, TouchEventType type)
{
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
		BackPackScene* backPack = BackPackScene::create();
		backPack->setCalledType(2);
		backPack->setPosition(Point(0, 0));
		this->addChild(backPack,2);
		break;
	}
}

void ItemMergeScene::onItemSelect2Click(Ref* ref, TouchEventType type)
{
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
		BackPackScene* backPack = BackPackScene::create();
		backPack->setCalledType(3);
		backPack->setPosition(Point(0, 0));
		this->addChild(backPack, 2);
		break;
	}
}

void ItemMergeScene::onItemSelect3Click(Ref* ref, TouchEventType type)
{
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
		BackPackScene* backPack = BackPackScene::create();
		backPack->setCalledType(4);
		backPack->setPosition(Point(0, 0));
		this->addChild(backPack, 2);
		break;
	}
}
void ItemMergeScene::onItemSelect4Click(Ref* ref, TouchEventType type)
{
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
		BackPackScene* backPack = BackPackScene::create();
		backPack->setCalledType(4);
		backPack->setPosition(Point(0, 0));
		this->addChild(backPack, 2);
		break;
	}
}

std::vector<PlayerItem*> ItemMergeScene::getAllSelectItems()
{
	std::vector<PlayerItem*>allItems;
	allItems.clear();
	for (int i = 0; i < m_selectedItems_1.size(); i++)
	{
		allItems.push_back(m_selectedItems_1.at(i));
	}
	for (int i = 0; i < m_selectedItems_2.size(); i++)
	{
		allItems.push_back(m_selectedItems_2.at(i));
	}
	for (int i = 0; i < m_selectedItems_3.size(); i++)
	{
		allItems.push_back(m_selectedItems_3.at(i));
	}
	for (int i = 0; i < m_selectedItems_4.size(); i++)
	{
		allItems.push_back(m_selectedItems_4.at(i));
	}
	return allItems;
}

int ItemMergeScene::getMergePotential()
{
	std::vector<PlayerItem*>allItems = getAllSelectItems();

	int mergePotential = 0;
	for (int i = 0; i < allItems.size(); i++)
	{
		PlayerItem* singleItem = allItems.at(i);
		mergePotential += singleItem->getItemPotential();
		for (int j = 0; j < singleItem->getItemPropeties().size(); j++)
		{
			Property p = singleItem->getItemPropeties().at(j);
			if (p.PropertyType == 5)    //待定，需要确定增加潜力值的属性编号
			{
				mergePotential += p.PropertyValue;
			}
		}
	}
	return mergePotential;
}
std::vector<Property> ItemMergeScene::getMergeAllPropeties(std::vector<PlayerItem*>items)
{
	std::vector < Property > allPropeties;
	std::vector<Property>availableMergePropeties;
	allPropeties.clear();
	availableMergePropeties.clear();
	for (int i = 0; i < items.size(); i++)
	{
		PlayerItem* singleItem = items.at(i);
		for (int j = 0; j < singleItem->getItemPropeties().size(); j++)
		{
			Property p = singleItem->getItemPropeties().at(j);
			
			allPropeties.push_back(p);
		}
	}

#if 0

	for (int i = 0; i < allPropeties.size(); i++)
	{
		Property p = allPropeties.at(i);
		availableMergePropeties = getAvailableMergePropeties(p);
		for (int j = 0; j < allPropeties.size(); j++)
		{
			if (i == j)
				continue;
			else
			{
				Property p2 = allPropeties.at(j);
				for (int k = 0; k < availableMergePropeties.size(); k++)
				{
					Property p3 = availableMergePropeties.at(k);
					if (p2.PropertyType == p3.PropertyType)
					{
						/*  存在可以组合的特性，将其组合,组合的逻辑待定*/
					}
				}

			}

		}
	}
#endif // 0



	return allPropeties;
}

void ItemMergeScene::DoMergeItem()
{
	int TotalPotential = getMergePotential();
	std::vector<PlayerItem*> allItems = getAllSelectItems();
	
	std::vector<Property>allPropeties = getMergeAllPropeties(allItems);

	for (int i = 0; i < allPropeties.size(); i++)
	{
		Property p = allPropeties.at(i);
		//Button* custom_button = Button::create("ItemMergeScene/button.png");
		PropertyBtn* custom_button = PropertyBtn::create("ItemMergeScene/button.png", "ItemMergeScene/button.png");
		custom_button->setScale9Enabled(true);
		custom_button->setSize(Size(200, 50));
		custom_button->setCurrentPropety(p);


		std::string text;
		std::string Propety_name = Value(allPropeties.at(i).PropertyType).asString();
		std::string Propety_cost = Value(allPropeties.at(i).PropetyCost).asString();
		text.append(Propety_name);
		text.append("    ");
		text.append(Propety_cost);                    //说明特性的名称和潜力消耗

		Label* btnText = Label::create();
		btnText->setString(text);
		custom_button->addChild(btnText);

		custom_button->addTouchEventListener(this, toucheventselector(ItemMergeScene::onPropetyBtnClick));

		//也应该自定义一个button？？

		Layout* custom_item = Layout::create();
		custom_item->setSize(custom_button->getSize());
		custom_button->setPosition(Point(custom_item->getSize().width / 2.0f, custom_item->getSize().height / 2.0f));
		custom_item->addChild(custom_button);
		propetyListView->pushBackCustomItem(custom_item);

	}

}

void ItemMergeScene::setSelectedItem1(std::vector<PlayerItem*>items1)
{
	m_selectedItems_1.clear();
	m_selectedItems_1 = items1;
}
void ItemMergeScene::setSelectedItem2(std::vector<PlayerItem*>items2)
{
	m_selectedItems_2.clear();
	m_selectedItems_2 = items2;
}
void ItemMergeScene::setSelectedItem3(std::vector<PlayerItem*>items3)
{
	m_selectedItems_3.clear();
	m_selectedItems_3 = items3;
}

std::vector<Property> ItemMergeScene::getAvailableMergePropeties(Property p)
{
	std::vector<Property> allAvailablePropeties;
	allAvailablePropeties.clear();
	
	//预留接口，通过寻表查看哪些特性可以合并
	return allAvailablePropeties;
}

void ItemMergeScene::onPropetyBtnClick(Ref* ref, TouchEventType type)
{
	PropertyBtn* propety_btn = dynamic_cast<PropertyBtn*>(ref);
	propety_btn->setOpacityModifyRGB(true);
	switch (type)
	{
		case TouchEventType::TOUCH_EVENT_ENDED:
		{
			if (propety_btn->getSelectedStatus() == true)
			{
				propety_btn->setSelectedStatus(false);
				propety_btn->setOpacity(255);
			}
			else
			{
				propety_btn->setSelectedStatus(true);
				propety_btn->setOpacity(100);
			}
			break;
		}
	}
}
