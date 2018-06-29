#include "BuildingAndSodier.h"
Vector<Basement*>allSprites;
extern int player_ID;
float Base::BaseBT = 2;

Base* Base::create()
{
	Base* base = new (std::nothrow) Base();
	if (base && base->initWithSpriteFrameName("factmake0.png"))
	{
		auto body = PhysicsBody::createBox(base->getContentSize());
		body->setCategoryBitmask(0x01);
		body->setCollisionBitmask(0x02);
		body->setContactTestBitmask(0x01);
		base->setPhysicsBody(body);
		base->addChild(base->BloodBackground);
		base->addChild(base->BloodProgress);
		base->BloodBackground->setPosition(Vec2(base->getContentSize().width / 2, base->getContentSize().height + 25));
		base->BloodProgress->setPosition(Vec2(base->getContentSize().width / 2, base->getContentSize().height + 25));
		base->setTag(TypeBase);
		base->setBuildTime(BaseBT);
		base->setAttackRate(0.02);
		allSprites.pushBack((Basement*)base);
		Animation* animation = Animation::create();
		for (int i = 0; i <= 31; i++)
		{
			__String* frameName = __String::createWithFormat("factmake%d.png", i);
			SpriteFrame* spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName->getCString());
			animation->addSpriteFrame(spriteFrame);
		}
		animation->setDelayPerUnit(0.1f);
		animation->setRestoreOriginalFrame(false);
		Animate* action = Animate::create(animation);
		base->runAction(Repeat::create(action, 1));
		base->autorelease();
		return base;
	}
	CC_SAFE_DELETE(base);
	return nullptr;
}
void Base::handleBulletCollidingWithEnemy(float attack)
{
	this->SetVisible(true);
	if (!this->minusBloodAmount(attack * attackRate))
	{
		this->setVisible(false);
		allSprites.eraseObject(this);
		auto parent = this->getParent();
		this->resetgmap();
		parent->removeChild(this);
		was_destroyed = true;
		selectedSprites.eraseObject(this);
	}
}

float Sodier::SodierBT = 5;

Sodier* Sodier::create()
{
	Sodier* sodier = new (std::nothrow) Sodier();
	if (sodier && sodier->initWithFile("roles/flakt0.png"))
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

		Animation* animation = Animation::create();
		for (int i = 0; i <= 5; i++)
		{
			__String* frameName = __String::createWithFormat("roles/flakt%d.png", i);
			//SpriteFrame* spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName->getCString());
			animation->addSpriteFrameWithFile(frameName->getCString());
		}
		animation->setDelayPerUnit(0.1f);
		animation->setRestoreOriginalFrame(false);
		auto action = Animate::create(animation);
		sodier->runAction(RepeatForever::create(action));
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
		Bullet* bullet = Bullet::createWithSpriteFrameName("rocktet.png");
		bullet->setScale(0.5);
		bullet->setVelocity(Vec2(1000 * cos(direction / 57.3), 1000 * sin(direction / 57.3)));
		bullet->setRotation(-direction);
		bullet->setAttack(20);
		this->addChild(bullet, 0, TypeBullet);
		bullet->shootBulletFromBasement((Basement*)this, direction, "sound/rocketfire.wav");
	}
}
void Sodier::handleBulletCollidingWithEnemy(float attack)
{
	this->SetVisible(true);
	if (!this->minusBloodAmount(attack * attackRate))
	{
		this->setVisible(false);
		allSprites.eraseObject(this);
		SimpleAudioEngine::getInstance()->playEffect("sound/died1.wav");
		auto parent = this->getParent();
		parent->removeChild(this);
		setTime(false);
		selectedSprites.eraseObject(this);
	}
}

float PowerPlant::PowerPlantBT = 10;

