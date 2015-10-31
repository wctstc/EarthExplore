#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class StartScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    virtual bool init();
    //°´Å¥ÊÂ¼þ
    void menuNewStartCallback(cocos2d::Ref* pSender);
    void menuShopCallback(cocos2d::Ref* pSender);
    CREATE_FUNC(StartScene);
};

#endif // __HELLOWORLD_SCENE_H__
