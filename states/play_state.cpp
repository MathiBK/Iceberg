#include "play_state.h"
#include "../abilities/grapplingHook.h"
#include "death_state.h"
#include "pause_state.h"
#include "../abilities/basic_attack.h"
#include "../abilities/none_ability.h"


#include <memory>
#include <iostream>
#include <SFML/Window.hpp>

static const float VIEW_HEIGHT = 900.f;
static const float VIEW_WIDTH = 1600.f;

playState::playState (StateMachine *stateMachine) : BaseState(stateMachine)
{
    this->saveManager = &this->stateMachine->saveManager;
    this->soundManager = &this->stateMachine->soundmgr;
    this->texManager = &this->stateMachine->texManager;

    map.loadFromFile("data/map_iceberg_test.json");

    std::copy(map.getLayers().begin(), map.getLayers().end(), std::back_inserter(layers));
    std::copy(map.getSprites().begin(), map.getSprites().end(), std::back_inserter(objects));

    this->zoomLevel = 1.0f;

    sf::Vector2f pos = sf::Vector2f(this->stateMachine->renderWindow.getSize());
    this->uiView.setSize(pos);
    this->gameView.setSize(pos);

    this->uiView.setCenter(pos);
    gameView.setCenter(pos);

    this->stateMachine->renderWindow.setView(gameView);

    resizeView(this->stateMachine->renderWindow, gameView, true);
    resizeView(this->stateMachine->renderWindow, uiView, false);
    sf::Vector2f centre(this->map.getWidth() * 0.5, this->map.getHeight() * 0.5);
    centre *= float(this->map.getTileWidth());

    closeCP = 0;

    for(auto &e : map.getSprites()){
        if(e->name == "spawn"){
            if (saveManager->m_checkPos.x == 0 && saveManager->m_checkPos.y == 0){
                saveManager->m_checkPos.x = e->x;
                saveManager->m_checkPos.y = e->y;
            }
            player = new Player(map, saveManager->m_checkPos.x, saveManager->m_checkPos.y, this->stateMachine->soundmgr, saveManager->m_enemiesDiedTooId, this->texManager->getRef("player"));
            player->setXp(saveManager->m_xp);
            player->setLvl(saveManager->m_lvl);
            activeEntities.push_back(player);
        }
        if(e->name == "cp"){
            checkPoints.push_back(new checkpoint(map, e->x, e->y));
            if(e->x == saveManager->m_checkPos.x && e->y == saveManager->m_checkPos.y){
                (*--checkPoints.end())->bought = true;
                (*--checkPoints.end())->texture.loadFromFile("media/capCP.png");
            }
        }
        if(e->name == "slime") {
            entities.push_back(new Slime(map, e->x, e->y, stateMachine->soundmgr));
        }
        if(e->name == "rSlime"){
            entities.push_back(new redSlime(map, e->x, e->y, stateMachine->soundmgr));
        }
        if(e->name == "gSlime"){
            entities.push_back(new greenSlime(map, e->x, e->y, stateMachine->soundmgr));
        }
        if(e->name == "bSlime"){
            entities.push_back(new bigSlime(map, e->x, e->y, stateMachine->soundmgr));
        }
        if(e->name == "skeleton"){
            entities.push_back(new Skeleton(map, e->x, e->y, stateMachine->soundmgr, *player));
        }
    }

    std::string q = saveManager->m_abilitiesConfigured.at("q");

    for(const auto& i : saveManager->m_abilitiesConfigured)
    {
        if(i.second == "Dash")
            player->activeAbilities.insert(std::pair<std::string, ability*>(i.first, new dash(player)));
        else if(i.second == "Grappling Hook")
            player->activeAbilities.insert(std::pair<std::string, ability*>(i.first, new grapplingHook(*this, player, soundManager->getRef("cannon"))));
        else if(i.second == "Sword")
            player->activeAbilities.insert(std::pair<std::string, ability*>(i.first, new BasicAttack(player, activeEntities)));

        else
            player->activeAbilities.insert(std::pair<std::string, ability*>(i.first, new NoneAbility(player)));
    }
    auto sd = new dash(player);
    auto s = new grapplingHook(*this, player, this->stateMachine->soundmgr.getRef("cannon"));
    auto basicAttack = new BasicAttack(player, activeEntities);

    abilites.push_back(sd);
    abilites.push_back(s);
    abilites.push_back(basicAttack);

    pos *= 0.5f;
    this->uiView.setCenter(pos);
    //gameView.zoom(0.5f);

    o = OnScreenObject("Press B to buy Checkpoint\n                 100 gold", sf::Color::Black, player->rectangle.getPosition().x-28, player->rectangle.getPosition().y - 20, 50);
    o.setScale(0.1f);

    goldText = OnScreenObject("0", sf::Color(0xd4, 0xaf, 0x37), player->rectangle.getPosition().x-28, player->rectangle.getPosition().y - 20, 50);
    goldText.setScale(1.f);
    goldText.loadFont("media/fonts/VCR_OSD_MONO_1.001.ttf");
    goldText.setOrigin(sf::Vector2f(0,0));

    abilityBox = sf::RectangleShape(sf::Vector2f(64, 64));
    abilityBox.setOutlineThickness(3);
    abilityBox.setOutlineColor(sf::Color::White);
    abilityBox.setFillColor(sf::Color::Transparent);

    qAbility.setTexture(stateMachine->texManager.getRef(player->activeAbilities.at("q")->id));
    wAbility.setTexture(stateMachine->texManager.getRef(player->activeAbilities.at("w")->id));
    eAbility.setTexture(stateMachine->texManager.getRef(player->activeAbilities.at("e")->id));


    qAbility.setScale(abilityBox.getSize().x/qAbility.getTexture()->getSize().x, abilityBox.getSize().y/qAbility.getTexture()->getSize().y);
    wAbility.setScale(abilityBox.getSize().x/wAbility.getTexture()->getSize().x, abilityBox.getSize().y/wAbility.getTexture()->getSize().y);
    eAbility.setScale(abilityBox.getSize().x/eAbility.getTexture()->getSize().x, abilityBox.getSize().y/eAbility.getTexture()->getSize().y);
    gameView.setCenter(player->getPosition());
    uiView.setCenter(player->getPosition());
    this->stateMachine->renderWindow.setView(gameView);



    resizeView(this->stateMachine->renderWindow, gameView, true);
    resizeView(this->stateMachine->renderWindow, uiView, false);
    gameView.zoom(0.2f);
}



