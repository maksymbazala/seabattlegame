#include "BattleScene.hpp"

USING_NS_CC;

Scene* BattleScene::createScene()
{
    auto scene = Scene::create();
    auto layer = BattleScene::create();
    
    scene->setTag(100);
    scene->addChild(layer);
    
    return scene;
}

bool BattleScene::init()
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
    
    //Battle Field label
    auto battlefieldLabel = Label::createWithTTF("Battle Field", "fonts/stencil_bt.ttf", 100);
    battlefieldLabel->setTextColor(Color4B(148,216,255,255));
    battlefieldLabel->setPosition(visibleSize.width / 4 + 90 , visibleSize.height - 100);
    battlefieldLabel->setTag(102);
    this->addChild(battlefieldLabel);
    
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
    
    //Menu button
    menuButton = Sprite::create("Buttons/button_menu.png");
    menuButton->setPosition(visibleSize.width - 150, visibleSize.height - 100);
    menuButton->setScale(0.2);
    menuButton->setTag(105);
    this->addChild(menuButton, 0);
    
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
    
    //Auto-shot tooltip
    auto fadeInLabel = FadeIn::create(1.5);
    auto fadeOutLabel = FadeOut::create(1.5);
    autoShotTooltip = Label::createWithTTF("Press \"R\" to shoot randomly", "fonts/stencil_bt.ttf", 30);
    autoShotTooltip->setTextColor(Color4B(148,216,255,80));
    autoShotTooltip->setOpacity(0);
    autoShotTooltip->setPosition(visibleSize.width / 2, 100);
    autoShotLabelSequence = Sequence::create(DelayTime::create(1),fadeInLabel,DelayTime::create(2),fadeOutLabel,nullptr);
    this->addChild(autoShotTooltip);
    autoShotTooltip->runAction(autoShotLabelSequence);
    
    //Sizes setup
    deckerSize = 40;
    initialWidth = visibleSize.width/2 + 100;
    initialHeight = visibleSize.height/2 + deckerSize*2;
    
    //Ships sprites
    if (Actor::actorsPool[0]->isRealPlayer)
        placeShipsOnField();
    
    Utils::log(INFO, "Battle Scene UI is completed.");
    
    //Players init
    currentPlayer = Actor::actorsPool[0];
    enemyPlayer = Actor::actorsPool[1];
    gameIsOver = false;
    
    //Start the game
    nextTurn();
    
    //Event listeners init
    mouseClickListener = EventListenerMouse::create();
    mouseClickListener->onMouseUp = [this](cocos2d::Event* event)
    {
        EventMouse* mouseUp = dynamic_cast<EventMouse*>(event);
        
        //Click on Menu button
        if (this->menuButton->getBoundingBox().containsPoint(mouseUp->getLocationInView()))
            openMenu();
        
        else if (enemyField->getBoundingBox().containsPoint(mouseUp->getLocationInView()))
        {
            auto clickCoordsX = mouseUp->getLocation().x - enemyField->getBoundingBox().getMinX();
            auto clickCoordsY = mouseUp->getLocation().y - (Director::getInstance()->getVisibleSize().height - enemyField->getBoundingBox().getMaxY());
            shot((unsigned int)clickCoordsX/40, (unsigned int)clickCoordsY/40);
        }
    };
    
    keyboardClickListener = EventListenerKeyboard::create();
    keyboardClickListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event)
    {
        if (keyCode == EventKeyboard::KeyCode::KEY_R)
            randomShot();
        
        else if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
            openMenu();
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseClickListener, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardClickListener, this);
    
    return true;
}

void BattleScene::placeShipsOnField()
{
    for (unsigned int i = 0; i < 10; i++)
    {
        Actor::actorsPool[0]->getPlayerShips()[i]->getShipSprite()->setParent(nullptr);
        this->addChild(Actor::actorsPool[0]->getPlayerShips()[i]->getShipSprite());
    }
}

void BattleScene::shot(unsigned int x, unsigned int y)
{
    if (enemyPlayer->getBattleField()[y][x] == SHIP)
        hit(x, y);
    
    else if (enemyPlayer->getBattleField()[y][x] == EMPTY)
        miss(x, y);
    
    //else is shot on existing miss or hit - not validated
}

