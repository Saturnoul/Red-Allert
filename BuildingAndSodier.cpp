#include "BuildingAndSodier.h"
#include "MoveControl.h"
#include "MoveController.h"
Vector<Basement*>allSprites;

float Base::BaseBT = 2;

Base* Base::create()
{
	Base* base = new (std::nothrow) Base();
	if (base && base->initWithSpriteFrameName("base.png"))
	{
		auto body = PhysicsBody::createBox(base->getContentSize());
		body->setCategoryBitmask(0x01);
		body->setCollisionBitmask(0x02);
		body->setContactTestBitmask(0x01);
		base->setPhysicsBody(body);
		base->addChild(base->BloodBackground);
		base->addChild(base->BloodProgress);
		base->BloodBackground->setPosition(Vec2(base->getContentSize().width/2, base->getContentSize().height + 25));
		base->BloodProgress->setPosition(Vec2(base->getContentSize().width / 2, base->getContentSize().height + 25));
		base->setTag(TypeBase);
		base->setBuildTime(BaseBT);
		base->setAttackRate(0.02);
		allSprites.pushBack((Basement*)base);
		base->autorelease();
		return base;
	}
	CC_SAFE_DELETE(base);
	return nullptr;
}
void Base::handleBulletCollidingWithEnemy(Layer* layer, float attack)
{
	this->SetVisible(true);
	if (!this->minusBloodAmount(attack * attackRate))
	{
		this->setVisible(false);
		allSprites.eraseObject(this);
		layer->removeChild(this);
		selectedSprites.eraseObject(this);
	}
}

float Sodier::SodierBT = 5;

Sodier* Sodier::create()
{
	Sodier* sodier = new (std::nothrow) Sodier();
	if (sodier && sodier->initWithSpriteFrameName("sodier.png"))
	{
		auto body = PhysicsBody::createBox(sodier->getContentSize());
		body->setCategoryBitmask(0x01);
		body->setCollisionBitmask(0x02);
		body->setContactTestBitmask(0x01);
		sodier->setPhysicsBody(body);
		sodier->addChild(sodier->BloodProgress);
		sodier->addChild(sodier->BloodBackground);
		sodier->setTag(TypeSodier);
		sodier->BloodBackground->setPosition(Vec2(sodier->getContentSize().width / 2, sodier->getContentSize().height + 5));
		sodier->BloodProgress->setPosition(Vec2(sodier->getContentSize().width / 2, sodier->getContentSize().height + 5));
		allSprites.pushBack((Basement*)sodier);
		sodier->schedule(schedule_selector(Sodier::updateSodier), 0.3f);
		sodier->setAttackRate(1);
		sodier->setBuildTime(SodierBT);
		sodier->autorelease();
		return sodier;
	} 
	CC_SAFE_DELETE(sodier);
	return nullptr;
}
void Sodier::updateSodier(float dt)
{
	Vec2 enemyPosition = searchEnemy(allSprites, (Basement*)this, direction);
	if (this && this->isVisible() && enemyPosition != Vec2(-1, -1))
	{
		this->stopAllActions();
		Bullet* bullet = Bullet::createWithSpriteFrameName("gameplay.bullet.png");
		bullet->setVelocity(Vec2(1000 * cos(direction / 57.3), 1000 * sin(direction / 57.3)));
		bullet->setRotation(-direction);
		bullet->setAttack(20);
		this->addChild(bullet, 0, TypeBullet);
		bullet->shootBulletFromBasement((Basement*)this, direction);
	}
}
void Sodier::handleBulletCollidingWithEnemy(Layer* layer, float attack)
{
	this->SetVisible(true);
	if (!this->minusBloodAmount(attack * attackRate))
	{
		this->setVisible(false);
		allSprites.eraseObject(this);
		layer->removeChild(this);
		setTime(false);
		selectedSprites.eraseObject(this);
	}
}

float PowerPlant::PowerPlantBT = 10;

PowerPlant* PowerPlant::create()
{
	PowerPlant* powerplant = new (std::nothrow) PowerPlant();
	if (powerplant && powerplant->initWithSpriteFrameName("powerplant.png"))
	{
		auto body = PhysicsBody::createBox(powerplant->getContentSize());
		body->setCategoryBitmask(0x01);
		body->setCollisionBitmask(0x02);
		body->setContactTestBitmask(0x01);
		powerplant->setPhysicsBody(body);
		powerplant->addChild(powerplant->BloodProgress);
		powerplant->addChild(powerplant->BloodBackground);
		powerplant->setTag(TypePowerPlant);
		powerplant->BloodBackground->setPosition(Vec2(powerplant->getContentSize().width / 2, powerplant->getContentSize().height + 5));
		powerplant->BloodProgress->setPosition(Vec2(powerplant->getContentSize().width / 2, powerplant->getContentSize().height + 5));
		powerplant->autorelease();
		powerplant->setBuildTime(PowerPlantBT);
		allSprites.pushBack((Basement*)powerplant);
		powerplant->setAttackRate(0.04);
		setTime(true);
		return powerplant;
	}
	CC_SAFE_DELETE(powerplant);
	return nullptr;
}
void PowerPlant::handleBulletCollidingWithEnemy(Layer* layer, float attack)
{
	this->SetVisible(true);
	if (!this->minusBloodAmount(attack * attackRate))
	{
		this->setVisible(false);
		allSprites.eraseObject(this);
		layer->removeChild(this);
		selectedSprites.eraseObject(this);
	}
}