void playState::resizeView(const sf::RenderWindow& window, sf::View& view, bool zoom)
{
    float aspectRatio = float(window.getSize().x) / float(window.getSize().y);

    sf::Vector2f prevSize = view.getSize();

    view.setSize(VIEW_HEIGHT * aspectRatio, VIEW_HEIGHT);
    if(zoom)
        view.zoom(prevSize.x/view.getSize().x);

}



void playState::draw(const float deltaTime)
{

    this->stateMachine->renderWindow.clear(sf::Color(129, 161, 188));

    this->stateMachine->renderWindow.setView(this->gameView);

    for (auto& layer: layers)
    {
        layer->draw(this->stateMachine->renderWindow, player->rectangle.getPosition());

    }

    for (auto& object: objects)
    {
        object->process(deltaTime);
        object->draw(this->stateMachine->renderWindow);

    }

    for (auto& ento: activeEntities)
    {
        ento->process(deltaTime);
        ento->draw(this->stateMachine->renderWindow);
    }

    for (auto& cp: checkPoints)
    {
        cp->process(deltaTime);
        cp->draw(this->stateMachine->renderWindow);

    }

    if(closeCP >= 0) {
       o.setPos(player->rectangle.getPosition().x-28, player->rectangle.getPosition().y - 20);
       o.setOrigin(sf::Vector2f(o.getBounds().width/2.f, o.getBounds().height/2.f));
       o.draw(this->stateMachine->renderWindow);
    }

    for (auto& shape: shapes)
    {
        this->stateMachine->renderWindow.draw(shape);
    }

    ///UI drawing
    this->stateMachine->renderWindow.setView(uiView);
    sf::Vector2f c = uiView.getCenter();

    sf::Vector2f uiSize = uiView.getSize();
    sf::Vector2f topLeft = sf::Vector2f(c.x - uiSize.x/2.f,c.y - uiSize.y/2.f);
    sf::RectangleShape center(sf::Vector2f(5, 5));
    center.setPosition(c);
    center.setFillColor(sf::Color::Green);

    sf::RectangleShape r(sf::Vector2f(350, 100));

    r.setOutlineColor(sf::Color(255, 255, 255));
    r.setOutlineThickness(10);

    r.setOutlineThickness(15.f);
    r.setFillColor(sf::Color::Transparent);
    r.setPosition(topLeft.x + 16, topLeft.y + 16);

    sf::Sprite hearty;
    hearty.setTexture(texManager->getRef("heart"));
    hearty.setScale(64.f/hearty.getTexture()->getSize().x, 64.f/hearty.getTexture()->getSize().y);
    for(int i = 0; i < player->hp; i++)
    {
        hearty.setPosition(topLeft.x + 64 + i * 96,topLeft.y + 64);
        stateMachine->renderWindow.draw(hearty);
    }

    sf::Sprite goldCoins;
    goldCoins.setTexture(texManager->getRef("coins"));
    goldCoins.setScale(64.f/goldCoins.getTexture()->getSize().x, 64.f/goldCoins.getTexture()->getSize().y);

    goldCoins.setPosition(topLeft.x + uiSize.x - 300,topLeft.y + 64);

    std::string gold = std::to_string(player->money);
    goldText.setText(gold);
    goldText.setPos(goldCoins.getPosition().x + 75, goldCoins.getPosition().y);
    goldText.draw(this->stateMachine->renderWindow);
    sf::Vector2f test = goldCoins.getPosition();
    stateMachine->renderWindow.draw(goldCoins);

    wAbility.setPosition(center.getPosition().x - 25, c.y - uiSize.y/2.f + 64);
    eAbility.setPosition(wAbility.getPosition().x + 75, wAbility.getPosition().y);
    qAbility.setPosition(wAbility.getPosition().x - 75, wAbility.getPosition().y);

    stateMachine->renderWindow.draw(qAbility);
    stateMachine->renderWindow.draw(eAbility);
    stateMachine->renderWindow.draw(wAbility);

    abilityBox.setPosition(wAbility.getPosition());
    stateMachine->renderWindow.draw(abilityBox);
    abilityBox.setPosition(eAbility.getPosition());
    stateMachine->renderWindow.draw(abilityBox);
    abilityBox.setPosition(qAbility.getPosition());
    stateMachine->renderWindow.draw(abilityBox);

    OnScreenObject charText("Q", sf::Color::White, 0, 0, 50);
    charText.setScale(0.5);
    charText.setOrigin(sf::Vector2f(charText.getBounds().width/2.f, 0));
    charText.setPos(qAbility.getPosition().x + qAbility.getGlobalBounds().width/2.f, qAbility.getPosition().y + qAbility.getGlobalBounds().height + 3);
    charText.draw(stateMachine->renderWindow);
    std::string w = "W";
    std::string e = "E";
    charText.setText(w);
    charText.setOrigin(sf::Vector2f(charText.getBounds().width/2.f, 0));
    charText.setPos(wAbility.getPosition().x + wAbility.getGlobalBounds().width/2.f, wAbility.getPosition().y + wAbility.getGlobalBounds().height + 3);
    charText.draw(stateMachine->renderWindow);
    charText.setText(e);
    charText.setPos(eAbility.getPosition().x + eAbility.getGlobalBounds().width/2.f, eAbility.getPosition().y + eAbility.getGlobalBounds().height + 3);
    charText.setOrigin(sf::Vector2f(charText.getBounds().width/2.f, 0));
    charText.draw(stateMachine->renderWindow);


}


