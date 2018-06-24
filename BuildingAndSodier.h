#pragma once
#include "BaseOfAll.h"
#include "Bullet.h"

class Base : public Basement
{
public:
	static Base* create();                  //静态构造函数
	virtual void handleBulletCollidingWithEnemy(Layer* layer, float attack);      //处理被攻击事件
	static float BaseBT;

};


class Sodier : public Basement
{
public:
	static Sodier* create();
	void updateSodier(float dt);       //实时检测有无敌人并发起攻击
	static float SodierBT;
	virtual void handleBulletCollidingWithEnemy(Layer* layer, float attack);
private:
	float direction; //记录敌人的方位
};


class PowerPlant : public Basement
{
public:
	static PowerPlant* create();
	virtual void handleBulletCollidingWithEnemy(Layer* layer, float attack);
	static float PowerPlantBT;

};


class MineField : public Basement
{
public:
	static MineField* create();
	virtual void handleBulletCollidingWithEnemy(Layer* layer, float attack);
	static float MineFieldBT;
};


class Barrack : public Basement
{
public:
	static Barrack* create();
	virtual void handleBulletCollidingWithEnemy(Layer* layer, float attack);
	static float BarrackBT;
};


class WarMachine_Factory : public Basement
{
public:
	static WarMachine_Factory* create();
	virtual void handleBulletCollidingWithEnemy(Layer* layer, float attack);
	static float WarMachineFactoryBT;
};


class PatrolDog : public Basement
{
public:
	static PatrolDog* create();
	virtual void update(float dt);
	virtual void handleBulletCollidingWithEnemy(Layer* layer, float attack);
	void setenemy(Sodier* _enemy) { enemy = _enemy; }
	static float PatrolDogBT;
private:
	float direction;
	Sodier* enemy;
};
class Tank : public Basement
{
public:
	static Tank* create();
	virtual void updateTank(float dt);
	virtual void handleBulletCollidingWithEnemy(Layer* layer, float attack);
	static float TankBT;
private:
	float direction;
};
bool isEnemy(Basement* basement)
{
	auto label = basement->getTag();
	if (label == TypeBase || label == TypeSodier || label == TypePowerPlant || label == TypeWarmachineFactory
		|| label == TypeBarrack || label == TypePatrolDog || label == TypeTank)
	{
		return true;
	}
	else
		return false;
}

void setTime(bool judge)
{
	if (judge == true)
	{
		if (Base::BaseBT > 4)
			Base::BaseBT -= 2;
		if (Sodier::SodierBT > 1)
			Sodier::SodierBT -= 1;
		if (PowerPlant::PowerPlantBT > 4)
			PowerPlant::PowerPlantBT -= 2;
		if (MineField::MineFieldBT > 4)
			MineField::MineFieldBT -= 2;
		if (Barrack::BarrackBT > 4)
			Barrack::BarrackBT -= 2;
		if (PatrolDog::PatrolDogBT > 1)
			PatrolDog::PatrolDogBT -= 1;
		if (Tank::TankBT > 5)
			Tank::TankBT -= 1;
	}
	else
	{
		Base::BaseBT += 2;
		Sodier::SodierBT += 1;
		PowerPlant::PowerPlantBT += 2;
		MineField::MineFieldBT += 2;
		Barrack::BarrackBT += 2;			
		PatrolDog::PatrolDogBT += 1;
		Tank::TankBT += 1;
	}
}

float directionAngle(Vec2 delta)
{
	if (delta.x == 0)
	{
		if (delta.y > 0)
			return 90.0;
		else
			return 270.0;
	}
	float angle = atanf(abs(delta.y / delta.x)) * 57.3;
	if (delta.x > 0)
	{
		if (delta.y >= 0)
			return angle;
		else                                                       //计算敌人相对于自己所在的方位角
			return 360.0 - angle;
	}
	else
	{
		if (delta.y >= 0)
			return 180.0 - angle;
		else
			return 180.0 + angle;
	}
}
Vec2 searchEnemy(Vector<Basement*> sprites, Basement* self, float& direction)
{
	for (auto i : sprites)
	{
		if (self->getPosition().getDistance(i->getPosition()) <= 150.0 && i != self)
		{
			direction = directionAngle(i->getPosition() - self->getPosition());
			if (self->getTag() == TypePatrolDog && i->getTag() == TypeSodier)                //搜索敌人
				((PatrolDog*)self)->setenemy((Sodier*)i);
			return i->getPosition();
		}
	}
	return Vec2(-1, -1);
}
