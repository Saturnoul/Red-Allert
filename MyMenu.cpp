#include"MyMenu.h"
USING_NS_CC;
MyMenu* MyMenu::create()
{
	MyMenu* ret = new (std::nothrow) MyMenu();
	if (ret&&ret->MyMenu::init())
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);

	return nullptr;


}

bool MyMenu::init()
{
	if (!Layer::init())
		return false;
	
	Barracks=MenuItemImage::create("menu/airplane-menu-up.png","menu/airplane-menu - down.png");
	Barracks->setPosition(500, 500);
	menu1 = Menu::create(Barracks, NULL);

	this->addChild(menu1);

	
	return true;
}