#include "PlacingShipsScene.h"

USING_NS_CC;

Scene* PlacingShipsScene::createScene()
{
    auto scene = Scene::create();
    auto layer = PlacingShipsScene::create();
    
    scene->setTag(321);
    scene->addChild(layer);
    
    return scene;
}

bool PlacingShipsScene::init()
{
    if ( !LayerColor::initWithColor(Color4B(0,0,0,255)) )
    {
        return false;
    }
    
    Utils::initLogFile();
    
    srand(time(0));
    placingShipsIsCompleted = false;
    
    //Params for createPlayers(bool isRealPlayer, bool isRealPlayer) - indicators of Real Player for 2 players.
    //True - if player is real, False - if AI.
    //Note, that setup with 2 real players is invalid.
    if (!createPlayers(true, false))
        return false;
    
    //Background setup
    visibleSize = Director::getInstance()->getVisibleSize();
    backgroundImage = Sprite::create("Field/background.png");
    backgroundImage->setScaleX(visibleSize.width / this->backgroundImage->getBoundingBox().size.width);
    backgroundImage->setScaleY(visibleSize.height / this->backgroundImage->getBoundingBox().size.height);
    backgroundImage->setPosition(visibleSize.width/2,visibleSize.height/2);
    this->addChild(backgroundImage);
    
    //Place Your Ships label
    auto placeYourShipsLabel = Label::createWithTTF("Place your Ships", "fonts/stencil_bt.ttf", 100);
    placeYourShipsLabel->setTextColor(Color4B(148,216,255,255));
    placeYourShipsLabel->setPosition(visibleSize.width / 3 + 85 , visibleSize.height - 100);
    this->addChild(placeYourShipsLabel);
    
    //Menu button
    menuButton = Sprite::create("Buttons/button_menu.png");
    menuButton->setPosition(visibleSize.width - 150, visibleSize.height - 100);
    menuButton->setScale(0.2);
    this->addChild(menuButton, 0);
    
    //Play button
    playButton = Sprite::create("Buttons/button_play.png");
    playButton->setPosition(visibleSize.width / 2, 100);
    playButton->setScale(0.3);
    playButton->setOpacity(0);
    this->addChild(playButton, 0);
    
    //TMX map
    field = TMXTiledMap::create("Field/tileMap_final.tmx");
    background = field->getLayer("Tile Layer 1");
    field->setScale(0.5);
    field->setAnchorPoint(Vec2(0.5,0.5));
    field->setPosition(visibleSize.width/4,visibleSize.height/2.1);
    this->addChild(field, 0);
    
    //Markup
    auto markup = Sprite::create("Field/markup.png");
    markup->setScale(0.5);
    markup->setPosition(visibleSize.width/4 - 24,visibleSize.height/2.1 + 24);
    this->addChild(markup, 0);
    
    //Tooltips setup
    auto fadeInLabel = FadeIn::create(1.5);
    auto fadeOutLabel = FadeOut::create(1.5);
    
    //Drag and Drop tooltip
    dragAndDropTooltip = Label::createWithTTF("Drag&Drop available ships to the field via Mouse", "fonts/stencil_bt.ttf", 30);
    dragAndDropTooltip->setTextColor(Color4B(148,216,255,80));
    dragAndDropTooltip->setOpacity(0);
    dragAndDropTooltip->setPosition(visibleSize.width / 2, 100);
    dragAndDropLabelSequence = Sequence::create(DelayTime::create(1),fadeInLabel,DelayTime::create(3),fadeOutLabel,nullptr);
    this->addChild(dragAndDropTooltip);
    
    //Rotate ships tooltip
    rotateSpaceTooltip = Label::createWithTTF("Press \"Space\" to rotate your ship while dragging to the field", "fonts/stencil_bt.ttf", 30);
    rotateSpaceTooltip->setTextColor(Color4B(148,216,255,80));
    rotateSpaceTooltip->setOpacity(0);
    rotateSpaceTooltip->setPosition(visibleSize.width / 2, 100);
    rotateSpaceLabelSequence = Sequence::create(DelayTime::create(7),fadeInLabel->clone(),DelayTime::create(3),fadeOutLabel->clone(),nullptr);
    this->addChild(rotateSpaceTooltip);
    
    //Auto-placing ships tooltip
    autoPlaceTooltip = Label::createWithTTF("Press \"R\" to Place your ships randomly", "fonts/stencil_bt.ttf", 30);
    autoPlaceTooltip->setTextColor(Color4B(148,216,255,80));
    autoPlaceTooltip->setOpacity(0);
    autoPlaceTooltip->setPosition(visibleSize.width / 2, 100);
    autoPlaceLabelSequence = Sequence::create(DelayTime::create(13),fadeInLabel->clone(),DelayTime::create(2),fadeOutLabel->clone(),nullptr);
    this->addChild(autoPlaceTooltip);
    
    dragAndDropTooltip->runAction(dragAndDropLabelSequence);
    rotateSpaceTooltip->runAction(rotateSpaceLabelSequence);
    autoPlaceTooltip->runAction(autoPlaceLabelSequence);
    
    //Sizes setup
    deckerSize = 40;
    initialWidth = visibleSize.width/2 + 100;
    initialHeight = visibleSize.height/2 + deckerSize*2;
    
    //Available ships label
    auto availableShipsLabel = Label::createWithTTF("Available Ships:", "fonts/stencil_bt.ttf", 30);
    availableShipsLabel->setTextColor(Color4B(148,216,255,255));
    availableShipsLabel->setPosition(initialWidth + availableShipsLabel->getBoundingBox().size.width/2, initialHeight + deckerSize*2);
    this->addChild(availableShipsLabel);
    
    //Ships and Ships Placeholders sprites
    setShipsPlaceholdersSpritesInitialPos();
    setShipsSpritesInitialPos();
    
    for (unsigned int i = 0; i < 10; i++)
    {
        this->addChild(actor1->getPlayerShips()[i]->getShipPlaceholderSprite());
        this->addChild(actor1->getPlayerShips()[i]->getShipSprite());
    }
    
    Utils::log(INFO, "Placing player ships UI is completed.");
    
    //Auto-setup for AI
    for (auto actor : Actor::actorsPool)
    {
        if (!actor->isRealPlayer)
            placeShipsRandomly(actor);
    }
    
    //AI vs AI battle
    if (!Actor::actorsPool[0]->isRealPlayer && !Actor::actorsPool[1]->isRealPlayer)
    {
        std::cout << "\n\n\nHello!\n\n\n" << std::endl;
        
        DelayTime* delayBetweenSteps = DelayTime::create(0);
        CallFunc* startBattleFunc = CallFunc::create(std::bind(&PlacingShipsScene::startBattleWithoutTransition,this));
        Sequence* seq = Sequence::create(delayBetweenSteps, startBattleFunc, NULL);
        this->runAction(seq);
    }
    
    
    //Event listeners
    auto mouseClickListener = EventListenerMouse::create();
    
    mouseClickListener->onMouseDown = [this](cocos2d::Event* event)
    {
        EventMouse* mouseDown = dynamic_cast<EventMouse*>(event);
        triggerDragAndDropShip(actor1, mouseDown);
    };
    
    mouseClickListener->onMouseMove = [this](cocos2d::Event *event)
    {
        EventMouse* mouseMove = dynamic_cast<EventMouse*>(event);
        
        if (dragAndDropShip != nullptr)
        {
            dragAndDropShip->getShipSprite()->setPosition(mouseMove->getLocationInView());
        }
    };
    
    mouseClickListener->onMouseUp = [this](cocos2d::Event* event)
    {
        EventMouse* mouseUp = dynamic_cast<EventMouse*>(event);
        
        //Click on Menu button
        if (this->menuButton->getBoundingBox().containsPoint(mouseUp->getLocationInView()))
            openMenu();
        
        //Click on Play after ships placing
        else if (placingShipsIsCompleted && this->playButton->getBoundingBox().containsPoint(mouseUp->getLocationInView()))
            startBattle();
        
        else
        {
            //Ship is triggered by mouseDown
            if (dragAndDropShip != nullptr)
                dragAndDropOnField(actor1, mouseUp);
        }
    };
    
    auto keyboardClickListener = EventListenerKeyboard::create();
    keyboardClickListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event)
    {
        if (keyCode == EventKeyboard::KeyCode::KEY_SPACE && dragAndDropShip != nullptr)
            rotateShipSprite(dragAndDropShip);
        
        else if (keyCode == EventKeyboard::KeyCode::KEY_R)
        {
            DelayTime* delayBetweenRandomCall = DelayTime::create(0.5);
            CallFunc* randomPlacingFunc = CallFunc::create(std::bind(&PlacingShipsScene::randomPlacingShipsForRealPlayer,this));
            CallFunc* disableEventListeners = CallFunc::create(std::bind(&PlacingShipsScene::disableEventListeners,this));
            CallFunc* enableEventListeners = CallFunc::create(std::bind(&PlacingShipsScene::enableEventListeners,this));
            Sequence* seq = Sequence::create(disableEventListeners, delayBetweenRandomCall, randomPlacingFunc, enableEventListeners, NULL);
            this->runAction(seq);
        }
        
        else if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
            openMenu();
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseClickListener, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardClickListener, this);
    
    return true;
}

