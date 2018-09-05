#include "MapSelectScene.h"
#include "GameBattleScene.h"
#include "GameData.h"

USING_NS_CC;
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
using namespace  rapidjson;

Scene *MapSelectScene::createScene()
{
	auto scene = Scene::create();
	auto layer = MapSelectScene::create();
	layer->setColor(Color3B(0, 0, 0));
	scene->addChild(layer);
	return scene;

}

bool MapSelectScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size winsize = Director::getInstance()->getWinSize();
	this->setContentSize(Size(winsize.width*0.87631, winsize.height));
	

	this->setBattleMap(1);    //设置显示的地图为map_1


	Button* runBtn = Button::create("MapSelectScene/RunBtnPic.png");        //按下按钮后进入到下一个地图点

	runBtn->setPosition(Point(800,100));
	runBtn->addTouchEventListener(runBtn, toucheventselector(MapSelectScene::onRunBtnClicked));

	this->addChild(runBtn,1,10);

	Button* returnBtn = Button::create("MapSelectScene/ReturnBtnPic.png");

	returnBtn->setPosition(Point(1000,100));
	returnBtn->addTouchEventListener(returnBtn,toucheventselector(MapSelectScene::onReturnBtnClicked));
	this->addChild(returnBtn,1,11);

	Button* beginBattleBtn = Button::create("MapSelectScene/beginBtnPic.png");
	beginBattleBtn->setPosition(Point(900,300));
	beginBattleBtn->addTouchEventListener(beginBattleBtn,toucheventselector(MapSelectScene::onBeginBattleBtnClicked));

	this->addChild(beginBattleBtn,1,12);

	return true;
}

void MapSelectScene::setBattleMap(int map_id)
{
	this->m_curBattleMap = readMapJson(map_id);
	for (int i = 0; i < m_curBattleMap->m_MapPointVec.size(); i++)
	{
		BattlePoint* battleP = (BattlePoint*)m_curBattleMap->m_MapPointVec.at(i);
		Point p = battleP->m_curPoint;
		Sprite* pointSP = Sprite::create("MapSelectScene/PointPic.png");              
		pointSP->setPosition(battleP->m_curPoint);
		this->addChild(pointSP,1,i);                 //将地图的点加入到界面中，线的话直接在背景图中表示
	} 

	m_curPlayerSp = Sprite::create("MapSelectScene/PlayerSpritePic.png");
	BattlePoint* startPoint = m_curBattleMap->m_MapPointVec.at(0);
	Point pos = startPoint->m_curPoint;
	m_curPlayerSp->setPosition(Point(pos.x,pos.y+30));
	this->addChild(m_curPlayerSp,3,30);        //玩家精灵的创建，显示玩家当前在哪一个地图点上

	m_curPoint = startPoint;        //起始点为第一个地图点


	
}

void MapSelectScene::setCurrentPoint(BattlePoint* point)
{
	m_curPoint = point;
}
BattlePoint* MapSelectScene::getCurrentPoint()
{
	return m_curPoint;
}


void MapSelectScene::onRunBtnClicked(Ref* ref, TouchEventType type)
{
	Button* clickedButton = dynamic_cast<Button*>(ref);
	MapSelectScene* mapScene = (MapSelectScene*)clickedButton->getParent();
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
	{
		if (mapScene->m_curPoint->m_nextPointVec.size() == 0)
		{
			//走到了尽头，不进行处理，提示信息说明已经到尽头？
		}
		else if (mapScene->getCurrentPoint()->m_nextPointVec.size() == 1)
		{
			//下一个点只有一个，直接走向下一个点
			BattlePoint* nextPoint = (BattlePoint*)mapScene->getCurrentPoint()->m_nextPointVec.at(0);
			mapScene->setCurrentPoint(nextPoint);
			ActionInterval* spMove = MoveTo::create(1.0f,Point(nextPoint->m_curPoint));
			mapScene->m_curPlayerSp->runAction(spMove);

		}
		else
		{
			//下一个点有多个点，通过随机的方式走地图
			float ran = CCRANDOM_0_1();
			int nextNum = (int)(ran*mapScene->getCurrentPoint()->m_nextPointVec.size());        //随机到哪一个点，目前采用无规律的随机方式，具体概率待定
			BattlePoint* nextPoint = (BattlePoint*)mapScene->getCurrentPoint()->m_nextPointVec.at(nextNum);
			mapScene->setCurrentPoint(nextPoint);
			ActionInterval* spMove = MoveTo::create(1.0f, Point(nextPoint->m_curPoint));
			mapScene->m_curPlayerSp->runAction(spMove);
		}
		break;
	}
	default:
		break;
	}
}

void MapSelectScene::onReturnBtnClicked(Ref* ref, TouchEventType type)
{
	Button* clickedButton = dynamic_cast<Button*>(ref);
	MapSelectScene* mapScene = (MapSelectScene*)clickedButton->getParent();
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
	{
		//点击后返回地图起点，测试用
		BattlePoint* startP = mapScene->m_curBattleMap->m_MapPointVec.at(0);
		mapScene->setCurrentPoint(startP);
		ActionInterval* returnAction = MoveTo::create(1.0f,startP->m_curPoint);
		mapScene->m_curPlayerSp->runAction(returnAction);
		break;
	}
	default:
		break;
	}
}

