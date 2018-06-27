#ifndef _MAP_H_
#define _MAP_H_

#include"cocos2d.h"
#include<vector>
USING_NS_CC;
using namespace std;
class MyMap : public cocos2d::Node
{

public:
	cocos2d::TMXTiledMap* up;
	cocos2d::TMXTiledMap* left;
	cocos2d::TMXTiledMap* right;
	cocos2d::TMXTiledMap* down;
	Size MapSize;
	static MyMap* create();
	Size getMapSize()const;
	bool setcolor(Vec2 pos, int color);
	Point getPositionAtMap(Vec2 pos);
	Point getPositionAtGL(Vec2 pos);
	Size tileSize;
	Vec2 staggeredCoordForPosition(Point position);
	void MyMap::getPositionAtGL(vector<Vec2>&  path);
	

private:
	virtual bool init();

};







#endif //_MAP_H_

