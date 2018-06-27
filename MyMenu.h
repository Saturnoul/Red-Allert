#ifndef _MY_MENU_
#define _MY_MENU_
#include"cocos2d.h"
USING_NS_CC;

class MyMenu :public cocos2d::Layer
{
public:

	static MyMenu* create();

	cocos2d::MenuItemImage * Barracks;
	cocos2d::Menu * menu1;
	
private:
	bool init();
	
};











#endif // !_MY_MENU_