float MineField::MineFieldBT = 10;

MineField* MineField::create()
{
	MineField* minefield = new (std::nothrow) MineField();
	if (minefield && minefield->initWithSpriteFrameName("minefield.png"))
	{
		auto body = PhysicsBody::createBox(minefield->getContentSize());
		body->setCategoryBitmask(0x01);
		body->setCollisionBitmask(0x02);
		body->setContactTestBitmask(0x01);
		minefield->setPhysicsBody(body);
		minefield->addChild(minefield->BloodProgress);
		minefield->addChild(minefield->BloodBackground);
		minefield->setTag(TypeMineField);
		minefield->BloodBackground->setPosition(minefield->getContentSize().width / 2, minefield->getContentSize().height + 5);
		minefield->BloodProgress->setPosition(minefield->getContentSize().width / 2, minefield->getContentSize().height + 5);
		minefield->autorelease();
		minefield->setBuildTime(MineFieldBT);
		allSprites.pushBack((Basement*)minefield);
		minefield->setAttackRate(0.04);
		return minefield;
	}
	CC_SAFE_DELETE(minefield);

	return nullptr;
}
void MineField::handleBulletCollidingWithEnemy(Layer* layer, float attack)
{
	this->SetVisible(true);
	if (!this->minusBloodAmount(attack * attackRate))
	{
		this->setVisible(false);
		allSprites.eraseObject(this);
		layer->removeChild(this);
		selectedSprites.eraseObject(this);
	}
}

float Barrack::BarrackBT = 10;

Barrack* Barrack::create()
{
	Barrack* barrack = new (std::nothrow) Barrack();
	if (barrack && barrack->initWithSpriteFrameName("barrack.png"))
	{
		auto body = PhysicsBody::createBox(barrack->getContentSize());
		body->setCategoryBitmask(0x01);
		body->setCollisionBitmask(0x02);
		body->setContactTestBitmask(0x01);
		barrack->setPhysicsBody(body);
		barrack->addChild(barrack->BloodProgress);
		barrack->addChild(barrack->BloodBackground);
		barrack->setTag(TypeBarrack);
		barrack->BloodBackground->setPosition(barrack->getContentSize().width / 2, barrack->getContentSize().height + 5);
		barrack->BloodProgress->setPosition(barrack->getContentSize().width / 2, barrack->getContentSize().height + 5);
		barrack->autorelease();
		barrack->setBuildTime(BarrackBT);
		allSprites.pushBack((Basement*)barrack);
		barrack->setAttackRate(0.05);
		return barrack;
	}
	CC_SAFE_DELETE(barrack);
	return nullptr;
}
void Barrack::handleBulletCollidingWithEnemy(Layer* layer, float attack)
{
	this->SetVisible(true);
	if (!this->minusBloodAmount(attack * attackRate))
	{
		this->setVisible(false);
		allSprites.eraseObject(this);
		layer->removeChild(this);
		selectedSprites.eraseObject(this);
	}
}

float WarMachine_Factory::WarMachineFactoryBT = 15;

WarMachine_Factory* WarMachine_Factory::create()
{
	WarMachine_Factory* warmachine_factory = new (std::nothrow) WarMachine_Factory();
	if (warmachine_factory && warmachine_factory->initWithSpriteFrameName("warmachinefactory.png"))
	{
		auto body = PhysicsBody::createBox(warmachine_factory->getContentSize());
		body->setCategoryBitmask(0x01);
		body->setCollisionBitmask(0x02);
		body->setContactTestBitmask(0x01);
		warmachine_factory->setPhysicsBody(body);
		warmachine_factory->addChild(warmachine_factory->BloodProgress);
		warmachine_factory->addChild(warmachine_factory->BloodBackground);
		warmachine_factory->setTag(TypeWarmachineFactory);
		warmachine_factory->BloodBackground->setPosition(warmachine_factory->getContentSize().width / 2, warmachine_factory->getContentSize().height + 5);
		warmachine_factory->BloodProgress->setPosition(warmachine_factory->getContentSize().width / 2, warmachine_factory->getContentSize().height + 5);
		warmachine_factory->autorelease();
		warmachine_factory->setBuildTime(WarMachineFactoryBT);
		allSprites.pushBack((Basement*)warmachine_factory);
		warmachine_factory->setAttackRate(0.04);
		return warmachine_factory;
	}
	CC_SAFE_DELETE(warmachine_factory);
	return nullptr;
}
void WarMachine_Factory::handleBulletCollidingWithEnemy(Layer* layer, float attack)
{
	this->SetVisible(true);
	if (!this->minusBloodAmount(attack * attackRate))
	{
		this->setVisible(false);
		allSprites.eraseObject(this);
		layer->removeChild(this);
		selectedSprites.eraseObject(this);
	}
}

