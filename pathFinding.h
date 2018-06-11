#pragma once
#include "cocos2d.h"
#include<vector>
using namespace std;
USING_NS_CC;
bool isInChosedPositions(vector<Vec2>& chosedPs, Vec2& possibleP)
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
int FindShortest(vector<int>& possibleCosts)
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
int getLength(Vec2& point1, Vec2& point2)
{
	int delta_x = point1.x - point2.x;
	int delta_y = point1.y - point2.y;
	float length = (float)(delta_x * delta_x + delta_y * delta_y);
	length = sqrtf(length);
	int Length = length * 10;
	return Length;
}
bool isAbandoned(Vec2& P, vector<Vec2>& abandonedPs)
{
	for (auto i : abandonedPs)
	{
		if (i.x == P.x && i.y == P.y)
			return true;
	}
	return false;
}
Vec2 isThrough(float delta_x, float delta_y, int order)
{
	if (delta_x == 0)
		return Vec2(0, delta_y);
	else if (delta_y == 0)
		return Vec2(delta_x, 0);
	else
	{
		int Delta_x = static_cast<int>(delta_x);
		int Delta_y = static_cast<int>(delta_y);
		int start = ((order - 1) * Delta_y) / Delta_x;
		int end = (order * Delta_y) / Delta_x;
		return Vec2(start, end);
	}
}
bool isWalkable(Vec2& P1, Vec2& P2, vector<vector<int> >& positionCondition)
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
		Vec2 s_n = isThrough(abs(x_diff) + 1, abs(y_diff) + 1, i + 1);
		for (int j = s_n.x; j <= s_n.y && j <= abs(y_diff); j++)
		{
			if (!positionCondition[P1.x + i * delta_x][P1.y + j * delta_y])
			{
				return false;
			}
		}
	}
	return true;
}
vector<Vec2> searchPathForTile(vector<vector<int> >& positionCondition, Vec2& startPosition, Vec2& endPosition)
{
	int index = -1;
	vector<Vec2> possiblePositions;
	vector<int> possibleCostsA;
	vector<int> possibleCosts;
	vector<int> chosedCosts;
	vector<int> chosedCostsA;
	vector<Vec2>abandonedPositions;
	vector<Vec2> chosedPositions;
	vector<Vec2> finalPositions;
	Vec2 possiblePosition;
	chosedPositions.push_back(startPosition);
	chosedCostsA.push_back(0);
	finalPositions.push_back(startPosition);
	do
	{
		for (int i = -1; i < 2; i++)
		{
			for (int j = -1; j < 2; j++)
			{
				if (i != 0 || j != 0)
				{
					possiblePosition.x = startPosition.x + i;
					possiblePosition.y = startPosition.y + j;
					if (possiblePosition.x >= 0 && possiblePosition.y >= 0 && possiblePosition.x < positionCondition.size() && possiblePosition.y < positionCondition[0].size() && positionCondition[possiblePosition.x][possiblePosition.y] && !isAbandoned(possiblePosition, abandonedPositions) && isWalkable(chosedPositions[chosedPositions.size() - 1], possiblePosition, positionCondition))
					{
						int manhattonDistance = 10 * (abs(endPosition.x - possiblePosition.x) + abs(endPosition.y - possiblePosition.y));
						//int manhattonDistance = getLength(possiblePosition, endPosition);
						if (!isInChosedPositions(chosedPositions, possiblePosition))
						{
							possiblePositions.push_back(possiblePosition);
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
		index = FindShortest(possibleCosts);
		if (index >= 0)
		{
			if (chosedPositions.size() >= 2 && getLength(chosedPositions[chosedPositions.size() - 2], possiblePositions[index]) < chosedCostsA[chosedCostsA.size() - 1] + possibleCostsA[index] && isWalkable(chosedPositions[chosedPositions.size() - 2], possiblePositions[index], positionCondition))
			{
				abandonedPositions.push_back(chosedPositions[chosedPositions.size() - 1]);
				chosedPositions.pop_back();
				chosedCostsA.pop_back();
				chosedCosts.pop_back();
			}
			chosedCostsA.push_back(possibleCostsA[index]);
			chosedPositions.push_back(possiblePositions[index]);
			chosedCosts.push_back(possibleCosts[index]);
			startPosition.x = possiblePositions[index].x;
			startPosition.y = possiblePositions[index].y;
		}
		else
			if (abandonedPositions.size() != 0)
				abandonedPositions.pop_back();
		if (possiblePositions.size() != 0)
			possiblePositions.clear();
		if (!possibleCostsA.empty())
			possibleCostsA.clear();
		if (possibleCosts.size() != 0)
			possibleCosts.clear();
	} while (chosedPositions[chosedPositions.size() - 1].x != endPosition.x || chosedPositions[chosedPositions.size() - 1].y != endPosition.y);
	Vec2 temp = chosedPositions[0];
	for (int i = 1; i < chosedPositions.size(); i++)
	{
		if (!isWalkable(temp, chosedPositions[i], positionCondition))
		{
			finalPositions.push_back(chosedPositions[i - 1]);
			temp = chosedPositions[i];
		}
	}
	finalPositions.push_back(chosedPositions[chosedPositions.size() - 1]);
	return finalPositions;
}
vector<Vec2> transferToOpenGLCoord(vector<Vec2>& chosedPs, float width, float height, int TileHeight, Vec2 MapPosition)
{
	for (int i = 0; i < chosedPs.size(); i++)
	{
		chosedPs[i].x = (chosedPs[i].x + 0.5) * width + MapPosition.x;
		chosedPs[i].y = (TileHeight - (chosedPs[i].y + 0.5)) * height + MapPosition.y;
	}
	return chosedPs;
}

Vec2 tileCoordFromPosition(Vec2 pos, TMXTiledMap* _tileMap)
{
	int x = (pos.x - _tileMap->getPosition().x) / _tileMap->getTileSize().width;
	int y = ((_tileMap->getMapSize().height * _tileMap->getTileSize().height) - pos.y + _tileMap->getPosition().y) / _tileMap->getTileSize().height;
	return Vec2(x, y);
}