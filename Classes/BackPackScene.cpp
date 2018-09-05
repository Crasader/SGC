#include "BackPackScene.h"
#include "ItemMergeScene.h"
#include "CastResourceScene.h"
#include "PlayerItem.h"
USING_NS_CC;

Scene* BackPackScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	//
	// 'layer' is an autorelease object
	auto layer = BackPackScene::create();


	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool BackPackScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	Size winSize = Director::getInstance()->getWinSize();
	memset(m_pBoxAllGrid, 0, sizeof(m_pBoxAllGrid));

	m_pIntCurPage = 0;
	this->setContentSize(Size(800,600));

	m_chosenItems.clear();
	m_playerItems.clear();  //初始化后再push进去
   //测试用，先创建一系列物品
	readItemsFromJson();
	for (int i = 0; i < 30; i++)
	{
		PlayerItem* item = new PlayerItem(1);
		m_playerItems.push_back(item);
	}
	for (int i = 0; i < 30; i++)
	{
		PlayerItem* item = new PlayerItem(2);
		m_playerItems.push_back(item);
	}
	for (int i = 0; i < 30; i++)
	{
		PlayerItem* item = new PlayerItem(3);
		m_playerItems.push_back(item);
	}

	m_ItemTotalNum = m_playerItems.size();

	auto UI = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("ItemScene/ItemPackUI_1.ExportJson");
	UI->setPosition(Point(0, 0));

	

	m_pImgLight[0] = (ImageView*)Helper::seekWidgetByName(UI, "ImgLight_1");
	m_pImgLight[1] = (ImageView*)Helper::seekWidgetByName(UI, "ImgLight_2");
	m_pImgLight[2] = (ImageView*)Helper::seekWidgetByName(UI, "ImgLight_3");
	m_pImgLight[3] = (ImageView*)Helper::seekWidgetByName(UI, "ImgLight_4");    //用来显示当前处于哪一页的标识

	auto ItemPageView=(PageView*)Helper::seekWidgetByName(UI, "ItemPageView");    //用来显示Item的PageView
	
	ItemPageView->setTouchEnabled(true);
	ItemPageView->setAnchorPoint(CCPointZero);

	isItemTouchEnable = true;	//初始化可以点击各个按钮

	for (int i = 0; i < m_ItemTotalNum/24+1; i++)   //需要多少页，每一页当前为24个道具
	{
		Layout* onePage = Layout::create();
		onePage->setAnchorPoint(CCPointZero);
		onePage->setContentSize(ItemPageView->getContentSize());
		
		Size layerSize = onePage->getContentSize();
		for (int j = 0; j < 24; j++)
		{   
			m_pBoxAllGrid[i][j] = ItemBox::create("ItemScene/Item.png", "ItemScene/Item.png");
			m_pBoxAllGrid[i][j]->setTouchEnabled(true);
			m_pBoxAllGrid[i][j]->setSelectStatus(false);
			m_pBoxAllGrid[i][j]->addTouchEventListener(m_pBoxAllGrid[i][j], toucheventselector(BackPackScene::onItemClick));

			if (i * 24 + j < m_ItemTotalNum)
			{
				PlayerItem* player = m_playerItems.at(i * 24 + j);
				m_pBoxAllGrid[i][j]->setCurrentItem(player);     //将道具的数据传入每个ItemBox中
			}

			Size boxSize = m_pBoxAllGrid[i][j]->getContentSize();
			float x = boxSize.width / 2+20 +(boxSize.width+ 20 )* (j % 8);
			float y =onePage->getContentSize().height- boxSize.height / 2 - 20-(boxSize.height+20)*(j / 8 );
			m_pBoxAllGrid[i][j]->setPosition(Point(x,y));
			onePage->addChild(m_pBoxAllGrid[i][j]);
		}
		ItemPageView->addPage(onePage);
	}

	ItemPageView->addEventListenerPageView(this, pagevieweventselector(BackPackScene::pageViewEvent));
	ItemPageView->setPosition(Point(30, 80));

	filterCheckBox[0] = (CheckBox*)Helper::seekWidgetByName(UI, "Checkbox1");
	filterCheckBox[1] = (CheckBox*)Helper::seekWidgetByName(UI, "Checkbox2");
	filterCheckBox[2] = (CheckBox*)Helper::seekWidgetByName(UI, "Checkbox3");
	filterCheckBox[3] = (CheckBox*)Helper::seekWidgetByName(UI, "Checkbox4");

	
	auto FilterBtn_1 = (Button*)Helper::seekWidgetByName(UI, "FilterBtn_1");
	auto FilterBtn_2 = (Button*)Helper::seekWidgetByName(UI, "FilterBtn_2");
	auto FilterBtn_3 = (Button*)Helper::seekWidgetByName(UI, "FilterBtn_3");
	auto FilterBtn_4 = (Button*)Helper::seekWidgetByName(UI, "FilterBtn_4");  
	auto ConfirmBtn = (Button*)Helper::seekWidgetByName(UI,"FilterBtn_5");   //作为确认按钮使用

	auto ExtBtn_1 = (Button*)Helper::seekWidgetByName(UI, "ExtBtnTest_1");
	auto ExtBtn_2 = (Button*)Helper::seekWidgetByName(UI, "ExtBtnTest_2");

	auto FilterConfirmBtn = (Button*)Helper::seekWidgetByName(UI, "FilterConfirmBtn");


	ExtBtn_1->addTouchEventListener(this, toucheventselector(BackPackScene::onExtBtn1Click));
	ExtBtn_1->addTouchEventListener(this, toucheventselector(BackPackScene::onExtBtn2Click));

	FilterBtn_1->addTouchEventListener(this, toucheventselector(BackPackScene::onFilterBtn1Click));
	FilterBtn_2->addTouchEventListener(this, toucheventselector(BackPackScene::onFilterBtn2Click));
	FilterBtn_3->addTouchEventListener(this, toucheventselector(BackPackScene::onFilterBtn3Click));
	FilterBtn_4->addTouchEventListener(this, toucheventselector(BackPackScene::onSortByTypeClick));
	ConfirmBtn->addTouchEventListener(this, toucheventselector(BackPackScene::onSelectConfirmClick));
	//为筛选按钮添加触发事件
	
	FilterConfirmBtn->addTouchEventListener(this, toucheventselector(BackPackScene::onFilterByCheckBoxClick));
	isExtension1Enable = false;
	isExtension2Enable = false;

	this->addChild(UI, 1,1);
	return true;
}

