/*
 Кот
 + Добавить кнопку Creds - skipped
 + Добавить спрайт логотипа - skipped
 + Добавить спрайт победы (Victory) - skipped
 + Добавить спрайт поражения (Defeat) - skipped
 - Добавить спрайт бомбочки (кастомный курсор который будет реплейсить дефолтный на ховере на игровом поле)
 - Добавить тень (контур) на все спрайты кораблей (посмотреть как будет выглядеть на фоне игрового поля)
 - Выровнять размер шрифта на всех кнопках (кнопки могут тянутся по ширине в зависимости от длинны текста, высота кнопки и высота шрифта должны быть одинаковыми)
 - Добавить спрайты всех кнопок в нажатом состоянии
 
 Я
 + Исправить дефект с выравниванием кораблей поставленных вручную
 + Исправить дефект с двойным щелчеком на кнопке Play в меню при изначальном входе в игру
 + Добавить звуковой эффект на клик кнопок
 + Добавить кастомные звуковые дорожки для Поражения / Победы
 + Добавить ESC в ивент листенеры для меню
 + Заблокировать ивент листенер для рендомного расставления кораблей
 + Проверить совместимость с платформой win32
 + Разобраться с инсталятором игры - skipped
 + Указать неабсолютный путь к логам - skipped
 - Подготовить продакшн код
 + Создать репозиторий в гите
 */

#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>

#include "cocos2d.h"

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
