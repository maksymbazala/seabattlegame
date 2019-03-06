#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "AppMacros.h"
#include <vector>
#include <string>

#include "LogoScene.h"
#include "PlacingShipsScene.h"

class  AppDelegate : private cocos2d::Application
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    virtual bool applicationDidFinishLaunching();
    virtual void applicationDidEnterBackground();
    virtual void applicationWillEnterForeground();
};
