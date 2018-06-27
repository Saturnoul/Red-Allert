#include"MoveControl.h"


float time(Vec2 P1, Vec2 P2, int label)
{
	auto distance = sqrtf((P1.x - P2.x) * (P1.x - P2.x) + (P1.y - P2.y)* (P1.y - P2.y));          //计算移动时间
	if (label == TypeSodier)
		return distance / 50;
	if (label == TypeTank)
		return distance / 80;
	if (label == TypePatrolDog)
		return distance / 60;
}

// judge if the player wants to cancel previous selection 
void isCancelSelection(Vec2 position, EventMouse::MouseButton button)
{
	if (button == EventMouse::MouseButton::BUTTON_RIGHT)
	{
		for (auto i : selectedSprites)
		{
			i->SetVisible(false);
		}
		selectedSprites.clear();
		isOperating = false;
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
void selectInRec(Vec2 position, MyMap* gamemap)
{
	if (isMoved && !isOperating)
	{
		for (auto i : allSprites)
		{
			if (isInRange(gamemap->convertToWorldSpace(i->getPosition()), startPoint, position))
			{
				selectedSprites.pushBack(i);
				i->SetVisible(true);
			}
		}
		isOperating = true;
		SimpleAudioEngine::getInstance()->playEffect("selecttarget.wav");
	}
}

//selected the sprite being clicked
void singleSelect(Vec2 position,MyMap* gamemap)
{
	isDown = true;
	for (auto i : allSprites)
	{
		if (isInRange(gamemap->convertToNodeSpace(position), i->getPosition() - i->getContentSize() / 2, i->getPosition() + i->getContentSize() / 2) && !isOperating)
		{
			SimpleAudioEngine::getInstance()->playEffect("selecttarget.wav");
			selectedSprites.pushBack(i);
			i->SetVisible(true);
			isOperating = true;
			break;
		}
	}
	startPoint = position;
}

//move the sprites being selected with the path generated
void Move(Vec2 position, MyMap* _tileMap, GridMap* gridmap)
{
	//Vec2 cusorTilePosition = tileCoordFromPosition(position, _tileMap);
	Vec2 cusorTilePosition = _tileMap->staggeredCoordForPosition(position);
	if (!selectedSprites.empty() && !allSprites.empty() && !isMoved && !isSelecting &&gridmap->gmap[int(cusorTilePosition.x)][int(cusorTilePosition.y)] && isOperating)
	{
		log("Moved");
		Vec2 contentSize = allSprites.at(0)->getContentSize();
		float area = selectedSprites.size() * contentSize.x * contentSize.y * 1.0;
		float equalEdge = sqrt(area) / 5;
		for (int i = 0; i < selectedSprites.size() && isMovable(selectedSprites.at(i)); i++)
		{
			srand((unsigned)time(NULL));
			Vec2 Position;
			Vec2 TilePosition;
			do
			{
				Position.x = ((float)random() / RAND_MAX - 0.5) * equalEdge + position.x;
				Position.y = ((float)random() / RAND_MAX - 0.5) * equalEdge + position.y;
				//TilePosition = tileCoordFromPosition(Position, _tileMap);
				TilePosition = _tileMap->staggeredCoordForPosition(position);
			} while (TilePosition.x < _tileMap->getMapSize().height && TilePosition.y < _tileMap->getMapSize().width && TilePosition.x >= 0 && TilePosition.y >= 0 && !gridmap->gmap[int(TilePosition.x)][int(TilePosition.y)]);
			positionList.push_back(Position);
		}
		for (int i = 0; i < selectedSprites.size() && isMovable(selectedSprites.at(i)); i++)
		{
			int label = selectedSprites.at(i)->getTag();
			Vec2 startTileCoord = _tileMap->staggeredCoordForPosition(_tileMap->convertToWorldSpace(selectedSprites.at(i)->getPosition()));
			Vec2 endTileCoord = _tileMap->staggeredCoordForPosition(position);
			if (startTileCoord != endTileCoord)
			{
				vector<Vec2> path = searchPathForTile(gridmap, startTileCoord, endTileCoord);
				_tileMap->getPositionAtGL(path);
				cocos2d::Vector<FiniteTimeAction*> pathAction;
				for (int j = 1; j < path.size(); j++)
				{
					pathAction.pushBack(static_cast<FiniteTimeAction*>(MoveTo::create(time(path[j - 1], path[j], label), path[j])));
				}
				pathAction.popBack();
				pathAction.pushBack(MoveTo::create(time(path[path.size() - 2],_tileMap->convertToNodeSpace(positionList[i]), label), _tileMap->convertToNodeSpace(positionList[i])));
				auto sequence = Sequence::create(pathAction);
				selectedSprites.at(i)->stopAllActions();
				selectedSprites.at(i)->runAction(sequence);
			}
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

bool isMovable(Basement* basement)
{
	auto tag = basement->getTag();
	if (tag == TypeSodier || tag == TypePatrolDog || tag == TypeTank)
		return true;
	else
		return false;
}

void cancleselected()
{
	for(auto i:selectedSprites)
	{
		i->SetVisible(false);
	}
	selectedSprites.clear();
	isOperating = false;
}