#ifndef _BUILD_AND_PRoDUCE_
#define _BUILD_AND_PRoDUCE_

#include"cocos2d.h"
#include<vector>
#include"BuildingAndsodier.h"
USING_NS_CC;
class Manager : public cocos2d::Node
{
public:
	static Manager* create();

	vector<int> ID;
	vector<int>type;
	vector<Vec2>position;
	vector<float>resttime;
	float gettime(int type);
	void additem(int id, int Type, Vec2 Pos, float Resttime);

	void buildcomplete(int ID, int type, Vec2 pos);

	void Erase(int i);
	MyMap* gamemap;
	GridMap* gridmap;
	void settilemap(MyMap * GameMap);
	void setgridmap(GridMap * Gridmap);
private:
	
    bool init();
	int map_width, map_height;
	void update1(float dt);

};

#endif //_BUILD_AND_PRoDUCE_