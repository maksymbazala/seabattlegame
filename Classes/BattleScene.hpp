#pragma once

#include <thread>
#include <chrono>

#include "Utils.hpp"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "Actor.hpp"
#include "Ship.hpp"

#include "MenuScene.h"
#include "GameOverScene.hpp"

class BattleScene : public cocos2d::LayerColor
{
private:
    cocos2d::EventListenerMouse* mouseClickListener;
    cocos2d::EventListenerKeyboard* keyboardClickListener;
    
    float deckerSize;
    float initialWidth;
    float initialHeight;
    
    Actor* currentPlayer;
    Actor* enemyPlayer;
    
    bool customCursorIsDisplayed;
    bool gameIsOver;
    
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    CREATE_FUNC(BattleScene);
    
    cocos2d::Sprite *backgroundImage;
    cocos2d::ui::Button *menuButton;
    cocos2d::Sprite *customCursor;
    cocos2d::Action *cursorAnimation;
    cocos2d::TMXTiledMap *playerField;
    cocos2d::TMXTiledMap *enemyField;
    cocos2d::TMXLayer *playerBackground;
    cocos2d::TMXLayer *enemyBackground;
    cocos2d::Label *autoShotTooltip;
    cocos2d::Sequence *autoShotLabelSequence;
    
    void placeShipsOnField();
    void shot(unsigned int x, unsigned int y);
    void randomShot();
    void hit(unsigned int x, unsigned int y);
    void miss(unsigned int x, unsigned int y);
    bool checkIfDestroed(Ship* ship);
    void boundariesOfShipDeckersDisable(Actor* actor, Ship* ship);
    
    void enableEventListeners();
    void disableEventListeners();
    void showCustomCursor();
    void hideCustomCursor();
    void startCustomCursotAnimation();
    void stopCustomCursorAnimation();
    void nextTurn();
    void renderFieldAction(unsigned int x, unsigned int y, unsigned int action);
    void playSoundEffect(int action);
    void changeCurrentPlayer();
    void openMenu();
    void gameOver();
    void renderGameOverScene();
};
