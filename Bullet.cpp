#include "Bullet.h"
#include "pathFinding.h"
#include "cocos2d.h"
#include<math.h>
#include "HelloWorldScene.h"
USING_NS_CC;
Bullet* Bullet::createWithSpriteFrameName(const char* filename)
{
	Bullet* bullet = new Bullet();
	if (bullet && bullet->initWithFile(filename))
	{
		bullet->autorelease();
		bullet->setVisible(false);
		auto body = PhysicsBody::createBox(bullet->getContentSize());
		body->setCategoryBitmask(0x01);
		body->setCollisionBitmask(0x02);
		body->setContactTestBitmask(0x01);
		bullet->setPhysicsBody(body);
		bullet->setTag(TypeBullet);
		return bullet;
	}
	CC_SAFE_DELETE(bullet);
	return  nullptr;
}
void Bullet::shootBulletFromBasement(Basement* basement, float direction)
{
	float radius = sqrtf(basement->getContentSize().height * basement->getContentSize().height + basement->getContentSize().width * basement->getContentSize().width) / 2;
	this->setPosition(Vec2((radius + 5 )* cosf(direction / 57.3) + radius, (radius + 5 ) * sinf(direction / 57.3) + radius));
	this->setVisible(true);
	this->unscheduleUpdate();
	this->scheduleUpdate();
}
void Bullet::update(float dt)
{
	Size screenSize = Director::getInstance()->getVisibleSize();
	this->setPosition(this->getPosition() + velocity * dt);
}