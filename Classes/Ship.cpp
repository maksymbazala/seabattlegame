#include "Ship.hpp"

void Ship::initShip()
{
    deckersCoordinates = new int*[size];
    for (int i = 0; i < size; i++)
    {
        deckersCoordinates[i] = new int[2];
    }
}

void Ship::setSprite()
{
    switch (size) {
        case 1:
        {
            shipSprite = cocos2d::Sprite::create("Ships/ship_1decker.png");
            break;
        }
        case 2:
        {
            shipSprite = cocos2d::Sprite::create("Ships/ship_2deckers.png");
            break;
        }
        case 3:
        {
            shipSprite = cocos2d::Sprite::create("Ships/ship_3deckers.png");
            break;
        }
        case 4:
        {
            shipSprite = cocos2d::Sprite::create("Ships/ship_4deckers.png");
            break;
        }
        default:
        {
            std::cout << "ERROR. Setting of Sprites." << std::endl;
            break;
        }
    }
    
    shipSprite->setRotation(90);
    shipSprite->setScale(0.5);
}

void Ship::setPlaceholderSprite()
{
    switch (size) {
        case 1:
        {
            shipPlaceholderSprite = cocos2d::Sprite::create("Ships/placeholder_1decker.png");
            break;
        }
        case 2:
        {
            shipPlaceholderSprite = cocos2d::Sprite::create("Ships/placeholder_2deckers.png");
            break;
        }
        case 3:
        {
            shipPlaceholderSprite = cocos2d::Sprite::create("Ships/placeholder_3deckers.png");
            break;
        }
        case 4:
        {
            shipPlaceholderSprite = cocos2d::Sprite::create("Ships/placeholder_4deckers.png");
            break;
        }
        default:
        {
            std::cout << "ERROR. Setting of placeholder Sprites" << std::endl;
            break;
        }
    }
    
    shipPlaceholderSprite->setRotation(90);
    shipPlaceholderSprite->setScale(0.5);
}

void Ship::setPlaceholderSpritePosition(cocos2d::Vec2 position)
{
    shipPlaceholderPosition = position;
}

void Ship::setShipSpriteMidPosition(cocos2d::Vec2 position)
{
    shipSpriteMidPosition = position;
}

cocos2d::Vec2 Ship::getShipSpriteMidPosition()
{
    return shipSpriteMidPosition;
}

void Ship::placeShip(int** coordinates)
{
    for (int i = 0; i < size; i++)
    {
        deckersCoordinates[i][0] = coordinates[i][0];
        deckersCoordinates[i][1] = coordinates[i][1];
    }
    
    isPlaced = true;
}

int Ship::getSize()
{
    return size;
}

void Ship::setRotationValue(int value)
{
    rotationValue = value;
}

int Ship::getRotationValue()
{
    return rotationValue;
}

cocos2d::Sprite* Ship::getShipSprite()
{
    return shipSprite;
}

cocos2d::Sprite* Ship::getShipPlaceholderSprite()
{
    return shipPlaceholderSprite;
}

