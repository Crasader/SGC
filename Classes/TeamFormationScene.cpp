#include "TeamFormationScene.h"
#include "SwordInformationCard.h"
#include "SwordBride.h"
#include "CastGroove.h"
#include "GameData.h"

USING_NS_CC;

Scene *TeamFormationScene::createScene()
{
	auto scene = Scene::create();
	auto layer = TeamFormationScene::create();
	layer->setColor(Color3B(0, 0, 0));
	scene->addChild(layer);
	return scene;

}

bool TeamFormationScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size winsize = Director::getInstance()->getWinSize();
	this->setContentSize(Size(winsize.width*0.87631, winsize.height));




	this->m_SwordCanBeChosen.clear();

	for (int i = 0; i < 30; i++)
	{
		SwordBride *test1 = new SwordBride();
		test1->setBrideAtk(100+i);
		test1->setBrideDef(100+i);
		test1->setBrideHP(1000+i);
		test1->setBrideMP(1000+i);
		m_SwordCanBeChosen.push_back(test1);
	}    //设置所有可选择的剑姬数组  待完善数据流，通用数据应建一个类统一存放

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			m_curTeamSwords[i][j] = new SwordBride();
			m_curTeamSwords[i][j]->setBrideAtk(600 + i * 100 + j);
			m_curTeamSwords[i][j]->setBrideDef(100 + i * 100 + j);
			m_curTeamSwords[i][j]->setBrideHP(1000 + i * 1000 + j);
			m_curTeamSwords[i][j]->setBrideMP(1000 + i * 1000 + j);
			m_curTeamSwords[i][j]->setBrideSpd(10*(j+1));
		}
	}
	//四个编队的SwordBride的声明，待完善数据流，需要从json中读取



	m_isChangeBtnClicked = false;      //是否有一个更改按钮被按下
	m_isEquipBtnClicked = false;         //是否有一个装备按钮被按下 

	m_ChangeBtnClickNum = -1;
	m_EquipBtnClickNum = -1;
	m_touchCardNum = -1; 
	m_curTeamNum = 1;            //初始化时默认显示第一编队

	Button* team1Btn = Button::create("TeamFormationScene/teamBtnIcon1.png");
	Button* team2Btn = Button::create("TeamFormationScene/teamBtnIcon2.png");
	Button* team3Btn = Button::create("TeamFormationScene/teamBtnIcon3.png");
	Button* team4Btn = Button::create("TeamFormationScene/teamBtnIcon4.png");


	Button* closeBtn = Button::create("TeamFormationScene/closeBtn.png");   //关闭界面按钮，界面图标待定

	team1Btn->addTouchEventListener(team1Btn, toucheventselector(TeamFormationScene::onTeam1BtnClick));
	team2Btn->addTouchEventListener(team2Btn, toucheventselector(TeamFormationScene::onTeam2BtnClick));
	team3Btn->addTouchEventListener(team3Btn, toucheventselector(TeamFormationScene::onTeam3BtnClick));
	team4Btn->addTouchEventListener(team4Btn, toucheventselector(TeamFormationScene::onTeam4BtnClick));
	//4个编队按钮，设置触发器


	closeBtn->addTouchEventListener(closeBtn, toucheventselector(TeamFormationScene::onCloseBtnClick));

	team1Btn->setPosition(Point(20,winsize.height-50));
	team2Btn->setPosition(Point(120,winsize.height-50));
	team3Btn->setPosition(Point(220,winsize.height-50));
	team4Btn->setPosition(Point(320,winsize.height-50));

	closeBtn->setPosition(Point(420,winsize.height-50));      //位置待定

	this->addChild(team1Btn, 1, 11);
	this->addChild(team2Btn, 1, 12);
	this->addChild(team3Btn, 1, 13);
	this->addChild(team4Btn, 1, 14);

	this->addChild(closeBtn,1,15);

	for (int i = 0; i < 6; i++)
	{
		m_teamSwordCard[i] =  SwordInformationCard::create(m_curTeamSwords[0][i],i);
		if (i < 3)
		{
			m_teamSwordCard[i]->setPosition(0,i*230+20);
		}
		else
		{
			m_teamSwordCard[i]->setPosition(500,(i-3)*230+20);
		}
		Widget* UI =(Widget*) m_teamSwordCard[i]->getChildByTag(1);
		Button* btn_change = (Button*)Helper::seekWidgetByName(UI, "Btn_change");
		Button* btn_equip = (Button*)Helper::seekWidgetByName(UI, "Btn_equip");
		btn_change->addTouchEventListener(btn_change, toucheventselector(TeamFormationScene::onSwordChangeBtnClick));
		btn_equip->addTouchEventListener(btn_equip, toucheventselector(TeamFormationScene::onSwordEquipBtnClick));

#if 0
		auto dispatcher = Director::getInstance()->getEventDispatcher();
		auto myListener = EventListenerTouchOneByOne::create();
		myListener->setSwallowTouches(false);
		myListener->onTouchBegan = CC_CALLBACK_2(TeamFormationScene::onTouchBegan, this);
		myListener->onTouchMoved = CC_CALLBACK_2(TeamFormationScene::onTouchMoved, this);
		myListener->onTouchEnded = CC_CALLBACK_2(TeamFormationScene::onTouchEnded, this);
		dispatcher->addEventListenerWithSceneGraphPriority(myListener, this->m_teamSwordCard[i]);                   //将响应器绑定到card中  
#endif // 0




		this->addChild(m_teamSwordCard[i],-1,i);
	}


	auto SceneListener = EventListenerTouchOneByOne::create();
	SceneListener->setSwallowTouches(false);
	//touchBegan的实现
	SceneListener->onTouchBegan = [=](Touch* touch, Event* event){
		auto formationScene = static_cast<TeamFormationScene*>(event->getCurrentTarget());    

		Point locationInNode = formationScene->convertToNodeSpace(touch->getLocation());

		for (int i = 0; i < 6; i++)
		{
			if (formationScene->m_teamSwordCard[i]->getBoundingBox().containsPoint(locationInNode))    //遍历寻找当前点击了哪一个InformationCard
			{
				formationScene->setTouchCardNum(i);        //找到后设置当前拖动点击的Card序号 
				formationScene->setPreviousTouchCardPoint(formationScene->m_teamSwordCard[i]->getPosition()) ;       //存放点击Card的初始位置，以便进行恢复
				return true;
				break;
			}
		}
		return false;
	};

	//touchMoved类型的实现
	SceneListener->onTouchMoved = [=](Touch* touch, Event* event){
		auto formationScene = static_cast<TeamFormationScene*>(event->getCurrentTarget());
		Point nowLocation = formationScene->convertToNodeSpace(touch->getLocation());         //move的当前位置
		Point previousLoation = formationScene->convertToNodeSpace(touch->getPreviousLocation());      //move的终点位置
		Point deltaPoint = Point(nowLocation.x-previousLoation.x,nowLocation.y-previousLoation.y);   //变化的位置
		if (formationScene->getTouchCardNum() >= 0 && formationScene->getTouchCardNum()<=5)
		{
			ActionInterval* moveAction = MoveBy::create(0.1f, deltaPoint);     //MoveBy动画在进行拖动Move时比MoveTo函数效果好
			formationScene->m_teamSwordCard[formationScene->getTouchCardNum()]->runAction(moveAction);
		}


	};
	//touchEnded类型的实现
	SceneListener->onTouchEnded = [=](Touch* touch, Event* event){
		auto formationScene = static_cast<TeamFormationScene*>(event->getCurrentTarget());
		bool isSwap = false;   //是否需要互换
		int SwapNum = -1;   //存放要交互哪一个Card

		Point endPoint = formationScene->convertToNodeSpace(touch->getLocation());
		for (int i = 0; i < 6; i++)
		{
			if (formationScene->m_teamSwordCard[i]->getBoundingBox().containsPoint(endPoint) && 
				i!=formationScene->getTouchCardNum())
			{
				isSwap = true;
				SwapNum = i;
				break;
			}
		}
		if (!isSwap)   //不需要互换
		{
			Point previousPoint = formationScene->getPreviousTouchCardPoint();
			ActionInterval* moveAction = MoveTo::create(1.0f, previousPoint);
			formationScene->m_teamSwordCard[formationScene->getTouchCardNum()]->runAction(moveAction);    //恢复到原来位置
		}
		else          //需要互换，同时需要调换保存的剑姬数组的位置
		{
			Point SwapCardPoint = formationScene->m_teamSwordCard[SwapNum]->getPosition();
			Point previousPoint = formationScene->getPreviousTouchCardPoint();
			ActionInterval* move1 = MoveTo::create(1.0f, previousPoint);
			ActionInterval* move2 = MoveTo::create(1.0f,SwapCardPoint);


			formationScene->m_teamSwordCard[SwapNum]->runAction(move1);
			formationScene->m_teamSwordCard[formationScene->getTouchCardNum()]->runAction(move2);

			SwordBride* previusSword = formationScene->m_teamSwordCard[getTouchCardNum()]->getSwordBride();
			SwordBride* SwapSword = formationScene->m_teamSwordCard[SwapNum]->getSwordBride();
			formationScene->m_curTeamSwords[formationScene->getCurTeamNum()][formationScene->getTouchCardNum()] = SwapSword;
			formationScene->m_curTeamSwords[formationScene->getCurTeamNum()][SwapNum] = previusSword;             //需要将类中的m_curTeamSwords也进行变化，前3个位前排，后3个为后排，效果待定
		}
		formationScene->setTouchCardNum(-1);
	};

	//将动作监听器绑定
	_eventDispatcher->addEventListenerWithSceneGraphPriority(SceneListener, this);

	return true;
}

