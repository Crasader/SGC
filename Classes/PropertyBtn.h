#ifndef __PROPETY_BTN_H__
#define __PROPETY_BTN_H__


#include "cocos2d.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
#include "PlayerItem.h"
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace cocostudio;
USING_NS_CC;
class PropertyBtn :public Button
{
public:
	PropertyBtn();
	static PropertyBtn* create(const std::string& normalImage, const std::string& selectedImage);
	void setCurrentPropety(Property p);
	Property getCurrentPropety(); 
	void setSelectedStatus(bool status);
	bool getSelectedStatus();



private:
	bool m_selectStatus;
	Property m_property;

};
#endif