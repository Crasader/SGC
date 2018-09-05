#include "MainScene2.h"
#include "CastSwordScene.h"
#include "ItemMergeScene.h"
#include "TeamFormationScene.h"
#include "MapSelectScene.h"
#include "PlayerItem.h"
USING_NS_CC;

#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
using namespace  rapidjson;

Scene *MainScene2::createScene()
{
	auto scene = Scene::create();
	auto layer = MainScene2::create();
	layer->setColor(Color3B(0,0,0));
	scene->addChild(layer);
	return scene;

}

bool MainScene2::init()
{

	if (!Layer::init())
	{
		return false;
	}
	Size size = Director::sharedDirector()->getWinSize();
	
	auto layercolor = LayerColor::create(Color4B(255, 255, 255, 255));
	this->addChild(layercolor);

	std::string actorname = UserDefault::getInstance()->getStringForKey("PalyerName","None");
	log(actorname.c_str());    //可跨界面保存信息


	Texture2D *texture = TextureCache::sharedTextureCache()->textureForKey("MainScene/bloom_effect.png");
	Sprite* bloomImg = Sprite::createWithTexture(texture);
	bloomImg->setPosition(Point(size.width*0.5f,size.height*0.5f));
	this->addChild(bloomImg,4);   //加入主背景光圈


	texture = TextureCache::sharedTextureCache()->textureForKey("MainScene/secretary_normal.png");
	this->sp_assitant = Sprite::createWithTexture(texture);
	this->sp_assitant->setPosition(Point(size.width-150,150));
	Animation * animation = CCAnimation::create();
	animation->addSpriteFrameWithFileName("MainScene/secretary_normal.png");
	animation->addSpriteFrameWithFileName("MainScene/secretary_eye_closed.png");
	animation->setDelayPerUnit(1);
	animation->setRestoreOriginalFrame(true);
	sp_assitant->runAction(RepeatForever::create(Animate::create(animation)));
	//小秘书动画创建
	int Stone_Num = UserDefault::getInstance()->getIntegerForKey("Stone",0);
	int Iron_Num = UserDefault::getInstance()->getIntegerForKey("Iron", 0);
	int Jade_Num = UserDefault::getInstance()->getIntegerForKey("Jade", 0);
	int Soul_Num = UserDefault::getInstance()->getIntegerForKey("Soul", 0);    //从UserDefault的数据中获取资源数据

	char* resource_str = new char[10];
	sprintf(resource_str, "石：%d", Stone_Num);
	LabelTTF *lab_StoneNum = LabelTTF::create(resource_str, "Marker Felt", 20);
	sprintf(resource_str, "铁：%d", Iron_Num);
	LabelTTF *lab_IronNum = LabelTTF::create(resource_str, "Marker Felt", 20);
	sprintf(resource_str, "玉：%d", Jade_Num);
	LabelTTF *lab_JadeNum = LabelTTF::create(resource_str, "Marker Felt", 20);
	sprintf(resource_str, "魂：%d", Soul_Num);
	LabelTTF *lab_SoulNum = LabelTTF::create(resource_str, "Marker Felt", 20);
	lab_StoneNum->setPosition(Point(size.width-100,size.height-50));
	lab_IronNum->setPosition(Point(size.width-90,size.height-70));
	lab_JadeNum->setPosition(Point(size.width - 80, size.height - 90));
	lab_SoulNum->setPosition(Point(size.width - 70, size.height - 110));
	//资源显示，石铁玉魂
	
	this->angle = 0;    //拖动menu1转动的角度
	this->isBtnClick = false;
	texture = TextureCache::sharedTextureCache()->textureForKey("MainScene/backgroud_test_1.png");
	disk_top = Sprite::createWithTexture(texture);
	disk_top->setPosition(Point(-453, 375));
	disk_top->setContentSize(Size(3693,3693));
	texture = TextureCache::sharedTextureCache()->textureForKey("MainScene/menu_L1.png");
	disk_menu1 = Sprite::createWithTexture(texture);
	disk_menu1->setPosition(Point(828, 375));
	texture = TextureCache::sharedTextureCache()->textureForKey("MainScene/menu_L2.png");
	disk_menu2 = Sprite::createWithTexture(texture);
	disk_menu2->setPosition(Point(828, 375)); //创建背景图


	auto layer_menu1 = Layer::create();
	layer_menu1->setAnchorPoint(ccp(0.62,0.5));
	layer_menu1->addChild(disk_menu1);



	auto layer_menu2 = Layer::create();
	layer_menu2->setAnchorPoint(ccp(0.62,0.5));
	layer_menu2->addChild(disk_menu2);

	auto layer_top = Layer::create();
	layer_top->setAnchorPoint(ccp(-0.3396,0.5));
	layer_top->addChild(disk_top);   //创建层
	this->layer_menu1 = layer_menu1;
	this->layer_menu2 = layer_menu2;
	this->layer_top = layer_top;

	this->btnSpite[0] = Sprite::create("MainScene/m1_renwu.png");
	this->btnSpite[1] = Sprite::create("MainScene/m1_chuji.png");
	this->btnSpite[2] = Sprite::create("MainScene/m1_gongfang.png");
	this->btnSpite[3] = Sprite::create("MainScene/m1_biancheng.png");
	this->btnSpite[4] = Sprite::create("MainScene/m1_kucun.png");  //创建顶层菜单按钮

	refreshL1Btn(2);
	for (int i = 0; i < 5; i++)
	{	
		this->btnSpite[i]->setContentSize(Size(120, 120));
		layer_menu1->addChild(btnSpite[i], 1, i);
	}
#if 0
	auto selectedBtn = Sprite::create("chosenMark.png");
	selectedBtn->setContentSize(Size(150, 150));
	selectedBtn->setPosition(Point(90, 380));
	this->addChild(selectedBtn, 3, 1);   //中圈按钮,现在不需要添加
#endif // 0



	btnSpite[0]->setPosition(Point(140, 661));
	btnSpite[1]->setPosition(Point(100, 533));
	btnSpite[2]->setPosition(Point(83, 375));
	btnSpite[3]->setPosition(Point(100, 216));
	btnSpite[4]->setPosition(Point(140, 89));   //设置按钮的位置，暂时位置


	Scale9Sprite* buttonNormal = Scale9Sprite::create("MainScene/normalBtnBG.png");
	Scale9Sprite* buttonPress = Scale9Sprite::create("MainScene/chosenBtnBG.png");
	LabelTTF* title = LabelTTF::create("CastSword","Marker Felt",10);
	auto btn_dunhun = ControlButton::create(title, buttonNormal);

	btn_dunhun->setBackgroundSpriteForState(buttonPress, Control::State::HIGH_LIGHTED);
	btn_dunhun->setPreferredSize(Size(90, 90));
	btn_dunhun->setPosition(Point(200, 375));
	this->addChild(btn_dunhun, 3);    //锻魂临时按钮
	btn_dunhun->addTargetWithActionForControlEvents(this, cccontrol_selector(MainScene2::EnterCastSwordScene), Control::EventType::TOUCH_UP_INSIDE);
	//点击时调用进入剑姬建造界面

	Scale9Sprite* buttonNormal2 = Scale9Sprite::create("MainScene/normalBtnBG.png");
	Scale9Sprite* buttonPress2 = Scale9Sprite::create("MainScene/chosenBtnBG.png");
	LabelTTF* title2 = LabelTTF::create("ItemMerge", "Marker Felt", 10);

	auto btn_hecheng = ControlButton::create(title2, buttonNormal2);

	btn_hecheng->setBackgroundSpriteForState(buttonPress2, Control::State::HIGH_LIGHTED);
	btn_hecheng->setPreferredSize(Size(90, 90));
	btn_hecheng->setPosition(Point(300, 715));
	this->addChild(btn_hecheng, 3);    //物品合成临时按钮
	btn_hecheng->addTargetWithActionForControlEvents(this, cccontrol_selector(MainScene2::EnterTeamFormationScene), Control::EventType::TOUCH_UP_INSIDE);
	//点击时调用进入物品合成界面


	Scale9Sprite* buttonNormal3 = Scale9Sprite::create("MainScene/normalBtnBG.png");
	Scale9Sprite* buttonPress3 = Scale9Sprite::create("MainScene/chosenBtnBG.png");
	LabelTTF* title3 = LabelTTF::create("ItemMerge", "Marker Felt", 10);
	auto btn_map = ControlButton::create(title3, buttonNormal3);

	btn_map->setBackgroundSpriteForState(buttonPress3, Control::State::HIGH_LIGHTED);
	btn_map->setPreferredSize(Size(90, 90));    
	btn_map->setPosition(Point(240, 300));       //地图位置待确定
	this->addChild(btn_map, 3);    //地图选择临时按钮
	btn_map->addTargetWithActionForControlEvents(this, cccontrol_selector(MainScene2::EnterMapSelectScene), Control::EventType::TOUCH_UP_INSIDE);
	



#if 0

	auto btn_biancheng = ControlButton::create(title2, buttonNormal2);

	btn_biancheng->setBackgroundSpriteForState(buttonPress2, Control::State::HIGH_LIGHTED);
	btn_biancheng->setPreferredSize(Size(90, 90));
	btn_biancheng->setPosition(Point(300, 715));
	this->addChild(btn_biancheng, 3);    //物品合成临时按钮
	btn_biancheng->addTargetWithActionForControlEvents(this, cccontrol_selector(MainScene2::EnterTeamFormationScene), Control::EventType::TOUCH_UP_INSIDE);
	//点击时调用进入物品合成界面  
#endif // 0



	auto listener_menu1 = EventListenerTouchOneByOne::create();
	listener_menu1->setSwallowTouches(false);
	listener_menu1->onTouchBegan = [=](Touch* touch, Event* event){
		auto target = static_cast<Sprite*>(event->getCurrentTarget());

		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Point p = target->getAnchorPointInPoints();
		float distance = locationInNode.getDistance(p);
		if (distance>=663&&distance<=828)    //半径的检测，后期需要更改为百分比计算
		{
			return true;
		}
		return false;
		  //检查点击是否在圆弧中		
	};
	listener_menu1->onTouchMoved = [=](Touch* touch, Event* event){
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		this->isBtnClick = false;
		this->doRotateTouch(touch);    //实现随着拖动进行转动

	};

	listener_menu1->onTouchEnded = [=](Touch* touch, Event* event){
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		if (this->isBtnClick==false)
		{
			doAdjustRotate();   //对滑动进行调整，把按钮放到中间，并转动其他两层
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener_menu1,this->layer_menu1);


	auto listener_btn0 = EventListenerTouchOneByOne::create();   //按钮的点击事件设置
	listener_btn0->setSwallowTouches(false);
	listener_btn0->onTouchBegan = [=](Touch* touch, Event* event){
		// event->getCurrentTarget() returns the *listener's* sceneGraphPriority node.
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);
		if (rect.containsPoint(locationInNode))
		{
			this->isBtnClick = true;
			return true;
		}
		return false;

	};
	listener_btn0->onTouchMoved = [=](Touch* touch, Event* event){
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		this->isBtnClick = false;  //如果移动了则按照拖动逻辑处理，置标识为false
	};

	listener_btn0->onTouchEnded = [=](Touch* touch, Event* event){
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		if (this->isBtnClick==true)
		{
			doRotateBtn(target, this->layer_menu1, this->layer_menu2, this->layer_top);  //点击按钮事件处理
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener_btn0, this->btnSpite[0]);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener_btn0->clone(), this->btnSpite[1]);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener_btn0->clone(), this->btnSpite[2]);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener_btn0->clone(), this->btnSpite[3]);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener_btn0->clone(), this->btnSpite[4]);



	this->addChild(this->layer_menu1,1,0);
	this->addChild(this->layer_menu2,2,0);
	this->addChild(this->layer_top,0,0);
	this->addChild(this->sp_assitant,3,0);

	this->addChild(lab_StoneNum,3,1);
	this->addChild(lab_IronNum,3,2);
	this->addChild(lab_JadeNum, 3, 3);
	this->addChild(lab_SoulNum,3,4);


	readBridesJson();

	readItemsJson();


	return true;
}