//物品点击函数，当单击时，会选中取消，当长按时，会显示物品详细信息
void BackPackScene::onItemClick(Ref* ref, TouchEventType type)
{
	Size winSize = Director::getInstance()->getWinSize();
	ItemBox* Item = dynamic_cast<ItemBox*>(ref);
	Item->setOpacityModifyRGB(true);
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
	{
		//为每个ItemBox添加触发事件，单机可以放大并选择数量
		if (m_isLongTouch)
		{
			PlayerItem* playerItem=Item->getCurrentItem();             // 
			ItemInfoScene* itemInfo = ItemInfoScene::create();
			itemInfo->setPlayerItem(playerItem);
			Point currentItemPos = Item->getPosition();
			itemInfo->setPosition(currentItemPos);
			itemInfo->setScale(0.16f, 0.16f);
			this->getParent()->getParent()->getParent()->getParent()->addChild(itemInfo, 10);   //在按钮处理函数中this是其本身？
			//addChild(itemInfo, 10);
			MoveTo* moveAction = MoveTo::create(0.5f, Point(winSize.width*0.4f, winSize.height*0.4f));
			ScaleTo* scaleAction = ScaleTo::create(0.5f, 1.0f);
			Spawn *spawn = Spawn::create(moveAction, scaleAction,NULL);
			itemInfo->runAction(spawn);

		}
		else
		{
			if (Item->getSelectStatus() == false)
			{
				Item->setSelectStatus(true);
				Item->setScale(1.2);
			}
			else
			{
				Item->setSelectStatus(false);
				Item->setScale(1);
			}
		}
		m_isTouch = false;
		m_isLongTouch = false;
		break;
	}
	case TouchEventType::TOUCH_EVENT_BEGAN:  
	{
		m_isTouch = true;
		m_isLongTouch = false;
		m_pressTime = 0;
		this->schedule(schedule_selector(BackPackScene::updateLongTouch), 0.5f);   //调用函数的周期为0.5s，1s则判断为长按
		break;
	}
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	default:
		break;
	}
}

