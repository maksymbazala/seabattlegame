#include "Utils.hpp"

int Utils::tmxArray[] = {1,2,3,4,5,6,7,8,9,10,
    11,12,13,14,15,16,17,18,19,20,
    21,22,23,24,25,26,27,28,29,30,
    31,32,33,34,35,36,37,38,39,40,
    41,42,43,44,45,46,47,48,49,50,
    51,52,53,54,55,56,57,58,59,60,
    61,62,63,64,65,66,67,68,69,70,
    71,72,73,74,75,76,77,78,79,80,
    81,82,83,84,85,86,87,88,89,90,
    91,92,93,94,95,96,97,98,99,100};

std::string Utils::logFileName;
int Utils::loggerLineCounter = 1;

void Utils::initLogFile()
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];
    
    time (&rawtime);
    timeinfo = localtime(&rawtime);
    
    //Set absolute path to existing logs folder, you will face a console error, otherwise
    strftime(buffer,sizeof(buffer),"/<seabattlegame logs folder path>/log_from_%d_%m_%Y__%H_%M_%S.log",timeinfo);
    std::string str(buffer);
    
    logFileName = str;
    
    std::ofstream fout;
    fout.open(logFileName,std::ios::app);
    
    fout << "\n======================================================================================================\n" << std::endl;
    
//    char path[1024];
//    getcwd(path, sizeof(path));
//    std::cout << (path) << std::endl;
}

void Utils::log(int FieldAction, std::string logMessage)
{
    std::ofstream fout;
    fout.open(logFileName,std::ios::app);
    
    if (!fout.is_open())
    {
        std::cout << "\nLOG WAS NOT SAVED! CHECK THE CORRECTNESS OF PATH TO THE FILE\n" << std::endl;
        return;
    }
    
    else
    {
        switch (FieldAction) {
            case BASIC:
            {
                fout << loggerLineCounter << "\t>>>   " << logMessage << std::endl;
                break;
            }
                
            case INFO:
            {
                fout << "\n" << loggerLineCounter << "\tINFO.\t" << logMessage << std::endl << std::endl;
                break;
            }
                
            case ERROR:
            {
                fout << "\n" << loggerLineCounter << "\tERROR.\t" << logMessage << std::endl << std::endl;
                break;
            }
                
                
            default:
                break;
        }
        
        loggerLineCounter++;
    }
}

cocos2d::Vec2 Utils::convertTileIdToCoords(int tileId)
{
    cocos2d::Vec2 coords;
    int tilePos;
    
    for (int i = 0; i < 100; i++)
    {
        if (tileId == tmxArray[i])
            tilePos = i;
    }
    
    coords.x = tilePos % 10;
    coords.y = tilePos / 10;
    
    return coords;
}

cocos2d::Vec2 Utils::convertToAbsoluteCoords(cocos2d::TMXTiledMap *field, unsigned int x, unsigned int y)
{
    int absoluteX, absoluteY;
    
    absoluteX = field->getBoundingBox().getMinX() + x*40 + 20;
    absoluteY = field->getBoundingBox().getMaxY() - y*40 - 20;
    
    return cocos2d::Vec2(absoluteX, absoluteY);
}

int Utils::randomInRange(int a, int b)
{
    int min, max, result;
    
    if(a == b)
    {
        std::cout << "incorrect range!";
        return -1;
    }
    else if (a > b)
    {
        min = b;
        max = a;
    }
    else
    {
        min = a;
        max = b;
    }
    
    if (max - min == 1)
    {
        if (rand() % 10 >= 5)
            result = max;
           
        else
            result = min;
    }
    else
        result = min + (rand() % (max - min + 1));
    
    return result;
}

int Utils::getMin(std::vector<int> numbers)
{
    int min = numbers[0];
    
    for (int value : numbers)
    {
        if (min < value)
            min = value;
    }
    
    return min;
}

int Utils::getMax(std::vector<int> numbers)
{
    int max = numbers[0];
    
    for (int value : numbers)
    {
        if (max < value)
            max = value;
    }
    
    return max;
}

bool Utils::firstPlayerVictory;

std::string Utils::getVictoryStatus()
{
    if (firstPlayerVictory)
        return "Victory";
    
    else
        return "Defeat";
}
