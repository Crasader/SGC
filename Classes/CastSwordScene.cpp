#include "CastSwordScene.h"
#include "CastResourceScene.h"
#include "PopDlg.h"
#include "SwordBuilder.h"
USING_NS_CC;

#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
using namespace  rapidjson;
Scene* CastSwordScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	//
	// 'layer' is an autorelease object
	auto layer = CastSwordScene::create();


	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool CastSwordScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	Size winsize = Director::getInstance()->getWinSize();

	LayerColor* bgcolor = LayerColor::create(Color4B(255,255,255,255));
	this->setContentSize(Size(winsize.width*0.87631, winsize.height));  //设置layer的长宽
	bgcolor->changeWidthAndHeight(winsize.width*0.87631,winsize.height);
	Size contenSize = this->getContentSize();
	this->addChild(bgcolor);
	for(int i=0;i<4;i++)
	{
		this->isBuilding[i]=false;
		this->isEnable[i]=false;
		this->buildingTime[i] = 0;
	}					// 初始化成员变量
	//创建剪裁节点实现遮罩
	clipper = ClippingNode::create();
	this->addChild(clipper,1,1);
	clipper->setAlphaThreshold(1000.5f);
	LayerColor* layer = LayerColor::create(Color4B(0,0,0,100));
	clipper->addChild(layer);
	DrawNode* front = DrawNode::create();
	Color4F yellow = { 1, 1, 0, 1 };
	Point rect[4] = { Point(-0.3186*winsize.width, winsize.height*0.884 / 2), Point(0.3186*winsize.width, winsize.height*0.884 / 2), Point(0.3186*winsize.width, -winsize.height*0.884 / 2), Point(-0.3186*winsize.width, -winsize.height*0.884 / 2) };
	//创建一个矩形剪裁区域，槽位可以飞入飞出
	front->setPosition(Point(winsize.width*0.49,winsize.height/2));
	front->drawPolygon(rect,4,yellow,0,yellow);
	clipper->setStencil(front);

	//创建

	this->isClicked = false;
	this->isEnable[0]=true;
	this->isEnable[1]=true;
	this->isBuilding[0] = true;    //for test
	this->buildingTime[0] = 3600;
	createGrooves();

	return true;
}
void CastSwordScene::createGrooves()
{
	Size winsize = Director::getInstance()->getWinSize();
	Size contentSize = this->getContentSize();
	for (int i = 0; i < 4; i++)
	{
		this->grooveSp[i] = Sprite::create("castGroove.png");
		Size spriteSize = this->grooveSp[i]->getContentSize();
		this->grooveSp[i]->setCascadeOpacityEnabled(true);
		if (isEnable[i] == true)
		{
			this->grooveSp[i]->setOpacity(255);
			if (isBuilding[i] == true)   //如果槽位正在建造，显示建造时间等信息
			{
				char* mtime = new char[10];
				sprintf(mtime, "%02d:%02d:%02d", (int)buildingTime[i] / 3600, ((int)buildingTime[i] % 3600) / 60, (int)buildingTime[i] % 3600 % 60);
				this->timeAtlas[i] = LabelAtlas::create("0123456789","number.png",15,25,'0');
				this->timeAtlas[i]->setString(mtime);
				this->timeAtlas[i]->setPosition(Point(spriteSize.width*0.25,spriteSize.height*0.9));
				this->grooveSp[i]->addChild(this->timeAtlas[i],1,1);    //建造时间的艺术数字的实现
				this->cancelBtn[i] = Button::create("buildCancel.png");
				this->cancelBtn[i]->setPosition(Point(spriteSize.width*0.5,spriteSize.height*0.2));
				switch (i)
				{
				case 0:
					this->cancelBtn[i]->addTouchEventListener(this, toucheventselector(CastSwordScene::OnClickCancel0));
					break;
				case 1:
					this->cancelBtn[i]->addTouchEventListener(this, toucheventselector(CastSwordScene::OnClickCancel1));
					break;
				case 2:
					this->cancelBtn[i]->addTouchEventListener(this, toucheventselector(CastSwordScene::OnClickCancel2));
					break;
				case 3:
					this->cancelBtn[i]->addTouchEventListener(this, toucheventselector(CastSwordScene::OnClickCancel3));
					break;
				default:
					break;
				}
				this->grooveSp[i]->addChild(this->cancelBtn[i], 1, 1);

			}

		}
		else
		{
			//槽位不可用，设置为不可用画面，可点击购买
			this->grooveSp[i]->setOpacity(50);
		}
		this->grooveSp[i]->setContentSize(Size(winsize.width*0.1536, winsize.height*0.88667));
		this->grooveSp[i]->setPosition(Point(winsize.width*0.178+winsize.width*0.1536 / 2 + winsize.width*0.16*i,contentSize.height/2));
		//this->addChild(this->grooveSp[i], 1, i);   //更改为放到节点中
		this->clipper->addChild(this->grooveSp[i],1,i);
	}
	schedule(schedule_selector(CastSwordScene::update),1.0f);
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(CastSwordScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(CastSwordScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(CastSwordScene::onTouchEnded, this);
	this->setSwallowsTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this->grooveSp[0]);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(), this->grooveSp[1]);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(), this->grooveSp[2]);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(), this->grooveSp[3]);
	//各个槽位添加按下触发器

}

