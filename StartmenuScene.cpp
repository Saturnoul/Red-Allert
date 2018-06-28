//
//  StartmenuScene.cpp
//  MyGame
//
//  Created by jiang on 2018/6/2.
//

#include "StartmenuScene.h"
USING_NS_CC;


cocos2d::Scene* StartMenu::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = StartMenu::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool StartMenu::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Sprite* background = Sprite::create("bg/hu_otherbackground.png");
    background -> setPosition(origin + visibleSize / 2);
    background -> setScaleX(visibleSize.width / background->getContentSize().width);
    background -> setScaleY(visibleSize.height / background->getContentSize().height);
    addChild(background, -5);
    
    auto server_label = MenuItemImage::create("Botton/ButtonStartAsServer.png",
                                              "Botton/ButtonStartAsServerSelected.png",
                                              CC_CALLBACK_1(StartMenu::menuServerCallback, this));
    auto client_label = MenuItemImage::create("Botton/ButtonStartAsClient.png",
                                              "Botton/ButtonStartAsClientSelected.png",
                                              CC_CALLBACK_1(StartMenu::menuClientCallback, this));
    auto back_label = MenuItemImage::create("Botton/ButtonBack.png",
                                            "Botton/ButtonBackSelected.png",
                                            CC_CALLBACK_1(StartMenu::menuBackCallback, this));
    
    auto menu = Menu::create(server_label, client_label,back_label, NULL);
    menu->setPosition(Vec2(origin.x + visibleSize.width / 2,
                           origin.y + visibleSize.height / 2));
    
    menu->alignItemsVerticallyWithPadding(10);
    this->addChild(menu, 1);
    
    return true;
}

void StartMenu::menuServerCallback(cocos2d::Ref* pSender)
{
    log("server button pressed");
    auto scene = ServerMenu::createScene();
    Director::getInstance()->replaceScene(TransitionSplitCols::create(0.5,scene));
}

void StartMenu::menuClientCallback(cocos2d::Ref* pSender)
{
    auto scene = ClientMenu::createScene();
    Director::getInstance()->replaceScene(TransitionSplitCols::create(0.5, scene));
}

void StartMenu::menuBackCallback(cocos2d::Ref* pSender)
{
    auto scene = Start::createScene();
    Director::getInstance()->replaceScene(TransitionSplitCols::create(0.5, scene));
}

//create server menu
cocos2d::Scene* ServerMenu::createScene()
{
    auto scene = Scene::create();
    auto layer = ServerMenu::create();
    scene->addChild(layer);
    return scene;
}

bool ServerMenu::init()
{
    if (!Layer::init())
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto inputbox = ui::EditBox::create(Size(80, 60), ui::Scale9Sprite::create("Botton/InputPort.png"));
    inputbox->setPosition(Vec2(origin.x + visibleSize.width / 2,
                               origin.y + visibleSize.height-inputbox->getContentSize().height));
    inputbox->setTextHorizontalAlignment(TextHAlignment::CENTER);
    inputbox->setMaxLength(7);
    inputbox->setFontColor(Color3B::WHITE);
    inputbox->setFontName("fonts/arial.TTF");
    inputbox->setFontSize(20);
    inputbox->setText("8008");
    inputbox->setInputMode(ui::EditBox::InputMode::NUMERIC);
    inputbox->setDelegate(this);
    
    this->addChild(inputbox, 1);
    
    auto background = Sprite::create("bg/hu_otherbackground.png");
    background->setPosition(origin + visibleSize / 2);
    background->setScaleX(visibleSize.width / background->getContentSize().width);
    background->setScaleY(visibleSize.height / background->getContentSize().height);
    addChild(background, -5);
    
    auto start_label = MenuItemImage::create("Botton/ButtonStartServer.png",
                                             "Botton/ButtonStartServerSelected.png",
                                             CC_CALLBACK_1(ServerMenu::menuStartServerCallback, this));
    auto start_game_label = MenuItemImage::create("Botton/ButtonStartGame.png",
                                                  "Botton/ButtonStartGameSelected.png",
                                                  CC_CALLBACK_1(ServerMenu::menuStartGameCallback, this));
    auto back_label = MenuItemImage::create("Botton/ButtonBack.png",
                                            "Botton/ButtonBackSelected.png",
                                            CC_CALLBACK_1(ServerMenu::menuBackCallback, this));
    
    auto menu = Menu::create(start_label, start_game_label,back_label, NULL);
    menu->setPosition(Vec2(origin.x + visibleSize.width / 2,
                           origin.y + visibleSize.height / 2));
    menu->alignItemsVerticallyWithPadding(10);
    
    connection_msg_ = Label::createWithTTF("", "fonts/arial.ttf", 18);
    connection_msg_->setAnchorPoint(Vec2(0.5, 0));
    connection_msg_->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                      origin.y));
    addChild(connection_msg_);
    this->addChild(menu, 1);
    return true;
}