BattleMap* MapSelectScene::readMapJson(int mapID)
{
	//本函数根据map的ID号，从json文件中获取地图数据
	BattleMap* battleMap = new BattleMap(mapID);

	rapidjson::Document _doc;
	bool bRet = false;
	ssize_t size = 0;
	unsigned char *pBytes = NULL;
	std::map<std::string, double>map_newSwords;
	do
	{
		char str[20];
		sprintf(str, "Map_JSON/map_%d.json", mapID);         //获取文件的地址
		pBytes = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(str, "r", &size);
		CC_BREAK_IF(pBytes == NULL || strcmp((char*)pBytes, "") == 0);
		std::string load_str((const char*)pBytes, size);
		CC_SAFE_DELETE_ARRAY(pBytes);
		_doc.Parse<0>(load_str.c_str());
		CC_BREAK_IF(_doc.HasParseError());
		if (!_doc.IsObject())
			return NULL;
		if (!_doc.HasMember("BattlePoints"))
			return NULL;
		const rapidjson::Value &pArray = _doc["BattlePoints"];

		if (!pArray.IsArray())   //如果出现了错误，退出循环
			return NULL;
		for (rapidjson::SizeType i = 0; i < pArray.Size(); i++)
		{
			BattlePoint* onePoint = new BattlePoint();
			const rapidjson::Value &p = pArray[i];
			if (p.HasMember("BattlePoint"))
			{
				const rapidjson::Value &valuePoint= p["BattlePoint"];
				if (valuePoint.HasMember("point_name") && valuePoint.HasMember("point_ID") && valuePoint.HasMember("position_x")
					&& valuePoint.HasMember("position_y") && valuePoint.HasMember("next_points"))   //确定json格式是否正确
				{
					const rapidjson::Value &V_pointName = valuePoint["point_name"];
					std::string point_name = V_pointName.GetString();   //名称

					const rapidjson::Value &V_pointID = valuePoint["point_ID"];
					int point_ID = V_pointID.GetInt();//点在地图中的ID号

					const rapidjson::Value &V_positionX = valuePoint["position_x"];
					int position_X = V_positionX.GetInt();
					const rapidjson::Value &V_positionY = valuePoint["position_y"];
					int position_Y = V_positionY.GetInt();       //横坐标和纵坐标

					const rapidjson::Value &V_nextPointsArr = valuePoint["next_points"];
					std::vector<int> nextPoints;   //下一个点的vector集合
					if (V_nextPointsArr.IsArray())
					{
						for (rapidjson::SizeType j = 0; j < V_nextPointsArr.Size(); j++)
						{
							const rapidjson::Value &V_nextPoint = V_nextPointsArr[j];
							const rapidjson::Value &V_PointID = V_nextPoint["point_ID"];
							int nextID = V_PointID.GetInt();
							nextPoints.push_back(nextID);
						}
					}
					const rapidjson::Value &V_weather = valuePoint["weather_type"];
					int weather_type = V_weather.GetInt();


					std::vector<SwordBride*>vec_enemies;
					vec_enemies.clear();
					const rapidjson::Value &V_enemyArr = valuePoint["enemies"];
					if (V_enemyArr.IsArray())
					{
						for (rapidjson::SizeType j = 0; j < V_enemyArr.Size(); j++)
						{
							const rapidjson::Value &V_enemy = V_enemyArr[j];
							int enemy_type = V_enemy["enemy_type"].GetInt();
							int enemy_atk = V_enemy["enemy_atk"].GetInt();
							int enemy_sp = V_enemy["enemy_sp"].GetInt();
							int enemy_hp = V_enemy["enemy_hp"].GetInt();
							SwordBride* oneEnemy = new SwordBride();
							oneEnemy->isEnemy = true;			//设置其类型为敌人
							oneEnemy->setBrideAtk(enemy_atk);
							oneEnemy->setBrideSpd(enemy_sp);
							oneEnemy->setBrideHP(enemy_hp);
							vec_enemies.push_back(oneEnemy);
						}
					}


					onePoint->m_ID = point_ID;
					onePoint->m_curPoint = Point(position_X, position_Y);
					onePoint->m_nextPoints_ID = nextPoints;
					onePoint->m_weather = weather_type;
					onePoint->m_enemySwords = vec_enemies;

					


					battleMap->m_MapPointVec.push_back(onePoint);    //将一个场景添加到该地图的vec中
				}
			}
		}

	} while (0);

	for (int i = 0; i < battleMap->m_MapPointVec.size(); i++)
	{
		BattlePoint* onePoint = battleMap->m_MapPointVec.at(i);
		for (int j = 0; j < onePoint->m_nextPoints_ID.size(); j++)
		{
			BattlePoint* nextPoint = battleMap->m_MapPointVec.at(onePoint->m_nextPoints_ID.at(j));
			battleMap->m_MapPointVec.at(i)->m_nextPointVec.push_back(nextPoint);
		}
	}   //构造每个point的nextPointVec，将int的vector转换成point的vector

	return battleMap;
}


void MapSelectScene::onBeginBattleBtnClicked(Ref* ref, TouchEventType type)
{
	Button* clickedButton = dynamic_cast<Button*>(ref);
	MapSelectScene* mapScene = (MapSelectScene*)clickedButton->getParent();
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:
		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
	{

		auto oneBattleScene = GameBattleScene::createScene(mapScene->getCurrentPoint());
		Director::sharedDirector()->replaceScene(CCTransitionSlideInL::create(1.0f, oneBattleScene));

		break;
	}
	default:
		break;
	}
}