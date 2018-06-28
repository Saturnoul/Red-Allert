#include "StartScene.h"
#include "SimpleAudioEngine.h"
#include "SettingScene.h"
#include "HelpScene.h"
#include "StartmenuScene.h"
//#include "SocketServer.h"
//#include "SocketClient.h"
#include "ui/UIEditBox/UIEditBox.h"
USING_NS_CC;

Scene* Start::createScene()
{
    return Start::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in StartScene.cpp\n");
}

// on "init" you need to initialize your instance
bool Start::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    //background
    Sprite * bg = Sprite::create("bg/background.png");
    bg -> setPosition(Vec2(origin.x + visibleSize.width/2,
                           origin.y + visibleSize.height/2));
    this -> addChild(bg,-5);

    /////////////////////////////
    // 2. add a menu item
    //startmenu
    auto startMenu = MenuItemImage::create("Botton/BottonStart.png",
                                           "Botton/ButtonStartSelected.png",
                                           CC_CALLBACK_1(Start::menuStartCallback, this));
     /*
     //settingmenu
    auto settingMenu = MenuItemImage::create("Botton/BottonSetting.png",
                                             "Botton/BottonSettingSelected.png",
                                             CC_CALLBACK_1(Start::menuSettingCallback, this));
     */
    //helpmenu
    auto helpMenu = MenuItemImage::create("Botton/BottonHelp.png",
                                          "Botton/BottonHelpSelected.png",
                                          CC_CALLBACK_1(Start::menuHelpCallback, this));
    //closemenu
    auto closeMenu = MenuItemImage::create("Botton/BottonClose.png",
                                           "Botton/BottonCloseSelected.png",
                                           CC_CALLBACK_1(Start::menuCloseCallback, this));
    // create menu, it's an autorelease object
    auto mainMenu = Menu::create(startMenu,helpMenu,closeMenu, NULL);
    mainMenu -> alignItemsVertically();
    mainMenu->setPosition(Vec2(origin.x + visibleSize.width/4*3,
                               origin.y + visibleSize.height/5*2));
    this->addChild(mainMenu, 2);

   //add soldier
    auto * bgsoldier = Sprite::create("bg/bgpeopel.png");
    bgsoldier -> setAnchorPoint(Vec2(0, 0));
    bgsoldier -> setPosition(origin);
    RepeatForever * bgsoiderAct = RepeatForever::create(Blink::create(1, 3));
    bgsoldier -> runAction(bgsoiderAct);
    this -> addChild(bgsoldier,1);
    
    //add smog
    auto * bgsmog  = Sprite::create("bg/bgsmog.png");
    bgsmog -> setPosition(Vec2(origin.x + visibleSize.width/2,
                           origin.y + visibleSize.height/2));
    FiniteTimeAction * bgsmogIn = (FiniteTimeAction *)bgsmog -> runAction(FadeIn::create(2));
    FiniteTimeAction * bgsmogOut = (FiniteTimeAction *)bgsmog -> runAction(FadeOut::create(2));
    ActionInterval * bgsmogseq = Sequence::create(bgsmogIn,bgsmogOut, NULL);
    bgsmog -> runAction(RepeatForever::create(bgsmogseq));
    this -> addChild(bgsmog,1);
    return true;
}

void Start::menuCloseCallback(Ref* pSender)
{
    unscheduleUpdate();
    Director::getInstance()->end();
}

void Start::menuHelpCallback(cocos2d::Ref* pSender)
{
    auto scene = Help::createScene();
    Director::getInstance()->pushScene(TransitionSplitCols::create(0.5, scene));
}

void Start::menuStartCallback(cocos2d::Ref* pSender)
{
    auto scene = StartMenu::createScene();
    Director::getInstance()->replaceScene(TransitionSplitCols::create(0.5, scene));
}
/*
void Start::menuSettingCallback(cocos2d::Ref* pSender)
{
    auto scene = Setting::createScene();
    Director::getInstance()->pushScene(TransitionSplitCols::create(0.5, scene));
}
*/
