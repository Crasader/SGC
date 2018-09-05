#include "MainScene.h"


USING_NS_CC;

Scene *MainScene::createScene()
{
	auto scene = Scene::create();
	auto layer = MainScene::create();
	scene->addChild(layer);
	return scene;

}

bool MainScene::init()
{

	auto UI = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("MainScene/MainScene.ExportJson");

	UI->setPosition(Point(0,0));
	Size visibleSize = Director::getInstance()->getVisibleSize();
	this->addChild(UI);

	//Menu* pMenu = Menu::create(pCloseItem, NULL);
	//pMenu->setPosition(Point(0, 0));
	//this->addChild(pMenu, 1);
	return true;
}
void MainScene::onClick(Ref*,TouchEventType type)
{
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_ENDED:
		break;
	}

}

