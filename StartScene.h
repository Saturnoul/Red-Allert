#ifndef __Start_SCENE_H__
#define __Start_SCENE_H__

#include "cocos2d.h"


class Start : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    //callback
    void menuCloseCallback(Ref* pSender);
    void menuHelpCallback(cocos2d::Ref* pSender);
    void menuStartCallback(cocos2d::Ref* pSender);
    //void menuSettingCallback(cocos2d::Ref* pSender);
    // implement the "static create()" method manually
    CREATE_FUNC(Start);
};

#endif // __Start_SCENE_H__
