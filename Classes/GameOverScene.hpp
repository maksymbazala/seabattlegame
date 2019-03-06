#pragma once

#include <thread>
#include <chrono>

#include "Utils.hpp"
#include "cocos2d.h"
#include "Actor.hpp"
#include "Ship.hpp"

#include "MenuScene.h"

class GameOverScene : public cocos2d::LayerColor
{
private:
    float deckerSize;
    float initialWidth;
    float initialHeight;
    
    bool gameIsOver;
    
    cocos2d::Sprite* newGameButton;
    cocos2d::Sprite* exitButton;
    
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    CREATE_FUNC(GameOverScene);
    
    cocos2d::Sprite *backgroundImage;
    cocos2d::TMXTiledMap *playerField;
    cocos2d::TMXTiledMap *enemyField;
    cocos2d::TMXLayer *playerBackground;
    cocos2d::TMXLayer *enemyBackground;
    
    void renderShipsOnField(Actor* actor);
    void renderEffectsOnField(Actor* actor);
    
    void startNewGame();
    void exitTheGame();
    void end(float dt);
};