bool CastSwordScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	Point pos = this->convertToNodeSpace(touch->getLocation());
	if (isClicked == true)
		return false;

	auto target = static_cast<Sprite*>(event->getCurrentTarget());
	Point locationInNode = target->convertToNodeSpace(touch->getLocation());
	Size s = target->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);
	if (rect.containsPoint(locationInNode))
	{
		for (int i = 0; i < 4; i++)
		{
			if (target->getPosition().x == this->grooveSp[i]->getPosition().x)
				this->selectedNum = i;
		}
		return true;
	}
	return false;
}
void CastSwordScene::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
}
void CastSwordScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	Size contentSize = this->getContentSize();
	if (this->isClicked == true)
		return;
	if (isEnable[selectedNum] == false)
	{
		PopDlg* dlg = PopDlg::create("popdlgbg.jpg");
		dlg->setContentSize(Size(400, 200));
		dlg->setTitle("warning");
		dlg->setContentText("确认氪金走上人生巅峰?");
		dlg->setCallbackFunc(this, callfuncN_selector(CastSwordScene::popbtnCallback));
		dlg->addButton("LittleRunner0.png", "LittleRunner0.png", "确定", 0);
		dlg->addButton("LittleRunner0.png", "LittleRunner0.png", "取消", 1);
		this->addChild(dlg,1,1);
		this->isClicked = true;
	}
	else if (isBuilding[selectedNum] == true)
	{
		//正在建造的槽是否能按进去
	}
	else
	{
		//移动槽位，新建选择资源界面
		int moveX = this->grooveSp[selectedNum]->getPosition().x - this->grooveSp[0]->getPosition().x;
		moveGrooveAfterClick(moveX);

		DelayTime* delaytime = DelayTime::create(2);
		CallFunc *callfun = CallFunc::create(this,callfunc_selector(CastSwordScene::createCastResourceScene));
		Sequence *acton = Sequence::create(delaytime,callfun,NULL);   //延时2s后执行该函数
		this->runAction(acton);
		//createCastResourceScene();
		//创建锻造界面，效果为淡出
	}
}
void CastSwordScene::popbtnCallback(Node* node)
{
	//this->removeChildByTag(1);
	this->isClicked = false;
}

void CastSwordScene::update(float delta)
{
	for (int i = 0; i < 4; i++)
	{
		if (isBuilding[i] == true)
		{
			buildingTime[i] -= delta;
			char* mtime = new char[10];
			sprintf(mtime, "%02d:%02d:%02d", (int)buildingTime[i] / 3600, ((int)buildingTime[i] % 3600) / 60, (int)buildingTime[i] % 3600 % 60);
			this->timeAtlas[i]->setString(mtime);
		}
	}

}
void CastSwordScene::moveGrooveAfterClick(int moveX)
{
	Size contenSize = this->getContentSize();
	for (int i = 0; i < 4; i++)
	{
		Point currentPos = this->grooveSp[i]->getPosition();
		if (i <= selectedNum)
		{
			ActionInterval* moveAction = MoveTo::create(1.0f, Point(currentPos.x-moveX,contenSize.height/2));
			moveAction->setTag(i);
			this->grooveSp[i]->runAction(moveAction);  //左边槽位左移
			if (i != selectedNum)
			{
				
			}
		}
		else if (i > selectedNum)
		{
			ActionInterval* moveAction = MoveTo::create(1.0f, Point(currentPos.x+contenSize.width-moveX, contenSize.height/2));
			moveAction->setTag(i);
			this->grooveSp[i]->runAction(moveAction);  //右边槽位右移
		}
	}

}