void MainScene2::doRotateTouch(Touch* touch)
{
	float maxangle = 35;
	Point center2 = this->layer_menu1->getAnchorPointInPoints();
	Point previousLocation =this->convertToNodeSpace( touch->getPreviousLocation());
	Point previouscgp = Point(previousLocation.x - center2.x, previousLocation.y - center2.y);
	float previousVector = atan2f(previouscgp.x, previouscgp.y);
	float previousDegress = CC_RADIANS_TO_DEGREES(previousVector);
	Point nowLocation = this->convertTouchToNodeSpace(touch);
	Point nowcgp = Point(nowLocation.x - center2.x, nowLocation.y - center2.y);
	float nowVector = atan2f(nowcgp.x, nowcgp.y);
	float nowDegress = CC_RADIANS_TO_DEGREES(nowVector);
	float angleNew = (nowDegress - previousDegress);
	this->angle += angleNew;
	if (this->angle > maxangle)
		this->angle = maxangle;
	else if (this->angle < -maxangle)
		this->angle = -maxangle;
	ActionInterval* rotateTo = RotateTo::create(0.1f, this->angle);	
	this->layer_menu1->runAction(rotateTo);


	
	for (int i = 0; i < 5; i++)
	{
		ActionInterval* rotateBtn = RotateTo::create(0.1f, -angle);
		this->btnSpite[i]->runAction(rotateBtn);
	}

}
void MainScene2::doRotateBtn(Sprite* sprite, Layer* layer_1,Layer* layer_2,Layer* layer_top)
{
	Point center2 = Point(828, 375);
	Point previousLocation = sprite->getPosition();
	Point previouscgp = Point(previousLocation.x - center2.x, previousLocation.y - center2.y);
	float previousVector = atan2f(previouscgp.x, previouscgp.y);
	float previousDegress = CC_RADIANS_TO_DEGREES(previousVector);
	Point nowLocation = Point(83, 375);
	Point nowcgp = Point(nowLocation.x - center2.x, nowLocation.y - center2.y);
	float nowVector = atan2f(nowcgp.x, nowcgp.y);
	float nowDegress = CC_RADIANS_TO_DEGREES(nowVector);
	float BtnAngle = (nowDegress - previousDegress);

	ActionInterval* rotateTo = RotateTo::create(0.5, BtnAngle);
	layer_1->runAction(rotateTo);

	int n = getBtnNum(sprite);

	for (int i = 0; i < 5; i++)
	{
		ActionInterval* rotateBtn = RotateTo::create(0.5, -BtnAngle);
		btnSpite[i]->runAction(rotateBtn);
	}   //新增按钮的自适应旋转，保证字体一直朝下


	ActionInterval* rotate1 = RotateTo::create(1, 90*(n-2));
	ActionInterval* rotate2 = RotateTo::create(1, 72*(n-2));
	layer_2->runAction(rotate1);
	layer_top->runAction(rotate2);    //其他两层的对应旋转
	refreshL1Btn(n);

}
//在拖动完成后，对位置进行调整，判断当前哪个按钮位于中间，进行转动
void MainScene2::doAdjustRotate()
{
	int temp;
	bool flag = false;
	Point center = Point(83, 375);   //中心点
	for (int i = 0; i < 5; i++)
	{ 
		Point p2 = this->calcPositionAfterRotate(this->btnSpite[i]->getPosition(), layer_menu1->getAnchorPointInPoints(), this->angle);  //获取转动后各个按钮在屏幕的位置
		Size s = btnSpite[i]->getContentSize();
		Rect rect = Rect(p2.x-s.width/2,p2.y-s.height/2,s.width,s.height);
		if (rect.containsPoint(center))
		{
			temp = i;
			flag = true;
			break;
		}
	}
	if (flag==true)
		doRotateBtn(this->btnSpite[temp], this->layer_menu1, this->layer_menu2, this->layer_top);
	else if (this->angle>0)
		doRotateBtn(this->btnSpite[4], this->layer_menu1, this->layer_menu2, this->layer_top);
	else
		doRotateBtn(this->btnSpite[0], this->layer_menu1, this->layer_menu2, this->layer_top);
}
void MainScene2::update(float delta)
{
	//没有用
}
//坐标计算函数，按钮转动后的实际坐标
Point MainScene2::calcPositionAfterRotate(Point previousPos, Point center, float angel)
{
	float radius;
	if (angel < 0)
	{
		radius = CC_DEGREES_TO_RADIANS(-angel);
	}
	else
	{
		radius = CC_DEGREES_TO_RADIANS(360 - angel);
	}
	float PosX = center.x + (previousPos.x - center.x)*cos(radius) - (previousPos.y - center.y)*sin(radius);
	float PosY = center.y + (previousPos.x - center.x)*sin(radius) + (previousPos.y - center.y)*cos(radius);

	Point p = Point(PosX,PosY);
	return p;

}
//通过坐标位置判断是第几个button
int MainScene2::getBtnNum(Sprite* sprite)   
{
	Point pos = sprite->getPosition();
	int temp = 2;
	for (int i = 0; i < 5; i++)
	{
		Point btnPos = this->btnSpite[i]->getPosition();
		if (pos.y == btnPos.y)
		{
			temp = i;
			break;
		}		
	}
	return temp;
}
//进入锻魂界面
void MainScene2::EnterCastSwordScene(Ref* pSender, Control::EventType event)
{
	auto castScene = CastSwordScene::create();
	castScene->setPosition(165,0);
	castScene->setLocalZOrder(0);
	this->addChild(castScene,5,6);
	this->removeChild(this->sp_assitant);
	this->layer_top->setCascadeOpacityEnabled(true);
	this->layer_top->setOpacity(50);


}

