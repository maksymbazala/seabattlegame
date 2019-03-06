#include "AppDelegate.h"

//USING_NS_CC;
//
//AppDelegate::AppDelegate() {
//
//}
//
//AppDelegate::~AppDelegate()
//{
//}
//
//bool AppDelegate::applicationDidFinishLaunching() {
//    auto director = Director::getInstance();
//    auto glview = director->getOpenGLView();
//    if(!glview) {
//        glview = GLViewImpl::create("Sea Battle");
//        glview->setFrameSize(800, 600);
//        director->setOpenGLView(glview);
//    }
//
////    auto logoScene = LogoScene::createScene();
////    director->runWithScene(logoScene);
//
//    auto helloWorldScene = HelloWorld::createScene();
//    director->runWithScene(helloWorldScene);
//
////    auto placingShipsScene = PlacingShipsScene::createScene();
////    director->runWithScene(placingShipsScene);
//
//
//
//    return true;
//}
//
//void AppDelegate::applicationDidEnterBackground() {
//}
//
//void AppDelegate::applicationWillEnterForeground() {
//}



USING_NS_CC;

AppDelegate::AppDelegate() {
    
}

AppDelegate::~AppDelegate()
{
}

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
//    glview->setIcon(icons);
    auto visibleSize = Director::getInstance()->getVisibleSize();
    glview->setDesignResolutionSize(visibleSize.width, visibleSize.height, ResolutionPolicy::NO_BORDER);
    director->setOpenGLView(glview);
    
    // Set the design resolution
//    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    
    
    
//    Size frameSize = glview->getFrameSize();
//
//    vector<string> searchPath;
    
    // In this demo, we select resource according to the frame's height.
    // If the resource size is different from design resolution size, you need to set contentScaleFactor.
    // We use the ratio of resource's height to the height of design resolution,
    // This can make sure that the resource's height could fit for the height of design resolution.
    
    // If the frame's height is larger than the height of medium resource size, select large resource.
//    if (frameSize.height > mediumResource.size.height)
//    {
//        searchPath.push_back(largeResource.directory);
//
//        director->setContentScaleFactor(MIN(largeResource.size.height/designResolutionSize.height, largeResource.size.width/designResolutionSize.width));
//    }
//    // If the frame's height is larger than the height of small resource size, select medium resource.
//    else if (frameSize.height > smallResource.size.height)
//    {
//        searchPath.push_back(mediumResource.directory);
//
//        director->setContentScaleFactor(MIN(mediumResource.size.height/designResolutionSize.height, mediumResource.size.width/designResolutionSize.width));
//    }
//    // If the frame's height is smaller than the height of medium resource size, select small resource.
//    else
//    {
//        searchPath.push_back(smallResource.directory);
//
//        director->setContentScaleFactor(MIN(smallResource.size.height/designResolutionSize.height, smallResource.size.width/designResolutionSize.width));
//    }
    
    // Create a scene. it's an autorelease object
//    auto scene = PlacingShipsScene::createScene();
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
