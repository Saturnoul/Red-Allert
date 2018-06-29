//
//  ResultScene.hpp
//  Red-Allert
//
//  Created by jiang on 2018/6/29.
//

#ifndef ResultScene_hpp
#define ResultScene_hpp

#include <stdio.h>
#include "cocos2d.h"

class Win : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    void menuBackCallback(cocos2d::Ref* pSender);
    
    CREATE_FUNC(Win);
private:
    cocos2d::Label *label;
};

class Lose : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    void menuBackCallback(cocos2d::Ref* pSender);
    
    CREATE_FUNC(Lose);
private:
    cocos2d::Label *label;
};
#endif /* ResultScene_hpp */
