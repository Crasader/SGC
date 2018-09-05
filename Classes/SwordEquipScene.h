#ifndef __SWORD_EQUIP_SCENE_H__
#define __SWORD_EQUIP_SCENE_H__
#include "cocos2d.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
USING_NS_CC_EXT;
#include"SwordBride.h"

using namespace cocos2d::ui;
using namespace cocostudio;
USING_NS_CC;

class SwordEquipScene : public CCNode
{
public:
	static SwordEquipScene* create(SwordBride* sword);
	virtual bool init(SwordBride* sword);
	~SwordEquipScene();

	void onConfirmBtnClick(Ref* ref,TouchEventType type);

protected:
	SwordBride* m_swordBride;

};



#endif