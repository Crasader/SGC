#ifndef __ITEMINFO_SCENE_H__
#define __ITEMINFO_SCENE_H__

#include "cocos2d.h"
#include "PlayerItem.h"

#include "editor-support/cocostudio/CCSGUIReader.h"
#include "ui/CocosGUI.h"
using namespace cocos2d::ui;
using namespace cocostudio;

class ItemInfoScene :public Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(ItemInfoScene);
	void setPlayerItem(PlayerItem* myItem);

	void onConfirmClick(Ref*, TouchEventType type);
	void onAddBtnClick(Ref*, TouchEventType type);
	void onDecreaseBtnClick(Ref*, TouchEventType type);
private:
	PlayerItem* m_playerItem;
	int selectedNum;
	LabelAtlas* numAtlas;
};

#endif
