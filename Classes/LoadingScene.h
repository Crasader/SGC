#ifndef __LOADING_SCENE_H__
#define __LOADING_SCENE_H__

#include "cocos2d.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace cocostudio;
USING_NS_CC;

class LoadingScene : public cocos2d::Layer
{
public:


	static cocos2d::Scene* createScene();
	virtual bool init();
	void LoadingCallback(Object* pSender);
	void loadSuccess();
	CREATE_FUNC(LoadingScene);
	

private:
	int currentNum;
	int totalNum;
	cocos2d::ProgressTimer* loadBar;
};


#endif