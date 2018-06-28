#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__
#include "cocos2d.h"
#include"Map.h"
#include"gedian.h"
#include "MoveControl.h"
#include"BuildAndProduce.h"
#include"MyMenu.h"
#include"SocketServer.h"
#include"SocketClient.h"
USING_NS_CC;
extern Vector<Basement*>selectedSprites;

class Manager;

class Money : public cocos2d::LabelBMFont
{
public:
	void update(float f) override;
	CREATE_FUNC(Money);
	bool init() override;
	/**
	* \brief update money on screen
	*/
	void updateMoneyDisplay();
	/**
	* \param cost cost
	* \return if money can cover the cost
	*/
	bool checkMoney(int cost) const;
	/**
	* \brief decrease cost in money
	* \param cost
	*/
	void spendMoney(int type);
	void resetMoney(int type);
	/**
	* \brief increase amount in money
	* \param amount
	*/
	void increaseMoney(int amount);
private:
	int money = 0;
	int timer = 0;
	//金钱增加周期
	int inc_prd = 20;
	//金钱每周期增加数额
	int inc_amt = 20;
	
};


class HelloWorld : public cocos2d::Scene
{


public:
	GridMap * gridmap;
	MyMap * gamemap;
	Manager * manager;
	Menu * mymenu;
	static cocos2d::Scene* createScene();
	cocos2d::Vec2 MouseLocation;
	virtual bool init();
	// a selector callback
	void createBarrack(cocos2d::Ref* pSender);
	void createpowerplant(cocos2d::Ref* pSender);
	void createfield(cocos2d::Ref* pSender);
	void createfactory(cocos2d::Ref* pSender);
	void createsodier(cocos2d::Ref* pSender);
	void createpetroldog(cocos2d::Ref* pSender);
	void createTank(cocos2d::Ref* pSender);
	void screen_move(Vec2 position);
	void update(float dt);
	Menu * createmenu();
	bool hasbuildbarrack=false;
	bool hasbuildfactory=false;
	EventListenerMouse*building;
	EventListenerMouse*MouseSelect;
	bool checkmap = true;
	Size Mapsize;
	int buildingitem=0;
	Layer* layer1;
	Money * money = nullptr;
	EventListenerPhysicsContact *contactListener;
	virtual void onEnter();

	MenuItemImage* Bar;
	MenuItemImage* power;
	MenuItemImage* field;
	MenuItemImage* factory;
	MenuItemImage* soldier;
	MenuItemImage* dog;
	MenuItemImage* tank;
	CREATE_FUNC(HelloWorld);
};
class CLientSceneTest : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	void startSchedule(float f);
	void wait_start();
	// a selector callback
	void menuCloseCallback1(cocos2d::Ref* pSender);
	void menuCloseCallback2(cocos2d::Ref* pSender);
	void menuCloseCallback3(cocos2d::Ref* pSender);
	//void update(float dt)override;
	// implement the "static create()" method manually
	CREATE_FUNC(CLientSceneTest);
	
private:
	SocketClient * socket_client = nullptr;
	int timer;
};
class ServerSceneTest : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	void startSchedule(float f);
	// a selector callback
	void menuCloseCallback1(cocos2d::Ref* pSender);
	void menuCloseCallback2(cocos2d::Ref* pSender);
	void menuCloseCallback3(cocos2d::Ref* pSender);
	//void update(float dt)override;
	// implement the "static create()" method manually
	CREATE_FUNC(ServerSceneTest);

private:
	SocketServer* socket_server = nullptr;
	SocketClient* socket_client = nullptr;
	int timer;
};




#endif // __HELLOWORLD_SCENE_H__
