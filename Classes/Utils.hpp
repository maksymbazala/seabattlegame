#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"

enum FieldAction {EMPTY = 100, MISS, SHIP, HIT, DESTROYED};
enum LoggerAction {BASIC = 1000, INFO, ERR};

class Utils
{
private:
    static int tmxArray[100];
    static int loggerLineCounter;
    static std::string logFileName;
public:
    static void initLogFile();
    static void log(int FieldAction, std::string logMessage);
    static cocos2d::Vec2 convertTileIdToCoords(int tileId);
    static cocos2d::Vec2 convertToAbsoluteCoords(cocos2d::TMXTiledMap *field, unsigned int x, unsigned int y);
    static int randomInRange(int a, int b);
    static int getMin(std::vector<int> numbers);
    static int getMax(std::vector<int> numbers);
    
    static bool firstPlayerVictory;
    static std::string getVictoryStatus();
    
    Utils();
    virtual ~Utils() = 0;
};