float PatrolDog::PatrolDogBT = 2;

PatrolDog* PatrolDog::create()
{
	PatrolDog* patroldog = new (std::nothrow) PatrolDog();
	if (patroldog && patroldog->initWithSpriteFrameName("patroldog.png"))
	{
		auto body = PhysicsBody::createBox(patroldog->getContentSize());
		body->setCategoryBitmask(0x01);
		body->setCollisionBitmask(0x02);
		body->setContactTestBitmask(0x01);
		patroldog->setPhysicsBody(body);
		patroldog->addChild(patroldog->BloodProgress);
		patroldog->addChild(patroldog->BloodBackground);
		patroldog->setTag(TypePatrolDog);
		patroldog->BloodBackground->setPosition(patroldog->getContentSize().width / 2, patroldog->getContentSize().height + 5);
		patroldog->BloodProgress->setPosition(patroldog->getContentSize().width / 2, patroldog->getContentSize().height + 5);
		patroldog->autorelease();
		patroldog->setBuildTime(PatrolDogBT);
		allSprites.pushBack((Basement*)patroldog);
		patroldog->setAttackRate(1);
		return patroldog;
	}
	CC_SAFE_DELETE(patroldog);
	return nullptr;
}
void PatrolDog::handleBulletCollidingWithEnemy(Layer* layer, float attack)
{
	this->SetVisible(true);
	if (!this->minusBloodAmount(attack * attackRate))
	{
		this->setVisible(false);
		allSprites.eraseObject(this);
		layer->removeChild(this);
		selectedSprites.eraseObject(this);
	}
}
void PatrolDog::update(float dt)
{
	Vec2 enemyPosition = searchEnemy(allSprites, (Basement*)this, direction);
	if (this && this->isVisible() && enemy && enemy->getTag() == TypeSodier && enemyPosition != Vec2(-1, -1))
	{
		this->runAction(MoveTo::create(0.2, enemyPosition));
		enemy->minusBloodAmount(100);
		enemy->setVisible(false);
		allSprites.eraseObject(enemy);
		enemy = nullptr;
	}
}

float Tank::TankBT = 10;

Tank* Tank::create()
{
	Tank* tank = new (std::nothrow) Tank();
	if (tank && tank->initWithSpriteFrameName("tank.png"))
	{
		auto body = PhysicsBody::createBox(tank->getContentSize());
		body->setCategoryBitmask(0x01);
		body->setCollisionBitmask(0x02);
		body->setContactTestBitmask(0x01);
		tank->setPhysicsBody(body);
		tank->addChild(tank->BloodProgress);
		tank->addChild(tank->BloodBackground);
		tank->setTag(TypeTank);
		tank->BloodBackground->setPosition(tank->getContentSize().width / 2, tank->getContentSize().height + 5);
		tank->BloodProgress->setPosition(tank->getContentSize().width / 2, tank->getContentSize().height + 5);
		tank->autorelease();
		tank->setBuildTime(TankBT);
		tank->schedule(schedule_selector(Tank::updateTank), 1.0f);
		allSprites.pushBack((Basement*)tank);
		tank->setAttackRate(0.067);
		return tank;
	}
	CC_SAFE_DELETE(tank);
	return nullptr;
}
void Tank::handleBulletCollidingWithEnemy(Layer* layer, float attack)
{
	this->SetVisible(true);
	if (!this->minusBloodAmount(attack * attackRate))
	{
		this->setVisible(false);
		allSprites.eraseObject(this);
		layer->removeChild(this);
		selectedSprites.eraseObject(this);
	}
}
void Tank::updateTank(float dt)
{
	Vec2 enemyPosition = searchEnemy(allSprites, (Basement*)this, direction);
	if (this && enemyPosition != Vec2(-1, -1))
	{
		this->stopAllActions();
		Bullet* bullet = Bullet::createWithSpriteFrameName("gameplay.bullet.png");
		bullet->setVelocity(Vec2(1000 * cos(direction / 57.3), 1000 * sin(direction / 57.3)));
		bullet->setRotation(-direction);
		bullet->setAttack(100);
		this->addChild(bullet, 0, TypeBullet);
		bullet->shootBulletFromBasement((Basement*)this, direction);
	}
}