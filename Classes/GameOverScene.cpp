#include "BattleScene.hpp"

USING_NS_CC;

Scene* GameOverScene::createScene()
{
    auto scene = Scene::create();
    auto layer = GameOverScene::create();
    
    scene->setTag(100);
    scene->addChild(layer);
    
    return scene;
}

bool GameOverScene::init()
{
    if ( !LayerColor::initWithColor(Color4B(0,0,0,255)) )
    {
        return false;
    }
    
    //Background setup
    auto visibleSize = Director::getInstance()->getVisibleSize();
    backgroundImage = Sprite::create("Field/background.png");
    backgroundImage->setScaleX(visibleSize.width / this->backgroundImage->getBoundingBox().size.width);
    backgroundImage->setScaleY(visibleSize.height / this->backgroundImage->getBoundingBox().size.height);
    backgroundImage->setPosition(visibleSize.width/2,visibleSize.height/2);
    backgroundImage->setTag(101);
    this->addChild(backgroundImage);
    
    //Victory/defeat label
    auto battlefieldLabel = Label::createWithTTF(Utils::getVictoryStatus(), "fonts/stencil_bt.ttf", 100);
    battlefieldLabel->setTextColor(Color4B(148,216,255,255));
    battlefieldLabel->setPosition(visibleSize.width / 2, visibleSize.height - 100);
    battlefieldLabel->setTag(102);
    this->addChild(battlefieldLabel);
    
    //Play button
    newGameButton = Sprite::create("Buttons/button_new_game.png");
    newGameButton->setPosition(visibleSize.width / 2 - 100, 100);
    newGameButton->setScale(0.2);
    this->addChild(newGameButton, 0);
    
    //Exit button
    exitButton = Sprite::create("Buttons/button_exit.png");
    exitButton->setPosition(visibleSize.width / 2 + 100, 100);
    exitButton->setScale(0.2);
    this->addChild(exitButton, 0);
    
    //Your field label
    auto yourFieldLabel = Label::createWithTTF("Your field", "fonts/stencil_bt.ttf", 20);
    yourFieldLabel->setTextColor(Color4B(148,216,255,80));
    yourFieldLabel->setPosition(visibleSize.width/4, visibleSize.height/2 - 250);
    yourFieldLabel->setTag(103);
    this->addChild(yourFieldLabel);
    
    //Enemy field label
    auto enemyFieldLabel = Label::createWithTTF("Enemy field", "fonts/stencil_bt.ttf", 20);
    enemyFieldLabel->setTextColor(Color4B(148,216,255,80));
    enemyFieldLabel->setPosition(visibleSize.width - visibleSize.width/4, visibleSize.height/2 - 250);
    enemyFieldLabel->setTag(104);
    this->addChild(enemyFieldLabel);
    
    //TMX map: player
    playerField = TMXTiledMap::create("Field/tileMap_final.tmx");
    playerBackground = playerField->getLayer("Tile Layer 1");
    playerField->setScale(0.5);
    playerField->setAnchorPoint(Vec2(0.5,0.5));
    playerField->setPosition(visibleSize.width/4,visibleSize.height/2.1);
    playerField->setTag(106);
    this->addChild(playerField, 0);
    
    //TMX map: enemy
    enemyField = TMXTiledMap::create("Field/tileMap_final.tmx");
    enemyBackground = enemyField->getLayer("Tile Layer 1");
    enemyField->setScale(0.5);
    enemyField->setAnchorPoint(Vec2(0.5,0.5));
    enemyField->setPosition(visibleSize.width - visibleSize.width/4,visibleSize.height/2.1);
    enemyField->setTag(107);
    this->addChild(enemyField, 0);
    
    //Markup: player
    auto playerMarkup = Sprite::create("Field/markup.png");
    playerMarkup->setScale(0.5);
    playerMarkup->setPosition(visibleSize.width/4 - 24,visibleSize.height/2.1 + 24);
    playerMarkup->setTag(108);
    this->addChild(playerMarkup, 0);
    
    //Markup: enemy
    auto enemyMarkup = Sprite::create("Field/markup.png");
    enemyMarkup->setScale(0.5);
    enemyMarkup->setPosition(visibleSize.width - visibleSize.width/4 - 24,visibleSize.height/2.1 + 24);
    enemyMarkup->setTag(109);
    this->addChild(enemyMarkup, 0);
    
    //Sizes setup
    deckerSize = 40;
    initialWidth = visibleSize.width/2 + 100;
    initialHeight = visibleSize.height/2 + deckerSize*2;
    
    //Event listeners init
    auto mouseClickListener = EventListenerMouse::create();
    mouseClickListener->onMouseUp = [this](cocos2d::Event* event)
    {
        EventMouse* mouseUp = dynamic_cast<EventMouse*>(event);
        
        //Click on Menu button
        if (newGameButton->getBoundingBox().containsPoint(mouseUp->getLocationInView()))
            startNewGame();
        
        else if (exitButton->getBoundingBox().containsPoint(mouseUp->getLocationInView()))
        {
            exitTheGame();
        }
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseClickListener, this);
    
    //Render ships and effects
    renderShipsOnField(Actor::actorsPool[0]);
    renderEffectsOnField(Actor::actorsPool[0]);
    
    renderShipsOnField(Actor::actorsPool[1]);
    renderEffectsOnField(Actor::actorsPool[1]);
    
    Utils::log(INFO, "GameOver Scene UI is completed.");
 
    return true;
}

