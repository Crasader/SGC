#include "HelloWorldScene.h"
#include "LoadingScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    //
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	UserDefault::getInstance()->setStringForKey("PlayerName", "dragonslayer");
	UserDefault::getInstance()->setIntegerForKey("XP",1);
	UserDefault::getInstance()->setIntegerForKey("Stone",1000);
	UserDefault::getInstance()->setIntegerForKey("Iron", 1000);
	UserDefault::getInstance()->setIntegerForKey("Jade",1000);
	UserDefault::getInstance()->setIntegerForKey("Soul",1000);


	auto UI = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("HelloUI_1.ExportJson");
	UI->setPosition(Point(0,0));
	this->addChild(UI);
	auto enterBtn = (Button*)Helper::seekWidgetByName(UI, "EnterBtn");

	enterBtn->addTouchEventListener(this, toucheventselector(HelloWorld::onClick));

    
    return true;
}
void HelloWorld::onClick(Ref*, TouchEventType type)
{
	switch (type)
	{
	case TouchEventType::TOUCH_EVENT_BEGAN:

		break;
	case TouchEventType::TOUCH_EVENT_MOVED:
		break;
	case TouchEventType::TOUCH_EVENT_ENDED:
		Director::getInstance()->pushScene(LoadingScene::createScene());
		break;
	case TouchEventType::TOUCH_EVENT_CANCELED:
		break;

	}
}



