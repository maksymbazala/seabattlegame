#pragma once

#include "Utils.hpp"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "Actor.hpp"
#include "Ship.hpp"

#include "MenuScene.h"
#include "BattleScene.hpp"

class PlacingShipsScene : public cocos2d::LayerColor
{
private:
    bool placingShipsIsCompleted;
    
    std::vector<cocos2d::Sprite*> ships;
    std::vector<cocos2d::Sprite*> shipsPlaceholdersSprites;
    Actor *actor1, *actor2;
    
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    CREATE_FUNC(PlacingShipsScene);
    
    cocos2d::Sprite *backgroundImage;
    cocos2d::Sprite *menuButton;
    cocos2d::Sprite *playButton;
    cocos2d::TMXTiledMap *field;
    cocos2d::TMXLayer *background;
    cocos2d::Size visibleSize;
    cocos2d::Label *dragAndDropTooltip;
    cocos2d::Label *rotateSpaceTooltip;
    cocos2d::Label *autoPlaceTooltip;
    cocos2d::Sequence *dragAndDropLabelSequence;
    cocos2d::Sequence *autoPlaceLabelSequence;
    cocos2d::Sequence *rotateSpaceLabelSequence;
    
    float deckerSize;
    float initialWidth;
    float initialHeight;
    Ship* dragAndDropShip;
    
    bool createPlayers(bool isRealPlayer1, bool isRealPlayer2);
    void setShipsPlaceholdersSpritesInitialPos();
    void setShipsSpritesInitialPos();
    void triggerDragAndDropShip(Actor *actor, cocos2d::EventMouse* mouseDown);
    void dragAndDropOnField(Actor* actor, cocos2d::EventMouse* mouseUp);
    void rotateShipSprite(Ship* ship);
    void moveShipSpriteToInitialPosition(Ship* ship);
    void changeRandomPosition(cocos2d::Sprite* shipSprite, cocos2d::Vec2 newPosition, unsigned int newRotationValue);
    bool shipIsPlacedInTheFieldValidation(Ship* ship, float targetX, float targetY);
    cocos2d::Vec2 getAlignedPosition(Ship* ship, float targetX, float targetY);
    void alignShipSprite(Ship* ship);
    
    void placeShipsRandomly(Actor *actor);
    void randomPlacingShipsForRealPlayer();
    void showPlayButton();
    void hidePlayButton();
    void checkPlacingShipsCompletion(Actor* actor);
    void startBattleWithoutTransition();
    void startBattle();
    void openMenu();
    void enableEventListeners();
    void disableEventListeners();
};
