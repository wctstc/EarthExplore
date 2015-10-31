#ifndef __MAINSCENE_H__
#define __MAINSCENE_H__

#include "cocos2d.h"

class MainScene : public cocos2d::Layer
{
public:
  bool _bUpdateList;
  class ui::Text* Text_Score;
  class ui::Text* Text_Level;
  class ui::Text* Text_Bullet;
  class ui::ListView* _Listview;
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    void NewGamecallback(cocos2d::Ref* pSender);
    void selectedItemEvent(Ref*, ui::ListViewEventType);
    void RefreshScore(int Score);
    void RefreshLevel(int Level);
    void RefreshBullet(int Bullet);
    void RefreshListView();
    void Update(float delta);
    // implement the "static create()" method manually
    CREATE_FUNC(MainScene);
};

#endif // __HELLOWORLD_SCENE_H__
