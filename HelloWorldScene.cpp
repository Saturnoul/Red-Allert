#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Vec2 ppos;
Scene* HelloWorld::createScene()
{
	auto sc = HelloWorld::create();
	sc->autorelease();
	PhysicsWorld* phyWorld = sc->getPhysicsWorld();
	phyWorld->setGravity(Vec2(0, 0));
	return sc;
}

void HelloWorld::onEnter()
{
	Scene::onEnter();
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//注册 接触事件监听器
	contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = [this](PhysicsContact& contact)
	{
		auto spriteA = contact.getShapeA()->getBody()->getNode();
		auto spriteB = contact.getShapeB()->getBody()->getNode();
		Node* enemy2 = nullptr;

		if (spriteA && spriteB && isEnemy((Basement*)spriteA) && spriteB->getTag() == TypeBullet && (Node*)(spriteB->getParent()) != spriteA)
		{
			//不可见的炮弹不发生接触
			if (!spriteB->isVisible())
				return false;
			//使得炮弹消失
			spriteB->setVisible(false);
			enemy2 = spriteA;
			((Basement*)spriteA)->handleBulletCollidingWithEnemy(((Bullet*)spriteB)->getAttack());
			return false;
		}
		if (spriteA && spriteB && spriteA->getTag() == TypeBullet && isEnemy((Basement*)spriteB) && (Node*)(spriteA->getParent()) != spriteB)
		{
			//不可见的炮弹不发生接触
			if (!spriteA->isVisible())
				return false;
			//使得炮弹消失
			spriteA->setVisible(false);
			enemy2 = spriteB;
			((Basement*)spriteB)->handleBulletCollidingWithEnemy(((Bullet*)spriteA)->getAttack());
			return false;
		}
	};
	_eventDispatcher->addEventListenerWithFixedPriority(contactListener, 1);
}
// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::initWithPhysics())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	MouseLocation = Vec2((visibleSize.width + origin.x) / 2, (visibleSize.height + origin.y) / 2);
	/////////////////////////////
	layer1 = Layer::create();
	this->addChild(layer1, 0, 100);

	gamemap = MyMap::create();
	layer1->addChild(gamemap, 0, 1);
    gamemap->setPosition(Vec2(-6000, -3000));
	Mapsize = Size(12800, 6400);

	gridmap = GridMap::create(gamemap);
	this->addChild(gridmap);

	//创建建造管理
	manager = Manager::create();
	manager->settilemap(gamemap);
	manager->setgridmap(gridmap);
	this->addChild(manager);

	//加载资源
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("roles/game.plist", "roles/game.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("roles/buildings.plist", "roles/buildings.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("roles/bullet.plist", "roles/bullet.png");

	mymenu = createmenu();

	auto layer2 = Layer::create();
	this->addChild(layer2, 1, 200);
	layer2->addChild(mymenu);

	auto mouse = EventListenerMouse::create();
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouse, this);
	mouse->onMouseDown = [&](EventMouse*event)
	{

		MouseLocation.x = event->getCursorX();
		MouseLocation.y = event->getCursorY();
	};
	mouse->onMouseMove = [&](EventMouse* event)
	{
		MouseLocation.x = event->getCursorX();
		MouseLocation.y = event->getCursorY();

	};
	mouse->onMouseScroll = [&](EventMouse*event)
	{
		if ((event->getScrollY()) > 0)
		{
			gamemap->setScale(gamemap->getScaleX() + 0.1f, gamemap->getScaleY() + 0.1f);
			gamemap->setPosition(gamemap->getPosition().x - Mapsize.width*0.05f, gamemap->getPosition().y - Mapsize.height *0.05f);

		}
		else
		{
			gamemap->setScale(gamemap->getScaleX() - 0.1f, gamemap->getScaleY() - 0.1f);
			gamemap->setPosition(gamemap->getPosition().x + Mapsize.width*0.05f, gamemap->getPosition().y + Mapsize.height *0.05f);

		}
	};
	//金钱
	auto *money_icon = Sprite::create("Picture/ui/gold.png");
	this->addChild(money_icon, 1, 100);
	money_icon->setPosition(visibleSize.width - 130, 500);
	money_icon->setScale(0.03);

	money = Money::create();
	this->addChild(money, 1, 101);
	money->setPosition(visibleSize.width - 80, 500);
	money->schedule(schedule_selector(Money::update));
	//屏幕滚动
	this->scheduleUpdate();

	building = EventListenerMouse::create();
	building->setEnabled(false);
	building->onMouseDown = [&](EventMouse* event)
	{
		if (event->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT)
		{

			auto pos = gamemap->staggeredCoordForPosition(MouseLocation);
			if (gridmap->checkPositionRect(pos) && buildingitem != 0)
			{
				for (int i = ppos.x; i < ppos.x + 2; i++)
					for (int j = ppos.y; j < ppos.y + 2; j++)
					{
						if (i >= 0 && i <= 199 && j >= 0 && j <= 199)
						{
							gamemap->setcolor(Vec2(i, j), 4);
						}
					}
				manager->additem(111, buildingitem, pos, manager->gettime(buildingitem));
				checkmap = false;
				buildingitem = 0;
				ppos = Vec2(-1, -1);
			}
		}
		else
		{
			for (int i = ppos.x; i < ppos.x + 2; i++)
				for (int j = ppos.y; j < ppos.y + 2; j++)
				{
					if (i >= 0 && i <= 199 && j >= 0 && j <= 199)
					{
						gamemap->setcolor(Vec2(i, j),3);
					}
				}
			money->resetMoney(buildingitem);
			checkmap = false;
			buildingitem = 0;
			ppos = Vec2(-1, -1);
			
		}
	};

	building->onMouseMove = [&](EventMouse* event)
	{
		selectedSprites.clear();
		Vec2 pos = gamemap->staggeredCoordForPosition(MouseLocation);
		if (pos != ppos)
		{
			for (int i = ppos.x; i < ppos.x + 2; i++)
				for (int j = ppos.y; j < ppos.y + 2; j++)
				{
					if (i >= 0 && i <= 199 && j >= 0 && j <= 199)
					{
						gamemap->setcolor(Vec2(i, j), 3);
					}
				}
			if (pos.x >= 0 && pos.x <= 199 && pos.y >= 0 && pos.y <= 199)
			{
				if (gridmap->checkPositionRect(Vec2(pos.x, pos.y)))
				{
					for (int i = pos.x; i < pos.x + 2; i++)
						for (int j = pos.y; j < pos.y + 2; j++) 
						{
							gamemap->setcolor(Vec2(i, j), 1);
						}
				}
				else
				{
					for (int i = pos.x; i < pos.x + 2; i++)
						for (int j = pos.y; j < pos.y + 2; j++)
						{
							if (i >= 0 && i <= 199 && j >= 0 && j <= 199)
							{
								gamemap->setcolor(Vec2(i, j), 2);
							}
						}
				}
			}
			ppos = pos;
		}
	};
	building->onMouseUp = [&](EventMouse*event)
	{
		setDefault(layer1);
		MouseSelect->setEnabled(true);

	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(building, this);

	MouseSelect = EventListenerMouse::create();
	_eventDispatcher->addEventListenerWithSceneGraphPriority(MouseSelect, this);
	MouseSelect->onMouseDown = [&](EventMouse*event)
	{

		singleSelect(MouseLocation,gamemap);
	};
	MouseSelect->onMouseMove = [&](EventMouse* event)
	{
		drawRec(MouseLocation, layer1);

	};

	MouseSelect->onMouseUp = [&](EventMouse* event)
	{
		isCancelSelection(MouseLocation, event->getMouseButton());
		selectInRec(MouseLocation,gamemap);
		if (!checkmap)
		{
			Move(MouseLocation, gamemap, gridmap);
		}
		setDefault(layer1);
	};

	return true;
}


