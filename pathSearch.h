#ifndef PATHSEARCH_H
#define PATHSEARCH_H
#include<vector>
#include<cmath>
#include "cocos2d.h"
using namespace cocos2d;

//judge if a tile is already being selected
bool isInChosedPs(vector<Vec2>& chosedPs, Vec2& possibleP)
{
	for (int i = 0; i < chosedPs.size(); i++)
	{
		if (chosedPs[i].x == possibleP.x && chosedPs[i].y == possibleP.y)
		{
			return true;
		}
	}
	return false;
}

//find the tile that has the least costs
int findShortest(vector<int>& possibleCosts)
{
	int shortest = 0;
	int size = possibleCosts.size();
	if (size != 0)
	{
		int smaller = possibleCosts[0];
		for (int i = 0; i < size; i++)
		{
			if (possibleCosts[i] < smaller)
			{
				smaller = possibleCosts[i];
				shortest = i;
			}
		}
		return shortest;
	}
	return -1;
}

//calculate the length of two positions, all by intergers
int getLength(Vec2& point1, Vec2& point2)
{
	int delta_x = point1.x - point2.x;
	int delta_y = point1.y - point2.y;
	float length = (float)(delta_x * delta_x + delta_y * delta_y);
	length = sqrtf(length);
	int Length = length * 10;
	return Length;
}

//judge if a tile is already dismissed
bool isAbandoned(Vec2& P, vector<Vec2>& abandonedPs)
{
	for (auto i : abandonedPs)
	{
		if (i.x == P.x && i.y == P.y)
			return true;
	}
	return false;
}

//judge if two tiles can be walked through
bool isWalkable(Vec2&P1, Vec2& P2, vector<vector<bool> >& positionCondition)
{
	int delta_x = -1;
	int delta_y = -1;
	int x_diff = P2.x - P1.x;
	int y_diff = P2.y - P1.y;
	if (x_diff > 0)
		delta_x = 1;
	if (y_diff > 0)
		delta_y = 1;
	for (int i = 0; i <= abs(x_diff); i++)
	{
		for (int j = 0; j <= abs(y_diff); j++)
		{
			if (!positionCondition[P1.x + i * delta_x][P1.y + j * delta_y])
			{
				return false;
			}
		}
	}
	return true;
}

//search the path in made of tiles
vector<Vec2> searchPathForTile(vector<vector<bool> >& positionCondition, Vec2& startPosition, Vec2& endPosition)
{
	int index = -1;
	vector<Vec2> possiblePs;
	vector<int> possibleCostsA;
	vector<int> possibleCosts;
	vector<int> chosedCosts;
	vector<int> chosedCostsA;
	vector<Vec2>abandonedPs;
	vector<Vec2> chosedPs;
	Vec2 possibleP;
	chosedPs.push_back(startPosition);
	chosedCostsA.push_back(0);

	do
	{
		for (int i = -1; i < 2; i++)
		{
			for (int j = -1; j < 2; j++)
			{
				if (i != 0 || j != 0)
				{
					possibleP.x = startPosition.x + i;
					possibleP.y = startPosition.y + j;
					if (possibleP.x >= 0 && possibleP.y >= 0 && possibleP.x < positionCondition.size() && possibleP.y < positionCondition[0].size() && positionCondition[possibleP.x][possibleP.y] && !isAbandoned(possibleP, abandonedPs) && isWalkable(chosedPs[chosedPs.size() - 1], possibleP, positionCondition))
					{
						int manhattonDistance = 10 * (abs(endPosition.x - possibleP.x) + abs(endPosition.y - possibleP.y));
						if (!isInChosedPs(chosedPs, possibleP))
						{
							possiblePs.push_back(possibleP);
							if (i * j == 0)
							{
								possibleCostsA.push_back(10);
								possibleCosts.push_back(manhattonDistance + 10);
							}
							else
							{
								possibleCosts.push_back(manhattonDistance + 14);
								possibleCostsA.push_back(14);
							}
						}
					}
				}
			}
		}
		index = findShortest(possibleCosts);
		if (index >= 0)
		{
			if (chosedPs.size() >= 2 && getLength(chosedPs[chosedPs.size() - 2], possiblePs[index]) < chosedCostsA[chosedCostsA.size() - 1] + possibleCostsA[index] && isWalkable(chosedPs[chosedPs.size() - 2], possiblePs[index], positionCondition))
			{
				abandonedPs.push_back(chosedPs[chosedPs.size() - 1]);
				chosedPs.pop_back();
				chosedCostsA.pop_back();
				chosedCosts.pop_back();
			}
			chosedCostsA.push_back(possibleCostsA[index]);
			chosedPs.push_back(possiblePs[index]);
			chosedCosts.push_back(possibleCosts[index]);
			startPosition.x = possiblePs[index].x;
			startPosition.y = possiblePs[index].y;
		}
		else
			if(abandonedPs.size() != 0)
				abandonedPs.pop_back();
		if (possiblePs.size() != 0) 
			possiblePs.clear(); 
		possibleCostsA.clear();
		if (possibleCosts.size() != 0)
			possibleCosts.clear();
	} while (chosedPs[chosedPs.size() - 1].x != endPosition.x || chosedPs[chosedPs.size() - 1].y != endPosition.y);
	return chosedPs;
}

//transfer the tile coordinate to openGl coordinate
vector<Vec2> transferToOpenGLCoord(vector<Vec2>& chosedPs, float width, float height,int TileHeight, Vec2 MapPosition)
{
	for (int i = 0; i < chosedPs.size(); i++)
	{
		chosedPs[i].x = (chosedPs[i].x + 0.5) * width + MapPosition.x;
		chosedPs[i].y = (TileHeight - (chosedPs[i].y + 0.5)) * height + MapPosition.y;
	}
	return chosedPs;
}

//transter the openGl coordinate to tile coordinate 
Vec2 tileCoordFromPosition(Vec2 pos, TMXTiledMap* _tileMap)
{
	int x = (pos.x - _tileMap->getPosition().x) / _tileMap->getTileSize().width;
	int y = ((_tileMap->getMapSize().height * _tileMap->getTileSize().height) - pos.y + _tileMap->getPosition().y) / _tileMap->getTileSize().height;
	return Vec2(x, y);
}
#endif
