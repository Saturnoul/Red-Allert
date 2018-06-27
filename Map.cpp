#include"Map.h"
#include"cocos2d.h"
#include<vector>

USING_NS_CC;

MyMap* MyMap::create()
{
	MyMap *ret = new (std::nothrow) MyMap();
	if (ret->init())
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}
bool MyMap::init()
{
	if (!Node::init())
	{
		return false;
	}

	tileSize.width = 64;
	tileSize.height = 32;
	this->setContentSize(Size(12800, 6400));
	up = TMXTiledMap::create("map.tmx");
	left = TMXTiledMap::create("map.tmx");
	right = TMXTiledMap::create("map.tmx");
	down = TMXTiledMap::create("map.tmx");
	this->addChild(up, 0, 1);
	this->addChild(left, 0, 2);
	this->addChild(right, 0, 3);
	this->addChild(down, 0, 4);
	up->setPosition(Vec2(3200, 3200));
	left->setPosition(Vec2(0, 1600));
	down->setPosition(Vec2(3200, 0));
	right->setPosition(Vec2(6400, 1600));


	MapSize = Size(200, 200);
	return true;
}

Size MyMap::getMapSize()const
{
	return Size(MapSize.width, MapSize.height);
}


bool MyMap::setcolor(Vec2 pos, int color)
{
	int x = pos.x;
	int y = pos.y;
	Sprite* target_tile;
	switch (color)
	{
	case 1:
		if ((x / 100) > 0)
		{
			if ((y / 100) > 0)
			{
				target_tile = down->getLayer("back")->getTileAt(Vec2(pos.x - 100, pos.y - 100));
				target_tile->setColor(Color3B::GREEN);
				return true;
			}
			else
			{
				target_tile = right->getLayer("back")->getTileAt(Vec2(pos.x - 100, pos.y));
				target_tile->setColor(Color3B::GREEN);
				return true;
			}
		}
		else
		{
			if ((y / 100) > 0)
			{
				target_tile = left->getLayer("back")->getTileAt(Vec2(pos.x, pos.y - 100));
				target_tile->setColor(Color3B::GREEN);
				return true;
			}
			else
			{
				target_tile = up->getLayer("back")->getTileAt(Vec2(pos.x, pos.y));
				target_tile->setColor(Color3B::GREEN);
				return true;
			}
		}
	case 2:
		if ((x / 100) > 0)
		{
			if ((y / 100) > 0)
			{
				target_tile = down->getLayer("back")->getTileAt(Vec2(pos.x - 100, pos.y - 100));
				target_tile->setColor(Color3B::RED);
				return true;
			}
			else
			{
				target_tile = right->getLayer("back")->getTileAt(Vec2(pos.x - 100, pos.y));
				target_tile->setColor(Color3B::RED);
				return true;
			}
		}
		else
		{
			if ((y / 100) > 0)
			{
				target_tile = left->getLayer("back")->getTileAt(Vec2(pos.x, pos.y - 100));
				target_tile->setColor(Color3B::RED);
				return true;
			}
			else
			{
				target_tile = up->getLayer("back")->getTileAt(Vec2(pos.x, pos.y));
				target_tile->setColor(Color3B::RED);
				return true;
			}
		}
	case 3:
		if ((x / 100) > 0)
		{
			if ((y / 100) > 0)
			{
				target_tile = down->getLayer("back")->getTileAt(Vec2(pos.x - 100, pos.y - 100));
				target_tile->setColor(Color3B(255, 255, 255));
				return true;
			}
			else
			{
				target_tile = right->getLayer("back")->getTileAt(Vec2(pos.x - 100, pos.y));
				target_tile->setColor(Color3B(255, 255, 255));
				return true;
			}
		}
		else
		{
			if ((y / 100) > 0)
			{
				target_tile = left->getLayer("back")->getTileAt(Vec2(pos.x, pos.y - 100));
				target_tile->setColor(Color3B(255, 255, 255));
				return true;
			}
			else
			{
				target_tile = up->getLayer("back")->getTileAt(Vec2(pos.x, pos.y));
				target_tile->setColor(Color3B(255, 255, 255));
				return true;
			}
		}
	case 4:
		if ((x / 100) > 0)
		{
			if ((y / 100) > 0)
			{
				target_tile = down->getLayer("back")->getTileAt(Vec2(pos.x - 100, pos.y - 100));
				target_tile->setColor(Color3B::BLUE);
				return true;
			}
			else
			{
				target_tile = right->getLayer("back")->getTileAt(Vec2(pos.x - 100, pos.y));
				target_tile->setColor(Color3B::BLUE);
				return true;
			}
		}
		else
		{
			if ((y / 100) > 0)
			{
				target_tile = left->getLayer("back")->getTileAt(Vec2(pos.x, pos.y - 100));
				target_tile->setColor(Color3B::BLUE);
				return true;
			}
			else
			{
				target_tile = up->getLayer("back")->getTileAt(Vec2(pos.x, pos.y));
				target_tile->setColor(Color3B::BLUE);
				return true;
			}
		}

	}
	return false;
}

Point MyMap::getPositionAtMap(Vec2 pos)
{
	int x = pos.x;
	int y = pos.y;
	Point position;
	if ((x / 100) > 0)
	{
		if ((y / 100) > 0)
		{
			position = down->getLayer("back")->getPositionAt(Vec2(x - 100, y - 100));
			position += down->getPosition();
		}
		else
		{
			position = right->getLayer("back")->getPositionAt(Vec2(x - 100, y));
			position += right->getPosition();
		}
	}
	else
	{
		if ((y / 100) > 0)
		{
			position = left->getLayer("back")->getPositionAt(Vec2(x, y - 100));
			position += left->getPosition();
		}
		else
		{
			position = up->getLayer("back")->getPositionAt(Vec2(x, y));
			position += up->getPosition();
		}
	}
	return position+Point(32,32);
}

Point MyMap::getPositionAtGL(Vec2 pos)
{
	return this->convertToWorldSpace(pos);
}
void MyMap::getPositionAtGL(vector<Vec2>&  path)
{
	for (int i = 0; i < path.size(); i++)
	{
		path[i] = this->getPositionAtMap(path[i]);
	}
}




Vec2 MyMap::staggeredCoordForPosition(Point position)
{
	Point offset(0, 16);
	Point pos = this->convertToNodeSpace(Vec2(position.x, position.y));
	float halfMapWidth = this->getMapSize().width*0.5f;
	float mapHeight = this->getMapSize().height;

	float tileWidth = 64.00;
	float tileheight = 32.00;

	Point tilePosDiv = CCPointMake(pos.x / tileWidth, pos.y / tileheight);
	float inverseTileY = mapHeight - tilePosDiv.y;
	//0.5是显示时的偏移量
	float posX = (int)(inverseTileY + tilePosDiv.x - halfMapWidth + 0.75);
	float posY = (int)(inverseTileY - tilePosDiv.x + halfMapWidth + 0.75);
	if (posY == -1)
	{
		posX = 0;
	}
	pos = CCPointMake(posX, posY);
	return pos;
}
