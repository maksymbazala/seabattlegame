#pragma once

#include "cocos2d.h"
#include "MenuScene.h"

class LogoScene : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    void onEnter();
    void finishLogo(float dt);
    
    cocos2d::Sprite *backgroundImage;
    
    CREATE_FUNC(LogoScene);
};
