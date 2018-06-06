#pragma once
#include "cocos2d.h"
#include "pathFinding.h"
#include<vector>
using namespace cocos2d;
using namespace std;


static Vec2 startPoint;
vector<Sprite*>selectedSprites;                //the sprites being selected 
static vector<Vec2>positionList;               //the ending positions for every sprite
static bool isSelecting = false;               //judge if the mouse is selecting a sprite
static bool isDown = false;                    //judge if the mouse is clicked down
static bool isMoved = false;                   //judge if the cursor is draged
static bool isOperating = false;               //judge if there are some sprites already being selected
static vector<vector<bool> > positionCondition;    // store if some position is walkable



//judge if a tile is walkable
bool isCollidable(Vec2 position, TMXTiledMap* _tileMap, TMXLayer* _collidable)
{
	int tileGid = _collidable->getTileGIDAt(position);

	if (tileGid > 0)
	{
		Value prop = _tileMap->getPropertiesForGID(tileGid);
		ValueMap propValueMap = prop.asValueMap();
		std::string collision = propValueMap["Collidable"].asString();

		if (collision == "true")
			return true;
		else
			return false;
	}
}

//generate a matrix to store if every tile is walkable
void generatePositionCondition(TMXTiledMap* _tileMap)
{
	vector<bool> everyRow;
	auto _collidable = _tileMap->getLayer("collidable");
	for (int i = 0; i < _tileMap->getMapSize().width; i++)
	{
		for (int j = 0; j < _tileMap->getMapSize().height; j++)
		{
			everyRow.push_back(!isCollidable(Vec2((float)i, (float)j), _tileMap, _collidable));
		}
		positionCondition.push_back(everyRow);
		everyRow.clear();
	}
}

// judge if the player wants to cancel previous selection 
void isCancelSelection(Vec2 position, EventMouse::MouseButton button, TMXTiledMap* _tileMap)
{
	if (button == EventMouse::MouseButton::BUTTON_RIGHT)
	{
		for (auto i : selectedSprites)
		{
			i->setConditionFalse();
		}
		selectedSprites.clear();
		isOperating = false;
		log("cancelled");
	}
}

//judge if a sprite is in the rectangle area
bool isInRange(Vec2 aimposition, Vec2 startposition, Vec2 endposition)
{
	Vec2 delta1 = aimposition - startposition;
	Vec2 delta2 = aimposition - endposition;
	if (delta1.x * delta2.x <= 0 && delta1.y * delta2.y <= 0)
		return true;
	return false;
}

//draw a rectangle as the cursor moving
void drawRec(Vec2 position, Layer* layer)
{
	if (isDown)
	{
		isMoved = true;
		layer->removeChildByTag(123);
		auto draw = DrawNode::create();
		draw->setTag(123);
		draw->drawRect(startPoint, position, Color4F(100, 100, 100, 100));
		layer->addChild(draw);
	}
}

//select all the sprites in the rectangle
void selectInRec(Vec2 position, vector<Sprite*> _Sprites)
{
	if (isMoved && !isOperating)
	{
		for (auto i : _Sprites)
		{
			if (isInRange(i->getPosition(), startPoint, position))
			{
				selectedSprites.push_back(i);
				i->setConditionTrue();
			}
		}
		isOperating = true;
		log("selectInRec");
	}
}

//selected the sprite being clicked
void singleSelect(Vec2 position, vector<Sprite*> _Sprites)
{
	isDown = true;
	for (auto i : _Sprites)
	{
		if (isInRange(position, i->getPosition() - i->getContentSize() / 2, i->getPosition() + i->getContentSize() / 2) && !isOperating)
		{
			selectedSprites.push_back(i);
			i->setConditionTrue();
			isOperating = true;
			isSelecting = true;
			log("click %d", selectedSprites.size());
		}
	}
	startPoint = position;
}

//move the sprites being selected with the path generated
void Move(Vec2 position, TMXTiledMap* _tileMap, vector<Sprite*> _Sprites)
{
	Vec2 cusorTilePosition = tileCoordFromPosition(position, _tileMap);
	if (!isMoved && !isSelecting && positionCondition[cusorTilePosition.x][cusorTilePosition.y])
	{
		log("Moved");
		Vec2 contentSize = _Sprites[0]->getContentSize();
		float area = selectedSprites.size() * contentSize.x * contentSize.y * 1.0;
		float equalEdge = sqrt(area) / 5;
		for (int i = 0; i < selectedSprites.size(); i++)
		{
			srand((unsigned)time(NULL));
			Vec2 Position;
			Vec2 TilePosition;
			do
			{
				Position.x = ((float)random() / RAND_MAX - 0.5) * equalEdge + position.x;
				Position.y = ((float)random() / RAND_MAX - 0.5) * equalEdge + position.y;
				TilePosition = tileCoordFromPosition(Position, _tileMap);
			} while (TilePosition.x < _tileMap->getMapSize().height && TilePosition.y < _tileMap->getMapSize().width && TilePosition.x >= 0 && TilePosition.y >= 0 && !positionCondition[TilePosition.x][TilePosition.y]);
			positionList.push_back(Position);
		}
		for (int i = 0; i < selectedSprites.size(); i++)
		{
			Vec2 startTileCoord(tileCoordFromPosition(selectedSprites[i]->getPosition(), _tileMap));
			Vec2 endTileCoord(tileCoordFromPosition(positionList[i], _tileMap));
			vector<Vec2> path = searchPathForTile(positionCondition, startTileCoord, endTileCoord);
			transferToOpenGLCoord(path, _tileMap->getTileSize().width, _tileMap->getTileSize().height, _tileMap->getMapSize().height, _tileMap->getPosition());
			cocos2d::Vector<FiniteTimeAction*> pathAction;
			for (int i = 1; i < path.size(); i++)
			{
				pathAction.pushBack(static_cast<FiniteTimeAction*>(MoveTo::create(0.5, path[i])));
			}
			pathAction.popBack();
			pathAction.pushBack(MoveTo::create(0.5, positionList[i]));
			auto sequence = Sequence::create(pathAction);
			selectedSprites[i]->runAction(sequence);
		}
		positionList.clear();
	}
}

//set the flags to default value
void setDefault(Layer* layer)
{
	layer->removeChildByTag(123);
	isSelecting = false;
	isDown = false;
	isMoved = false;
}
