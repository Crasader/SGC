#ifndef __POP_DLG__
#define __POP_DLG__

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
using namespace cocos2d::extension;

class PopDlg :public Layer
{
public:
	PopDlg();
	~PopDlg();
	virtual bool init();
	CREATE_FUNC(PopDlg);

	//virtual void registerWithTouchDispatcher(void);
	bool onTouchBegan(Touch *touch, Event *unused_event);

	static PopDlg * create(const char* backgroundImage);
	void setTitle(const char* title, int fontsize = 20);
	void setContentText(const char* text, int fontsize = 20, int padding = 50, int paddintTop = 100);
	void setCallbackFunc(Object* target, SEL_CallFuncN callfun);

	bool addButton(const char* normalImage, const char* selectedImage, const char* title, int tag = 0);
	virtual void onEnter();
	virtual void onExit();

private:
	void buttonCallback(CCObject* pSender);

	// 文字内容两边的空白区
	int m_contentPadding;
	int m_contentPaddingTop;

	CCObject* m_callbackListener;
	SEL_CallFuncN m_callback;

	CC_SYNTHESIZE_RETAIN(Menu*, m__pMenu, MenuButton);
	CC_SYNTHESIZE_RETAIN(Sprite*, m__sfBackGround, SpriteBackGround);
	CC_SYNTHESIZE_RETAIN(Scale9Sprite*, m__s9BackGround, Sprite9BackGround);
	CC_SYNTHESIZE_RETAIN(LabelTTF*, m__ltTitle, LabelTitle);
	CC_SYNTHESIZE_RETAIN(LabelTTF*, m__ltContentText, LabelContentText);


};


#endif