void BattleScene::randomShot()
{
    if (currentPlayer->isRealPlayer)
    {
        autoShotTooltip->stopAction(autoShotLabelSequence);
        autoShotTooltip->setOpacity(0);
    }
    
    int randomX, randomY;
    
    //Enemy ship was dammaged at previous step
    if (currentPlayer->lastHitShip != nullptr)
    {
        int deckerCoordY, deckerCoordX;
        std::vector<cocos2d::Vec2> damagedDeckers;
        
        for (unsigned int i = 0; i < currentPlayer->lastHitShip->getSize(); i++)
        {
            deckerCoordY = currentPlayer->lastHitShip->getDeckersCoordinates()[i][0];
            deckerCoordX = currentPlayer->lastHitShip->getDeckersCoordinates()[i][1];
            
            
            if (enemyPlayer->getBattleField()[deckerCoordY][deckerCoordX] == HIT)
            {
                damagedDeckers.push_back(Vec2(deckerCoordX, deckerCoordY));
                Utils::log(BASIC, ("Smart shot. damagedDeckers: Y " + std::to_string(deckerCoordY) + ", X " + std::to_string(deckerCoordX)));
            }
        }
        
        if (damagedDeckers.size() == 0)
        {
            Utils::log(ERR, "Random shot size.");
            return;
        }
        
        //Ship alignement is unknown
        else if (damagedDeckers.size() == 1)
        {
            int nearbyCells[4][2];
            int randomZ;
            bool cellIsTriggered = false;
            
            nearbyCells[0][0] = damagedDeckers[0].y + 1;
            nearbyCells[0][1] = damagedDeckers[0].x;
            
            nearbyCells[1][0] = damagedDeckers[0].y;
            nearbyCells[1][1] = damagedDeckers[0].x + 1;
            
            nearbyCells[2][0] = damagedDeckers[0].y - 1;
            nearbyCells[2][1] = damagedDeckers[0].x;
            
            nearbyCells[3][0] = damagedDeckers[0].y;
            nearbyCells[3][1] = damagedDeckers[0].x - 1;

            do
            {
                randomZ = Utils::randomInRange(0, 3);
                
                if (nearbyCells[randomZ][0] < 0 || nearbyCells[randomZ][0] > 9 || nearbyCells[randomZ][1] < 0 || nearbyCells[randomZ][1] > 9)
                    continue;
                
                randomY = nearbyCells[randomZ][0];
                randomX = nearbyCells[randomZ][1];
                
                if (enemyPlayer->getBattleField()[randomY][randomX] == SHIP || enemyPlayer->getBattleField()[randomY][randomX] == EMPTY)
                    cellIsTriggered = true;
                
            }while (!cellIsTriggered);
            
            Utils::log(BASIC, ("Smart shot. Ship alignement is unknown. Y " + std::to_string(randomY) + ", X " + std::to_string(randomX)));
        }
        
        //Smart shot -> ship alignement is known
        else
        {
            bool increasedDirection = Utils::randomInRange(0, 1);
            int increment;
            int dynamicCoord;
            bool dynamicCoordIsSet = false;
            
            if (currentPlayer->lastHitShip->spriteAlignedVertically)
            {
                randomX = (int)damagedDeckers[0].x;
                dynamicCoord = (int)damagedDeckers[0].y;
                
                Utils::log(BASIC, ("Smart shot. Ship alignement is known. Vertical alignement. Initial Y " + std::to_string(dynamicCoord) + ", initial X " + std::to_string(randomX)));
                
                while (!dynamicCoordIsSet && dynamicCoord >= 0 && dynamicCoord <= 9)
                {
                    if (increasedDirection)
                        increment = 1;
                    else
                        increment = -1;
                    
                    dynamicCoord += increment;
                    
                    Utils::log(BASIC, ("Smart shot. Ship alignement is known. Vertical alignement. Dynamic Coord = " + std::to_string(dynamicCoord)) + ", increasedDirection = " + std::to_string(increasedDirection));
                    
                    if (dynamicCoord >= 0 && dynamicCoord <= 9)
                    {
                        if (enemyPlayer->getBattleField()[dynamicCoord][randomX] == SHIP || enemyPlayer->getBattleField()[dynamicCoord][randomX] == EMPTY)
                        {
                            randomY = dynamicCoord;
                            dynamicCoordIsSet = true;
                            
                            Utils::log(BASIC, ("Smart shot. Ship alignement is known. Vertical alignement. randomY is known: " + std::to_string(randomY)));
                        }
                        //
                        Utils::log(BASIC, ("Smart shot. Ship alignement is known. Horizontal alignement. Not SHIP or EMPTY. Exact status = " + std::to_string(enemyPlayer->getBattleField()[dynamicCoord][randomX])));
                    }
                    
                    
                    if ((!dynamicCoordIsSet && (dynamicCoord <= 0 || dynamicCoord >= 9)))
                    {
                        Utils::log(BASIC, ("Smart shot. Ship alignement is known. Vertical alignement. Dynamic Coord -> out of field. old increasedDirection = " + std::to_string(increasedDirection)) + ", old dynamicCoord = " + std::to_string(dynamicCoord));
                        
                        increasedDirection = !increasedDirection;
                        dynamicCoord = (int)damagedDeckers[0].y;
                        
                        Utils::log(BASIC, ("Smart shot. Ship alignement is known. Vertical alignement. Dynamic Coord -> out of field. new increasedDirection = " + std::to_string(increasedDirection)) + ", new dynamicCoord = " + std::to_string(dynamicCoord));
                    }
                    else if (enemyPlayer->getBattleField()[dynamicCoord][randomX] == MISS)
                    {
                        Utils::log(BASIC, ("Smart shot. Ship alignement is known. Vertical alignement. Dynamic Coord -> MISSED. old increasedDirection = " + std::to_string(increasedDirection)) + ", old dynamicCoord = " + std::to_string(dynamicCoord));
                        
                        increasedDirection = !increasedDirection;
                        dynamicCoord = (int)damagedDeckers[0].y;
                        
                        Utils::log(BASIC, ("Smart shot. Ship alignement is known. Vertical alignement. Dynamic Coord -> MISSED. new increasedDirection = " + std::to_string(increasedDirection)) + ", new dynamicCoord = " + std::to_string(dynamicCoord));
                    }
                }
            }
            
            else
            {
                randomY = damagedDeckers[0].y;
                dynamicCoord = (int)damagedDeckers[0].x;
                
                Utils::log(BASIC, ("Smart shot. Ship alignement is known. Horizontal alignement. Initial Y " + std::to_string(randomY) + ", initial X " + std::to_string(dynamicCoord)));
                
                while (!dynamicCoordIsSet && dynamicCoord >= 0 && dynamicCoord <= 9)
                {
                    if (increasedDirection)
                        increment = 1;
                    else
                        increment = -1;
                    
                    dynamicCoord += increment;
                    
                    Utils::log(BASIC, ("Smart shot. Ship alignement is known. Horizontal alignement. Dynamic Coord = " + std::to_string(dynamicCoord)) + ", increasedDirection = " + std::to_string(increasedDirection));
                    
                    if (dynamicCoord >= 0 && dynamicCoord <= 9)
                    {
                        if (enemyPlayer->getBattleField()[randomY][dynamicCoord] == SHIP || enemyPlayer->getBattleField()[randomY][dynamicCoord] == EMPTY)
                        {
                            randomX = dynamicCoord;
                            dynamicCoordIsSet = true;
                            
                            Utils::log(BASIC, ("Smart shot. Ship alignement is known. Horizontal alignement. randomX is known: " + std::to_string(randomX)));
                        }
                        Utils::log(BASIC, ("Smart shot. Ship alignement is known. Horizontal alignement. Not SHIP or EMPTY. Exact status = " + std::to_string(enemyPlayer->getBattleField()[randomY][dynamicCoord])));
                    }
                    
                    if (!dynamicCoordIsSet && (dynamicCoord <= 0 || dynamicCoord >= 9))
                    {
                        Utils::log(BASIC, ("Smart shot. Ship alignement is known. Horizontal alignement. Dynamic Coord -> out of field. old increasedDirection = " + std::to_string(increasedDirection)) + ", old dynamicCoord = " + std::to_string(dynamicCoord));
                        
                        increasedDirection = !increasedDirection;
                        dynamicCoord = (int)damagedDeckers[0].x;
                        
                        Utils::log(BASIC, ("Smart shot. Ship alignement is known. Horizontal alignement. Dynamic Coord -> out of field. new increasedDirection = " + std::to_string(increasedDirection)) + ", new dynamicCoord = " + std::to_string(dynamicCoord));
                    }
                    else if (enemyPlayer->getBattleField()[randomY][dynamicCoord] == MISS)
                    {
                        Utils::log(BASIC, ("Smart shot. Ship alignement is known. Horizontal alignement. Dynamic Coord -> MISSED. old increasedDirection = " + std::to_string(increasedDirection)) + ", old dynamicCoord = " + std::to_string(dynamicCoord));
                        
                        increasedDirection = !increasedDirection;
                        dynamicCoord = (int)damagedDeckers[0].x;
                        
                        Utils::log(BASIC, ("Smart shot. Ship alignement is known. Horizontal alignement. Dynamic Coord -> MISSED. new increasedDirection = " + std::to_string(increasedDirection)) + ", new dynamicCoord = " + std::to_string(dynamicCoord));
                    }
                }
            }
            
            Utils::log(BASIC, ("Smart shot. Final coords(y.x): " + std::to_string(randomY) + "." + std::to_string(randomX)));
        }
    }
    
    else
    {
        do
        {
            randomX = Utils::randomInRange(0, 9);
            randomY = Utils::randomInRange(0, 9);
            
            Utils::log(BASIC, ("Smart shot. No damaged deckers. Trying to shot at coords(y.x): " + std::to_string(randomY) + "." + std::to_string(randomX)));
            
        }while (enemyPlayer->getBattleField()[randomY][randomX] == HIT || enemyPlayer->getBattleField()[randomY][randomX] == MISS);
        
        Utils::log(BASIC, ("Smart shot. No damaged deckers. Shot at coords(y.x): " + std::to_string(randomY) + "." + std::to_string(randomX)));
    }
    
    if (randomX < 0 || randomX > 9 || randomY < 0 || randomY > 9)
        Utils::log(ERR, "Incorrect random coords provided.");
    
    shot(randomX, randomY);
}

