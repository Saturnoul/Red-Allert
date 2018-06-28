#pragma once
#pragma once
//
//  HelpScene.h
//  MyGame
//
//  Created by jiang on 2018/6/2.
//

#ifndef HelpScene_h
#define HelpScene_h

#include "cocos2d.h"

class Help : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void menuBackCallback(cocos2d::Ref* pSender);
	// void scheduleMove(float f);

	// implement the "static create()" method manually
	CREATE_FUNC(Help);
private:
	cocos2d::Label *label;
};

#endif /* HelpScene_h */
