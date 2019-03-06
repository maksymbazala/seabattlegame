#include "LogoScene.h"

USING_NS_CC;

Scene* LogoScene::createScene()
{
    auto scene = Scene::create();
    auto layer = LogoScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool LogoScene::init()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    if ( !LayerColor::initWithColor(Color4B(0,0,0,255)) )
    {
        return false;
    }
    
    this->backgroundImage = Sprite::create("bg_with_logo.jpg");
    this->backgroundImage->setScaleX(visibleSize.width / this->backgroundImage->getBoundingBox().size.width);
    this->backgroundImage->setScaleY(visibleSize.height / this->backgroundImage->getBoundingBox().size.height);
    this->backgroundImage->setPosition(visibleSize.width/2,visibleSize.height/2);
    this->addChild(this->backgroundImage);
    
    backgroundImage->setOpacity(0);
    auto fadeInAction = FadeIn::create(2);
    auto fadeOutAction = FadeOut::create(1);
    auto delay = DelayTime::create(2);
    auto showLogo = Sequence::create(fadeInAction,delay,fadeOutAction,nullptr);

    backgroundImage->runAction(showLogo);

    //Pushing to scenes stack
    Director::getInstance()->pushScene(PlacingShipsScene::createScene());
    
    return true;
}

void LogoScene::onEnter()
{
    Layer::onEnter();
    
    this->scheduleOnce(schedule_selector(LogoScene::finishLogo), 5.0f);
}

void LogoScene::finishLogo(float dt)
{
    Director::getInstance()->replaceScene(MenuScene::createScene());
}