void BattleScene::hit(unsigned int x, unsigned int y)
{
    enemyPlayer->updateBattleField(y, x, HIT);
    Utils::log(BASIC, ("Hit on coords - Y: " + std::to_string(y) + ", X: " + std::to_string(x)));
    
    if(checkIfDestroed(enemyPlayer->getShipByDeckerCoords(y, x)))
    {
        enemyPlayer->getShipByDeckerCoords(y, x)->isAlive = false;
        renderFieldAction(x, y, DESTROYED);
        playSoundEffect(DESTROYED);
        boundariesOfShipDeckersDisable(enemyPlayer, enemyPlayer->getShipByDeckerCoords(y, x));
        
        if (currentPlayer->lastHitShip == enemyPlayer->getShipByDeckerCoords(y, x))
            currentPlayer->lastHitShip = nullptr;
    }
    
    else
    {
        renderFieldAction(x, y, HIT);
        playSoundEffect(HIT);
        currentPlayer->lastHitShip = enemyPlayer->getShipByDeckerCoords(y, x);
    }
    
    //Check if game is over
    gameIsOver = true;
    for (unsigned int i = 0; i < 10; i++)
    {
        if (enemyPlayer->getPlayerShips()[i]->isAlive)
            gameIsOver = false;
    }
    
    if (gameIsOver)
    {
        Utils::log(INFO, "Game is over.");
        if (currentPlayer == Actor::actorsPool[0])
            Utils::firstPlayerVictory = true;
        else
            Utils::firstPlayerVictory = false;
        
        gameOver();
    }
    else
        nextTurn();
}

