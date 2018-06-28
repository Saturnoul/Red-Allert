#ifndef _BUILD_AND_PRoDUCE_
#define _BUILD_AND_PRoDUCE_

#include"cocos2d.h"
#include<vector>
#include"BuildingAndsodier.h"
#include"GameMessage.pb.h"
#include"SocketClient.h"
USING_NS_CC;
class Manager : public cocos2d::Node
{
public:
	static Manager* create();
	//建造列表
	vector<int> ID;
	vector<int>type;
	vector<int>CAMP;
	vector<Vec2>position;
	vector<float>resttime;
	float gettime(int type);
	void additem(int id,int camp, int Type, Vec2 Pos, float Resttime);
	void buildcomplete(int ID,int Camp, int type, Vec2 pos);
	void Erase(int i);


	//设置调用资源
	MyMap* gamemap;
	GridMap* gridmap;
	void settilemap(MyMap * GameMap);
	void setgridmap(GridMap * Gridmap);
	void setmesssageset(GameMessageSet* msg);
	GameMessageSet* msgs = nullptr;
	void updatestates();
	void setSocketClient(SocketClient* _socket_client);
	






	//数据
	cocos2d::Map<int, Basement*> id_unit;
	SocketClient* socket_client = nullptr;
	int playerid;


private:
	
    bool init();
	int map_width, map_height;
	void update1(float dt);

	

};

#endif //_BUILD_AND_PRoDUCE_