void CastSwordScene::createCastResourceScene()
{
	Size winSize = Director::getInstance()->getWinSize();
	auto resourceScene = CastResourceScene::create();
	resourceScene->setPosition(Point(winSize.width*0.178,winSize.height*0.05667));
	clipper->addChild(resourceScene, 3, 4);
	ActionInterval* moveAction = FadeIn::create(2.0f);
	resourceScene->runAction(moveAction);


}
void CastSwordScene::setGrooveToDefaultAndUpdate(int stone_num,int iron_num,int jade_num,int soul_num)
{
	Size winsize = Director::getInstance()->getWinSize();
	Size contentSize = this->getContentSize();
	for (int i = 0; i < 4; i++)
	{
		Point defaultPos = Point(winsize.width*0.178 + winsize.width*0.1536 / 2 + winsize.width*0.16*i, contentSize.height / 2);
		ActionInterval* action = MoveTo::create(1.0f,defaultPos);
		this->grooveSp[i]->runAction(action);               //将各个槽位回归原位
	}

	//把设置的新槽的建设信息显示出来
	isBuilding[selectedNum] = true;
	Size spriteSize = this->grooveSp[selectedNum]->getContentSize();
	buildingTime[selectedNum] = calcBuildingTime(stone_num, iron_num, jade_num, soul_num);
	char* mtime = new char[10];
	sprintf(mtime, "%02d:%02d:%02d", (int)buildingTime[selectedNum] / 3600, ((int)buildingTime[selectedNum] % 3600) / 60, (int)buildingTime[selectedNum] % 3600 % 60);
	this->timeAtlas[selectedNum] = LabelAtlas::create("0123456789", "number.png", 15, 25, '0');
	this->timeAtlas[selectedNum]->setString(mtime);
	this->timeAtlas[selectedNum]->setPosition(Point(spriteSize.width*0.25, spriteSize.height*0.9));
	this->grooveSp[selectedNum]->addChild(this->timeAtlas[selectedNum], 1, 1);    //建造时间的艺术数字的实现

	this->cancelBtn[selectedNum] = Button::create("buildCancel.png");
	this->cancelBtn[selectedNum]->setPosition(Point(spriteSize.width*0.5, spriteSize.height*0.2));
	switch (selectedNum)
	{
	case 0:
		this->cancelBtn[selectedNum]->addTouchEventListener(this, toucheventselector(CastSwordScene::OnClickCancel0));
		break;
	case 1:
		this->cancelBtn[selectedNum]->addTouchEventListener(this, toucheventselector(CastSwordScene::OnClickCancel1));
		break;
	case 2:
		this->cancelBtn[selectedNum]->addTouchEventListener(this, toucheventselector(CastSwordScene::OnClickCancel2));
		break;
	case 3:
		this->cancelBtn[selectedNum]->addTouchEventListener(this, toucheventselector(CastSwordScene::OnClickCancel3));
		break;
	default:
		break;
	}
	this->grooveSp[selectedNum]->addChild(this->cancelBtn[selectedNum], 1, 1);    //添加取消按钮
	selectedNum = -1;

}
int CastSwordScene::calcBuildingTime(int stone_num, int iron_num, int jade_num, int soul_num)
{
	int buildTime;
	buildTime = stone_num / 1000 + iron_num / 100 + jade_num / 10 - soul_num;
	CalcSwordRate(stone_num, iron_num, jade_num, soul_num);

	return buildTime;
}