bool PlacingShipsScene::createPlayers(bool isRealPlayer1, bool isRealPlayer2)
{
    if (isRealPlayer1 && isRealPlayer2)
    {
        Utils::log(ERR, "Attempt to start game with > 1 player");
        return false;
    }
    
    //actor1 will be a Real Player. Also it can be AI in case all players are AI
    if (isRealPlayer2)
    {
        actor1 = new Actor(true);
        actor2 = new Actor(false);
    }
    else
    {
        actor1 = new Actor(isRealPlayer1);
        actor2 = new Actor(isRealPlayer2);
    }
    
    actor1->setEnemyBattleField(actor2->getBattleField());
    actor2->setEnemyBattleField(actor1->getBattleField());
    
    Utils::log(BASIC, "Players are created.");
    Utils::log(BASIC, ("Player1 is Real: " + std::to_string(actor1->isRealPlayer)));
    Utils::log(BASIC, ("Player2 is Real: " + std::to_string(actor2->isRealPlayer)));
    
    return true;
}

void PlacingShipsScene::setShipsPlaceholdersSpritesInitialPos()
{
    actor1->getPlayerShips()[0]->getShipPlaceholderSprite()->setPosition(initialWidth + actor1->getPlayerShips()[0]->getShipPlaceholderSprite()->getBoundingBox().size.width/2, initialHeight);
    actor1->getPlayerShips()[1]->getShipPlaceholderSprite()->setPosition(initialWidth + actor1->getPlayerShips()[1]->getShipPlaceholderSprite()->getBoundingBox().size.width/2, initialHeight - deckerSize*2);
    actor1->getPlayerShips()[2]->getShipPlaceholderSprite()->setPosition(initialWidth + actor1->getPlayerShips()[2]->getShipPlaceholderSprite()->getBoundingBox().size.width/2 + actor1->getPlayerShips()[2]->getShipPlaceholderSprite()->getBoundingBox().size.width + deckerSize, initialHeight - deckerSize*2);
    actor1->getPlayerShips()[3]->getShipPlaceholderSprite()->setPosition(initialWidth + actor1->getPlayerShips()[3]->getShipPlaceholderSprite()->getBoundingBox().size.width/2, initialHeight - deckerSize*4);
    actor1->getPlayerShips()[4]->getShipPlaceholderSprite()->setPosition(initialWidth + actor1->getPlayerShips()[4]->getShipPlaceholderSprite()->getBoundingBox().size.width/2 + actor1->getPlayerShips()[4]->getShipPlaceholderSprite()->getBoundingBox().size.width + deckerSize, initialHeight - deckerSize*4);
    actor1->getPlayerShips()[5]->getShipPlaceholderSprite()->setPosition(initialWidth + actor1->getPlayerShips()[4]->getShipPlaceholderSprite()->getBoundingBox().size.width/2 + actor1->getPlayerShips()[4]->getShipPlaceholderSprite()->getBoundingBox().size.width*2 + deckerSize*2, initialHeight - deckerSize*4);
    actor1->getPlayerShips()[6]->getShipPlaceholderSprite()->setPosition(initialWidth + deckerSize/2, initialHeight - deckerSize*6);
    actor1->getPlayerShips()[7]->getShipPlaceholderSprite()->setPosition(initialWidth + deckerSize *2.5, initialHeight - deckerSize*6);
    actor1->getPlayerShips()[8]->getShipPlaceholderSprite()->setPosition(initialWidth + deckerSize *4.5, initialHeight - deckerSize*6);
    actor1->getPlayerShips()[9]->getShipPlaceholderSprite()->setPosition(initialWidth + deckerSize *6.5, initialHeight - deckerSize*6);
    
    float coordX, coordY;
    for (unsigned int i = 0; i < 10; i++)
    {
        coordX = actor1->getPlayerShips()[i]->getShipPlaceholderSprite()->getPosition().x + visibleSize.width/2;
        coordY = actor1->getPlayerShips()[i]->getShipPlaceholderSprite()->getPosition().y;
        actor2->getPlayerShips()[i]->getShipPlaceholderSprite()->setPosition(Vec2(coordX, coordY));
    }
    
    Utils::log(BASIC, "Ships placeholders Sprites are set.");
}