void TeamFormationScene::onSwordChangeBtnClick(Ref* ref, TouchEventType type)    //在编成界面中绑定更改队伍的按钮事件
{
	Button* clickedButton = dynamic_cast<Button*>(ref);
	SwordInformationCard* clickedCard = (SwordInformationCard*)clickedButton->getParent()->getParent();
	TeamFormationScene* formationScene = (TeamFormationScene*)clickedCard->getParent();         //获取当前的编程界面对象
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
	{
		if (formationScene->getChangeBtnClickStatus()== false)        //待完善功能，点击一个另外几个界面不能点击，需要关闭界面后回调函数实现
		{
			formationScene->setChangeBtnClickNum(clickedCard->getNumInTeam());


			LayerColor* SwordsList = createSwordList(formationScene->getSwordBeChosenVec());
			SwordsList->setAnchorPoint(ccp(0.5, 0.5));
			SwordsList->setPosition(400, 0);
			clickedButton->getParent()->getParent()->getParent()->addChild(SwordsList, 10);   //放在队伍编成界面中显示
			formationScene->setChangeBtnClickStatus(true);           //设置界面有变更按钮被按下

		}
		else
		{
		}
		break;
	}
	default:
		break;
	}
}

void TeamFormationScene::onSwordEquipBtnClick(Ref* ref, TouchEventType type)    //在编成界面中绑定显示装备界面按钮事件
{
	Button* clickedButton = dynamic_cast<Button*>(ref);
	SwordInformationCard* clickedCard = (SwordInformationCard*)clickedButton->getParent()->getParent();
	TeamFormationScene* formationScene = (TeamFormationScene*)clickedCard->getParent();
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
	{
		if (formationScene->getEquipBtnClickStatus() == false)     //待完善功能，点击一个另外几个界面不能点击，需要关闭界面后回调函数实现
		{
			formationScene->setEquipBtnClickNum(clickedCard->getNumInTeam());

			SwordEquipScene* equipScene =  SwordEquipScene::create(clickedCard->getSwordBride());
			equipScene->setAnchorPoint(ccp(0.5, 0.5));
			equipScene->setPosition(0, 0);    //具体坐标待定
			clickedButton->getParent()->getParent()->getParent()->addChild(equipScene, 10);   //渲染层数待定

			formationScene->setEquipBtnClickStatus(true);
		}
		//else
		{

		}

		break;
	}
	default:
		break;
	}
}

