//该类没有用可以删除
#ifndef __CAST_GROOVE_H__
#define __CAST_GROOVE_H__
#include "cocos2d.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace cocostudio;
USING_NS_CC;
class CastGroove :public CCNode
{
public:
	static CastGroove* create(bool isbuyed, bool isbuiding);
	virtual bool init(bool isbuyed, bool isbuiding);
	void popbtnCallback(Node* node);
	void update(float delta);
	~CastGroove();
private:
	bool isbuyed;
	bool isbuiding;
	float m_timer;
	bool isclicked;
	LabelTTF* time_label;


};

#endif