#include "ResourceNumBtn.h"
ResourceNumBtn::ResourceNumBtn()
{

	
}
ResourceNumBtn::ResourceNumBtn(std::string url, int resType, int resScale)
{
	this->setContentSize(Size(100, 120));
	Size contentSize = this->getContentSize();
	Sprite* sp_bg = Sprite::create(url);
	sp_bg->setContentSize(contentSize);
	sp_bg->setPosition(contentSize.width*0.5,contentSize.height*0.5);
  //设置背景图片位置
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(ResourceNumBtn::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(ResourceNumBtn::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(ResourceNumBtn::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, sp_bg);   //控件增加触发器
	

	this->m_resType = resType;
	this->m_resScale = resScale;
	this->m_num = 0;

	numAtlas = LabelAtlas::create("0123456789", "number2.png", 46, 80, '0');
	char* str_num = new char[2];
	sprintf(str_num, "%d", m_num);
	numAtlas->setString(str_num);
	numAtlas->setAnchorPoint(ccp(0.5,0.5));
	numAtlas->setPosition(contentSize.width*0.5,contentSize.height*0.5);
	this->addChild(sp_bg, 1, 1);
	this->addChild(numAtlas,2,2);
}
ResourceNumBtn::~ResourceNumBtn()
{
}
bool ResourceNumBtn::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	auto target = static_cast<Sprite*>(event->getCurrentTarget());
	Size s = target->getContentSize();
	Point locationInNode = target->convertToNodeSpace(touch->getLocation());
	Rect rect = Rect(0, 0, s.width, s.height);
	if (rect.containsPoint(locationInNode))
	{
		return true;
	}
	return false;
}
void ResourceNumBtn::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
}
void ResourceNumBtn::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	auto target = static_cast<Sprite*>(event->getCurrentTarget());
	Size s = target->getContentSize();
	Point locationInNode = target->convertToNodeSpace(touch->getLocation());
	Rect half = Rect(0, 0, s.width, s.height / 2);
	if (half.containsPoint(locationInNode))
	{
		//在下半部分，则减少
		if (m_num == 0)
			return;
		m_num--;
	}
	else
	{
		//在上半部分，则增加
		if (m_num == 9)
			return;
		m_num++;
	}
	char* str_num = new char[2];
	sprintf(str_num, "%d", m_num);
	numAtlas->setString(str_num);

}
int ResourceNumBtn::getResScale()
{
	return this->m_resScale;
}
int ResourceNumBtn::getResType()
{
	return this->m_resType;
}
int ResourceNumBtn::getNum()
{
	return this->m_num;
}
