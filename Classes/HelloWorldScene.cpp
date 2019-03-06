#include "HelloWorldScene.h"
#include <iostream>

USING_NS_CC;

Scene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
//    auto scene = Scene::create();
    auto layer = HelloWorld::create();

//    scene->addChild(layer);

    return layer;
}

bool HelloWorld::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
    
    auto mouseClickListener = EventListenerMouse::create();
    mouseClickListener->onMouseUp = [this](cocos2d::Event* event){
        
        EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);
        std::cout << mouseEvent->getLocation().x << std::endl;
        std::cout << mouseEvent->getLocation().y << std::endl;
        std::cout << mouseEvent->getLocationInView().x << std::endl;
        std::cout << mouseEvent->getLocationInView().y << std::endl;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseClickListener,this);
    
    _tileMap = TMXTiledMap::create("second.tmx");
    this->addChild(_tileMap);
    
    
    return true;
}
