#include "AppDelegate.h"
#include "logicHeader.h"
#include "GUIHeader.h"
#include "PayClient.h"

USING_NS_CC;
extern CDataManager* DataMgr;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        //glview = GLViewImpl::createWithRect("EarthExplore", Rect(0, 0, 480, 800));
#ifdef WIN32  
        glview = GLViewImpl::createWithRect("EarthExplore", cocos2d::Rect(0,0,480,800));  
#else  
        glview = GLViewImpl::create("EarthExplore");  
#endif // WIN32  
        director->setOpenGLView(glview);
        glview->setDesignResolutionSize(480, 800, ResolutionPolicy::EXACT_FIT );
    }

    director->getOpenGLView()->setDesignResolutionSize(480, 800, ResolutionPolicy::SHOW_ALL);

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    FileUtils::getInstance()->addSearchPath("res");
    FileUtils::getInstance()->addSearchPath("Image");

    // create a scene. it's an autorelease object
	PayClient::GetInstance()->Init();
    auto scene = StartScene::createScene();

    // run
    director->runWithScene(scene);
    GlobalSchedule::startSchedule(0.01f, 0.0f);
    DataMgr->LoadConfig();
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