//进入物品合成界面
void MainScene2::EnterItemMergeScene(Ref* pSender, Control::EventType event)
{
	auto itemMergeScene = ItemMergeScene::create();
	itemMergeScene->setPosition(300,0);
	this->addChild(itemMergeScene,5);
	this->removeChild(this->sp_assitant);
	this->layer_top->setCascadeOpacityEnabled(true);
	this->layer_top->setOpacity(50);
}

//进入编成界面
void MainScene2::EnterTeamFormationScene(Ref* pSender, Control::EventType event)
{
	auto teamFormationScene = TeamFormationScene::create();
	teamFormationScene->setPosition(300, 0);
	this->addChild(teamFormationScene, 5);
	this->removeChild(this->sp_assitant);
	this->layer_top->setCascadeOpacityEnabled(true);
	this->layer_top->setOpacity(50);
}
void MainScene2::EnterMapSelectScene(Ref* pSender, Control::EventType event)
{
	auto mapScene = MapSelectScene::create();
	mapScene->setPosition(300,0);
	this->addChild(mapScene,5);
	this->removeChild(this->sp_assitant);
	this->layer_top->setCascadeOpacityEnabled(true);
	this->layer_top->setOpacity(50);
}




//刷新按钮函数，将被选中的按钮设置为另一种图标，刷新未被选中的按钮
void MainScene2::refreshL1Btn(int selectedNum)
{
	switch (selectedNum)
	{
	case 0:
		btnSpite[0]->setTexture("MainScene/m1_renwu_chosen.png");
		btnSpite[1]->setTexture("MainScene/m1_chuji.png");
		btnSpite[2]->setTexture("MainScene/m1_gongfang.png");
		btnSpite[3]->setTexture("MainScene/m1_biancheng.png");
		btnSpite[4]->setTexture("MainScene/m1_kucun.png");
		break;
	case 1:
		btnSpite[0]->setTexture("MainScene/m1_renwu.png");
		btnSpite[1]->setTexture("MainScene/m1_chuji_chosen.png");
		btnSpite[2]->setTexture("MainScene/m1_gongfang.png");
		btnSpite[3]->setTexture("MainScene/m1_biancheng.png");
		btnSpite[4]->setTexture("MainScene/m1_kucun.png");
		break;
	case 2:
		btnSpite[0]->setTexture("MainScene/m1_renwu.png");
		btnSpite[1]->setTexture("MainScene/m1_chuji.png");
		btnSpite[2]->setTexture("MainScene/m1_gongfang_chosen.png");
		btnSpite[3]->setTexture("MainScene/m1_biancheng.png");
		btnSpite[4]->setTexture("MainScene/m1_kucun.png");
		break;
	case 3:
		btnSpite[0]->setTexture("MainScene/m1_renwu.png");
		btnSpite[1]->setTexture("MainScene/m1_chuji.png");
		btnSpite[2]->setTexture("MainScene/m1_gongfang.png");
		btnSpite[3]->setTexture("MainScene/m1_biancheng_chosen.png");
		btnSpite[4]->setTexture("MainScene/m1_kucun.png");
		break;
	case 4:
		btnSpite[0]->setTexture("MainScene/m1_renwu.png");
		btnSpite[1]->setTexture("MainScene/m1_chuji.png");
		btnSpite[2]->setTexture("MainScene/m1_gongfang.png");
		btnSpite[3]->setTexture("MainScene/m1_biancheng.png");
		btnSpite[4]->setTexture("MainScene/m1_kucun_chosen.png");
		break;
	default:
		break;
	}
}