void  HelloWorld::createBarrack(cocos2d::Ref* pSender)
{
	cancleselected();
	if (money->checkMoney(100))
	{
		MouseSelect->setEnabled(false);
		selectedSprites.clear();
		money->spendMoney(1);
		buildingitem = 5;
		checkmap = true;
		building->setEnabled(true);
	}
}
void  HelloWorld::createpowerplant(cocos2d::Ref* pSender)
{
	cancleselected();
	if (money->checkMoney(100))
	{
		MouseSelect->setEnabled(false);
		selectedSprites.clear();
		money->spendMoney(2);
		buildingitem = 3;
		checkmap = true;
		building->setEnabled(true);
	}
}
void  HelloWorld::createfield(cocos2d::Ref* pSender)
{
	cancleselected();
	if (money->checkMoney(100))
	{
		MouseSelect->setEnabled(false);
		selectedSprites.clear();
		money->spendMoney(3);
		buildingitem = 4;
		checkmap = true;
		building->setEnabled(true);
	}
}
void  HelloWorld::createfactory(cocos2d::Ref* pSender)
{
	cancleselected();
	if (money->checkMoney(100))
	{
		MouseSelect->setEnabled(false);
		selectedSprites.clear();
		money->spendMoney(4);
		buildingitem = 6;
		checkmap = true;
		building->setEnabled(true);
	}
}
void  HelloWorld::createsodier(cocos2d::Ref* pSender)
{
	cancleselected();
	if (money->checkMoney(100))
	{
		MouseSelect->setEnabled(false);
		selectedSprites.clear();
		money->spendMoney(5);
		buildingitem = 2;
		checkmap = true;
		building->setEnabled(true);
	}
}
void  HelloWorld::createpetroldog(cocos2d::Ref* pSender)
{
	cancleselected();
	if (money->checkMoney(100))
	{
		MouseSelect->setEnabled(false);
		selectedSprites.clear();
		money->spendMoney(6);
		buildingitem = 7;
		checkmap = true;
		building->setEnabled(true);
	}
}
void  HelloWorld::createTank(cocos2d::Ref* pSender)
{
	cancleselected();
	if (money->checkMoney(100))
	{
		MouseSelect->setEnabled(false);
		selectedSprites.clear();
		money->spendMoney(7);
		buildingitem = 8;
		checkmap = true;
		building->setEnabled(true);
	}
}
void HelloWorld::update(float dt)
{
	screen_move(MouseLocation);
	if (!checkmap)
	{
		building->setEnabled(false);
	}
	hasbuildbarrack = false;
	hasbuildfactory = false;
	for (int i = 0; i < allSprites.size(); i++)
	{
		auto temp = allSprites.at(i)->getTag();
		if (temp == 4)
		{
			hasbuildbarrack = true;
		}
		if (temp==5 )
		{
			hasbuildfactory = true;
		}
	}

	if (!hasbuildbarrack)
	{
		this->soldier->setEnabled(false);
	}
	else
	{
		this->soldier->setEnabled(true);
	}
	if (!hasbuildfactory)
	{
		this -> tank->setEnabled(false);
	}
	else
	{
		this->tank->setEnabled(true);
	}

}
void HelloWorld::screen_move(Vec2 position)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float x = position.x;
	float y = position.y - origin.y;
	float center_x = visibleSize.width / 2;
	float center_y = visibleSize.height / 2;

	if (abs(x - center_x) > (center_x * 7 / 8) && !(abs(y - center_y) > (center_y * 7 / 8)))
	{

		if (x > center_x)

		{

			//log("move right");

			//log("%f    %f", tilemap->getPositionX(), tilemap->getContentSize().width / 2);

			if (x - center_x > center_x * 15 / 16)
			{
				gamemap->setPositionX(gamemap->getPositionX() - 20);

			}
			else
			{
				gamemap->setPositionX(gamemap->getPositionX() - 5);

			}
		}
		else

		{

			//log("move left");

			if (center_x - x > center_x * 15 / 16)
			{
				gamemap->setPositionX(gamemap->getPositionX() + 20);

			}
			else
			{
				gamemap->setPositionX(gamemap->getPositionX() + 5);

			}
		}
	}

	if (abs(y - center_y) > center_y * 7 / 8 && !(abs(x - center_x) > center_x * 7 / 8))

	{

		if (y > center_y)

		{

			if (y - center_y > center_y * 15 / 16)
			{
				gamemap->setPositionY(gamemap->getPositionY() - 20);

			}
			else
			{
				gamemap->setPositionY(gamemap->getPositionY() - 5);

			}
			log("up");
		}

		else

		{

			if (center_y - y > center_y * 15 / 16)
			{
				gamemap->setPositionY(gamemap->getPositionY() + 20);

			}
			else
			{
				gamemap->setPositionY(gamemap->getPositionY() + 5);

			}
			log("down");

		}

	}

	if (abs(y - center_y) > center_y * 7 / 8 && (abs(x - center_x) > center_x * 7 / 8))

	{

		if (x > center_x&&y>center_y)

		{

			if (x - center_x > center_x * 15 / 16)

			{

				gamemap->setPositionX(gamemap->getPositionX() - 20);

				gamemap->setPositionY(gamemap->getPositionY() - 20);

			}

			else

			{

				gamemap->setPositionX(gamemap->getPositionX() - 5);

				gamemap->setPositionY(gamemap->getPositionY() - 5);

			}

		}

		if (x > center_x&&y < center_y)

		{

			if (x - center_x > center_x * 15 / 16)

			{

				gamemap->setPositionX(gamemap->getPositionX() - 20);

				gamemap->setPositionY(gamemap->getPositionY() + 20);

			}

			else

			{

				gamemap->setPositionX(gamemap->getPositionX() - 5);

				gamemap->setPositionY(gamemap->getPositionY() + 5);

			}

		}

		if (x < center_x&&y < center_x)

		{



			if (center_x - x > center_x * 15 / 16)

			{

				gamemap->setPositionX(gamemap->getPositionX() + 20);

				gamemap->setPositionY(gamemap->getPositionY() + 20);

			}

			else

			{

				gamemap->setPositionX(gamemap->getPositionX() + 5);

				gamemap->setPositionY(gamemap->getPositionY() + 5);

			}

		}

		if (x<center_x&&y>center_y)

		{



			if (center_x - x > center_x * 15 / 16)

			{

				gamemap->setPositionX(gamemap->getPositionX() + 20);

				gamemap->setPositionY(gamemap->getPositionY() - 20);

			}

			else

			{

				gamemap->setPositionX(gamemap->getPositionX() + 5);

				gamemap->setPositionY(gamemap->getPositionY() - 5);

			}


		}
	}
}

