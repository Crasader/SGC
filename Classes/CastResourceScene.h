#ifndef __CAST_RESOURCE_SCENE_H__
#define __CAST_RESOURCE_SCENE_H__

#include "cocos2d.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
#include "PlayerItem.h"
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace cocostudio;
USING_NS_CC;
#include "ResourceNumBtn.h"

class CastResourceScene : public cocos2d::LayerColor
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	void onClickQuick(Ref*, TouchEventType type);
	void onClickStart(Ref*, TouchEventType type);
	Sprite* createNumberSprite(std::string imgurl,int type,int scale);
	void getResourceNum();
	void ChangeToBigCast();
	void ChangeToNormalCast();
	void setSelectedItem(std::vector<PlayerItem*>items);
	CREATE_FUNC(CastResourceScene);


private:
	Sprite* sp_blueprint;
	int m_StoneNum;
	int m_IronNum;
	int m_JadeNum;
	int m_SoulNum;
	Button* btn_quick;
	Button* btn_start;
	Text* text_stone;
	Text* text_iron;
	Text* text_jade;
	Text* text_soul;
	bool isquick;
	ResourceNumBtn* numBtn[4][4];
	bool isBigCast;

	std::vector<PlayerItem*>m_selectedItems;
};


#endif