PowerPlant* PowerPlant::create()
{
	PowerPlant* powerplant = new (std::nothrow) PowerPlant();
	if (powerplant && powerplant->initWithSpriteFrameName("powrmake.png"))
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
		Animation* animation = Animation::create();
		animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("powrmake.png"));
		for (int i = 1; i <= 12; i++)
		{
			__String* frameName = __String::createWithFormat("powrmake%d.png", i);
			SpriteFrame* spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName->getCString());
			animation->addSpriteFrame(spriteFrame);
		}
		animation->setDelayPerUnit(0.1f);
		animation->setRestoreOriginalFrame(false);
		Animate* action = Animate::create(animation);
		powerplant->runAction(Repeat::create(action, 1));
		return powerplant;
	}
	CC_SAFE_DELETE(powerplant);
	return nullptr;
}
void PowerPlant::handleBulletCollidingWithEnemy(float attack)
{
	this->SetVisible(true);
	if (!this->minusBloodAmount(attack * attackRate))
	{
		this->setVisible(false);
		allSprites.eraseObject(this);
		this->resetgmap();
		auto parent = this->getParent();
		parent->removeChild(this);
		selectedSprites.eraseObject(this);
	}
}

float MineField::MineFieldBT = 10;

MineField* MineField::create()
{
	MineField* minefield = new (std::nothrow) MineField();
	if (minefield && minefield->initWithSpriteFrameName("procmake0.png"))
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
		Animation* animation = Animation::create();
		for (int i = 0; i <= 9; i++)
		{
			__String* frameName = __String::createWithFormat("procmake%d.png", i);
			SpriteFrame* spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName->getCString());
			animation->addSpriteFrame(spriteFrame);
		}
		animation->setDelayPerUnit(0.1f);
		animation->setRestoreOriginalFrame(false);
		Animate* action = Animate::create(animation);
		minefield->runAction(Repeat::create(action, 1));
		return minefield;
	}
	CC_SAFE_DELETE(minefield);

	return nullptr;
}
void MineField::handleBulletCollidingWithEnemy(float attack)
{
	this->SetVisible(true);
	if (!this->minusBloodAmount(attack * attackRate))
	{
		this->setVisible(false);
		allSprites.eraseObject(this);
		this->resetgmap();
		auto parent = this->getParent();
		parent->removeChild(this);
		selectedSprites.eraseObject(this);
	}
}

float Barrack::BarrackBT = 10;

Barrack* Barrack::create()
{
	Barrack* barrack = new (std::nothrow) Barrack();
	if (barrack && barrack->initWithSpriteFrameName("barrmake0.png"))
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
		Animation* animation = Animation::create();
		for (int i = 0; i <= 12; i++)
		{
			__String* frameName = __String::createWithFormat("barrmake%d.png", i);
			SpriteFrame* spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName->getCString());
			animation->addSpriteFrame(spriteFrame);
		}
		animation->setDelayPerUnit(0.1f);
		animation->setRestoreOriginalFrame(false);
		Animate* action = Animate::create(animation);
		barrack->runAction(Repeat::create(action, 1));
		return barrack;
	}
	CC_SAFE_DELETE(barrack);
	return nullptr;
}
void Barrack::handleBulletCollidingWithEnemy(float attack)
{
	this->SetVisible(true);
	if (!this->minusBloodAmount(attack * attackRate))
	{
		this->setVisible(false);
		allSprites.eraseObject(this);
		this->resetgmap();
		auto parent = this->getParent();
		parent->removeChild(this);
		selectedSprites.eraseObject(this);
	}
}

float WarMachine_Factory::WarMachineFactoryBT = 15;