void BackPackScene::updateLongTouch(float dt)     //判断是长按还是短按的函数，程序会在按下时周期调用该函数计算时间
{
	if (m_isTouch)
	{
		m_pressTime++;
		if (m_pressTime > 2)
		{
			m_isLongTouch = true;
		}
	}
	else
	{
		m_pressTime = 0;
	}
}

void BackPackScene::onFilterBtn1Click(Ref* ref, TouchEventType type)
{
	std::vector<PlayerItem*>filterItems;
	filterItems.clear();
	switch (type)
	{
	//为第一个筛选按钮添加触发，重新刷新pageview
	case TouchEventType::TOUCH_EVENT_ENDED:
	{
		for (int i = 0; i < m_playerItems.size(); i++)
		{
			PlayerItem* item = m_playerItems.at(i);
			if (item->getItemID() == 1)
			{
				filterItems.push_back(item);
			}
		}
		refreshGrid(filterItems);
		break;
	}
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	default:
		break;
	}
}
void BackPackScene::onFilterBtn2Click(Ref* ref, TouchEventType type)
{
	std::vector<PlayerItem*>filterItems;
	filterItems.clear();
	switch (type)
	{
		//为第二个筛选按钮添加触发，重新刷新pageview
	case TouchEventType::TOUCH_EVENT_ENDED:
	{
		for (int i = 0; i < m_playerItems.size(); i++)
		{
			PlayerItem* item = m_playerItems.at(i);
			if (item->getItemID() == 2)
			{
				filterItems.push_back(item);
			}
		}
		refreshGrid(filterItems);
		break;
	}
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	default:
		break;
	}
}
void BackPackScene::onFilterBtn3Click(Ref* ref, TouchEventType type)
{
	std::vector<PlayerItem*>filterItems;
	filterItems.clear();
	switch (type)
	{
		//为第三个筛选按钮添加触发，重新刷新pageview
	case TouchEventType::TOUCH_EVENT_ENDED:
	{
		for (int i = 0; i < m_playerItems.size(); i++)
		{
			PlayerItem* item = m_playerItems.at(i);
			if (item->getItemID() == 3)
			{
				filterItems.push_back(item);
			}
		}
		refreshGrid(filterItems);
		break;
	}
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	default:
		break;
	}
}

void BackPackScene::onExtBtn1Click(Ref* ref, TouchEventType type)
{
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
		FilterExtensionIn();
		break;
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	default:
		break;
	}
}

void BackPackScene::onExtBtn2Click(Ref* ref, TouchEventType type)
{
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
		SortExtensionIn();
		break;
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	default:
		break;
	}
}

void BackPackScene::onFilterByCheckBoxClick(Ref* ref, TouchEventType type)
{
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
	{
		for (int i = 0; i < 4; i++)
		{
			if (filterCheckBox[i]->getSelectedState())
			{
				log("checkbox clicked!");
			}
			
		}
		break;
	}
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	default:
		break;
	}
}


void BackPackScene::onSortByTypeClick(Ref* ref, TouchEventType type)
{
	//重新排序m_playerItems,按照类别写入不同的vector中再进行合并
	std::vector<PlayerItem*>ItemsType1;    
	std::vector<PlayerItem*>ItemsType2;
	std::vector<PlayerItem*>ItemsType3;
	ItemsType1.clear();
	ItemsType2.clear();
	ItemsType3.clear();

	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
	{
		for (int i = 0; i < m_playerItems.size(); i++)
		{
			PlayerItem* item = m_playerItems.at(i);
			switch (item->getItemID())
			{
			case 1:
				ItemsType1.push_back(item);
				break;
			case 2:
				ItemsType2.push_back(item);
				break;
			case 3:
				ItemsType3.push_back(item);
				break;
			default:
				break;
			}

		}
		for (int i = 0; i < ItemsType2.size(); i++)
			ItemsType3.push_back(ItemsType2.at(i));
		for (int i = 0; i < ItemsType1.size(); i++)
			ItemsType3.push_back(ItemsType1.at(i));
		refreshGrid(ItemsType3);
		break;
	}
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	default:
		break;
	}
}

void BackPackScene::onSelectConfirmClick(Ref* ref, TouchEventType type)
{

	switch (type)
	{
		//为第一个筛选按钮添加触发，重新刷新pageview
	case TouchEventType::TOUCH_EVENT_ENDED:
	{
		ReturnToLastScene();
		break;
	}
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	default:
		break;
	}
}

