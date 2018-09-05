#ifndef __RESOURCE_NUM_BTN_H__
#define __RESOURCE_NUM_BTN_H__
#include "cocos2d.h"
#include "Entity.h"
USING_NS_CC;
class ResourceNumBtn :public Entity
{
public:
	ResourceNumBtn();
	ResourceNumBtn(std::string url, int resType, int resScale);
	~ResourceNumBtn();
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	int getResType();
	int getResScale();
	int getNum();

private:
	int m_resType;
	int m_resScale;
	int m_num;
	LabelAtlas* numAtlas;
};
#endif