void playState::inputHandler()
{
    sf::Event event {};

    while(this->stateMachine->renderWindow.pollEvent(event))
    {
        switch(event.type)
        {
            case sf::Event::Closed:
            {
                this->saveManager->save();
                this->stateMachine->renderWindow.close();
                break;
            }

            case sf::Event::Resized:
            {
                //this->gameView.setSize(event.size.width, event.size.height);

                float prevX = gameView.getSize().x;
                float zoomFactor = prevX/event.size.width;
                this->resizeView(this->stateMachine->renderWindow, gameView, true);
                //gameView.zoom(zoomFactor);
                //gameView = sf::View(sf::FloatRect(0,0, event.size.width, event.size.height));
                //gameView.setSize(event.size.width, event.size.height);
                this->resizeView(this->stateMachine->renderWindow, uiView, false);
                //uiView.setCenter(stateMachine->renderWindow.getSize().x * 0.5f, stateMachine->renderWindow.getSize().y * 0.5f);


                break;
            }
            case sf::Event::KeyPressed:
            {
                if(event.key.code == sf::Keyboard::Escape)
                {
                    this->stateMachine->pushState(new PauseState(this->stateMachine));
                    return;
                }

                if(event.key.code == sf::Keyboard::Space)
                {
                    this->stateMachine->pushState(new PauseState(this->stateMachine));
                    return;
                }

                if(event.key.code == sf::Keyboard::B)
                {
                    if(closeCP == -1)
                        return;

                    int counter = 0;
                    for(auto& cp : checkPoints){
                        if(closeCP == counter){
                            int returnCode = cp->buyCP(player->money, saveManager->m_checkPos);
                            if(returnCode == 1){
                                saveManager->m_checkPos = cp->rectangle.getPosition();
                                player->setCurrentState(EntityState::ITEM);
                            } else if(returnCode == 2){
                                saveManager->m_checkPos = cp->rectangle.getPosition();
                            }
                        }
                        counter++;
                    }

                }

                if(event.key.code == sf::Keyboard::Q)
                {
                    player->activeAbilities.at("q")->fire();
                }
                if(event.key.code == sf::Keyboard::W)
                {
                    player->activeAbilities.at("w")->fire();
                }

                break;
            }
            case sf::Event::KeyReleased:
            {
                if(event.key.code == sf::Keyboard::Up)
                {
                    player->canJump = true;
                }
                break;
            }
        }

    }
}