void PlacingShipsScene::setShipsSpritesInitialPos()
{
    for (unsigned int i = 0; i < 10; i++)
    {
        Actor::actorsPool[0]->getPlayerShips()[i]->getShipSprite()->setPosition(Actor::actorsPool[0]->getPlayerShips()[i]->getShipPlaceholderSprite()->getPosition());
        Actor::actorsPool[1]->getPlayerShips()[i]->getShipSprite()->setPosition(Actor::actorsPool[1]->getPlayerShips()[i]->getShipPlaceholderSprite()->getPosition());
    }
    
    Utils::log(BASIC, "Ships Sprites are set.");
}

void PlacingShipsScene::triggerDragAndDropShip(Actor *actor, cocos2d::EventMouse* mouseDown)
{
    for (int i = 0; i < 10; i++)
    {
        if (actor->getPlayerShips()[i]->getShipSprite()->getBoundingBox().containsPoint(mouseDown->getLocationInView()))
        {
            dragAndDropShip = actor->getPlayerShips()[i];
            if (dragAndDropShip->isPlaced)
            {
                //Remove already placed ship from field via drag&drop
                for(unsigned int i = 0; i < dragAndDropShip->getSize(); i++)
                {
                    actor->updateBattleField(dragAndDropShip->getDeckersCoordinates()[i][0], dragAndDropShip->getDeckersCoordinates()[i][1], EMPTY);
                }
                dragAndDropShip->isPlaced = false;
            }
            
            break;
        }
    }
    
    Utils::log(BASIC, ("triggerDragAndDropShip()"));
}

