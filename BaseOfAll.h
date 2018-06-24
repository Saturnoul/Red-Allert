#pragma once
#include "cocos2d.h"
#include<string>
using namespace std;
USING_NS_CC;

typedef enum
{
	TypeBase = 0,
	TypeSodier,
	TypePowerPlant,
	TypeMineField,
	TypeBarrack,
	TypeWarmachineFactory,
	TypePatrolDog,
	TypeTank,
	TypeBullet
}Types;
class Basement : public Sprite
{
	CC_SYNTHESIZE(float, attackRate, AttackRate);          //受攻击的折合率
	CC_SYNTHESIZE(float, buildTime, BuildTime);            //建造耗时
public:
	Basement();
	bool minusBloodAmount(float attack);              //受到攻击后减少血量
	void SetVisible(bool judge);         //设置血条是否可见
	virtual void handleBulletCollidingWithEnemy(Layer* layer, float attack) = 0;
protected:
	Sprite* BloodBackground = Sprite::createWithSpriteFrameName("bloodBackground.png");   //血条背景
	Sprite* Blood = Sprite::createWithSpriteFrameName("blood.png");               //血条         
	ProgressTimer* BloodProgress = ProgressTimer::create(Blood);             //血条进度条
};
