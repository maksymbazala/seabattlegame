//#pragma once
//
//#include "cocos2d.h"
//
//class HelloWorld : public cocos2d::Layer
//{
//public:
//    static cocos2d::Scene* createScene();
//    virtual bool init();
//
//    CREATE_FUNC(HelloWorld);
//};

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Scene
{
private:
    cocos2d::CCTMXTiledMap *_tileMap;
    cocos2d::CCTMXLayer *_background;
public:
    virtual bool init() override;
    
    static cocos2d::Scene* scene();
    
    // a selector callback
    void menuCloseCallback(Ref* sender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
