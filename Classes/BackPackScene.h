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

	void refreshGrid(std::vector<PlayerItem*>playerItems);   //����ɸѡ��������ˢ����Ʒ��
	void pageViewEvent(Ref* ref,PageViewEventType type);  //��ҳ��Ҫִ�е��¼�
	void onItemClick(Ref* ref, TouchEventType type);   //��Ʒ���еİ�ť������е��¼�
	
	void onFilterBtn1Click(Ref* ref,TouchEventType type); //ɸѡ���1��Ʒ
	void onFilterBtn2Click(Ref* ref, TouchEventType type);//ɸѡ���2��Ʒ
	void onFilterBtn3Click(Ref* ref, TouchEventType type);//ɸѡ���3��Ʒ

	void onSelectConfirmClick(Ref* ref, TouchEventType type);    //��ʱ��Ϊȷ�ϰ�ťʹ��

	void onExtBtn1Click(Ref* ref, TouchEventType type); //������ߵ�ɸѡ��
	void onExtBtn2Click(Ref* ref, TouchEventType type);	//�����ұߵ������

	void onSortByTypeClick(Ref* ref,TouchEventType type);  //�������������Ʒ��

	void onFilterByCheckBoxClick(Ref* ref,TouchEventType type);  //����ɸѡ��checkbox����ˢ����Ʒ��

	void sortItemsByType();      //������Ʒ����������Ʒ��

	void FilterExtensionIn();   //ɸѡ�����뻬��
	void SortExtensionIn();		//���������뻬��
	void readItemsFromJson();

	void updateLongTouch(float dt);

	void exportSelectedItem();

	void setCalledType(int type);
	int getCalledType();

	void ReturnToLastScene();

	void getSelectedItems();

private:
	bool isItemTouchEnable;    //�Ƿ���Ե��
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

	int m_calledType;               //�ж��Ǳ��ĸ�����������ɵ�

};





#endif