void BackPackScene::pageViewEvent(Ref* ref, PageViewEventType type)
{
	//当翻页时下发小按钮会进行改变，目前小按钮为固定的数目，需要根据物品栏的实际页数进行改变

	switch (type)
	{
	case PAGEVIEW_EVENT_TURNING:
	{
		PageView* pageview = dynamic_cast<PageView*>(ref);
		if (m_pIntCurPage != pageview->getCurPageIndex())
		{
			m_pIntCurPage = pageview->getCurPageIndex();
		}
		for (int i = 0; i < PAGE_SUM_NUM; i++)
		{
			if (i == m_pIntCurPage)
			{
				m_pImgLight[i]->loadTexture("ItemScene/green.png");
			}
			else
			{
				m_pImgLight[i]->loadTexture("ItemScene/red.png");
			}
			
		}
	}
	default:
		break;
	}
}
void BackPackScene::refreshGrid(std::vector<PlayerItem*>playerItems)
{
	auto ItemPageView = (PageView*)	this->getChildByTag(1)->getChildByTag(22);
	ItemPageView->removeAllPages();
	for (int i = 0; i < playerItems.size() / 24 + 1; i++)   //需要多少页，每一页当前为24个道具
	{
		Layout* onePage = Layout::create();
		onePage->setAnchorPoint(CCPointZero);
		onePage->setContentSize(ItemPageView->getContentSize());

		Size layerSize = onePage->getContentSize();
		for (int j = 0; j < 24; j++)
		{
			auto itembox = ItemBox::create("ItemScene/Item.png", "ItemScene/Item.png");
			itembox->setTouchEnabled(true);
			itembox->addTouchEventListener(itembox, toucheventselector(BackPackScene::onItemClick));

			if (i * 24 + j <  playerItems.size())
			{
				PlayerItem* player = playerItems.at(i * 24 + j);
				itembox->setCurrentItem(player);     //将道具的数据传入每个ItemBox中
			}

			Size boxSize = itembox->getContentSize();
			float x = boxSize.width / 2 + 20 + (boxSize.width + 20)* (j % 8);
			float y = onePage->getContentSize().height - boxSize.height / 2 - 20 - (boxSize.height + 20)*(j / 8);
			itembox->setPosition(Point(x, y));
			onePage->addChild(itembox);
		}
		ItemPageView->addPage(onePage);
	}
}
void BackPackScene::sortItemsByType()
{

}

void BackPackScene::FilterExtensionIn()
{
	auto filterExtension = (Layout*)this->getChildByTag(1)->getChildByTag(27);
	if (isExtension1Enable == false)
	{
		ActionInterval* moveAction = MoveBy::create(1.0f, Point(120, 0));
		filterExtension->runAction(moveAction);
		isExtension1Enable = true;
	}
	else
	{
		ActionInterval* moveAction = MoveBy::create(1.0f, Point(-120, 0));
		filterExtension->runAction(moveAction);
		isExtension1Enable = false;
	}
}

void BackPackScene::SortExtensionIn()
{
	auto SortExtension = (Layout*)this->getChildByTag(1)->getChildByTag(28);
	if (isExtension2Enable == false)
	{
		ActionInterval* moveAction = MoveBy::create(1.0f, Point(-120, 0));
		SortExtension->runAction(moveAction);
		isExtension2Enable = true;
	}
	else
	{
		ActionInterval* moveAction = MoveBy::create(1.0f, Point(120, 0));
		SortExtension->runAction(moveAction);
		isExtension2Enable = false;
	}
}
void BackPackScene::getSelectedItems()
{
	for (int i = 0; i < m_ItemTotalNum / 24 + 1; i++)
	{
		for (int j = 0; j < 24; j++)
		{
			if (m_pBoxAllGrid[i][j]->getSelectStatus() == true)
			{
				m_chosenItems.push_back(m_pBoxAllGrid[i][j]->getCurrentItem());
			}
		}
	}
}



