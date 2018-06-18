#pragma once
#ifndef MONER-AND-PANEL-H-
#define __HELLOWORLD_SCENE_H__
#include "cocos2d.h"
#define MONEY_INC_PERIOD 20 
#define MONEY_INC_AMOUNT 20
USING_NS_CC;
class UnitsProduce :public cocos2d::Menu
{
public:
	CREATE_FUNC(UnitsProduce);
	bool init() override;
	void setSoldierCallback(std::function<void(Ref*)>);
	void setTankCallback(std::function<void(Ref*)>);
	void setDogCallback(std::function<void(Ref*)>);
	void MenuItemCallBack(Ref*psender);
private:
	cocos2d::MenuItemImage *dog;
	cocos2d::MenuItemImage *tank;
	cocos2d::MenuItemImage *soldier;

};
class Money : public cocos2d::LabelBMFont
{
public:
	void update(float f) override;
	CREATE_FUNC(Money);
	bool init() override;

	void updateMoneyDisplay();
	bool checkMoney(int cost) const;

	void spendMoney(int cost);
	void increaseMoney(int amount);
private:
	int money = 0;
	int timer = 0;
	int inc_prd = MONEY_INC_PERIOD;
	int inc_amt = MONEY_INC_AMOUNT;
};
#endif  MONER-AND-PANEL-H-
