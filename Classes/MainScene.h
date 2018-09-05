#ifndef __MainScene_H__
#define __MainScene_H__

#include "cocos2d.h"

#include "editor-support/cocostudio/CCSGUIReader.h"
#include "ui/CocosGUI.h"
using namespace cocos2d::ui;
using namespace cocostudio;

class MainScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	void onClick(Ref*, TouchEventType type);
	CREATE_FUNC(MainScene);
};
#endif