void PlacingShipsScene::dragAndDropOnField(Actor* actor, cocos2d::EventMouse* mouseUp)
{
    if (field->getBoundingBox().containsPoint(mouseUp->getLocationInView()))
    {
        //Place ship on field
        float clickCoordsX = mouseUp->getLocation().x - field->getBoundingBox().getMinX();
        float clickCoordsY = mouseUp->getLocation().y - (Director::getInstance()->getVisibleSize().height - field->getBoundingBox().getMaxY());
        
        Utils::log(BASIC, "dragAndDropOnField");
        Utils::log(BASIC, ("Place ship on field. clickCoordsX: " + std::to_string(clickCoordsX) + ", clickCoordsY: " + std::to_string(clickCoordsY)));
        
        //Tech variable for proper shipSprite alignment.
        //True if mouseUp tile was triggered to the right of the midX for horizontally-oriented shipSprite
        //Or to the bottom for vertically-oriented shipSprite
        bool shiftToIncreasedCoordinate = false;
        
        if (dragAndDropShip->spriteAlignedVertically)
        {
            if ((mouseUp->getLocation().y - (Director::getInstance()->getVisibleSize().height - field->getBoundingBox().getMaxY())) / 40 - (int)((mouseUp->getLocation().y - (Director::getInstance()->getVisibleSize().height - field->getBoundingBox().getMaxY())) / 40) >= 0.5)
                shiftToIncreasedCoordinate = true;
        }
        else
        {
            if ((mouseUp->getLocation().x - field->getBoundingBox().getMinX()) / 40 - (int)((mouseUp->getLocation().x - field->getBoundingBox().getMinX()) / 40) >= 0.5)
                shiftToIncreasedCoordinate = true;
        }
        
        //Validation and placement
        if(shipIsPlacedInTheFieldValidation(dragAndDropShip, round(clickCoordsX), round(clickCoordsY)))
        {
            dragAndDropShip->setDraggingShipDeckers(clickCoordsX/40, clickCoordsY/40);
            
            if (actor->boundariesOfShipDeckersCheck(dragAndDropShip))
            {
                //Remove old position of replaced ship
                if (dragAndDropShip->isPlaced)
                {
                    for(unsigned int i = 0; i < dragAndDropShip->getSize(); i++)
                    {
                        actor->updateBattleField(dragAndDropShip->getDeckersCoordinates()[i][0], dragAndDropShip->getDeckersCoordinates()[i][1], EMPTY);
                    }
                }
                
                //Placing ship
                dragAndDropShip->placeShip(dragAndDropShip->placeShipDeckersCoordinates);
                for(unsigned int i = 0; i < dragAndDropShip->getSize(); i++)
                {
                    actor->updateBattleField(dragAndDropShip->placeShipDeckersCoordinates[i][0], dragAndDropShip->placeShipDeckersCoordinates[i][1], SHIP);
                }
                
                dragAndDropShip->getShipSprite()->setPosition(mouseUp->getLocationInView());
                dragAndDropShip->setShipSpriteMidPosition(getAlignedPosition(dragAndDropShip, clickCoordsX/40, clickCoordsY/40));
                alignShipSprite(dragAndDropShip);
                dragAndDropShip->setRotationValue(dragAndDropShip->getShipSprite()->getRotation());
                
                Utils::log(BASIC, ("Placed ship with size " + std::to_string(dragAndDropShip->getSize())));
            }
            
            else
                moveShipSpriteToInitialPosition(dragAndDropShip);
            
            dragAndDropShip->freeDraggingShipDeckersMemory();
            
            //If all ships are placed
            checkPlacingShipsCompletion(actor1);
        }
        
        //Validation failed - remove ship
        else
            moveShipSpriteToInitialPosition(dragAndDropShip);
        
        dragAndDropShip = nullptr;
    }
    
    //Remove already placed ship from field via drag&drop
    else
    {
        for(unsigned int i = 0; i < dragAndDropShip->getSize(); i++)
        {
            actor->updateBattleField(dragAndDropShip->getDeckersCoordinates()[i][0], dragAndDropShip->getDeckersCoordinates()[i][1], EMPTY);
        }
        
        Utils::log(BASIC, ("Remove already placed ship from field via drag&drop. Ship with size " + std::to_string(dragAndDropShip->getSize())));
        
        moveShipSpriteToInitialPosition(dragAndDropShip);
        dragAndDropShip->isPlaced = false;
        dragAndDropShip = nullptr;
        
        checkPlacingShipsCompletion(actor1);
    }
}