void BattleScene::miss(unsigned int x, unsigned int y)
{
    enemyPlayer->updateBattleField(y, x, MISS);
    renderFieldAction(x, y, MISS);
    playSoundEffect(MISS);
    
    Utils::log(BASIC, ("Miss on coords - Y: " + std::to_string(y) + ", X: " + std::to_string(x)));
    
    changeCurrentPlayer();
    nextTurn();
}

bool BattleScene::checkIfDestroed(Ship* ship)
{
    int deckerY, deckerX;
    
    for (int i = 0; i < ship->getSize(); i++)
    {
        deckerY = ship->getDeckersCoordinates()[i][0];
        deckerX = ship->getDeckersCoordinates()[i][1];
        
        if (enemyPlayer->getBattleField()[deckerY][deckerX] == SHIP)
            return false;
    }
    
    Utils::log(BASIC, ("Ship destroyed on coords - deckerY: " + std::to_string(deckerY) + ", deckerX: " + std::to_string(deckerX)));
    
    return true;
}

void BattleScene::boundariesOfShipDeckersDisable(Actor* actor, Ship* ship)
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
        tempY = ship->getDeckersCoordinates()[m][0];
        tempX = ship->getDeckersCoordinates()[m][1];
        
        for (unsigned int i = 0; i < 3; i++)
        {
            for (unsigned int j = 0; j < 3; j++)
            {
                tempY += deckersBordersScanningIterators[i][j][0];
                tempX += deckersBordersScanningIterators[i][j][1];
                
                if (tempX >= 0 && tempX <=9 && tempY >= 0 && tempY <= 9)
                {
                    if (actor->getBattleField()[tempY][tempX] == EMPTY)
                    {
                        actor->updateBattleField(tempY, tempX, MISS);
                        renderFieldAction(tempX, tempY, MISS);
                    }
                }
                
                tempY = ship->getDeckersCoordinates()[m][0];
                tempX = ship->getDeckersCoordinates()[m][1];
            }
        }
    }
}

