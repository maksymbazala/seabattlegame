#include "MenuScene.h"

USING_NS_CC;

Scene* MenuScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    auto layer = MenuScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool MenuScene::init()
{
    if ( !LayerColor::initWithColor(Color4B(0,0,0,255)) )
    {
        return false;
    }
    
    if (this->menu == nullptr)
    { 
        auto visibleSize = Director::getInstance()->getVisibleSize();
        this->backgroundImage = Sprite::create("Field/background.png");
        this->backgroundImage->setScaleX(visibleSize.width / this->backgroundImage->getBoundingBox().size.width);
        this->backgroundImage->setScaleY(visibleSize.height / this->backgroundImage->getBoundingBox().size.height);
        this->backgroundImage->setPosition(visibleSize.width/2,visibleSize.height/2);
        this->addChild(this->backgroundImage);
        
        auto play_menu_item = MenuItemImage::create("Buttons/button_play.png", "Buttons/button_play.png", CC_CALLBACK_1(MenuScene::Play, this));
        auto exit_menu_item = MenuItemImage::create("Buttons/button_exit.png", "Buttons/button_exit.png", CC_CALLBACK_1(MenuScene::Exit, this));
        
        play_menu_item->setScale(0.2);
        exit_menu_item->setScale(0.2);
        
        this->menu = Menu::create(play_menu_item, exit_menu_item, nullptr);
        menu->alignItemsVerticallyWithPadding(50);
        this->addChild(this->menu);
    }
    
    auto keyboardClickListener = EventListenerKeyboard::create();
    keyboardClickListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event)
    {
        if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
            closeMenu();
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardClickListener, this);
    
    return true;
}

void MenuScene::Play(cocos2d::Ref *pSender)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Sounds/simple_click.wav");
    Director::getInstance()->popScene();
}

void MenuScene::Exit(cocos2d::Ref *pSender)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Sounds/simple_click.wav");
    auto fadeOutAction = FadeOut::create(1);
    
    this->scheduleOnce(schedule_selector(MenuScene::closeGame), 1.0f);
    this->menu->runAction(fadeOutAction);
    this->backgroundImage->runAction(fadeOutAction->clone());
}

void MenuScene::closeMenu()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Sounds/simple_click.wav");
    Director::getInstance()->popScene();
}

void MenuScene::closeGame(float dt)
{
    Director::getInstance()->end();
}
