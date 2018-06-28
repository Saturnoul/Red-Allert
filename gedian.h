#ifndef _GEDIAN_
#define _GEDIAN_

#include"cocos2d.h"
#include"Map.h"
USING_NS_CC;

class GridPoint
{

public:
	int x, y;
	GridPoint(int _x = 0, int _y = 0) : x(_x), y(_y) {}
	bool operator==(const GridPoint& gp2) const;
	GridPoint operator +(const GridPoint &gp) const;
	GridPoint operator -(const GridPoint &gp)const;
};
typedef GridPoint GridVec;
typedef std::vector<GridPoint>  GridPath;


class GridMap : public cocos2d::Node
{
public:
	static GridMap* create(const MyMap*gamemap);
	bool checkPosition(const Vec2& gp);
	bool checkPositionRect(const Vec2& gp);
	bool occupyPosition(int id, const Vec2& pos, bool occupy_grid = true);
	bool occupyPositionInRect(int id, const Vec2& pos);
	bool checkPointInMap(const GridPoint& gp) const;
	bool checkPointInMap(int x, int y) const;
	void leavePosition(const GridPoint& pos, bool occupy_grid = true);
	int gmap[200][200];
private:
	bool initWithTiledMap(const MyMap*gamemap);

	int map_width, map_height;
};









#endif //_GEDIAN_


