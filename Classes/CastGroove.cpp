//没有用可以删除
#include "CastGroove.h"
#include "PopDlg.h"

CastGroove* CastGroove::create(bool isbuyed, bool isbuiding)
{
	CastGroove* groove = new CastGroove();
	groove->init(isbuyed, isbuiding);
	return groove;

}
bool CastGroove::init(bool isbuyed, bool isbuiding)
{
	this->isbuyed = isbuyed;
	this->isbuiding = isbuiding;

	Size size = Director::getInstance()->getWinSize();
	this->setContentSize(Size(size.width*0.1, size.height*0.667));
	this->setCascadeOpacityEnabled(true);
	//log("width:%f height:%f",this->getContentSize().width,this->getContentSize().height);
	Sprite* bgspite = Sprite::create("castGroove.png");
	bgspite->setPosition(ccp(0.5, 0.5));
	this->addChild(bgspite);
	if (this->isbuyed)
	{
		this->setOpacity(255);
	}
	else
	{
		this->setOpacity(50);
	}

	auto listenr_cast = EventListenerTouchOneByOne::create();
	listenr_cast->setSwallowTouches(true);
	listenr_cast->onTouchBegan = [](Touch* touch, Event* event){
		// event->getCurrentTarget() returns the *listener's* sceneGraphPriority node.
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		if (rect.containsPoint(locationInNode))
		{
			return true;
		}
		return false;
	};
	listenr_cast->onTouchMoved = [](Touch* touch, Event* event){
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
	};

	listenr_cast->onTouchEnded = [=](Touch* touch, Event* event){
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		if (this->isbuyed)
		{
			log("show status");
		}
		else
		{
			PopDlg* dlg = PopDlg::create("popdlgbg.jpg");
			dlg->setContentSize(Size(400, 200));
			dlg->setTitle("warning");
			dlg->setContentText("确认氪金走上人生巅峰?");
			dlg->setCallbackFunc(this, callfuncN_selector(CastGroove::popbtnCallback));
			dlg->addButton("LittleRunner0.png", "LittleRunner0.png", "确定", 0);
			dlg->addButton("LittleRunner0.png", "LittleRunner0.png", "取消", 1);
			this->addChild(dlg);
		}


	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenr_cast, bgspite);

	this->schedule(schedule_selector(CastGroove::update));

	return true;
}
void CastGroove::popbtnCallback(Node* node)
{
	log("button call back. tag: %d", node->getTag());
}
CastGroove::~CastGroove()
{

}
void CastGroove::update(float delta)
{
	m_timer -= delta;
	char* mtime = new char[10];
	sprintf(mtime, "%d:%d:%d", (int)m_timer / 3600, ((int)m_timer % 3600) / 60, (int)m_timer % 3600 % 60);
	this->time_label->setString(mtime);
}