#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

#include "PlacingShipsScene.h"

class MenuScene : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    CREATE_FUNC(MenuScene);
    
    void closeMenu();
    void closeGame(float dt);
    
    cocos2d::Sprite *backgroundImage;
    cocos2d::Menu *menu;
    
    void Play(Ref *pSender);
    void Exit(Ref *pSender);
};
