#include "BaseOfAll.h"

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
Basement::Basement()
{
	SetVisible(false);
	BloodProgress->setType(kCCProgressTimerTypeBar);
	BloodProgress->setMidpoint(ccp(0, 0));
	BloodProgress->setBarChangeRate(ccp(1, 0));            //构造函数并设置血条不可见
	BloodProgress->setPercentage(100.0f);
}
void Basement::SetVisible(bool judge)
{
	BloodBackground->setVisible(judge);                  //设置血条是否可见
	BloodProgress->setVisible(judge);
}
bool Basement::minusBloodAmount(float attack)
{
	if (BloodProgress->getPercentage() - attack <= 0)
	{
		BloodProgress->setPercentage(0);              //受到攻击血量减少
		return false;
	}
	else
	{
		BloodProgress->setPercentage(BloodProgress->getPercentage() - attack);
		return true;
	}
}
void Basement::setPath(const MsgGridPath& _msg_grid_path)
{
	path.clear();
	int grid_point_size = _msg_grid_path.grid_point_size();
	path = GridPath(grid_point_size);
	for (int i = 0; i < grid_point_size; i++)
		path[i] = GridPoint{ _msg_grid_path.grid_point(i).x(), _msg_grid_path.grid_point(i).y() };
}
void Basement::generateGLPath()
{
	Vec2 contentSize = this->getContentSize();
	float area = path[path.size() - 1].x * contentSize.x * contentSize.y * 1.0;
	float equalEdge = sqrt(area) / 5;
	srand((unsigned)time(NULL));
	Vec2 position = tilemap->getPositionAtMap(Vec2(path[path.size() - 2].x, path[path.size() - 2].y));
	Vec2 Position;
	Vec2 TilePosition;
	do
	{
		Position.x = ((float)random() / RAND_MAX - 0.5) * equalEdge + position.x;
		Position.y = ((float)random() / RAND_MAX - 0.5) * equalEdge + position.y;
		TilePosition = tilemap->staggeredCoordForPosition(position);
	} while (TilePosition.x < tilemap->getMapSize().height && TilePosition.y < tilemap->getMapSize().width && TilePosition.x >= 0 && TilePosition.y >= 0 && !gridmap->gmap[int(TilePosition.x)][int(TilePosition.y)]);
	tilemap->getPositionAtGL(path, GLpath);
	GLpath.push_back(Position);
}

GridPath Basement::getpath()
{
	return this->path;
}
void Basement::move()
{
	if (path.size() > 2)
	{
		auto label = this->getTag();
		GLpath.clear();
		generateGLPath();
		cocos2d::Vector<FiniteTimeAction*> pathAction;
		for (int j = 1; j < GLpath.size(); j++)
		{
			pathAction.pushBack(static_cast<FiniteTimeAction*>(MoveTo::create(time(GLpath[j - 1], GLpath[j], label), GLpath[j])));
		}
		auto sequence = Sequence::create(pathAction);
		this->stopAllActions();
		this->runAction(sequence);
	}
}

MyMap* Basement::tilemap = nullptr;

void Basement::setpos(Vec2 position)
{
	this->pos = position;
}
void Basement::setgamemap(MyMap* Gamemap)
{
	this->gamemap = Gamemap;
}
void Basement::setgridmap(GridMap* Gridmap)
{
	this->gridmap = Gridmap;
}
void Basement::setgmap()
{
	for (int i = pos.x; i < pos.x + 2; i++)
	{
		for (int j = pos.y; j < pos.y + 2; j++)
		{
			gridmap->gmap[i][j] = 0;
		}
	}
}
void Basement::resetgmap()
{
	for (int i = pos.x; i < pos.x + 2; i++)
	{
		for (int j = pos.y; j < pos.y + 2; j++)
		{
			gridmap->gmap[i][j] = 1;
		}
	}
}