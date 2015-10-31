#include "logicHeader.h"
#include "GUIHeader.h"

USING_NS_CC;
using namespace cocos2d::ui;

Scene* MainScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    GameMgr->_MainScene = this;
    _bUpdateList = false;
    schedule(schedule_selector(MainScene::Update));
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Sprite* Sprite_Back = Sprite::create("bg_mainscene.png");
    Sprite_Back->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    addChild(Sprite_Back);

    Text_Score = Text::create("0", "楷体", 30);
    Text_Score->setPosition(Vec2(220, 760));
    addChild(Text_Score);
    RefreshScore(GSelfCtl->_PlayerData._Score);

    Text_Level = Text::create("0", "楷体", 30);
    Text_Level->setPosition(Vec2(60, 760));
    addChild(Text_Level);
    RefreshLevel(GameMgr->_Level);

    Text_Bullet = Text::create("0", "楷体", 30);
    Text_Bullet->setPosition(Vec2(410, 760));
    addChild(Text_Bullet);
    RefreshBullet(GSelfCtl->_PlayerData._BulletNum);

    //物品栏
    Layout* layout = Layout::create();
    addChild(layout);
    _Listview = ListView::create();
    //SCROLLVIEW_DIR_VERTICAL  SCROLLVIEW_DIR_HORIZONTAL
    _Listview->setDirection(ScrollView::Direction::HORIZONTAL);
    _Listview->setTouchEnabled(true);
    _Listview->setBounceEnabled(true);
    _Listview->setBackGroundImage("list_bk.png");
    _Listview->setBackGroundImageScale9Enabled(true);
    _Listview->setContentSize(Size(visibleSize.width, 60));
    _Listview->setPosition(Vec2(0, 0.f));
    _Listview->addEventListenerListView(this, listvieweventselector(MainScene::selectedItemEvent));
    layout->addChild(_Listview);
    RefreshListView();

    Button* btnNew = Button::create("BlockStone.png");
    btnNew->setTitleText(DataMgr->GameString.getString(2));
    btnNew->addClickEventListener(CC_CALLBACK_1(MainScene::NewGamecallback, this));
    btnNew->setPosition(Vec2(100, 700));
    addChild(btnNew);
    //背景砖块
    CStarMatrix* StarMatrix = CStarMatrix::create("Matric.png");
    StarMatrix->initMap();
    StarMatrix->initSprite();
    StarMatrix->setPosition(Vec2(StartX, StartY));
    StarMatrix->setAnchorPoint(Vec2(0, 0));
    addChild(StarMatrix);
    GameMgr->_StarMatrix = StarMatrix;

    //英雄
    CPlayerSprite* PlayerSprite = CPlayerSprite::Create("Hero.png");
    addChild(PlayerSprite);

    //GameMgr->GetReward();
    GameMgr->_Hero = PlayerSprite;

    return true;
}


//测试按钮，随机开始新一局游戏
void MainScene::NewGamecallback( cocos2d::Ref* pSender )
{
#ifdef _DEBUG
  GameMgr->NewGame();
  RefreshScore(0);
  return;
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
  MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
  return;
#endif

  Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
  exit(0);
#endif
}

void MainScene::RefreshScore(int Score)
{
  int Socre = GSelfCtl->GetSocre();
  char Str[32] = "0";
  sprintf(Str, DataMgr->GameString.getString(1).c_str(), Socre);
  Text_Score->setString(Str);
}

void MainScene::RefreshLevel( int Level )
{
  char Str[32] = "0";
  sprintf(Str, DataMgr->GameString.getString(3).c_str(), Level);
  Text_Level->setString(Str);
}

void MainScene::RefreshBullet( int Bullet )
{
  char Str[32] = "0";
  sprintf(Str, DataMgr->GameString.getString(4).c_str(), Bullet);
  Text_Bullet->setString(Str);
}

void MainScene::selectedItemEvent( Ref* ref,ListViewEventType EventType)
{
  switch (EventType) {
  case LISTVIEW_ONSELECTEDITEM_START:
    {
    }
    break;
  case LISTVIEW_ONSELECTEDITEM_END:
    {
      ListView* listView = static_cast<ListView*>(ref);
      int Index = listView->getCurSelectedIndex();
      GSelfCtl->UseItem(Index);
      _bUpdateList = true;
    }
    break;
  default:
    break;
  }
}

void MainScene::RefreshListView()
{
  _Listview->removeAllItems();
  size_t size = GSelfCtl->_Items.size();
  for (int i = 0; i < size; ++i) 
  {
    ItemProperty* Property = DataMgr->GetItemProperty(GSelfCtl->_Items[i].ItemId);
    if (!Property)
      continue;
    Button* custom_button = Button::create(Property->Pic);
    custom_button->setTitleText( StringUtils::format("%d",GSelfCtl->_Items[i].ItemNum));
    //custom_button->setScale9Enabled(true);
    //custom_button->setSize(default_button->getSize());
    Layout* custom_item = Layout::create();
    custom_item->setContentSize(custom_button->getContentSize());
    custom_button->setPosition(Point(custom_item->getContentSize().width / 2.0f, custom_item->getContentSize().height / 2.0f));
    custom_item->addChild(custom_button);
    _Listview->pushBackCustomItem(custom_item);
  }
}

void MainScene::Update( float delta )
{
  if (_bUpdateList)
  {
    _bUpdateList = false;
    RefreshListView();
  }
}