void playState::update(const float deltaTime)
{
    int counter = 0;
    closeCP = -1;
    for(auto& cp : checkPoints){
        if(abs(cp->rectangle.getPosition().x - player->getPosition().x) < 10 && abs(cp->rectangle.getPosition().y - player->getPosition().y) < 10 && !cp->bought){
            closeCP = counter;
        }
        counter++;
    }
    ///Oppdaterer entitites
    for (auto it = entities.begin(), end = entities.end(); it != end; it++){
        if(abs(player->rectangle.getPosition().x - (*it)->rectangle.getPosition().x) < 330 && abs(player->rectangle.getPosition().y - (*it)->rectangle.getPosition().y) < 180){
            if((*it)->hp > 0) {
                activeEntities.push_back(*it);
                entities.erase(it);
                break;
                //må breake ut når man endrer på listen man itererer gjennom
            }
        }
    }
    for (auto it = activeEntities.begin(), end = activeEntities.end(); it != end; ++it){
        if(abs(player->rectangle.getPosition().x - (*it)->rectangle.getPosition().x) > 350 || abs(player->rectangle.getPosition().y - (*it)->rectangle.getPosition().y) > 200){
            entities.push_back(*it);
            activeEntities.erase(it);
            break;
        }
    }

    for (auto& entity: activeEntities)
    {
        //entity->collision(activeEntities);
        physics.applyPhysics(entity, deltaTime);

        if(entity->hp <= 0) {
            entity->setCurrentState(EntityState::DYING);
            entity->deathTimer += deltaTime;

            if(entity->player)
            {
                if (player->currentAnimation.getLength() <= player->deathTimer && player->currentState == EntityState::DYING) {
                    ///Set alle nødvendige variabler i saveManager
                    saveManager->setXp(player->getXp(), player->getLvl());
                    player->money = 0;
                    saveManager->setEnemiesDiedToo(player->enemiesDiedTooId);
                    this->stateMachine->changeState(new DeathState(stateMachine));
                    return;
                }
            }
            if (entity->currentAnimation.getLength() <= entity->deathTimer) {
                saveManager->m_enemiesDefeatedId.push_back(entity->id);
                player->money += entity->money;
                delete entity;
                entity = nullptr;
                activeEntities.remove(entity);
                break;
            }
        }

        ///Cooldown icons
        if(player->activeAbilities.at("q")->getElapsed() < player->activeAbilities.at("q")->cooldown)
            qAbility.setColor(sf::Color(0x80, 0x80, 0x80));
        else
            qAbility.setColor(sf::Color::White);

        if(player->activeAbilities.at("w")->getElapsed() < player->activeAbilities.at("w")->cooldown)
            wAbility.setColor(sf::Color(0x80, 0x80, 0x80));
        else
            wAbility.setColor(sf::Color::White);

        if(player->activeAbilities.at("e")->getElapsed() < player->activeAbilities.at("e")->cooldown)
            eAbility.setColor(sf::Color(0x80, 0x80, 0x80));
        else
            eAbility.setColor(sf::Color::White);

        ///Sette states for fiender
        if(!entity->player)
        {
            if(entity->currentState != EntityState::DYING) {
                if (entity->velocityY == 0 && entity->velocityX == 0)
                    entity->setCurrentState(EntityState::IDLE);
                else if (entity->velocityX != 0)
                    entity->setCurrentState(EntityState::RUNNING);
                else if (entity->velocityY != 0)
                    entity->setCurrentState(EntityState::IDLE);
            }
            if(entity->velocityX != 0)
            {
                if(entity->velocityX > 0)
                    entity->d_Left = false;
                else if (entity->velocityX < 0)
                    entity->d_Left = true;
                if(entity->invertedSprite)
                    entity->d_Left = !entity->d_Left;
            }

        }

        if(!entity->isReady)
        {
            entity->velocityY = 0.0f;
            entity->velocityX = 0.0f;
        }

        sf::Vector2f entityMove = entity->move(deltaTime);
        sf::Vector2f moveCorrected = this->moveEntity(entityMove, *entity);

        entity->sprite.setPosition(entity->rectangle.getPosition()); //        entity->sprite.setPosition(entity->rectangle.getPosition().x,entity->rectangle.getPosition().y-entity->rectangle.getOrigin().y/4);
        entity->hitArea.setPosition(entity->sprite.getPosition());


        if(entity->d_Left) // hvis sprite er vendt mot venstre speilvend den
        {
            entity->sprite.setScale(-entity->spriteScale, entity->spriteScale);
        }
        else {
            entity->sprite.setScale(entity->spriteScale, entity->spriteScale);
        }

        entity->currentAnimation.update(deltaTime,false);

        if(entity->id == 1)
        {
            float xTresh = this->gameView.getCenter().x;
            float yTresh = this->gameView.getCenter().y;
            float rectPosX = entity->rectangle.getPosition().x;
            float rectPosY = entity->rectangle.getPosition().y;
            if(rectPosX >= (xTresh + (gameView.getSize().x / 7) ))
            {
                this->gameView.move(rectPosX - (xTresh + (gameView.getSize().x / 7)), 0.f);

            } else if (rectPosX <= (xTresh - (gameView.getSize().x / 7)))
            {
                this->gameView.move(rectPosX - (xTresh - (gameView.getSize().x / 7)), 0.f);
            }
            if(rectPosY >= (yTresh + (gameView.getSize().y / 8)))
            {
                this->gameView.move(0.0f, rectPosY - (yTresh + (gameView.getSize().y / 8)));

            } else if (rectPosY <= (yTresh - (gameView.getSize().y / 8) ))
            {
                this->gameView.move(0.0f, rectPosY - (yTresh - (gameView.getSize().y / 8)));
            }
        }

//        if(entity->currentState == (EntityState::ATTACKING))
//            player->activeAbilities.at("e")->

        /*
        sf::Vector2f currentPos = entity->rectangle.getPosition();
        map.checkCollision(*entity, prevPos);
        */
    }
}

