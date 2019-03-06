#include "Actor.hpp"

std::vector<Actor*> Actor::actorsPool;

void Actor::initPlayerShips()
{
    playerShips = new Ship*[10];
    
    playerShips[0] = new Ship(4);
    playerShips[1] = new Ship(3);
    playerShips[2] = new Ship(3);
    playerShips[3] = new Ship(2);
    playerShips[4] = new Ship(2);
    playerShips[5] = new Ship(2);
    playerShips[6] = new Ship(1);
    playerShips[7] = new Ship(1);
    playerShips[8] = new Ship(1);
    playerShips[9] = new Ship(1);
}

void Actor::initBattleField()
{
    battleField = new int*[10];
    for (int i = 0; i < 10; i++)
    {
        battleField[i] = new int[10];
    }
    
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            battleField[i][j] = EMPTY;
        }
    }
}

int Actor::getTotalShipsAlive()
{
    return totalShipsAlive;
}

Ship** Actor::getPlayerShips()
{
    return playerShips;
}

int** Actor::getBattleField()
{
    return battleField;
}

void Actor::setEnemyBattleField(int** battleField)
{
    enemyBattleField = battleField;
}

int** Actor::getEnemyBattleField()
{
    return enemyBattleField;
}

bool Actor::boundariesOfShipDeckersCheck(Ship* ship)
{
    int tempX, tempY = 0;
    int deckersBordersScanningIterators[3][3][2];
    
    for (unsigned int i = 0; i < 3; i++)
    {
        for (unsigned int j = 0; j < 3; j++)
        {
            deckersBordersScanningIterators[i][j][0] = i - 1;
            deckersBordersScanningIterators[i][j][1] = j - 1;
        }
    }
    
    for (unsigned int m = 0; m < ship->getSize(); m++)
    {
        tempY = ship->placeShipDeckersCoordinates[m][0];
        tempX = ship->placeShipDeckersCoordinates[m][1];
        
        for (unsigned int i = 0; i < 3; i++)
        {
            for (unsigned int j = 0; j < 3; j++)
            {
                tempY += deckersBordersScanningIterators[i][j][0];
                tempX += deckersBordersScanningIterators[i][j][1];
                
                if (tempX >= 0 && tempX <=9 && tempY >= 0 && tempY <= 9)
                {
                    if (getBattleField()[tempY][tempX] != EMPTY)
                        return false;
                }
                
                tempY = ship->placeShipDeckersCoordinates[m][0];
                tempX = ship->placeShipDeckersCoordinates[m][1];
            }
        }
    }
    
    return true;
}

void Actor::updateBattleField(int coordY, int coordX, int updatedStatus)
{
    if (coordY < 0 || coordX < 0 || coordY > 9 || coordX > 9)
        std::cout << "ERROR. updateBattleField: " << std::endl << "coordY = " << coordY << ", coordX = " << coordX << std::endl;
    
    else
        battleField[coordY][coordX] = updatedStatus;
}

Ship* Actor::getShipByDeckerCoords(unsigned int coordY, unsigned int coordX)
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < getPlayerShips()[i]->getSize(); j++)
        {
            if (getPlayerShips()[i]->getDeckersCoordinates()[j][0] == coordY && getPlayerShips()[i]->getDeckersCoordinates()[j][1] == coordX)
                return getPlayerShips()[i];
        }
    }
    
    std::cout << "ERROR. No ship was found" << std::endl;
    return nullptr;
}

Actor::Actor(bool isRealPlayer)
{
    this->isRealPlayer = isRealPlayer;
    
    totalShipsAlive = 10;
    initPlayerShips();
    initBattleField();
    lastHitShip = nullptr;
    
    actorsPool.push_back(this);
}

Actor::~Actor()
{
    for (int i = 0; i < 10; i++)
    {
        delete playerShips[i];
        delete battleField[i];
    }
    
    delete [] playerShips;
    delete [] battleField;
}