void ServerMenu::menuStartServerCallback(cocos2d::Ref* pSender)
{

    if (!socket_server_)
    {
        socket_server_ = SocketServer::create();
        socket_client_ = SocketClient::create("192.168.0.11",8008);
        log("create server and client on 8008");
        schedule(schedule_selector(ServerMenu::connectionSchdeule), 0.1);//鍒锋柊鏄剧ず杩炴帴鏁伴噺
    }
    log("create server and client on 8008");
    
}

void ServerMenu::menuStartGameCallback(cocos2d::Ref* pSender)
{
    
    if (socket_server_)
    {

        socket_server_ -> button_start();
		auto scene =HelloWorld::createScene(socket_server_,socket_client_);
        Director::getInstance() -> replaceScene( scene);
        log("start game");
    }
     
      log("start game");
}

void ServerMenu::menuBackCallback(cocos2d::Ref* pSender)
{
    if (socket_server_)
    {
        unscheduleAllCallbacks();
        socket_client_ -> close();
        delete socket_client_;
        socket_client_ = nullptr;
       // std::this_thread::sleep_for(std::chrono::milliseconds(200));
        socket_server_ -> close();
        delete socket_server_;
        socket_server_  = nullptr;
    }
    
    auto scene = StartMenu::createScene();
    Director::getInstance() -> replaceScene(scene);
}

void ServerMenu::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
    log("%s", editBox->getText());
    int port = atoi(editBox->getText());//change string to int
    
}

void ServerMenu::connectionSchdeule(float f)
{
    if (socket_server_ -> connection_num())
        connection_msg_ -> setString("Total connection num: " + std::to_string(socket_server_ -> connection_num()));
    else
        connection_msg_ -> setString("Port already used, please change another one");
     
    log("connection");
}

//create client menu
cocos2d::Scene* ClientMenu::createScene()
{
    auto scene = Scene::create();
    auto layer = create();
    scene->addChild(layer);
    return scene;
}