void PlacingShipsScene::rotateShipSprite(Ship* ship)
{
    if (ship->getShipSprite()->getRotation() == 270)
        ship->getShipSprite()->setRotation(0);
    else
        ship->getShipSprite()->setRotation(ship->getShipSprite()->getRotation() + 90);
    
    ship->spriteAlignedVertically = !(ship->spriteAlignedVertically);
    
    Utils::log(BASIC, ("Rotate ship with size " + std::to_string(ship->getSize())));
}

void PlacingShipsScene::moveShipSpriteToInitialPosition(Ship* ship)
{
    auto moveTo = MoveTo::create(0.5f, ship->getShipPlaceholderSprite()->getPosition());
    if (ship->getShipSprite()->getRotation() != 90)
    {
        auto rotateTo = RotateTo::create(0.5f, 90);
        ship->getShipSprite()->runAction(rotateTo);
    }
    ship->getShipSprite()->runAction(moveTo);
    ship->spriteAlignedVertically = false;
    
    Utils::log(BASIC, ("moveShipSpriteToInitialPosition. Ship with size " + std::to_string(ship->getSize())));
}

void PlacingShipsScene::changeRandomPosition(cocos2d::Sprite* shipSprite, cocos2d::Vec2 newPosition, unsigned int newRotationValue)
{
    auto moveTo = MoveTo::create(0.5f, newPosition);
    auto rotateTo = RotateTo::create(0.5f, newRotationValue);
    shipSprite->runAction(rotateTo);
    shipSprite->runAction(moveTo);
    
    Utils::log(BASIC, "changeRandomPosition.");
}

