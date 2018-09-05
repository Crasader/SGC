#ifndef __MainScene2_H__
#define __MainScene2_H__

#include "cocos2d.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
#include "network/HttpClient.h"
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace cocostudio;
USING_NS_CC;
using namespace network;
class MainScene2 : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	void doRotateTouch(Touch* touch);
	void doRotateBtn(Sprite* sprite,Layer* layer_1,Layer* layer2,Layer* layer_top);
	void doAdjustRotate();
	int getBtnNum(Sprite* sprite);
	virtual void update(float  delta);
	Point calcPositionAfterRotate(Point previousPos, Point center, float angel);
	void EnterCastSwordScene(Ref* pSender, Control::EventType event);
	void EnterItemMergeScene(Ref* pSender,Control::EventType event);

	void EnterMapSelectScene(Ref* pSender,Control::EventType event);

	void EnterTeamFormationScene(Ref* pSender, Control::EventType event);
	void refreshL1Btn(int selectedNum);


	void readBridesJson();    //获取玩家剑姬列表
	void readItemsJson();	  //获取玩家物品列表

	void onBridesHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
	void onItemsHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);

	CREATE_FUNC(MainScene2);
	
private:
	Sprite *disk_top;
	Sprite *disk_menu1;
	Sprite *disk_menu2;
	Layer* layer_menu1;
	Layer* layer_menu2;
	Layer* layer_top;
	Button *menu2_btn[24];
	float angle;
	Sprite* btnSpite[5];
	Sprite* sp_assitant;
	bool isBtnClick;   //点击还是拖动


};

#endif