void Ship::setDraggingShipDeckers(float coordX, float coordY)
{
    placeShipDeckersCoordinates = new int*[getSize()];
    for (unsigned int i = 0; i< getSize(); i++)
    {
        placeShipDeckersCoordinates[i] = new int[2];
    }
    
    //Setting
    switch (getSize()) {
            
        case 1:
        {
            placeShipDeckersCoordinates[0][0] = floor(coordY);
            placeShipDeckersCoordinates[0][1] = floor(coordX);
            
            break;
        }
            
        case 2:
        {
            if(spriteAlignedVertically)
            {
                placeShipDeckersCoordinates[0][0] = floor(coordY - 0.5);
                placeShipDeckersCoordinates[0][1] = floor(coordX);
                placeShipDeckersCoordinates[1][0] = floor(coordY + 0.5);
                placeShipDeckersCoordinates[1][1] = floor(coordX);
            }
            
            else
            {
                placeShipDeckersCoordinates[0][0] = floor(coordY);
                placeShipDeckersCoordinates[0][1] = floor(coordX - 0.5);
                placeShipDeckersCoordinates[1][0] = floor(coordY);
                placeShipDeckersCoordinates[1][1] = floor(coordX + 0.5);
            }
            
            break;
        }
            
        case 3:
        {
            if(spriteAlignedVertically)
            {
                placeShipDeckersCoordinates[0][0] = floor(coordY - 1);
                placeShipDeckersCoordinates[0][1] = floor(coordX);
                placeShipDeckersCoordinates[1][0] = floor(coordY);
                placeShipDeckersCoordinates[1][1] = floor(coordX);
                placeShipDeckersCoordinates[2][0] = floor(coordY + 1);
                placeShipDeckersCoordinates[2][1] = floor(coordX);
            }
            
            else
            {
                placeShipDeckersCoordinates[0][0] = floor(coordY);
                placeShipDeckersCoordinates[0][1] = floor(coordX - 1);
                placeShipDeckersCoordinates[1][0] = floor(coordY);
                placeShipDeckersCoordinates[1][1] = floor(coordX);
                placeShipDeckersCoordinates[2][0] = floor(coordY);
                placeShipDeckersCoordinates[2][1] = floor(coordX + 1);
            }
            
            break;
        }
            
        case 4:
        {
            if(spriteAlignedVertically)
            {
                placeShipDeckersCoordinates[0][0] = floor(coordY - 1.5);
                placeShipDeckersCoordinates[0][1] = floor(coordX);
                placeShipDeckersCoordinates[1][0] = floor(coordY - 0.5);
                placeShipDeckersCoordinates[1][1] = floor(coordX);
                placeShipDeckersCoordinates[2][0] = floor(coordY + 0.5);
                placeShipDeckersCoordinates[2][1] = floor(coordX);
                placeShipDeckersCoordinates[3][0] = floor(coordY + 1.5);
                placeShipDeckersCoordinates[3][1] = floor(coordX);
            }
            
            else
            {
                placeShipDeckersCoordinates[0][0] = floor(coordY);
                placeShipDeckersCoordinates[0][1] = floor(coordX - 1.5);
                placeShipDeckersCoordinates[1][0] = floor(coordY);
                placeShipDeckersCoordinates[1][1] = floor(coordX - 0.5);
                placeShipDeckersCoordinates[2][0] = floor(coordY);
                placeShipDeckersCoordinates[2][1] = floor(coordX + 0.5);
                placeShipDeckersCoordinates[3][0] = floor(coordY);
                placeShipDeckersCoordinates[3][1] = floor(coordX + 1.5);
            }
            
            break;
        }
            
        default:
        {
            std::cout << "ERROR. placeShipDeckersCoordinates error." << std::endl;
            break;
        }
    }
}

void Ship::freeDraggingShipDeckersMemory()
{
    if (placeShipDeckersCoordinates == nullptr)
        return;
    
    for(unsigned int f = 0; f < getSize(); f++)
        delete [] placeShipDeckersCoordinates[f];
    
    delete [] placeShipDeckersCoordinates;
}

int** Ship::getDeckersCoordinates()
{
    return deckersCoordinates;
}

void Ship::updateSprite()
{
    setSprite();
    shipSprite->setPosition(shipSpriteMidPosition);
    shipSprite->setRotation(rotationValue);
}

Ship::Ship() = default;

Ship::Ship(int size)
{
    isPlaced = false;
    isAlive = true;
    spriteAlignedVertically = false;
    
    this->size = size;
    
    initShip();
    setSprite();
    setPlaceholderSprite();
}

Ship::~Ship()
{
    for (int i = 0; i < size; i++)
    {
        delete [] deckersCoordinates[i];
    }
    delete [] deckersCoordinates;
}