bool PlacingShipsScene::shipIsPlacedInTheFieldValidation(Ship* ship, float targetX, float targetY)
{
    if (ship->spriteAlignedVertically && (targetY/40 - 0.5*(ship->getSize() - 1) < 0 || targetY/40 + 0.5*(ship->getSize() - 1) >= 10 ))
        return false;
    
    else if (!ship->spriteAlignedVertically && (targetX/40 - 0.5*(ship->getSize() - 1) < 0 || targetX/40 + 0.5*(ship->getSize() - 1) >= 10 ))
        return false;
    
    else
        return true;
}

cocos2d::Vec2 PlacingShipsScene::getAlignedPosition(Ship* ship, float targetX, float targetY)
{
    float alignedX;
    float alignedY;
    
    if (ship->getSize() == 1 || ship->getSize() == 3)
    {
        alignedX = field->getBoundingBox().getMinX() + floor(targetX) * 40 + 20;
        alignedY = field->getBoundingBox().getMinY() + abs(floor(targetY) - 9) * 40 + 20;
    }
    
    else if (ship->getSize() == 2 || ship->getSize() == 4)
    {
        if (ship->spriteAlignedVertically)
        {
            alignedX = field->getBoundingBox().getMinX() + floor(targetX) * 40 + 20;
            
            if (targetY - (int)targetY > 0.5)
                alignedY = field->getBoundingBox().getMinY() + abs(floor(targetY) - 9) * 40;
            else
                alignedY = field->getBoundingBox().getMinY() + abs(floor(targetY) - 9) * 40 + 40;
            
            Utils::log(BASIC, ("getAlignedPosition. Ship with size " + std::to_string(ship->getSize()) + ". Vertical alignement. alignedX = " + std::to_string(alignedX) + ", state: " + std::to_string(targetY - (int)targetY) + ", alignedY = " + std::to_string(alignedY)));
        }
        
        else
        {
            alignedY = field->getBoundingBox().getMinY() + abs(floor(targetY) - 9) * 40 + 20;
            
            if (targetX - (int)targetX > 0.5)
                alignedX = field->getBoundingBox().getMinX() + floor(targetX) * 40 + 40;
            else
                alignedX = field->getBoundingBox().getMinX() + floor(targetX) * 40;
            
            Utils::log(BASIC, ("getAlignedPosition. Ship with size " + std::to_string(ship->getSize()) + ". Horizontal alignement. alignedY = " + std::to_string(alignedY) + ", state: " + std::to_string(targetX - (int)targetX) + ", alignedY = " + std::to_string(alignedX)));
        }
    }
    
    else
        Utils::log(ERR, "getAlignedPosition");
    
    return cocos2d::Vec2(alignedX,alignedY);
}