bool ClientMenu::init()
{
    if (!Layer::init())
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto ip_box = ui::EditBox::create(Size(150, 60), ui::Scale9Sprite::create("Botton/InputIP.png"));
    ip_box->setPosition(Vec2(origin.x + visibleSize.width / 2-ip_box->getContentSize().width/2,
                             origin.y + visibleSize.height - ip_box->getContentSize().height));
    ip_box->setTextHorizontalAlignment(TextHAlignment::CENTER);
    ip_box->setFontName("fonts/arial.TTF");
    ip_box->setFontSize(20);
    ip_box->setMaxLength(20);
    ip_box->setFontColor(Color3B::WHITE);
    ip_box->setText("127.0.0.1");
    ip_box->setTag(1);
    
    auto port_box = ui::EditBox::create(Size(80, 60), ui::Scale9Sprite::create("Botton/InputPort.png"));
    port_box->setPosition(Vec2(origin.x + visibleSize.width / 2 + port_box->getContentSize().width,
                               origin.y + visibleSize.height - port_box->getContentSize().height));
    port_box->setTextHorizontalAlignment(TextHAlignment::CENTER);
    port_box->setFontName("fonts/arial.TTF");
    port_box->setFontSize(20);
    port_box->setMaxLength(20);
    port_box->setFontColor(Color3B::WHITE);
    port_box->setText("8008");
    //    inputbox->setPlaceHolder("8008");
    port_box->setInputMode(ui::EditBox::InputMode::NUMERIC);
    //    portbox->setDelegate(this);
    port_box->setTag(2);
    
    this->addChild(ip_box, 1);
    this->addChild(port_box, 1);
    
    auto background = Sprite::create("bg/hu_otherbackground.png");
    background->setPosition(origin + visibleSize / 2);
    background->setScaleX(visibleSize.width / background->getContentSize().width);
    background->setScaleY(visibleSize.height / background->getContentSize().height);
    addChild(background, -5);
    
    auto start_label = MenuItemImage::create("Botton/ButtonJoinGame.png",
                                             "Botton/ButtonJoinGameSelected.png",
                                             CC_CALLBACK_1(ClientMenu::menuStartGameCallback, this));
    auto back_label = MenuItemImage::create("Botton/ButtonBack.png",
                                            "Botton/ButtonBackSelected.png",
                                            CC_CALLBACK_1(ClientMenu::menuBackCallback, this));
    auto menu = Menu::create(start_label, back_label, NULL);
    menu->setPosition(Vec2(origin.x + visibleSize.width / 2,
                           origin.y + visibleSize.height / 2));
    
    menu->alignItemsVerticallyWithPadding(40);
    
    connection_msg_ = Label::createWithTTF("","fonts/arial.ttf",18);
    connection_msg_->setAnchorPoint(Vec2(0.5, 0));
    connection_msg_->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                      origin.y));
    addChild(connection_msg_);
    
    this->addChild(menu, 1);
    
    return true;
}

void ClientMenu::menuStartGameCallback(cocos2d::Ref* pSender)
{
    if (!socket_client_)
    {
        auto ip_box = static_cast<ui::EditBox*>(getChildByTag(1));
        std::string ip = ip_box -> getText();
        auto port_box = static_cast<ui::EditBox*>(getChildByTag(2));
        int port = atoi(port_box -> getText());
        log("ip:%s, port:%d", ip.c_str(), port);
        socket_client_ = SocketClient::create(ip, port);
        schedule(schedule_selector(ClientMenu::startSchedule), 0.1);

    }
    
    log("star game");
}

void ClientMenu::menuBackCallback(cocos2d::Ref* pSender)
{
    if (socket_client_)
    {
        unscheduleAllSelectors();
        socket_client_->close();
        delete socket_client_;
        socket_client_ = nullptr;
    }
    auto scene = StartMenu::createScene();
    Director::getInstance()->replaceScene(TransitionSplitCols::create(0.5, scene));
    
}

void ClientMenu::startSchedule(float f)
{
    if (socket_client_ -> error())
    {
        unscheduleAllCallbacks();
        socket_client_->close();
        delete socket_client_;
        socket_client_ = nullptr;
        connection_msg_->setString("Cannot connect to the server, please try again");
        return;
    }
    switch((timer_++ % 32)/4)
    {
        case 0: connection_msg_->setString("Connected, wait for server"); break;
        case 1: connection_msg_->setString("Connected, wait for server."); break;
        case 2: connection_msg_->setString("Connected, wait for server.."); break;
        case 3: connection_msg_->setString("Connected, wait for server..."); break;
        default: break;
    }
    if (socket_client_ -> started())
        wait_start();
    
    log("start schedule");
}
 
 void ClientMenu::wait_start()
{
    unscheduleAllCallbacks();
    log("get the camp");
    log("start game");

	auto scene = HelloWorld::createScene(nullptr,socket_client_);
    Director::getInstance()->replaceScene(scene);
}

