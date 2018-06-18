#include "money-and-panel.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}
// on "init" you need to initialize your instance
void Money::update(float f)
{
	if (++timer % inc_prd == 0)
	{
		money += inc_amt;
		updateMoneyDisplay();
	}
}

bool Money::init()
{
	money = 10000;
	char money_str[30];
	sprintf(money_str, "%d", money);
	return initWithString(money_str, "fonts/BMFont.fnt");
}

void Money::updateMoneyDisplay()
{
	char money_str[30];
	sprintf(money_str, "%d", money);
	setString(money_str);
}

void Money::spendMoney(int cost)
{
	money -= cost;
	updateMoneyDisplay();
}

void Money::increaseMoney(int amount)
{
	money += amount;
}

bool Money::checkMoney(int cost) const
{
	return(money >= cost);
}
bool UnitsProduce::init()
{
	if (!Menu::init())
		return false;
	tank = MenuItemImage::create("/Picture/menu/tank-menu-up.png",
		"/Picture/menu/tank-menu-down.png"
	);
	tank->setScale(0.8);
	tank->setAnchorPoint(Vec2(1, 1));
	tank->setPosition(Menu::getContentSize().width,
		Menu::getContentSize().height - tank->getContentSize().height);
	dog = MenuItemImage::create("/Picture/menu/tank-menu-up.png",
		"/Picture/menu/tank-menu-down.png"
	);
	dog->setScale(0.8);
	dog->setAnchorPoint(Vec2(1, 1));
	dog->setPosition(Menu::getContentSize().width,
		Menu::getContentSize().height - tank->getContentSize().height);
	soldier = MenuItemImage::create("/Picture/menu/soldier-menu-up.png",
		"/Picture/menu/soldier-menu-down.png"
	);
	soldier->setScale(0.8);
	soldier->setAnchorPoint(Vec2(1, 1));
	soldier->setPosition(Menu::getContentSize().width,
		Menu::getContentSize().height - 2 * soldier->getContentSize().height);
	Menu::addChild(dog);
	Menu::addChild(tank);
	Menu::addChild(soldier);

	Menu::alignItemsVertically();
	return true;
}
void UnitsProduce::setDogCallback(std::function<void(Ref*)> callback)
{
	dog->setCallback(callback);
}
void UnitsProduce::MenuItemCallBack(Ref * psender)
{
	log("MenuItemCallback");
}
void UnitsProduce::setTankCallback(std::function<void(Ref*)> callback)
{
	tank->setCallback(callback);
}
void UnitsProduce::setSoldierCallback(std::function<void(Ref*)> callback)
{
	soldier->setCallback(callback);
}
