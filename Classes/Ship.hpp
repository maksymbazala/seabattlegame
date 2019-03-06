#pragma once
#include "Utils.hpp"

class Ship
{
private:
    int size;
    int rotationValue;
    int** deckersCoordinates;
    cocos2d::Sprite* shipSprite;
    cocos2d::Sprite* shipPlaceholderSprite;
    cocos2d::Vec2 shipPlaceholderPosition;
    cocos2d::Vec2 shipSpriteMidPosition;
    
    void initShip();
    void setSprite();
    void setPlaceholderSprite();
    
public:
    void placeShip(int** coordinates);
    int getSize();
    void setRotationValue(int value);
    int getRotationValue();
    cocos2d::Sprite* getShipSprite();
    cocos2d::Sprite* getShipPlaceholderSprite();
    void setPlaceholderSpritePosition(cocos2d::Vec2 position);
    void setShipSpriteMidPosition(cocos2d::Vec2 position);
    cocos2d::Vec2 getShipSpriteMidPosition();
    void setDraggingShipDeckers(float clickCoordsX, float clickCoordsY);
    void freeDraggingShipDeckersMemory();
    int** getDeckersCoordinates();
    
    int** placeShipDeckersCoordinates;
    bool spriteAlignedVertically;
    bool isPlaced;
    bool isAlive;
    std::vector<cocos2d::Sprite*> wreckedDeckers;
    
    void updateSprite();

    Ship();
    Ship(int size);
    ~Ship();
};