void BackPackScene::readItemsFromJson()
{
	rapidjson::Document _doc;
	bool bRet = false;
	ssize_t size = 0;
	unsigned char *pBytes = NULL;
	std::map<std::string, double>map_Items;
	do 
	{	
		pBytes = cocos2d::CCFileUtils::sharedFileUtils()->getFileData("JsonFile/Items.json", "r", &size);
		CC_BREAK_IF(pBytes == NULL || strcmp((char*)pBytes, "") == 0);
		std::string load_str((const char*)pBytes, size);
		CC_SAFE_DELETE_ARRAY(pBytes);
		_doc.Parse<0>(load_str.c_str());
		CC_BREAK_IF(_doc.HasParseError());
		if (!_doc.IsObject())
			return;
		if (!_doc.HasMember("Items"))
			return;
		const rapidjson::Value &pArray = _doc["Items"];
		if (!pArray.IsArray())   //如果出现了错误，退出循环
			return;
		for (rapidjson::SizeType i = 0; i < pArray.Size(); i++)
		{
			const rapidjson::Value &p = pArray[i];
			if (p.HasMember("Item"))
			{
				const rapidjson::Value &valueItem = p["Item"];
				if (valueItem.HasMember("Item_ID") && valueItem.HasMember("Item_Name") && valueItem.HasMember("Item_Types")
					&& valueItem.HasMember("Item_Rarity") && valueItem.HasMember("Item_Propeties") && valueItem.HasMember("Item_Potential")
					&& valueItem.HasMember("Item_Effect") && valueItem.HasMember("Item_Quality"))
				{
					const rapidjson::Value &V_ItemID = valueItem["Item_ID"];
					int id = V_ItemID.GetInt();
					PlayerItem* item = new PlayerItem(id);     //创建PlayerItem类存储数据

					const rapidjson::Value &V_ItemName = valueItem["Item_Name"];
					std::string item_name = V_ItemName.GetString();
					item->setItemName(item_name);
					
					const rapidjson::Value &V_TypesArray = valueItem["Item_Types"];
					std::vector<ItemType>vec_types;   //物品类型都是int类型
					if (V_TypesArray.IsArray())
					{
						for (rapidjson::SizeType j = 0; j < V_TypesArray.Size(); j++)
						{
							const rapidjson::Value &V_type = V_TypesArray[j];
							ItemType oneType;
							oneType.typeName = V_type["typeName"].GetString();
							oneType.typeCode = V_type["typeCode"].GetString();
							vec_types.push_back(oneType);
						}   //提取类型数据
						item->setItemTypes(vec_types);
					}

					const rapidjson::Value &V_rarity = valueItem["Item_Rarity"];
					int rarity = V_rarity.GetInt();
					item->setItemRarity(rarity);
					
					const rapidjson::Value &V_potential = valueItem["Item_Potential"];
					int potential = V_potential.GetInt();
					item->setItemPotential(potential);

					const rapidjson::Value &V_quality = valueItem["Item_Quality"];
					int quality = V_quality.GetInt();
					item->setItemQuality(quality);

					const rapidjson::Value &V_PropetiesArray = valueItem["Item_Propeties"];
					std::vector<Property>vec_propeties;
					if (V_PropetiesArray.IsArray())
					{
						for (rapidjson::SizeType j = 0; j < V_PropetiesArray.Size(); j++)
						{
							const rapidjson::Value &V_propety = V_PropetiesArray[j];
							const rapidjson::Value &V_propetyType = V_propety["Propety_Type"];
							const rapidjson::Value &V_propetyValue = V_propety["Propety_Value"];
							const rapidjson::Value &V_propetyCost = V_propety["Propety_PotentialCost"];
							const rapidjson::Value &V_propetyInfluence = V_propety["Propety_Influence"];
							std::vector<ValueInfluence>vec_influence;
							for (rapidjson::SizeType k = 0; k < V_propetyInfluence.Size(); k++)
							{
								const rapidjson::Value &V_influence = V_propetyInfluence[k];							
								const rapidjson::Value &V_influenceType = V_influence["Influence_Type"];
								const rapidjson::Value &V_influenceValue = V_influence["Influence_Value"];
								struct ValueInfluence VI;
								VI.InfluenceType = V_influenceType.GetInt();
								VI.InfluenceValue = V_influenceValue.GetInt();
								vec_influence.push_back(VI);
							}
							struct  Property p;
							p.PropertyType = V_propetyType.GetInt();
							p.PropertyValue = V_propetyValue.GetInt();
							p.PropetyCost = V_propetyCost.GetInt();
							p.PropetyInfluence = vec_influence;							
							vec_propeties.push_back(p);
						}
						item->setItemPropeties(vec_propeties);
					}

					const rapidjson::Value &V_Effect = valueItem["Item_Effect"];
					const rapidjson::Value &V_EffectName = V_Effect["Effect_Type"];
					const rapidjson::Value &V_EffectValue = V_Effect["Effect_Value"];
					struct  Property p;
					p.PropertyType = V_EffectName.GetInt();
					p.PropertyValue = V_EffectValue.GetInt();

					item->setItemEffect(p);

					m_playerItems.push_back(item);
				}
			}			
		}

	} while (0);
}