void Money::update(float f)
{
	if (++timer % inc_prd == 0)
	{
		money += inc_amt;
		updateMoneyDisplay();
	}
	inc_amt = 0;
	for (int i = 0; i < allSprites.size(); i++)
	{
		if ((allSprites.at(i)->getTag()) == 3)
			inc_amt += 20;
	}
}

bool Money::init()
{
	money = 10000;
	char money_str[30];
	sprintf(money_str, "%d", money);
	return initWithString(money_str, "fonts/MoneyFont.fnt");
}

void Money::updateMoneyDisplay()
{
	char money_str[30];
	sprintf(money_str, "%d", money);
	setString(money_str);
}

void Money::spendMoney(int type)
{
	switch (type)
	{
	case 1:
	{
		money -= 100;
		break;
	}
	case 2:
	{
		money -= 100;
		break;
	}
	case 3:
	{
		money -= 100;
		break;
	}
	case 4:
	{
		money -= 100;
		break;
	}
	case 5:
	{
		money -= 100;
		break;
	}
	case 6:
	{
		money -= 100;
		break;
	}
	case 7:
	{
		money -= 100;
		break;
	}
	}
	updateMoneyDisplay();
}
void Money::resetMoney(int type)
{
	switch (type)
	{
	case 1:
	{
		money += 100;
		break;
	}
	case 2:
	{
		money += 100;
		break;
	}
	case 3:
	{
		money += 100;
		break;
	}
	case 4:
	{
		money += 100;
		break;
	}
	case 5:
	{
		money += 100;
		break;
	}
	case 6:
	{
		money += 100;
		break;
	}
	case 7:
	{
		money += 100;
		break;
	}
	}
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


//建立菜单

Menu* HelloWorld::createmenu()
{
	auto visiblesize = Director::getInstance()->getVisibleSize();
	Bar = MenuItemImage::create("menu/airplane-menu-up.png", "airplane-menu-up.png", CC_CALLBACK_1(HelloWorld::createBarrack,this));
	power = MenuItemImage::create("menu/airplane-menu-up.png", "airplane-menu-up.png", CC_CALLBACK_1(HelloWorld::createpowerplant, this));
	field = MenuItemImage::create("menu/airplane-menu-up.png", "airplane-menu-up.png", CC_CALLBACK_1(HelloWorld::createfield, this));
	factory = MenuItemImage::create("menu/airplane-menu-up.png", "airplane-menu-up.png", CC_CALLBACK_1(HelloWorld::createfactory, this));
	soldier = MenuItemImage::create("menu/airplane-menu-up.png", "airplane-menu-up.png", CC_CALLBACK_1(HelloWorld::createsodier, this));
	dog = MenuItemImage::create("menu/airplane-menu-up.png", "airplane-menu-up.png", CC_CALLBACK_1(HelloWorld::createpetroldog, this));
	tank = MenuItemImage::create("menu/airplane-menu-up.png", "airplane-menu-up.png", CC_CALLBACK_1(HelloWorld::createTank, this));
	
	Menu* ret = Menu::create(Bar,power,field,factory,soldier,dog,tank, NULL);
	Bar->setPosition(visiblesize.width - Bar->getContentSize().width, visiblesize.height - Bar->getContentSize().height);
	ret->alignItemsVertically();
	ret->setPosition(visiblesize.width - Bar->getContentSize().width/2-15, visiblesize.height - Bar->getContentSize().height * 2);
	return ret;
}