void GameOverScene::renderShipsOnField(Actor* actor)
{
    for (unsigned int i = 0; i < 10; i++)
    {
        actor->getPlayerShips()[i]->updateSprite();
        if (!actor->getPlayerShips()[i]->isAlive)
            actor->getPlayerShips()[i]->getShipSprite()->setOpacity(200);
        
        this->addChild(actor->getPlayerShips()[i]->getShipSprite());
    }
}

void GameOverScene::renderEffectsOnField(Actor* actor)
{
    cocos2d::Vec2 spritePosition;
    cocos2d::TMXTiledMap *field;
    if (actor == Actor::actorsPool[0])
        field = playerField;
    else
        field = enemyField;
    
    for (unsigned int i = 0; i < 10; i++)
    {
        for (unsigned int j = 0; j < 10; j++)
        {
            spritePosition = Utils::convertToAbsoluteCoords(field, j, i);
            
            switch (actor->getBattleField()[i][j]) {
                case MISS:
                {
                    auto miss = Sprite::create("Effects/miss.png");
                    miss->setScale(0.3);
                    miss->setPosition(spritePosition);
                    this->addChild(miss, 0);
                    
                    break;
                }
                    
                case HIT:
                {
                    cocos2d::Sprite* hit;
                    if (actor->getShipByDeckerCoords(i, j)->isAlive)
                        hit = Sprite::create("Effects/explosion_active.png");
                    else
                        hit = Sprite::create("Effects/explosion_inactive.png");
                    
                    hit->setScale(0.5);
                    hit->setPosition(spritePosition);
                    this->addChild(hit, 0);
                    
                    break;
                }
                    
                case EMPTY:
                    break;
                    
                default:
                {
                    Utils::log(ERR, ("Cannot render effect. Coords: Y " + std::to_string(i) + ", X " + std::to_string(j)));
                    break;
                }
            }
        }
    }
}

void GameOverScene::startNewGame()
{
    Actor::actorsPool.clear();
    
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Sounds/simple_click.wav");
    Utils::log(BASIC, "Start new game.");
    
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Sounds/bg_music.mp3", true);
    
    if (Actor::actorsPool[0]->isRealPlayer)
        Director::getInstance()->replaceScene(TransitionFade::create(1.0f, PlacingShipsScene::createScene(), Color3B(0, 0, 0)));
        else
            Director::getInstance()->replaceScene(PlacingShipsScene::createScene());
    
}

void GameOverScene::exitTheGame()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Sounds/simple_click.wav");
    Utils::log(BASIC, "Exit.");
    
    auto fadeOutAction = FadeOut::create(1);
    scheduleOnce(schedule_selector(GameOverScene::end), 1.0f);
    this->runAction(fadeOutAction);
    
    for (auto sprite : this->getChildren())
        sprite->runAction(fadeOutAction->clone());
    
    Sprite *playerTile, *enemyTile;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            playerTile = playerBackground->getTileAt(Vec2(i,j));
            enemyTile = enemyBackground->getTileAt(Vec2(i,j));
            playerTile->runAction(fadeOutAction->clone());
            enemyTile->runAction(fadeOutAction->clone());
        }
    }
}

void GameOverScene::end(float dt)
{
    Director::getInstance()->end();
}