void CastSwordScene::OnClickCancel0(Ref*, TouchEventType type)
{
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
		this->grooveSp[0]->removeAllChildrenWithCleanup(true);
		this->isBuilding[0] = false;
		this->buildingTime[0] = 0;
		this->newSwordName [0]= "";
		break;
	case TouchEventType::TOUCH_EVENT_CANCELED:
		break;

	}
}
void CastSwordScene::OnClickCancel1(Ref*, TouchEventType type)
{
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
		this->grooveSp[1]->removeAllChildrenWithCleanup(true);
		this->isBuilding[1] = false;
		this->buildingTime[1] = 0;
		this->newSwordName[1] = "";
		break;
	case TouchEventType::TOUCH_EVENT_CANCELED:
		break;

	}
}
void CastSwordScene::OnClickCancel2(Ref*, TouchEventType type)
{
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
		this->grooveSp[2]->removeAllChildrenWithCleanup(true);
		this->isBuilding[2] = false;
		this->buildingTime[2] = 0;
		this->newSwordName[2] = "";
		break;
	case TouchEventType::TOUCH_EVENT_CANCELED:
		break;

	}
}
void CastSwordScene::OnClickCancel3(Ref*, TouchEventType type)
{
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
		this->grooveSp[3]->removeAllChildrenWithCleanup(true);
		this->isBuilding[3] = false;
		this->buildingTime[3] = 0;
		this->newSwordName[3] = "";
		break;
	case TouchEventType::TOUCH_EVENT_CANCELED:
		break;

	}
}