sf::Vector2f playState::moveEntity(sf::Vector2f& movement, Entity& ent)
{
    ///Fiks entity referansene Axel!!!

    sf::Vector2f prev = ent.rectangle.getPosition();

    sf::Vector2f current = prev;
    if(!ent.player ) {
        if(ent.currentState == EntityState::ATTACKING || ent.currentState == EntityState::HURT)
        {
            ent.rectangle.move(movement.x/12, movement.y/3);
            //ent.checkCollision(ent);
            ent.collision(activeEntities);
            current = ent.rectangle.getPosition();

            return {current.x - prev.x, current.y - prev.y};
        }
        else if (ent.currentState == EntityState::DYING)
        {
            ent.rectangle.move(movement.x*0, movement.y*0);
            //ent.checkCollision(ent);
            ent.collision(activeEntities);
            current = ent.rectangle.getPosition();

            return {current.x - prev.x, current.y - prev.y};
        }
    }
    if(player->hp > 0) {
        ent.rectangle.move(movement.x, movement.y);
        //ent.checkCollision(ent);
        ent.collision(activeEntities);
        current = ent.rectangle.getPosition();

        return {current.x - prev.x, current.y - prev.y};
    } else{
        player->rectangle.move(movement.x/8, movement.y/4);
        //ent.checkCollision(ent);
        player->collision(activeEntities);
        current = player->rectangle.getPosition();

        return {current.x - prev.x, current.y - prev.y};
    }
}

playState::~playState() {
    for(auto e : abilites){
        delete(e);
        e = nullptr;
    }
    for(auto e : activeEntities){
        delete(e);
        e = nullptr;
    }
}



