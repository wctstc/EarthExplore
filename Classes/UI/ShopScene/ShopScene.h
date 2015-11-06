#pragma once

#include "cocos2d.h"
#include "GUIHeader.h"

class ShopScene : public cocos2d::Layer
{
private:
  class ui::ListView* _Listview;
public:
  void RefreshListView();
  void selectedItemEvent(Ref*, ui::ListViewEventType);
  void BackStartcallback(cocos2d::Ref* pSender);
  void HideOtherImage(int Index = 0);
  void Check( float dt );
public:
  static cocos2d::Scene* createScene();
  virtual bool init();
  CREATE_FUNC(ShopScene);
  int m_price;
  int m_index;
};