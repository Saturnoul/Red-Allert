#ifndef _MOVE_CONTROL_
#define _MOVE_CONTROL_


#include "cocos2d.h"
#include "pathFinding.h"
#include<vector>
#include "BuildingAndSodier.h"
#include "SimpleAudioEngine.h"
using namespace cocos2d;
using namespace std;
using namespace CocosDenshion;

extern Vector<Basement*>allSprites;
static Vec2 startPoint;
static Vector<Basement*>selectedSprites;                //the sprites being selected 
static vector<Vec2>positionList;               //the ending positions for every sprite
static bool isSelecting = false;               //judge if the mouse is selecting a sprite
static bool isDown = false;                    //judge if the mouse is clicked down
static bool isMoved = false;                   //judge if the cursor is draged
static bool isOperating = false;            //judge if there are some sprites already being selected
//static vector<vector<int> >  positionCondition;    // store if some position is walkable


//judge if a tile is walkable
//bool isCollidable(Vec2 position, TMXTiledMap* _tileMap, TMXLayer* _collidable);

//generate a matrix to store if every tile is walkable
//void generatePositionCondition(TMXTiledMap* _tileMap);

float time(Vec2 P1, Vec2 P2, int label);



// judge if the player wants to cancel previous selection 
void isCancelSelection(Vec2 position, EventMouse::MouseButton button);


//judge if a sprite is in the rectangle area
bool isInRange(Vec2 aimposition, Vec2 startposition, Vec2 endposition);


//draw a rectangle as the cursor moving
void drawRec(Vec2 position, Layer* layer);

//select all the sprites in the rectangle
void selectInRec(Vec2 position, MyMap* gamemap);

//selected the sprite being clicked
void singleSelect(Vec2 position, MyMap* gamemap);

//move the sprites being selected with the path generated
void Move(Vec2 position, MyMap* _tileMap, GridMap* gmap);

//set the flags to default value
void setDefault(Layer* layer);

//judge if basement is movable
bool isMovable(Basement* basement);

void cancleselected();

#endif //3_MOVE_CONTROL_