LayerColor* TeamFormationScene::createSwordList(std::vector<SwordBride*> vec_allBrides)
{



	LayerColor* SwordsListLayer = LayerColor::create(Color4B(0, 0, 0, 100), 600, 750);
	SwordsListLayer->setPosition(400, 0);    //具体坐标待定


	ListView* SwordsList = ListView::create();    //采用ListView显示列表


	SwordsList->setDirection(SCROLLVIEW_DIR_VERTICAL);
	SwordsList->setTouchEnabled(true);
	SwordsList->setBounceEnabled(true);
	SwordsList->setBackGroundImageScale9Enabled(true);
	SwordsList->setSize(Size(600, 750));
	SwordsList->setPosition(Point(0, 0));


	for (int i = 0; i < vec_allBrides.size(); i++)
	{
		SwordBride* brideItem = (SwordBride*)vec_allBrides.at(i);
		Layout* layerItem = Layout::create();
		Button* buttonItem = Button::create("TeamFormationScene/custom_button.png");

		buttonItem->setTitleText("not clicked");   //用TitleText记录是否被点击



		buttonItem->setOpacityModifyRGB(true);
		layerItem->setSize(buttonItem->getSize());

		buttonItem->setPosition(Point(layerItem->getSize().width / 2.0, layerItem->getSize().height / 2.0));

		char* str = new char[100];
		sprintf(str, "Attack:%d  Defence:%d  HP:%d  MP:%d", brideItem->getBrideAtk(), brideItem->getBrideDef(), brideItem->getBrideHp(), brideItem->getBrideMp());

		Label* label_name = Label::createWithTTF(str, "fonts/Marker Felt.ttf", 20, layerItem->getSize());
		label_name->setTextColor(Color4B(0, 0, 0, 255));
		label_name->setPosition(Point(label_name->getContentSize().width / 2.0, label_name->getContentSize().height / 2.0));

		buttonItem->addTouchEventListener(buttonItem, toucheventselector(TeamFormationScene::onSwordListItemClick));


		buttonItem->addChild(label_name, 1, 1);

		layerItem->addChild(buttonItem, 1, 1);

		SwordsList->pushBackCustomItem(layerItem);

	}
	Button* confirmBtn = Button::create("TeamFormationScene/ComfirmBtn.png");    //确定按钮

	confirmBtn->setPosition(Point(400, 50));
	confirmBtn->addTouchEventListener(confirmBtn, toucheventselector(TeamFormationScene::onSwordListConfirmClick));



	SwordsListLayer->addChild(confirmBtn, 3, 3);

	SwordsListLayer->addChild(SwordsList, 2, 2);

	return SwordsListLayer;
}

