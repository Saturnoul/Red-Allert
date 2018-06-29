//
//  ResultScene.cpp
//  Red-Allert
//
//  Created by jiang on 2018/6/29.
//

#include "ResultScene.hpp"
USING_NS_CC;

cocos2d::Scene* Win::createScene()
{
    auto scene = Scene::create();
    auto layer = create();
    scene -> addChild(layer);
    return scene;
}

bool Win::init()
{
    if (!Layer::init())
        return false;
    auto visibleSize = Director::getInstance() -> getVisibleSize();
    Vec2 origin = Director::getInstance() -> getVisibleOrigin();

    Sprite * bg = Sprite::create("bg/hu_otherbackground.png");
    bg -> setPosition(Vec2(origin.x + visibleSize.width/2,
                           origin.y + visibleSize.height/2));
    this -> addChild(bg,0);
    std::string HelpContent = "YOU WIN";
    
    label = Label::createWithTTF(HelpContent, "fonts/arial.ttf",50);
    label -> setHorizontalAlignment(TextHAlignment::CENTER);
    label -> setPosition(origin + visibleSize / 2);
    this -> addChild(label,1);
  
    
    MenuItemFont::setFontName("fonts/Marker Felt.ttf");
    MenuItemFont::setFontSize(24);
    auto BackLabel = MenuItemFont::create("EXIT", CC_CALLBACK_1(Win::menuBackCallback, this));
    BackLabel -> setPosition(Vec2(origin.x + visibleSize.width - BackLabel -> getContentSize().width,
                                  origin.y + BackLabel -> getContentSize().height));
    auto menu = Menu::create(BackLabel, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    return true;
    
}

void Win::menuBackCallback(cocos2d::Ref* pSender)
{
    unscheduleUpdate();
    Director::getInstance()->end();
}

cocos2d::Scene* Lose::createScene()
{
    auto scene = Scene::create();
    auto layer = create();
    scene -> addChild(layer);
    return scene;
}

bool Lose::init()
{
    if (!Layer::init())
        return false;
    auto visibleSize = Director::getInstance() -> getVisibleSize();
    Vec2 origin = Director::getInstance() -> getVisibleOrigin();
    
    Sprite * bg = Sprite::create("bg/hu_otherbackground.png");
    bg -> setPosition(Vec2(origin.x + visibleSize.width/2,
                           origin.y + visibleSize.height/2));
    this -> addChild(bg,0);
    std::string HelpContent = "YOU LOSE";
    
    label = Label::createWithTTF(HelpContent, "fonts/arial.ttf",50);
    label -> setHorizontalAlignment(TextHAlignment::CENTER);
    label -> setPosition(origin + visibleSize / 2);
    this -> addChild(label,1);
    
    
    MenuItemFont::setFontName("fonts/Marker Felt.ttf");
    MenuItemFont::setFontSize(24);
    auto BackLabel = MenuItemFont::create("EXIT", CC_CALLBACK_1(Lose::menuBackCallback, this));
    BackLabel -> setPosition(Vec2(origin.x + visibleSize.width - BackLabel -> getContentSize().width,
                                  origin.y + BackLabel -> getContentSize().height));
    auto menu = Menu::create(BackLabel, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    return true;
    
}

void Lose::menuBackCallback(cocos2d::Ref* pSender)
{
    unscheduleUpdate();
    Director::getInstance()->end();
}
