#pragma once
#include "Utils.hpp"
#include "Ship.hpp"

class Actor
{
private:
    Ship** playerShips;
    int** battleField;
    int** enemyBattleField;
    int totalShipsAlive;
    
    void initPlayerShips();
    void initBattleField();
    
public:
    bool isRealPlayer;
    static std::vector<Actor*> actorsPool;
    Ship* lastHitShip;
    
    Ship** getPlayerShips();
    int getTotalShipsAlive();
    int** getBattleField();
    void setEnemyBattleField(int** battleField);
    int** getEnemyBattleField();
    bool boundariesOfShipDeckersCheck(Ship* ship);
    void updateBattleField(int coordY, int coordX, int updatedStatus);
    Ship* getShipByDeckerCoords(unsigned int coordY, unsigned int coordX);
    
    Actor() = default;
    Actor(bool isRealPlayer);
    ~Actor();
};
