#ifndef __BACK_PACK_SCENE_H__
#define __BACK_PACK_SCENE_H__

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

#define PAGE_SUM_NUM 4
#define EACH_PAGE_SUM 40



class ItemBox :public Button
{
public:
	ItemBox();
	static ItemBox* create(const std::string& normalImage, const std::string& selectedImage);
	~ItemBox();
	void setCurrentItem(PlayerItem* item);
	PlayerItem* getCurrentItem();
	void setPlayerItem(PlayerItem* item);
	void setEmptyStatus(bool status);
	bool getEmptyStatus();

	bool getSelectStatus();
	void setSelectStatus(bool status);
private:
	PlayerItem* m_currentItem;
	bool isSelected;
	bool isEmpty;
};

class BackPackScene : public Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(BackPackScene);

	void refreshGrid(std::vector<PlayerItem*>playerItems);   //根据筛选条件重新刷新物品栏
	void pageViewEvent(Ref* ref,PageViewEventType type);  //翻页所要执行的事件
	void onItemClick(Ref* ref, TouchEventType type);   //物品栏中的按钮点击进行的事件
	
	void onFilterBtn1Click(Ref* ref,TouchEventType type); //筛选类别1物品
	void onFilterBtn2Click(Ref* ref, TouchEventType type);//筛选类别2物品
	void onFilterBtn3Click(Ref* ref, TouchEventType type);//筛选类别3物品

	void onSelectConfirmClick(Ref* ref, TouchEventType type);    //暂时作为确认按钮使用

	void onExtBtn1Click(Ref* ref, TouchEventType type); //弹出左边的筛选框
	void onExtBtn2Click(Ref* ref, TouchEventType type);	//弹出右边的排序框

	void onSortByTypeClick(Ref* ref,TouchEventType type);  //按照类别排序物品栏

	void onFilterByCheckBoxClick(Ref* ref,TouchEventType type);  //按照筛选的checkbox重新刷新物品栏

	void sortItemsByType();      //按照物品类型排序物品栏

	void FilterExtensionIn();   //筛选栏滑入滑出
	void SortExtensionIn();		//排序栏滑入滑出
	void readItemsFromJson();

	void updateLongTouch(float dt);

	void exportSelectedItem();

	void setCalledType(int type);
	int getCalledType();

	void ReturnToLastScene();

	void getSelectedItems();

private:
	bool isItemTouchEnable;    //是否可以点击
	ItemBox* m_pBoxAllGrid[PAGE_SUM_NUM][EACH_PAGE_SUM];
	int m_pIntCurPage;
	ImageView* m_pImgLight[PAGE_SUM_NUM];
	int m_ItemTotalNum;
	std::vector<PlayerItem*>m_playerItems;
	std::vector<PlayerItem*>m_chosenItems;
	bool isExtension1Enable;
	bool isExtension2Enable;

	bool m_isTouch;
	bool m_isLongTouch;
	int m_pressTime;

	CheckBox* filterCheckBox[4];

	int m_calledType;               //判定是被哪个界面调用生成的

};





#endif