void MainScene2::readBridesJson()
{
	HttpRequest* request = new HttpRequest();
	request->setUrl("http://182.61.16.5:8080/sbc/rest/brides/a/1");
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback(this, httpresponse_selector(MainScene2::onBridesHttpRequestCompleted));
	request->setTag("GET Brides JSON");
	HttpClient::getInstance()->send(request);
	request->release();

}

void MainScene2::onBridesHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
	if (!response) 	
	{
		return;
	}
	if (0 != strlen(response->getHttpRequest()->getTag())) {
		log("%s completed", response->getHttpRequest()->getTag());
	}
	if (!response->isSucceed()) {
		log("response failed");
		log("error buffer: %s", response->getErrorBuffer());
		return;

	}
	std::vector<char>* buffer = response->getResponseData();

	std::string backStr = std::string(&(*buffer->begin()), buffer->size());
	std::vector<SwordBride*>m_myBrides;
	m_myBrides.clear();

	rapidjson::Document _doc;
	_doc.Parse<0>(backStr.c_str());
	for (rapidjson::SizeType i = 0; i < _doc.Size(); i++)
	{
		const rapidjson::Value &p = _doc[i];
		SwordBride* oneBride = new SwordBride();
		oneBride->setBrideLv(p["lv"].GetInt());
		oneBride->setBrideHP(p["hp"].GetInt());
		oneBride->setBrideMP(p["mp"].GetInt());
		oneBride->setBrideAtk(p["atk"].GetInt());
		oneBride->setBrideDef(p["def"].GetInt());
		m_myBrides.push_back(oneBride);


	}
	
}

