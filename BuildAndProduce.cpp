#include"BuildAndProduce.h"
#include<vector>

Manager* Manager::create()
{
	Manager* ret = new (std::nothrow) Manager();
	if (ret&&ret->Manager::init())
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);

	return nullptr;
}

bool Manager::init()
{
	if (!Node::init())
		return false;
	this->schedule(schedule_selector(Manager::update1),0.1f);
	return true;
}
void Manager::settilemap(MyMap * GameMap)
{
	this->gamemap = GameMap;
}
void Manager::setgridmap(GridMap * Gridmap)
{
	this->gridmap = Gridmap;
}
void Manager::update1(float dt)
{
	for (int i = 0; i < ID.size(); i++)
	{
		if (resttime[i] <= 0.1f)
		{
			buildcomplete(ID[i], type[i], position[i]);
			Erase(i);
			i--;
		}
		else
			resttime[i] -= 0.1f;
	}

}
void Manager::buildcomplete(int ID,int type,Vec2 pos)
{
	switch (type)
  {
	
	case 1:
	{
		auto Base = Base::create();
	    Base->setPosition(gamemap->getPositionAtMap(pos));
		Base->setpos(pos);
		Base->setgamemap(this->gamemap);
		Base->setgridmap(this->gridmap);
		Base->setgmap();
	    gamemap->addChild(Base, 1);
	    break;
	}
	case 2:
	{
		auto sodier = Sodier::create();
	    sodier->setPosition(gamemap->getPositionAtMap(pos));
		sodier->setpos(pos);
		sodier->setgamemap(this->gamemap);
		sodier->setgridmap(this->gridmap);
	    gamemap->addChild(sodier, 1);
	    break; 
	}
	
	case 3:
	{
		auto powerplant = PowerPlant::create();
	    powerplant->setPosition(gamemap->getPositionAtMap(pos));
		powerplant->setpos(pos);
		powerplant->setgamemap(this->gamemap);
		powerplant->setgridmap(this->gridmap);
		powerplant->setgmap();
	    gamemap->addChild(powerplant, 1);
	    break;
	}
	case 4:
	{
		auto  field = MineField::create();
	    field->setPosition(gamemap->getPositionAtMap(pos));
		field->setpos(pos);
		field->setgamemap(this->gamemap);
		field->setgridmap(this->gridmap);
		field->setgmap();
	    gamemap->addChild(field, 1);
	    break;
	}
	case 5:
	{
		auto barrack = Barrack::create();
	    barrack->setPosition(gamemap->getPositionAtMap(pos));
		barrack->setpos(pos);
		barrack->setgamemap(this->gamemap);
		barrack->setgridmap(this->gridmap);
		barrack->setgmap();
	    gamemap->addChild(barrack, 1);
	    break; 
	}
	case 6:
	{
		auto warmaching = WarMachine_Factory::create();
	    warmaching->setPosition(gamemap->getPositionAtMap(pos));
		warmaching->setpos(pos);
		warmaching->setgamemap(this->gamemap);
		warmaching->setgridmap(this->gridmap);
		warmaching->setgmap();
	    gamemap->addChild(warmaching, 1);
	    break; 
	}
	case 7:
	{
		auto dog = PatrolDog::create();
	    dog->setPosition(gamemap->getPositionAtMap(pos));
		dog->setpos(pos);
		dog->setgamemap(this->gamemap);
		dog->setgridmap(this->gridmap);
	    gamemap->addChild(dog, 1);
	    break; 
	}
	case 8:
	{
		auto tank = Tank::create();
	    tank->setPosition(gamemap->getPositionAtMap(pos));
		tank->setpos(pos);
		tank->setgamemap(this->gamemap);
		tank->setgridmap(this->gridmap);
	    gamemap->addChild(tank, 1);
	    break; 
	}
  }
	for (int i = pos.x; i < pos.x + 2; i++)
		for (int j = pos.y; j < pos.y + 2; j++)
		{
			if (i >= 0 && i <= 199 && j >= 0 && j <= 199)
			{
				gamemap->setcolor(Vec2(i, j), 3);
			}
		}
}

     void Manager::Erase(int i)
	{
		ID.erase(ID.begin() + i);
		type.erase(type.begin() + i);
		position.erase(position.begin() + i);
		resttime.erase(resttime.begin() + i);
	}


void Manager::additem(int id, int Type, Vec2 Pos, float Resttime)
{
	ID.push_back(id);
	type.push_back(Type);
	position.push_back(Pos);
	resttime.push_back(Resttime);
}

float Manager::gettime(int type)
{
	switch (type)
	{
	case 1:
		return Base::BaseBT;
	case 2:
		return Sodier::SodierBT;
	case 3:
		return PowerPlant::PowerPlantBT;
	case 4:
		return MineField::MineFieldBT;
	case 5:
		return Barrack::BarrackBT;
	case 6:
		return WarMachine_Factory::WarMachineFactoryBT;
	case 7:
		return PatrolDog::PatrolDogBT;
	case 8:
		return Tank::TankBT;
	}

	return 0.0f;
}