void PlacingShipsScene::alignShipSprite(Ship* ship)
{
    auto moveTo = MoveTo::create(0.3f, ship->getShipSpriteMidPosition());
    ship->getShipSprite()->runAction(moveTo);
    Utils::log(BASIC, ("alignShipSprite. Ship with size " + std::to_string(ship->getSize())));
}

void PlacingShipsScene::placeShipsRandomly(Actor *actor)
{
    float randomX;
    float randomY;
    bool randomRotation;
    unsigned int randomRotationValue;
    bool randomShift;
    
    //Defaults
    placingShipsIsCompleted = false;
    hidePlayButton();
    
    for (unsigned int i = 0; i < 10; i++)
    {
        actor->getPlayerShips()[i]->isPlaced = false;
        actor->getPlayerShips()[i]->spriteAlignedVertically = false;
        
        for (unsigned int j = 0; j < 10; j++)
        {
            actor->getBattleField()[i][j] = EMPTY;
        }
    }
    
    //Placing ships randomly
    for (unsigned int i = 0; i < 10; i++)
    {
        while (!actor->getPlayerShips()[i]->isPlaced)
        {
            randomX = Utils::randomInRange(0, 9);
            randomY = Utils::randomInRange(0, 9);
            randomRotation = Utils::randomInRange(0, 1);
            randomShift = Utils::randomInRange(0, 1);
            
            if (randomRotation == 1)
                actor->getPlayerShips()[i]->spriteAlignedVertically = true;
            
            if (randomShift)
            {
                if (actor->getPlayerShips()[i]->spriteAlignedVertically)
                    randomY = (int)randomY + 0.75;
                else
                    randomX = (int)randomX + 0.75;
            }
            else
            {
                if (actor->getPlayerShips()[i]->spriteAlignedVertically)
                    randomY = (int)randomY + 0.25;
                else
                    randomX = (int)randomX + 0.25;
            }
            
            if(shipIsPlacedInTheFieldValidation(actor->getPlayerShips()[i], randomX*40, randomY*40))
            {
                actor->getPlayerShips()[i]->setDraggingShipDeckers(randomX, randomY);
                
                if(actor->boundariesOfShipDeckersCheck(actor->getPlayerShips()[i]))
                {
                    actor->getPlayerShips()[i]->placeShip(actor->getPlayerShips()[i]->placeShipDeckersCoordinates);
                    
                    for(unsigned int j = 0; j < actor->getPlayerShips()[i]->getSize(); j++)
                    {
                        actor->updateBattleField(actor->getPlayerShips()[i]->placeShipDeckersCoordinates[j][0], actor->getPlayerShips()[i]->placeShipDeckersCoordinates[j][1], SHIP);
                    }
                    
                        actor->getPlayerShips()[i]->setShipSpriteMidPosition(getAlignedPosition(actor->getPlayerShips()[i], randomX, randomY));
                    
                    if (actor == Actor::actorsPool[1])
                        actor->getPlayerShips()[i]->setShipSpriteMidPosition(Vec2(actor->getPlayerShips()[i]->getShipSpriteMidPosition().x + visibleSize.width/2, actor->getPlayerShips()[i]->getShipSpriteMidPosition().y));
                    
                    //Ship random rotation
                    if (actor->getPlayerShips()[i]->spriteAlignedVertically)
                    {
                        if (randomRotation)
                            randomRotationValue = 0;
                        else
                            randomRotationValue = 180;
                    }
                    else
                    {
                        if (randomRotation)
                            randomRotationValue = 90;
                        else
                            randomRotationValue = 270;
                    }
                    
                    //Set position for real player ship sprite
                    if (actor->isRealPlayer)
                    {
                        changeRandomPosition(actor->getPlayerShips()[i]->getShipSprite(),actor->getPlayerShips()[i]->getShipSpriteMidPosition(),randomRotationValue);
                    }
                }
                actor->getPlayerShips()[i]->freeDraggingShipDeckersMemory();
            }
        }
        
        Utils::log(BASIC, ("placeShipsRandomly. Place ship with size " + std::to_string(actor->getPlayerShips()[i]->getSize())));
        actor->getPlayerShips()[i]->setRotationValue(randomRotationValue);
    }
    
    if (actor->isRealPlayer)
    {
        placingShipsIsCompleted = true;
        showPlayButton();
    }
    
    Utils::log(INFO, "Random placing ships is completed");
    if (actor->isRealPlayer)
        Utils::log(INFO, "Real Player.");
    else
        Utils::log(INFO, "AI.");
}