void BackPackScene::exportSelectedItem()        //是否需要删除？？
{
	for (int i = 0; i < PAGE_SUM_NUM; i++)
	{
		for (int j = 0; j < EACH_PAGE_SUM; j++)
		{
			if (m_pBoxAllGrid[i][j]->getSelectStatus() == true)
			{
				PlayerItem *chosenItem = m_pBoxAllGrid[i][j]->getCurrentItem();    //获取是哪一个道具别选中
				break;
			}
		}
		
	}
	ItemMergeScene* itemMergeScene = (ItemMergeScene*)this->getParent();
	
	
	
}

void BackPackScene::setCalledType(int type)
{
	this->m_calledType = type;
}
int BackPackScene::getCalledType()
{
	return m_calledType;
}

void BackPackScene::ReturnToLastScene()
{
	int type = this->m_calledType;
	getSelectedItems();     //获取选择的按钮
	switch (type)
	{
	case 1:          //被锻造资源选择界面调用时的回调实现
	{
		auto lastScene = (CastResourceScene*)(this->getParent());
		lastScene->setSelectedItem(m_chosenItems);
		this->removeFromParent();
		break;
	}
	case 2:        //在物品合成界面的第一个选择框时调用时的回调实现
	{
		auto MergeScene = (ItemMergeScene*)(this->getParent());
		MergeScene->setSelectedItem1(m_chosenItems);
		this->retain();
		this->removeFromParentAndCleanup(true);
		break;
	}
	case 3:			 //在物品合成界面的第二个选择框时调用时的回调实现
	{
		auto MergeScene = (ItemMergeScene*)(this->getParent());
		MergeScene->setSelectedItem2(m_chosenItems);
		this->removeFromParentAndCleanup(true);
		//this->removeFromParent();
		break;
	}
	case 4:			 //在物品合成界面的第三个选择框时调用时的回调实现
	{
		ItemMergeScene* MergeScene = (ItemMergeScene*)(this->getParent());
		MergeScene->setSelectedItem3(m_chosenItems);
		this->removeFromParent();
		break;
	}
	default:
		break;
	}


}











ItemBox::ItemBox()
{
	

}

ItemBox* ItemBox::create(const std::string& normalImage, const std::string& selectedImage)
{
	ItemBox *posBox = new ItemBox();
	if (posBox&&posBox->init(normalImage, selectedImage))
	{
		posBox->loadTextures(normalImage, selectedImage);
		posBox->autorelease();
		return posBox;
	}
	CC_SAFE_DELETE(posBox);
	return nullptr;
}




ItemBox::~ItemBox()
{

}
void ItemBox::setCurrentItem(PlayerItem* item)
{
	Size contentSize = this->getContentSize();
	m_currentItem = item;

	Sprite* itemImg = Sprite::create("ItemScene/Item.png");
	itemImg->setAnchorPoint(ccp(0.5f, 0.5f));
	itemImg->setPosition(Point(contentSize.width*0.5f,contentSize.height*0.5f));
	if (m_currentItem->getItemID() == 1)
	{
		itemImg->setTexture("ItemScene/Item1.png");
	}
	else if (m_currentItem->getItemID() == 2)
	{
		itemImg->setTexture("ItemScene/Item2.png");
	}
	else if (m_currentItem->getItemID() == 3)
	{
		itemImg->setTexture("ItemScene/Item3.png");
	}
	else
	{
	}

	this->addChild(itemImg,1,1);
}
PlayerItem* ItemBox::getCurrentItem()
{
	return m_currentItem;
}


void ItemBox::setSelectStatus(bool status)
{
	isSelected = status;
}
bool ItemBox::getSelectStatus()
{
	return isSelected;
}
void ItemBox::setEmptyStatus(bool status)
{
	isEmpty = status;
}
bool ItemBox::getEmptyStatus()
{
	return isEmpty;
}