void MainScene2::readItemsJson()
{
	HttpRequest* request = new HttpRequest();
	request->setUrl("http://182.61.16.5:8080/sbc/rest/items/a/1");
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback(this, httpresponse_selector(MainScene2::onItemsHttpRequestCompleted));
	request->setTag("GET Items JSON");
	HttpClient::getInstance()->send(request);
	request->release();

}
void MainScene2::onItemsHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{

	if (!response)
	{
		return;
	}
	if (0 != strlen(response->getHttpRequest()->getTag())) {
		log("%s completed", response->getHttpRequest()->getTag());
	}
	if (!response->isSucceed()) {
		log("response failed");
		log("error buffer: %s", response->getErrorBuffer());
		return;

	}
	std::vector<char>* buffer = response->getResponseData();

	std::string backStr = std::string(&(*buffer->begin()), buffer->size());
	std::vector<PlayerItem*>m_myItems;
	m_myItems.clear();

	rapidjson::Document _doc;
	_doc.Parse<0>(backStr.c_str());
	for (rapidjson::SizeType i = 0; i < _doc.Size(); i++)
	{
		const rapidjson::Value &p = _doc[i];
		PlayerItem* oneItem = new PlayerItem();
		oneItem->setItemName(p["itemName"].GetString());
		oneItem->setItemQuality(p["quality"].GetInt());

		//oneItem->setItemRarity(p["rarity"].GetInt());    //rarity为级别，json中为字母，转为数字存储？
		oneItem->setItemPotential(p["potentialMax"].GetInt());
		oneItem->setMetalValue(p["metalValue"].GetInt());
		oneItem->setWoodValue(p["woodValue"].GetInt());
		oneItem->setWaterValue(p["waterValue"].GetInt());
		oneItem->setFireValue(p["fireValue"].GetInt());
		oneItem->setEarthValue(p["earthValue"].GetInt());

		const rapidjson::Value &V_PropertyArr = p["properties"];
		std::vector<Property> property_vec;
		if (V_PropertyArr.IsArray())   //property的数据格式与提供的json文件不一致??
		{
			for (rapidjson::SizeType j = 0; j < V_PropertyArr.Size(); j++)
			{
				const rapidjson::Value &V_property = V_PropertyArr[j];
				Property p;
				p.PropertyType = V_property["itemPropertyId"].GetInt();
				const rapidjson::Value  &V_InfluenceArr = V_property["propertyValues"];
				if (V_InfluenceArr.IsArray())
				{
					std::vector<ValueInfluence> influence_vec;
					influence_vec.clear();
					for (rapidjson::SizeType k = 0; k < V_InfluenceArr.Size(); k++)
					{
						ValueInfluence influence;
						const rapidjson::Value &V_influence = V_InfluenceArr[k];
						influence.InfluenceType = V_influence["influenceValue"].GetInt();
						influence.InfluenceValue = V_influence["potentialCost"].GetInt();
						influence_vec.push_back(influence);
					}
					p.PropetyInfluence = influence_vec;
				}
				property_vec.push_back(p);

			}
		}
		oneItem->setItemPropeties(property_vec);


		const rapidjson::Value &V_typeArr = p["types"];
		if (V_typeArr.IsArray())
		{
			std::vector<ItemType> types_vec;
			for (rapidjson::SizeType k = 0; k < V_typeArr.Size(); k++)
			{
				const rapidjson::Value &V_type = V_typeArr[k];
				ItemType oneType;
				oneType.typeName = V_type["typeName"].GetString();
				oneType.typeCode = V_type["typeCode"].GetString();
				types_vec.push_back(oneType);
			}
			oneItem->setItemTypes(types_vec);
		}



		m_myItems.push_back(oneItem);


	}



}