void TeamFormationScene::onSwordListItemClick(Ref* ref, TouchEventType type)
{
	Button* clickedButton = dynamic_cast<Button*>(ref);          //获取点击按钮的对象方法	
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
	{
		if (clickedButton->getTitleText() == "not clicked")
		{
			clickedButton->setTitleText("clicked");
			clickedButton->setOpacity(100);
		}
		else if (clickedButton->getTitleText() == "clicked")
		{
			clickedButton->setTitleText("not clicked");
			clickedButton->setOpacity(255);
		}
		else
		{
		}

	}
	default:
		break;
	}

}



void TeamFormationScene::onSwordListConfirmClick(Ref* ref, TouchEventType type)
{



	Button* clickedButton = dynamic_cast<Button*>(ref);
	ListView* swordList = (ListView*)(clickedButton->getParent()->getChildByTag(2));   //获取剑姬列表的ListView控件

	TeamFormationScene* formationScene = (TeamFormationScene*)clickedButton->getParent()->getParent();        //获取当前的编成界面对象

	std::vector<SwordBride*>vec_allBrides = formationScene->getSwordBeChosenVec();   //获取所有的可选剑姬列表
	Vector<Widget*> items = swordList->getItems();
	std::vector<SwordBride*>chosenSword;    //存放哪些剑姬被选中
	chosenSword.clear();
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
	{
		for (int i = 0; i < items.size(); i++)
		{
			Layout* oneItem = (Layout*)items.at(i);
			Button* buttonItem = (Button*)oneItem->getChildByTag(1);
			if (buttonItem->getTitleText() == "clicked")
			{
				chosenSword.push_back((SwordBride*)vec_allBrides.at(i));   //获取选中的剑姬数据,待完善数据流
			}
		}

		if (chosenSword.size()==1)
		{
			SwordBride* sword = (SwordBride*)chosenSword.at(0);

			Widget* UI = (Widget*)clickedButton->getParent()->getParent()->getChildByTag(1);
			if (formationScene->getChangeBtnClickNum() >= 0 && formationScene-> getChangeBtnClickNum() <= 6)
			{
				formationScene->setChangeBtnClickStatus(false);                //点击确定后列表界面消失，设置编成界面可点击更改按钮
				formationScene->refreshInformationCard(sword, formationScene->getChangeBtnClickNum());
				clickedButton->getParent()->removeFromParentAndCleanup(true);
			}
		}
		else if (chosenSword.size()==0)
		{
			clickedButton->getParent()->removeFromParentAndCleanup(true);
				//如果一个没选择，不做处理
		}
		else
		{
			//选择大于1一个，应该显示说明信息报错，弹出提示信息未实现？
		}
		break;

	}
	default:
		break;
	}
}

