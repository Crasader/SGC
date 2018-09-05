#ifndef __CASTSWORD_SCENE_H__
#define __CASTSWORD_SCENE_H__

#include "cocos2d.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace cocostudio;
USING_NS_CC;
class CastSwordScene : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	void createGrooves();
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void update(float delta);   //定时更新时间
	void popbtnCallback(Node* node);    //弹出窗口回调，功能待实现
	void moveGrooveAfterClick(int moveX);   //点击后槽位的移动
	void createCastResourceScene();       //初始化创建槽位
	void setGrooveToDefaultAndUpdate(int stone_num,int iron_num,int jade_num,int soul_num);    //选择资源确定建造后槽位移动和建造新信息获取
	int calcBuildingTime(int stone_num, int iron_num, int jade_num, int soul_num);   //计算建造时间
	void CalcSwordRate(int stone_num, int iron_num, int jade_num, int soul_num);    //从json文件中读取建造数据,计算建造各个剑姬的概率


	void OnClickCancel0(Ref*, TouchEventType type);		//第一个槽位取消建造
	void OnClickCancel1(Ref*, TouchEventType type);		//第二个槽位取消建造
	void OnClickCancel2(Ref*, TouchEventType type);		//第三个槽位取消建造
	void OnClickCancel3(Ref*, TouchEventType type);		//第四个槽位取消建造
	CREATE_FUNC(CastSwordScene);


private:
	bool isEnable[4];
	bool isBuilding[4];
	bool isClicked;
	int selectedNum;
	Sprite* grooveSp[4];
	int buildingTime[4];
	LabelAtlas* timeAtlas[4];
	ClippingNode* clipper;
	Button* cancelBtn[4];

	std::string newSwordName[4];

};
#endif
