#include"gamemanager.h"
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
			buildcomplete(ID[i],CAMP[i], type[i], position[i]);
			Erase(i);
			i--;
		}
		else
			resttime[i] -= 0.1f;
	}

}
void Manager::buildcomplete(int ID,int Camp,int type,Vec2 pos)
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
		Base->camp = Camp;
		Base->id = ID;
		id_unit.insert(ID, Base);
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
		sodier->camp = Camp;
		sodier->id = ID;
		id_unit.insert(ID, sodier);
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
		powerplant->camp = Camp;
		powerplant->id = ID;
		id_unit.insert(ID, powerplant);
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
		field->camp = Camp;
		field->id = ID;
		id_unit.insert(ID, field);
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
		barrack->camp = Camp;
		barrack->id = ID;
		id_unit.insert(ID, barrack);
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
		warmaching->camp = Camp;
		warmaching->id = ID;
		id_unit.insert(ID, warmaching);
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
		dog->camp = Camp;
		dog->id = ID;
		id_unit.insert(ID, dog);
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
		tank->camp = Camp;
		tank->id = ID;
		id_unit.insert(ID, tank);
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
		CAMP.erase(CAMP.begin() + i);
		position.erase(position.begin() + i);
		resttime.erase(resttime.begin() + i);
	}


void Manager::additem(int id,int camp, int Type, Vec2 Pos, float Resttime)
{
	ID.push_back(id);
	type.push_back(Type);
	CAMP.push_back(camp);
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


void Manager::setmesssageset(GameMessageSet* _msg)
{
	this->msgs = _msg;
}

void Manager::setSocketClient(SocketClient* _socket_client)
{
	socket_client = _socket_client;
}

void Manager::updatestates()
{
	msgs->add_game_message()->genGameMessage(GameMessage::CmdCode::GameMessage_CmdCode_EMP,0,0,0,{});
	
	auto sent_msg = msgs->SerializeAsString();
	socket_client->send_string(sent_msg);
	
	auto rec = socket_client->get_string();
	msgs->ParseFromString(rec);

	int sent_msg_num = msgs->game_message_size();
	for (int i = 0; i < msgs->game_message_size(); i++)
	{
		const GameMessage&  msg = msgs->game_message(i);
		if (msg.cmd_code() == GameMessage::CmdCode::GameMessage_CmdCode_EMP)
		{
			log("Empty Message, there must be something wrong");
		}
		else if (msg.cmd_code() == GameMessage::CmdCode::GameMessage_CmdCode_CRT)
		{
			log("Build beginning");
			this->additem(msg.unit_0(), msg.camp(), msg.unit_type(),Vec2(msg.grid_path().grid_point(0).x(), msg.grid_path().grid_point(0).y()) ,this->gettime(msg.unit_type()));
		}
		else if (msg.cmd_code() == GameMessage::CmdCode::GameMessage_CmdCode_UDP)
		{
			auto temp = id_unit.at(msg.unit_0());
			temp->setPath(msg.grid_path());
			temp->move();
		}
	}
	
	log("right now");

	msgs->clear_game_message();
}