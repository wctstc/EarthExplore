#include "logicHeader.h"
#include "GUIHeader.h"
#include "PayClient.h"

USING_NS_CC;
using namespace cocos2d::ui;

Scene* ShopScene::createScene()
{
  // 'scene' is an autorelease object
  auto scene = Scene::create();

  // 'layer' is an autorelease object
  auto layer = ShopScene::create();

  // add layer as a child to scene
  scene->addChild(layer);

  // return the scene
  return scene;
}

bool ShopScene::init()
{
  if ( !Layer::init() )
    return false;

  Size visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();

  //背景色
  Sprite* Sprite_Back = Sprite::create("bg_mainscene.png");
  Sprite_Back->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
  addChild(Sprite_Back);

  Button* btnShop = Button::create("su_backmenubtn.png");
  btnShop->setPosition(Vec2(400.f, 40.f));
  btnShop->addClickEventListener(CC_CALLBACK_1(ShopScene::BackStartcallback, this));
  addChild(btnShop);

  //listview
  Layout* layout = Layout::create();
  addChild(layout);
  _Listview = ListView::create();
  //VERTICAL  HORIZONTAL
  _Listview->setDirection(ScrollView::Direction::VERTICAL);
  _Listview->setTouchEnabled(true);
  _Listview->setBounceEnabled(true);
  _Listview->setBackGroundImage("list_bk.png");
  _Listview->setBackGroundImageScale9Enabled(true);
  float left = 25.0f;
  float top = 50.0f;
  _Listview->setPosition(Vec2(left, 2*top));
  _Listview->setContentSize(Size(visibleSize.width-left*2, visibleSize.height-top*4));
  _Listview->addEventListenerListView(this, listvieweventselector(ShopScene::selectedItemEvent));
  layout->addChild(_Listview);
  RefreshListView();
  return true;
}

void ShopScene::RefreshListView()
{
  Size ListSize = _Listview->getContentSize();
  const TArray<ShopData>& ShopItems = DataMgr->Shop.GetShopItemData();
  int Num = ShopItems.Num();
  for (int i = 0; i < Num; ++i) 
  {
    ItemProperty* Property = DataMgr->GetItemProperty(ShopItems(i).Id);
    if (!Property)
      continue;
    Sprite* ItemSprite = Sprite::create(Property->Pic);
    ItemSprite->setPosition(Vec2(50.f, 50.0f));
    ImageView* BkSprite = ImageView::create("bg_shopitem.png");
    BkSprite->setTouchEnabled(true);
    Size Bksize = BkSprite->getContentSize();
    BkSprite->setPosition(Vec2(ListSize.width/2,Bksize.height/2));
    BkSprite->setContentSize(Size(ListSize.width, Bksize.height));
    ImageView* Hilight = ImageView::create("bg_mostvalue.png");
    Hilight->setPosition(Vec2(ListSize.width/2,Bksize.height/2));
    Hilight->setContentSize(Size(ListSize.width, Bksize.height));
    Hilight->setVisible(false);
    Button* BtnBuy = Button::create("btn_buy.png");
    Size BtnSize = BtnBuy->getContentSize();
    BtnBuy->setPosition(Vec2(ListSize.width-BtnSize.width+BtnSize.width/4,BtnSize.height-10));
    BtnBuy->setTitleText( StringUtils::format(DataMgr->GetGameString(5), ShopItems(i).money));
    BtnBuy->setTitleFontSize(25);
    Layout* ItemLayout = Layout::create();
    ItemLayout->setContentSize(Size(Bksize.width, Bksize.height+10));
    ItemLayout->addChild(BkSprite, 0, 0);
    ItemLayout->addChild(Hilight, 0, 1);
    ItemLayout->addChild(BtnBuy, 0, 2);
    ItemLayout->addChild(ItemSprite, 0, 3);
    _Listview->pushBackCustomItem(ItemLayout);
  }
}

void ShopScene::selectedItemEvent( Ref* ref, ListViewEventType EventType)
{
  switch (EventType) {
  case LISTVIEW_ONSELECTEDITEM_START:
    {
    }
    break;
  case LISTVIEW_ONSELECTEDITEM_END:
    {
      int Index = _Listview->getCurSelectedIndex();
	  Layout* layout = static_cast<Layout*>(_Listview->getItem(Index));
	  ImageView* BkView = static_cast<ImageView*>(layout->getChildByTag(0));
	  Button* BtnBuy = static_cast<Button*>(layout->getChildByTag(2));
      if (BkView == ref)
      {
        ImageView* Hilight = static_cast<ImageView*>(layout->getChildByTag(1));
        HideOtherImage(Index);
        Hilight->setVisible(true);
      }
	  else if( BtnBuy == ref )
	  {
		  m_price = DataMgr->Shop.GetShopItemData()(Index).money;

		  DataMgr->GetItemProperty( Index );
		  m_index = Index;

		  PayClient::GetInstance()->Pay( m_price, "道具","道具充值" );
		  schedule(schedule_selector(ShopScene::Check), 1.0f);
	  }
    }
    break;
  default:
    break;
  }
}

void ShopScene::BackStartcallback( cocos2d::Ref* pSender )
{
  TransitionScene * reScene = NULL;
  Scene * s = StartScene::createScene();
  float t = 1.2f;
  reScene = CCTransitionProgressVertical::create(t, s);
  Director::getInstance()->replaceScene(reScene);
}

void ShopScene::HideOtherImage(int Index)
{
  int Num = DataMgr->Shop.GetShopItemData().Num();
  for (int i = 0; i < Num; ++i) 
  {
    if (Index==i)
      continue;
    Layout* layout = static_cast<Layout*>(_Listview->getItem(i));
    ImageView* Hilight = static_cast<ImageView*>(layout->getChildByTag(1));
    Hilight->setVisible(false);
  }
}

void ShopScene::Check(float dt)
{
	string result;
	PayClient::GetInstance()->Check(result);
	if( !result.empty() )
	{
		Size size = getContentSize();
		string content;
		if( result == "succeed" )
		{
			const TArray<ShopData>& ShopItems = DataMgr->Shop.GetShopItemData();

			//ShopItems(m_index)
			content = "充值成功";
		}
		else
		{
			content = "充值失败";
		}

		CCLabelTTF* _label = CCLabelTTF::create( content.c_str(),"", 30);  
		_label->setPosition( size.width/2, size.height/2 );   
		addChild(_label);
		_label->runAction(CCSequence::create(CCFadeOut::create(1),CCRemoveSelf::create(true),NULL)); 
		unschedule(schedule_selector(ShopScene::Check));
	}
}