void BattleScene::enableEventListeners()
{
    Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(this);
}

void BattleScene::disableEventListeners()
{
    Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(this);
}

void BattleScene::nextTurn()
{
    if (!currentPlayer->isRealPlayer)
    {
        DelayTime* delayBetweenSteps = DelayTime::create(1.0);
        CallFunc* randomShotFunc = CallFunc::create(std::bind(&BattleScene::randomShot,this));
        CallFunc* disableEventListeners = CallFunc::create(std::bind(&BattleScene::disableEventListeners,this));
        CallFunc* enableEventListeners = CallFunc::create(std::bind(&BattleScene::enableEventListeners,this));
        Sequence* seq = Sequence::create(disableEventListeners, delayBetweenSteps, randomShotFunc, enableEventListeners, NULL);
        this->runAction(seq);
    }
    
    Utils::log(BASIC, "Next turn");
}

void BattleScene::renderFieldAction(unsigned int x, unsigned int y, unsigned int action)
{
    cocos2d::TMXTiledMap *field;
    if (currentPlayer == Actor::actorsPool[0])
        field = enemyField;
    else
        field = playerField;
    
    cocos2d::Vec2 spritePosition = Utils::convertToAbsoluteCoords(field, x, y);
    
    switch (action) {
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
            auto hit = Sprite::create("Effects/explosion_active.png");
            hit->setScale(0.5);
            hit->setPosition(spritePosition);
            this->addChild(hit, 0);
            
            enemyPlayer->getShipByDeckerCoords(y, x)->wreckedDeckers.push_back(hit);
            break;
        }
            
        case DESTROYED:
        {
            auto destroyedShip = enemyPlayer->getShipByDeckerCoords(y, x);
            
            for (auto wreckedDecker : destroyedShip->wreckedDeckers)
            {
                this->removeChild(wreckedDecker);
            }
            
            destroyedShip->updateSprite();
            destroyedShip->getShipSprite()->setOpacity(200);
            this->addChild(destroyedShip->getShipSprite());
            
            int deckerX, deckerY;
            
            for (unsigned int i = 0; i < destroyedShip->getSize(); i++)
            {
                deckerX = destroyedShip->getDeckersCoordinates()[i][1];
                deckerY = destroyedShip->getDeckersCoordinates()[i][0];
                
                auto destroyedDecker = Sprite::create("Effects/explosion_inactive.png");
                destroyedDecker->setScale(0.5);
                destroyedDecker->setOpacity(200);
                destroyedDecker->setPosition(Utils::convertToAbsoluteCoords(field, deckerX, deckerY));
                this->addChild(destroyedDecker, 0);
            }
            
            break;
        }
            
        default:
        {
            Utils::log(ERR, "Incorrect Field Action.");
            break;
        }
    }
}

void BattleScene::playSoundEffect(int action)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
    
    switch (action) {
        case MISS:
        {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Sounds/miss.wav");
            break;
        }
            
        case HIT:
        {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Sounds/hit.wav");
            break;
        }
            
        case DESTROYED:
        {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Sounds/destroyed.wav");
            break;
        }
            
            
        default:
        {
            Utils::log(ERR, "Incorrect action to play sound effect");
            break;
        }
    }
}

void BattleScene::changeCurrentPlayer()
{
    Actor* temp = currentPlayer;
    currentPlayer = enemyPlayer;
    enemyPlayer = temp;
    Utils::log(BASIC, "Player is changed.");
    if (currentPlayer == Actor::actorsPool[0])
        Utils::log(BASIC, "currentPlayer = Player1");
    else
        Utils::log(BASIC, "currentPlayer = Player2");
}

void BattleScene::openMenu()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Sounds/simple_click.wav");
    Utils::log(BASIC, "Open menu.");
    Director::getInstance()->pushScene(MenuScene::createScene());
}

void BattleScene::gameOver()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    
    if (Utils::firstPlayerVictory)
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Sounds/victory_music.mp3");
    else
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Sounds/defeat_music.mp3");
    
    CallFunc* renderGameOverSceneFunc = CallFunc::create(std::bind(&BattleScene::renderGameOverScene,this));
    Sequence* seq = Sequence::create(DelayTime::create(3.0), renderGameOverSceneFunc, NULL);
    
    this->runAction(seq);
}

void BattleScene::renderGameOverScene()
{
    Director::getInstance()->replaceScene(TransitionFade::create(1.0f, GameOverScene::createScene(), Color3B(0, 0, 0)));
}
