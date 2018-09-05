#ifndef __ITEM_MERGE_SCENE_H__
#define __ITEM_MERGE_SCENE_H__

#include "cocos2d.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
#include "Entity.h"
#include "PlayerItem.h"
#include "ItemInfoScene.h"
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace cocostudio;
USING_NS_CC;

class ItemMergeScene : public Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(ItemMergeScene);

	void onItemSelect1Click(Ref* ref, TouchEventType type);
	void onItemSelect2Click(Ref* ref, TouchEventType type);
	void onItemSelect3Click(Ref* ref, TouchEventType type);
	void onItemSelect4Click(Ref* ref, TouchEventType type);
	void onConfirmClick(Ref* ref, TouchEventType type);
	void addSelectedItems(PlayerItem* item);

	void onPropetyBtnClick(Ref* ref, TouchEventType type);

	int getMergePotential();
	std::vector<Property> getMergeAllPropeties(std::vector<PlayerItem*>items);
	void DoMergeItem();

	void setSelectedItem1(std::vector<PlayerItem*>items1);
	void setSelectedItem2(std::vector<PlayerItem*>items2);
	void setSelectedItem3(std::vector<PlayerItem*>items3);
	void setSelectedItem4(std::vector<PlayerItem*>items4);
	std::vector<PlayerItem*> getAllSelectItems();

	std::vector<Property> getAvailableMergePropeties(Property p);




private:
	Button* m_btnItemSelectBox[4];
	bool m_ItemIsSelected[4];
	std::vector<PlayerItem*>m_selectedItems_1;
	std::vector<PlayerItem*>m_selectedItems_2;
	std::vector<PlayerItem*>m_selectedItems_3;
	std::vector<PlayerItem*>m_selectedItems_4;

	ListView* propetyListView;

};

#endif
