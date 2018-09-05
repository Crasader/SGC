#include "LoadingScene.h"
#include "MainScene.h"
#include "MainScene2.h"

USING_NS_CC;

Scene *LoadingScene::createScene()
{
	auto scene = Scene::create();
	auto layer = LoadingScene::create();
	scene->addChild(layer);
	return scene;

}

bool LoadingScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size size = Director::sharedDirector()->getWinSize();

	currentNum = 0;
	totalNum = 6;


	//返回
	/* //
	MenuItemImage *back = MenuItemImage::create("backA.png", "backB.png", this, menu_selector(LoadingScene::menuBackCallback));
	if (size.height > 500)
	{
		back->setScale(2.0f);
	}
	back->setPosition(ccp(size.width - 60, size.height - 60));
	back->setEnabled(false);
	Menu* mainmenu = Menu::create(back, NULL);
	mainmenu->setPosition(ccp(0, 0));
	this->addChild(mainmenu, 3, 4);
	*/

	//加载效果  1.进度条   2.奔跑小人
	//1.进度条
	auto loadBkSpr = Sprite::create("loading_1.png");//加载进度条的边框精灵
	loadBkSpr->setPosition(ccp(size.width / 2, size.height / 2 + size.height / 4));
	loadBkSpr->setScale(3.0f);
	this->addChild(loadBkSpr,1);
	 loadBar = ProgressTimer::create(Sprite::create("loading_2.png"));
	loadBar->setBarChangeRate(Point(1,0)); //设置进度条的宽高变化，此处为宽度变化
	loadBar->setType(ProgressTimer::Type::BAR);
	loadBar->setPosition(ccp(size.width / 2, size.height / 2 + size.height / 4));
	loadBar->setMidpoint(ccp(0, 1));//设置动画运动方向从左至右
	loadBar->setScale(3.0f);
	loadBar->setPercentage(0.0f);
	this->addChild(loadBar,2);
	//设置一个动作，令进度条10秒内读取到百分之100

//	ProgressTo * action = ProgressTo::create(1, 100);
	//加载完毕.移除加载动画,进入游戏场景
//	loadBar->runAction(Sequence::create(action, CCCallFunc::create(this, callfunc_selector(LoadingScene::loadSuccess)), NULL));

#if 0
	//CCTextureCache::sharedTextureCache()->addImageAsync("HelloWorld.png”,this,callfuncO_selector(newScene::loadCallBack)); //实现真实的Loading进度 
	//2.奔跑小人
	auto loadSprite = Sprite::create("run1.png");
	Animation * animation = CCAnimation::create();
	animation->addSpriteFrameWithFileName("run1.png");
	animation->addSpriteFrameWithFileName("run2.png");
	animation->addSpriteFrameWithFileName("run3.png");
	animation->addSpriteFrameWithFileName("run4.png");
	animation->addSpriteFrameWithFileName("run5.png");
	animation->addSpriteFrameWithFileName("run6.png");
	animation->setDelayPerUnit(0.1f);
	animation->setRestoreOriginalFrame(true);
	loadSprite->setPosition(ccp(100, size.height / 4));
	//运行奔跑动画
	loadSprite->runAction(RepeatForever::create(Animate::create(animation)));
	MoveTo *moveTo = MoveTo::create(4, ccp(1000, size.height / 4));
	loadSprite->runAction(moveTo);
	this->addChild(loadSprite);
#endif // 0
	
	Director::getInstance()->getTextureCache()->addImageAsync("MainScene/menu_L1.png", CC_CALLBACK_1(LoadingScene::LoadingCallback,this));
	Director::getInstance()->getTextureCache()->addImageAsync("MainScene/menu_L2.png",CC_CALLBACK_1(LoadingScene::LoadingCallback,this));
	Director::getInstance()->getTextureCache()->addImageAsync("MainScene/secretary_eye_closed.png",CC_CALLBACK_1(LoadingScene::LoadingCallback,this));
	Director::getInstance()->getTextureCache()->addImageAsync("MainScene/secretary_normal.png", CC_CALLBACK_1(LoadingScene::LoadingCallback, this));
	Director::getInstance()->getTextureCache()->addImageAsync("MainScene/backgroud_test_1.png", CC_CALLBACK_1(LoadingScene::LoadingCallback, this));
	Director::getInstance()->getTextureCache()->addImageAsync("MainScene/bloom_effect.png", CC_CALLBACK_1(LoadingScene::LoadingCallback, this));
	return true;



}
void LoadingScene::loadSuccess()
{
	//Director::getInstance()->pushScene(MainScene2::createScene());
	Director::sharedDirector()->replaceScene(CCTransitionSlideInL::create(1.0f, MainScene2::createScene()));
}

void LoadingScene::LoadingCallback(Object* pSender)
{
	this->currentNum++;
	float newPercent = 100 - ((float)this->totalNum - (float)this->currentNum) / ((float)this->totalNum / 100);

	this->loadBar->setPercentage(newPercent);

	if (this->currentNum == this->totalNum)
	{ 
		this->removeChild(this->loadBar);
		loadSuccess();
	}
}