WarMachine_Factory* WarMachine_Factory::create()
{
	WarMachine_Factory* warmachine_factory = new (std::nothrow) WarMachine_Factory();
	if (warmachine_factory && warmachine_factory->initWithSpriteFrameName("weapmake0.png"))
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
		Animation* animation = Animation::create();
		for (int i = 0; i <= 14; i++)
		{
			__String* frameName = __String::createWithFormat("weapmake%d.png", i);
			SpriteFrame* spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName->getCString());
			animation->addSpriteFrame(spriteFrame);
		}
		animation->setDelayPerUnit(0.1f);
		animation->setRestoreOriginalFrame(false);
		Animate* action = Animate::create(animation);
		warmachine_factory->runAction(Repeat::create(action, 1));
		return warmachine_factory;
	}
	CC_SAFE_DELETE(warmachine_factory);
	return nullptr;
}
void WarMachine_Factory::handleBulletCollidingWithEnemy(float attack)
{
	this->SetVisible(true);
	if (!this->minusBloodAmount(attack * attackRate))
	{
		this->setVisible(false);
		allSprites.eraseObject(this);
		this->resetgmap();
		auto parent = this->getParent();
		parent->removeChild(this);
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
		patroldog->schedule(schedule_selector(PatrolDog::updatePatrolDog), 0.5f);
		patroldog->setAttackRate(1);
		return patroldog;
	}
	CC_SAFE_DELETE(patroldog);
	return nullptr;
}
void PatrolDog::handleBulletCollidingWithEnemy(float attack)
{
	this->SetVisible(true);
	if (!this->minusBloodAmount(attack * attackRate))
	{
		this->setVisible(false);
		allSprites.eraseObject(this);
		selectedSprites.eraseObject(this);
	}
}
void PatrolDog::updatePatrolDog(float dt)
{
	static bool isMoved = false;
	Vec2 enemyPosition = searchEnemy(allSprites, (Basement*)this, direction);
	if (this && this->isVisible() && enemy && enemy->getTag() == TypeSodier && enemyPosition != Vec2(-1, -1))
	{
		SimpleAudioEngine::getInstance()->playEffect("sound/dog.wav");
		if (!isMoved)
		{
			this->runAction(MoveTo::create(2.5, enemyPosition));
			isMoved = true;
		}

		if (!enemy->minusBloodAmount(30))
		{
			isMoved = false;
			allSprites.eraseObject(enemy);
			enemy->getParent()->removeChild(enemy);
		}
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
void Tank::handleBulletCollidingWithEnemy(float attack)
{
	this->SetVisible(true);
	if (!this->minusBloodAmount(attack * attackRate))
	{
		SimpleAudioEngine::getInstance()->playEffect("sound/died1.wav");
		this->setVisible(false);
		allSprites.eraseObject(this);
		auto parent = this->getParent();
		parent->removeChild(this);
		selectedSprites.eraseObject(this);
	}
}
void Tank::updateTank(float dt)
{
	Vec2 enemyPosition = searchEnemy(allSprites, (Basement*)this, direction);
	if (this && enemyPosition != Vec2(-1, -1))
	{
		this->stopAllActions();
		Bullet* bullet = Bullet::createWithSpriteFrameName("tankbullet.png");
		bullet->setScale(1.2);
		bullet->setVelocity(Vec2(1000 * cos(direction / 57.3), 1000 * sin(direction / 57.3)));
		bullet->setRotation(-direction);
		bullet->setAttack(100);
		this->addChild(bullet, 0, TypeBullet);
		bullet->shootBulletFromBasement((Basement*)this, direction, "sound/tankfire.wav");
	}
}

bool isEnemy(Basement* basement)
{
	auto label = basement->getTag();
	if (label == TypeBase || label == TypeSodier ||label==TypeMineField|| label == TypePowerPlant || label == TypeWarmachineFactory
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
	auto P1 = self->getPosition();
	for (auto i : sprites)
	{
		auto P2 = i->getPosition();
		auto distance = sqrtf((P1.x - P2.x) * (P1.x - P2.x) + (P1.y - P2.y)* (P1.y - P2.y));
		if (distance <= 300.0 && i->camp != self->camp)
		{
			direction = directionAngle(i->getPosition() - self->getPosition());
			if (self->getTag() == TypePatrolDog && i->getTag() == TypeSodier)                //搜索敌人
				((PatrolDog*)self)->setenemy((Sodier*)i);
			return i->getPosition();
		}
	}
	return Vec2(-1, -1);
}