void TeamFormationScene::onTeam1BtnClick(Ref* ref, TouchEventType type)
{
	Button* clickedButton = dynamic_cast<Button*>(ref);          //获取点击按钮的对象方法
	TeamFormationScene* formationScene = (TeamFormationScene*)clickedButton->getParent();
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
	{
		formationScene->setCurTeamNum(1);
		for (int i = 0; i < 6; i++)
		{
			formationScene->refreshInformationCard(formationScene->m_curTeamSwords[0][i], i);
		}

	}
	default:
		break;
	}

}

void TeamFormationScene::onTeam2BtnClick(Ref* ref, TouchEventType type)
{
	Button* clickedButton = dynamic_cast<Button*>(ref);          //获取点击按钮的对象方法
	TeamFormationScene* formationScene = (TeamFormationScene*)clickedButton->getParent();
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
	{
		formationScene->setCurTeamNum(2);
		for (int i = 0; i < 6; i++)
		{
			formationScene->refreshInformationCard(formationScene->m_curTeamSwords[1][i], i);
		}
	}
	default:
		break;
	}

}

void TeamFormationScene::onTeam3BtnClick(Ref* ref, TouchEventType type)
{
	Button* clickedButton = dynamic_cast<Button*>(ref);          //获取点击按钮的对象方法
	TeamFormationScene* formationScene = (TeamFormationScene*)clickedButton->getParent();
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
	{
		formationScene->setCurTeamNum(3);
		for (int i = 0; i < 6; i++)
		{
			formationScene->refreshInformationCard(formationScene->m_curTeamSwords[2][i], i);
		}
	}
	default:
		break;
	}

}

void TeamFormationScene::onTeam4BtnClick(Ref* ref, TouchEventType type)
{
	Button* clickedButton = dynamic_cast<Button*>(ref);          //获取点击按钮的对象方法
	TeamFormationScene* formationScene = (TeamFormationScene*)clickedButton->getParent();
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
	{
		formationScene->setCurTeamNum(4);
		for (int i = 0; i < 6; i++)
		{
			formationScene->refreshInformationCard(formationScene->m_curTeamSwords[3][i], i);
		}
	}
	default:
		break;
	}

}

Point TeamFormationScene::getPreviousTouchCardPoint()
{
	return previousLoactionOfTouchedCard;
}

void TeamFormationScene::setPreviousTouchCardPoint(Point p)
{
	previousLoactionOfTouchedCard = p;
}