void CastSwordScene::CalcSwordRate(int stone_num, int iron_num, int jade_num, int soul_num)
{
#if 0
	rapidjson::Document writeObj;
	writeObj.SetObject();
	rapidjson::Document::AllocatorType& allocator = writeObj.GetAllocator();
	rapidjson::Value array(rapidjson::kArrayType);
	rapidjson::Value object(rapidjson::kObjectType);
	object.AddMember("inttag", 1, allocator);
	object.AddMember("doubletag", 2, allocator);

	array.PushBack(object, allocator);
	writeObj.AddMember("json", "json string", allocator);
	writeObj.AddMember("Array", array, allocator);
	StringBuffer buffer;
	rapidjson::Writer<StringBuffer> writer(buffer);
	writeObj.Accept(writer);
	auto path = FileUtils::getInstance()->getWritablePath();
	path.append("myhero.json");
	FILE* file = fopen(path.c_str(), "wb");
	if (file)
	{
		fputs(buffer.GetString(), file);
		fclose(file);
	}
	//将数据写为json文件的方法  
#endif // 0

	int currentRes[4];
	currentRes[0] = stone_num;
	currentRes[1] = iron_num;
	currentRes[2] = jade_num;
	currentRes[3] = soul_num;    //将当前玩家选择的资源存入数组中



	rapidjson::Document _doc;
	bool bRet = false;
	ssize_t size = 0;
	unsigned char *pBytes = NULL;
	std::map<std::string, double>map_newSwords;
	do
	{
		pBytes = cocos2d::CCFileUtils::sharedFileUtils()->getFileData("test.json", "r", &size);   //从json文件中读取各个剑姬的建造数据，计算得出会获得哪个剑姬
		CC_BREAK_IF(pBytes == NULL || strcmp((char*)pBytes, "") == 0);
		std::string load_str((const char*)pBytes, size);
		CC_SAFE_DELETE_ARRAY(pBytes);
		_doc.Parse<0>(load_str.c_str());
		CC_BREAK_IF(_doc.HasParseError());
		if (!_doc.IsObject())
			return;
		if (!_doc.HasMember("swordBuilders"))
			return;
		const rapidjson::Value &pArray = _doc["swordBuilders"];

		if (!pArray.IsArray())   //如果出现了错误，退出循环
			return;

		for (rapidjson::SizeType i = 0; i < pArray.Size(); i++)   //按照json文件中的数量进行循环
		{
			SwordBuilder* swordbuilder = new SwordBuilder();   //创建类来计算使用当前资源时每一种剑姬的获得概率
			const rapidjson::Value &p = pArray[i];
			if (p.HasMember("swordBuilder"))
			{
				const rapidjson::Value &valueSword = p["swordBuilder"];
				if (valueSword.HasMember("sword_name") && valueSword.HasMember("thresholds") && valueSword.HasMember("weight")
					&& valueSword.HasMember("rarity") && valueSword.HasMember("infl_items") && valueSword.HasMember("blueprint") &&
					valueSword.HasMember("rate_max") && valueSword.HasMember("succeed"))          //判断json文件中的格式是否正确
				{
					const rapidjson::Value &V_swordName = valueSword["sword_name"];
					std::string sword_name = V_swordName.GetString();
					swordbuilder->setName(sword_name);             //获取名称

					const rapidjson::Value &V_thresholds = valueSword["thresholds"];
					const rapidjson::Value &V_stoneThres = V_thresholds["stone_thre"];
					const rapidjson::Value &V_ironThres = V_thresholds["iron_thre"];
					const rapidjson::Value &V_jadeThres = V_thresholds["jade_thre"];
					const rapidjson::Value &V_soulThres = V_thresholds["soul_thre"];
					int thres[4];
					thres[0] = V_stoneThres.GetInt();
					thres[1] = V_ironThres.GetInt();
					thres[2] = V_jadeThres.GetInt();
					thres[3] = V_soulThres.GetInt();
					swordbuilder->setThres(thres);   //获取锻造阈值

					const rapidjson::Value &V_weight = valueSword["weight"];
					const rapidjson::Value &V_stoneWeight = V_weight["stone_weight"];
					const rapidjson::Value &V_ironWeight = V_weight["iron_weight"];
					const rapidjson::Value &V_jadeWeight = V_weight["jade_weight"];
					const rapidjson::Value &V_soulWeight = V_weight["soul_weight"];
					double weights[4];
					weights[0] = V_stoneWeight.GetDouble();
					weights[1] = V_ironWeight.GetDouble();
					weights[2] = V_jadeWeight.GetDouble();
					weights[3] = V_soulWeight.GetDouble();
					swordbuilder->setWeight(weights);   //获取各个资源权重

					const rapidjson::Value &V_rarity = valueSword["rarity"];
					double rarity = V_rarity.GetDouble();
					swordbuilder->setRarity(rarity);  //获取稀有度

					const rapidjson::Value &V_maxRate = valueSword["rate_max"];
					double maxRate = V_maxRate.GetDouble();
					swordbuilder->setMaxRate(maxRate);  //获取最大概率  

					const rapidjson::Value &V_succeed = valueSword["succeed"];
					double succeed = V_succeed.GetDouble();
					swordbuilder->setSucceed(succeed);   //获取成功率

					const rapidjson::Value &V_itemsArray = valueSword["infl_items"];
					std::map<std::string, double>map_items;
					if (V_itemsArray.IsArray())
					{
						for (rapidjson::SizeType j = 0; j < V_itemsArray.Size(); j++)
						{
							const rapidjson::Value &V_item = V_itemsArray[j];
							const rapidjson::Value &V_itemName = V_item["item_name"];
							const rapidjson::Value &V_itemInfl = V_item["infl_weight"];
							map_items.insert(std::map<std::string, double>::value_type(V_itemName.GetString(), V_itemInfl.GetDouble()));
						}
						swordbuilder->setItems(map_items);    //获取影响该剑姬的道具的集合，包括名称和影响因子
					}


					swordbuilder->setCurrentResource(currentRes);//设置当前设置的资源值
					double currentRate = swordbuilder->calcRate();
					if (currentRate != 0)
					{
						map_newSwords.insert(std::map<std::string, double>::value_type(swordbuilder->getName(), currentRate));
						//如果是该剑姬的建造概率大于0，加入可建造集合，进行随机
					}
				}
			}

		}

	} while (0);

	int sumRate=0;
	std::map<std::string, double>::iterator it = map_newSwords.begin();
	while (it != map_newSwords.end())
	{
		sumRate = sumRate + (int)(it->second * 100);
		it++;
	}
	int randomNum = rand() % sumRate;   //在所有可能的概率中相加数进行随机
	
	it = map_newSwords.begin();
	sumRate = 0;
	while (it != map_newSwords.end())
	{
		if (randomNum>=sumRate&&randomNum<sumRate+(int)(it->second*100))
		{
			std::string SwordName = (std::string)(it->first);
			newSwordName[selectedNum] = SwordName;   //获取随机数所在的范围，确定随机到了哪个剑姬
			break;
		}
		sumRate = sumRate + (int)(it->second * 100);
		it++;
	}
	
}