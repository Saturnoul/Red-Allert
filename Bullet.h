#ifndef _BULLET_
#define _BULLET_
#include "cocos2d.h"
#include "BaseOfAll.h"
#include "pathFinding.h"
#include<math.h>
#include"HelloWorldScene.h"
USING_NS_CC;


class Bullet : public Sprite
{
	CC_SYNTHESIZE(Vec2, velocity, Velocity);    //子弹速度
	CC_SYNTHESIZE(float, attack, Attack);       //子弹攻击力
public:
	void shootBulletFromBasement(Basement* basement, float direction, const char* effect);         //产生子弹并射向敌人
	virtual void update(float dt);             //使子弹移动
	static Bullet* createWithSpriteFrameName(const char* spriteFrameName);       //子弹静态构造函数
};

#endif //_BULLET_