void TeamFormationScene::refreshInformationCard(SwordBride* bride, int i)
{

	char* m_value = new char[10];
	Widget* UI = (Widget*)(this->m_teamSwordCard[i]->getChildByTag(1));


	this->m_teamSwordCard[i]->setSwordBirde(bride);    //更新当前编队的剑姬

	auto label_Atk = (TextAtlas*)Helper::seekWidgetByName(UI, "Value_attack");             //cocosStudio中的LabelAtlas用TextAtlas才可以
	sprintf(m_value, "%d", (int)bride->getBrideAtk());
	label_Atk->setStringValue(m_value);

	auto label_Def = (TextAtlas*)Helper::seekWidgetByName(UI, "Value_Defence");
	sprintf(m_value, "%d", bride->getBrideDef());
	label_Def->setStringValue(m_value);

	auto label_HP = (TextAtlas*)Helper::seekWidgetByName(UI, "Value_HP");
	sprintf(m_value, "%d", bride->getBrideHp());
	label_HP->setStringValue(m_value);

	auto label_MP = (TextAtlas*)Helper::seekWidgetByName(UI, "Value_MP");
	sprintf(m_value, "%d", bride->getBrideMp());
	label_MP->setStringValue(m_value);

}


void TeamFormationScene::setChangeBtnClickNum(int num)
{
	m_ChangeBtnClickNum = num;

}
int TeamFormationScene::getChangeBtnClickNum()
{
	return m_ChangeBtnClickNum;
}

void TeamFormationScene::setEquipBtnClickNum(int num)
{
	m_EquipBtnClickNum = num;
}
int TeamFormationScene::getEquipBtnClickNum()
{
	return m_EquipBtnClickNum;
}

std::vector<SwordBride*> TeamFormationScene::getSwordBeChosenVec()
{
	return m_SwordCanBeChosen;
}

void TeamFormationScene::setChangeBtnClickStatus(bool status)
{
	m_isChangeBtnClicked = status;
}
void TeamFormationScene::setEquipBtnClickStatus(bool status)
{
	m_isEquipBtnClicked = status;
}
bool TeamFormationScene::getChangeBtnClickStatus()
{
	return m_isChangeBtnClicked;
}
bool TeamFormationScene::getEquipBtnClickStatus()
{
	return m_isEquipBtnClicked;
}
//onTouchBegan ,onTouchMoved,onTouchEnded函数目前未使用
bool TeamFormationScene::onTouchBegan(Touch *touch, Event *event)
{
	return true;

}

void TeamFormationScene::onTouchMoved(Touch *touch, Event *event)
{

}

void TeamFormationScene::onTouchEnded(Touch *touch, Event *event)
{


}
SwordInformationCard* TeamFormationScene::getSwordCards()
{
	return m_teamSwordCard[6];
}

void TeamFormationScene::setTouchCardNum(int num)
{
	m_touchCardNum = num;
}
int TeamFormationScene::getTouchCardNum()
{
	return m_touchCardNum;
}

void TeamFormationScene::setCurTeamNum(int num)
{
	if (num > 0 && num < 5)
	{
		m_curTeamNum = num;
	}
}

int TeamFormationScene::getCurTeamNum()
{
	return m_curTeamNum;
}



void TeamFormationScene::onCloseBtnClick(Ref* ref, TouchEventType type)
{
	Button* clickedButton = dynamic_cast<Button*>(ref);          //获取点击按钮的对象方法
	TeamFormationScene* formationScene = (TeamFormationScene*)clickedButton->getParent();

	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
	{
		std::vector<SwordBride*>currentTeamsVec;
		currentTeamsVec.clear();
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				currentTeamsVec.push_back(formationScene->m_curTeamSwords[i][j]);
			}
		}
		GameData::shareGameData()->setCurrentSwordTeam(currentTeamsVec);     //将队伍数据存储到全局变量中
		formationScene->removeFromParentAndCleanup(true);   //关闭界面
	}
	default:
		break;
	}
	
}
