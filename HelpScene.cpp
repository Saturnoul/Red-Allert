//
//  HelpScene.cpp
//  MyGame
//
//  Created by jiang on 2018/6/2.
//

#include "HelpScene.h"
#include "StartScene.h"
USING_NS_CC;

cocos2d::Scene* Help::createScene()
{
    auto scene = Scene::create();
    auto layer = create();
    scene -> addChild(layer);
    return scene;
}

bool Help::init()
{
    if (!Layer::init())
        return false;
    auto visibleSize = Director::getInstance() -> getVisibleSize();
    Vec2 origin = Director::getInstance() -> getVisibleOrigin();
    //CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("audio/mainv.wav",true);
    Sprite * bg = Sprite::create("bg/hu_otherbackground.png");
    bg -> setPosition(Vec2(origin.x + visibleSize.width/2,
                           origin.y + visibleSize.height/2));
    this -> addChild(bg,0);
    std::string HelpContent = "Zhang Xin\n\nFu Qijun\n\nLuo Hao\n\nJiang Qisheng";
    
    label = Label::createWithTTF(HelpContent, "fonts/arial.ttf",40);
    label -> setHorizontalAlignment(TextHAlignment::CENTER);
    label -> setPosition(origin + visibleSize / 2);
    this -> addChild(label,1);
  //  schedule(schedule_selector(Help::scheduleMove));
    
    MenuItemFont::setFontName("fonts/Marker Felt.ttf");
    MenuItemFont::setFontSize(24);
    auto BackLabel = MenuItemFont::create("Back", CC_CALLBACK_1(Help::menuBackCallback, this));
    BackLabel -> setPosition(Vec2(origin.x + visibleSize.width - BackLabel -> getContentSize().width,
                                  origin.y + BackLabel -> getContentSize().height));
    auto menu = Menu::create(BackLabel, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    return true;
    
}

void Help::menuBackCallback(cocos2d::Ref* pSender)
{
    
    //CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic()
    Director::getInstance() -> popScene();
}
/*
void Help::scheduleMove(float f)
{
    label -> setPosition(label -> getPosition() + Vec2(0, 0.7));
    if (label -> getPosition().y - label -> getContentSize().height/2> Director::getInstance()->getVisibleSize().height)
        label->setPosition(Vec2(Director::getInstance() -> getVisibleSize().width/2,
                                label -> getContentSize().height/2));
}
*/