void PlacingShipsScene::randomPlacingShipsForRealPlayer()
{
    placeShipsRandomly(actor1);
}

void PlacingShipsScene::showPlayButton()
{
    playButton->setOpacity(255);
    dragAndDropTooltip->stopAction(dragAndDropLabelSequence);
    rotateSpaceTooltip->stopAction(rotateSpaceLabelSequence);
    autoPlaceTooltip->stopAction(autoPlaceLabelSequence);
    dragAndDropTooltip->setOpacity(0);
    rotateSpaceTooltip->setOpacity(0);
    autoPlaceTooltip->setOpacity(0);
    
    //Log battle ships
    Utils::log(INFO, "Completed battle fields.");
    
    std::string logString = "";
    for(int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (Actor::actorsPool[0]->getBattleField()[i][j]-100 == 0)
                logString += "-";
            else
                logString += "O";
        }
        Utils::log(BASIC, logString);
        logString = "";
    }
    
    Utils::log(BASIC, "\n");
    for(int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (Actor::actorsPool[1]->getBattleField()[i][j]-100 == 0)
                logString += "-";
            else
            {
                logString += "O";
            }
        }
        Utils::log(BASIC, logString);
        logString = "";
    }
}

void PlacingShipsScene::hidePlayButton()
{
    playButton->setOpacity(0);
}

void PlacingShipsScene::checkPlacingShipsCompletion(Actor* actor)
{
    for (unsigned int i = 0; i < 10; i++)
    {
        if (actor->getPlayerShips()[i]->isPlaced)
            placingShipsIsCompleted = true;
        else
        {
            placingShipsIsCompleted = false;
            hidePlayButton();
            break;
        }
    }
    
    if (placingShipsIsCompleted == true)
    {
        Utils::log(INFO, "Player ships placing is completed.");
        showPlayButton();
    }
}

void PlacingShipsScene::startBattleWithoutTransition()
{
    Utils::log(BASIC, "Start Battle AI vs AI.");
    Director::getInstance()->replaceScene(BattleScene::createScene());
}

void PlacingShipsScene::startBattle()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Sounds/artillery_load.wav");
    Utils::log(BASIC, "Start Battle.");
    Director::getInstance()->replaceScene(TransitionFade::create(3.0f, BattleScene::createScene(), Color3B(0, 0, 0)));
}

void PlacingShipsScene::openMenu()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Sounds/simple_click.wav");
    Utils::log(BASIC, "Open menu.");
    Director::getInstance()->pushScene(MenuScene::createScene());
}

void PlacingShipsScene::enableEventListeners()
{
    Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(this);
}

void PlacingShipsScene::disableEventListeners()
{
    Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(this);
}

/*
 Handle clicks:
 this->menuButton->getBoundingBox().containsPoint(mouseEvent->getLocationInView())
 event->getCurrentTarget()->getBoundingBox().containsPoint(mouseEvent->getLocationInView())
*/

/*
 getTileGIDAt works with 0.0 up to 9.9
 int mouseUpTileId = background->getTileGIDAt(Vec2(round(clickCoordsX) / 40, round(clickCoordsY) / 40));
 int convertedCoordX = Utils::convertTileIdToCoords(mouseUpTileId).x;
 int convertedCoordY = Utils::convertTileIdToCoords(mouseUpTileId).y;
*/
