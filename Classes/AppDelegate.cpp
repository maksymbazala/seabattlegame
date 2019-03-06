#include "AppDelegate.h"

USING_NS_CC;

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate(){}

bool AppDelegate::applicationDidFinishLaunching()
{
    // Initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::createWithFullScreen("Sea Battle");
        director->setOpenGLView(glview);
    }
    
    // Set window icon with best resolution (windows and linux)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
    // assert error when using listFilesRecursively on Windows 10
    // refer to: https://github.com/cocos2d/cocos2d-x/issues/18835
    // don't use listFiles as workaround (it will return directory itself)
    // refer to: https://github.com/cocos2d/cocos2d-x/issues/18834
    std::vector<std::string> icons;
    FileUtils::getInstance()->listFilesRecursively("icons", &icons);
#else
    std::string icons = "icons/Icon-60@3x.png";
#endif /* (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX) */
    auto visibleSize = Director::getInstance()->getVisibleSize();
    glview->setDesignResolutionSize(visibleSize.width, visibleSize.height, ResolutionPolicy::NO_BORDER);
    director->setOpenGLView(glview);
    
    // Create a scene. it's an autorelease object
    auto scene = LogoScene::createScene();
    
    // Run
    director->runWithScene(scene);
    
    //Background music and sound effects preload
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("Sounds/bg_music.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("Sounds/victory_music.mp3");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Sounds/simple_click.wav");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Sounds/miss.wav");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Sounds/hit.wav");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("Sounds/destroyed.wav");
    
    //Background music and sound effects setup
    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.1);
    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0.3);
    
    //Playing background music on game start
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Sounds/bg_music.mp3", true);

    return true;
}

void AppDelegate::applicationDidEnterBackground() {}

void AppDelegate::applicationWillEnterForeground() {}
