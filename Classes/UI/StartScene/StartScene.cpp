#include "logicHeader.h"
#include "GUIHeader.h"

USING_NS_CC;
using namespace cocos2d::ui;

Scene* StartScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = StartScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool StartScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Sprite* Sprite_Back = Sprite::create("bg_menuscene.png");
    Sprite_Back->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    addChild(Sprite_Back);

    Button* btnNewGame = Button::create("menu_start.png");
    btnNewGame->setPosition(Vec2(visibleSize.width/2 + origin.x, 480));
    btnNewGame->addClickEventListener(CC_CALLBACK_1(StartScene::menuNewStartCallback, this));
    addChild(btnNewGame);
    Button* btnConGame = Button::create("menu_continue.png");
    btnConGame->setPosition(Vec2(visibleSize.width/2 + origin.x, 360));
    addChild(btnConGame);
    Button* btnShop = Button::create("menu_shop.png");
    btnShop->setPosition(Vec2(visibleSize.width/2 + origin.x, 240));
    btnShop->addClickEventListener(CC_CALLBACK_1(StartScene::menuShopCallback, this));
    addChild(btnShop);

    return true;
}

void StartScene::menuNewStartCallback( cocos2d::Ref* pSender)
{
  TransitionScene * reScene = NULL;
  Scene * s = MainScene::createScene();
  float t = 1.2f;
  reScene = CCTransitionProgressHorizontal ::create(t,s);
  Director::getInstance()->replaceScene(reScene);
}

void StartScene::menuShopCallback( cocos2d::Ref* pSender )
{
  TransitionScene * reScene = NULL;
  Scene * s = ShopScene::createScene();
  float t = 1.2f;
  reScene = CCTransitionProgressVertical::create(t, s);
  Director::getInstance()->replaceScene(reScene);
}
