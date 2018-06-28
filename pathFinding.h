#ifndef _PATH_FINDING_
#define _PATH_FINDING_


#include "cocos2d.h"
#include<vector>
#include<math.h>
#include "gedian.h"
using namespace std;
USING_NS_CC;
bool isInChosedPositions(vector<GridPoint>& chosedPs, GridPoint& possibleP);
int FindShortest(vector<int>& possibleCosts);
int getLength(GridPoint& point1, GridPoint& point2);
bool isAbandoned(GridPoint& P, vector<GridPoint>& abandonedPs);
GridPoint isThrough(int delta_x, int delta_y, int order);
bool isWalkable(GridPoint& P1, GridPoint& P2, const GridMap* gamemap);


//生成最优的路径
vector<GridPoint> searchPathForTile(const GridMap* gamemap, GridPoint& startPosition, GridPoint& endPosition);


//将瓦片坐标转化为OpenGL坐标
//vector<Vec2> transferToOpenGLCoord(vector<Vec2>& chosedPs, MyMap* tileMap);


//将OpenGL坐标转化为瓦片坐标
//Vec2 tileCoordFromPosition(Vec2 pos, TMXTiledMap* _tileMap);

/*Vec2 tileCoordFromPosition(Vec2 pos, TMXTiledMap* _tileMap)
{
	Vec2 size = _tileMap->getMapSize();
	Vec2 Size = _tileMap->getTileSize();
	Vec2 TileMapPos = _tileMap->getPosition();
	int x = (2 * Size.y * (pos.x - TileMapPos.x) - Size.x * (2 * pos.y - size.x * Size.y - 2 * TileMapPos.y)) / (2 * Size.x * Size.y) - 1;
	int y = (Size.x * (size.y * Size.y - 2 * pos.y + 2 * TileMapPos.y) + Size.x * Size.y * (size.x + size.y) + 2 * Size.y * (TileMapPos.x - pos.x)) / (2 * Size.x * Size.y) - 1;
	return Vec2((float)x, (float)y);
}
vector<Vec2> transferToOpenGLCoord(vector<Vec2>& chosedPs,TMXLayer* tilelayer, TMXTiledMap* tilemap)
{
	for (int i = 0; i < chosedPs.size(); i++)
	{
		chosedPs[i] = tilelayer->getPositionAt(chosedPs[i]) + Vec2(tilemap->getTileSize().width / 2, tilemap->getTileSize().height / 2);
	}
	return chosedPs;
}*/

#endif //_PATH_FINDING_