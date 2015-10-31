#pragma once

#include "cocos2d.h"

class ShopScene : public cocos2d::Layer
{
private:
  class ui::ListView* _Listview;
public:
  void RefreshListView();
  void selectedItemEvent(Ref*, ui::ListViewEventType);
  void BackStartcallback(cocos2d::Ref* pSender);
  void HideOtherImage(int Index = 0);
public:
  static cocos2d::Scene* createScene();
  virtual bool init();
  CREATE_FUNC(ShopScene);
};