#include <iostream>

#include "player.h"
#include "../layer.h"

#include <math.h>


Player::Player(maptest &map, float x, float y, SoundManager& sm, std::list<int> &enemiesDiedTooId, sf::Texture& tex)

        : Entity(map, x, y, 14.0f, 28.0f, 3, 2.0f, 180.0f, 0.5f, sf::Color::Red, true, 1, 37.0f, 50.0f, sm)

{
    invincible = false;
    xp = 0;
    lvl = 1;
    money = 0;
    this->x = x;
    this->y = y;
    this->currentState = EntityState ::IDLE;

    this->enemiesDiedTooId = enemiesDiedTooId;


    rectangle.setSize(sf::Vector2f(hitboxWidth*spriteScale, hitboxHeight*spriteScale));
    rectangle.setPosition(x, y);
    xp = 0;


    std::vector<sf::Vector2f> entPoints;
    for(size_t i = 0; i < rectangle.getPointCount(); i++)
    {
        entPoints.emplace_back(rectangle.getPoint(i));
    }
    entPoly = Polygon(entPoints);
    entPoly.name = "entpoly";

    inTimer.restart();
    //NB: om du fjerner denne, blir kollisjonen ødelagt
    rectangle.setOrigin(rectangle.getSize() / 2.0f);

    activeAbilities.clear();
    animationMap[EntityState::RUNNING] = running();

    animationMap[EntityState ::CROUCHING] = crouched();
    animationMap[EntityState ::IDLE] = idle();
    animationMap[EntityState ::FALLING] =  falling();
    animationMap[EntityState ::SLIDING] =  sliding();
    animationMap[EntityState ::JUMPING] = jump();
    animationMap[EntityState ::DOUBLEJUMPING] = doublejump();
    animationMap[EntityState ::ATTACKING] =  attacking();
    animationMap[EntityState ::AIR_ATTACK] =  airAttack();
    animationMap[EntityState ::DYING] =  dying();
    animationMap[EntityState ::HURT] =  hurt();
    animationMap[EntityState ::ITEM] =  item();


    animationMap[EntityState ::ATTACK1] = attack1();
    animationMap[EntityState ::ATTACK2] = attack2();
    animationMap[EntityState ::ATTACK3] =  attack3();
    animationMap[EntityState ::AIR_ATTACK1] =  air_attack1();
    animationMap[EntityState ::AIR_ATTACK2] =  air_attack2();
    animationMap[EntityState ::AIR_ATTACK3] =  air_attack3();
    animationMap[EntityState ::BOW] =  bow();
    animationMap[EntityState ::AIR_BOW] =  air_bow();


    loadHurtBoxes();

    rectangle.setFillColor(sf::Color::Red);
    sprite.setTexture(tex);

    sprite.setScale(sf::Vector2f(spriteScale, spriteScale));
    sprite.setOrigin(sf::Vector2f(spriteWidth/2.f,spriteHeight-rectangle.getSize().y-1));



    loadSounds();
    for(int i = 0; i < 4; i++)
    {
        currentAnimation.addFrame({sf::IntRect(spriteWidth * i, 0, spriteWidth, spriteHeight)});
    }
    /*
    currentAnimation.addFrame({sf::IntRect(0, 0, spriteWidth, spriteHeight)});
    currentAnimation.addFrame({sf::IntRect(spriteWidth, 0, spriteWidth, spriteHeight)});
    currentAnimation.addFrame({sf::IntRect(spriteWidth * 2, 0, spriteWidth, spriteHeight)});
    currentAnimation.addFrame({sf::IntRect(spriteWidth * 3, 0, spriteWidth, spriteHeight)});
    */


    entPoly.setFillColor(sf::Color::Green);


    initHitArea();
}

sf::Vector2f Player::move(float delta)
{
    bool noKeyWasPressed = true;
    sf::Vector2f movement(0.f, 0.f);

    if (grounded) {
        currentJumpAmount = totalJumpAmount;
    }

    if (currentState != EntityState::DYING) { // Låser kontroll nå player dør

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            if (grounded) {
                if (velocityX != 0) {
                    setCurrentState(EntityState::SLIDING);
                } else {
                    setCurrentState(EntityState::CROUCHING);
                }
            }
            noKeyWasPressed = false;
        } else {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                velocityX += ms;
                d_Left = false;
                if (grounded)
                    setCurrentState(EntityState::RUNNING);

                isIdle = false;
                noKeyWasPressed = false;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                velocityX += -ms;
                if (grounded)
                    setCurrentState(EntityState::RUNNING);

                d_Left = true;
                isIdle = false;
                noKeyWasPressed = false;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            if (currentJumpAmount > 0 && canJump) {
                if(!grounded){
                    velocityY = -jumpPower*0.7f;
                    setCurrentState(EntityState::DOUBLEJUMPING);
                } else {
                    setCurrentState(EntityState::JUMPING);
                    velocityY = -jumpPower;
                }
                currentJumpAmount -= 1;
                canJump = false;
                noKeyWasPressed = false;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && activeAbilities.at("e")->id == "Sword") {
            if (grounded)
                setCurrentState(EntityState::ATTACKING);
            else
                setCurrentState(EntityState::AIR_ATTACK);

            currentHurtBox = hurtBoxes[EntityState::ATTACKING];
            activeAbilities.at("e")->fire();
            noKeyWasPressed = false;
        }

        if (noKeyWasPressed && grounded) {
            if (velocityX == 0)
                setCurrentState(EntityState::IDLE);
            isIdle = true;
        }
    }

    if(grounded) {
        if (velocityX > maxMs) {
            velocityX *= 0.94f;
        } else if (velocityX < -maxMs) {
            velocityX *= 0.94f;
        }
    } else {
        if (velocityX > maxMs*0.9) {
            velocityX *= 0.97f;
        } else if (velocityX < -maxMs*0.9) {
            velocityX *= 0.97f;
        }
    }

    if (velocityY < -maxMs-50) {
        velocityY *= 0.92f;
    }

    if (grounded) {
        if(currentState == EntityState::SLIDING){
            if(velocityX > 1)
                velocityX *= 0.97f;
            if (velocityX <= 1 && velocityX > 0)
                velocityX = 0;
            if(velocityX < -1)
                velocityX *= 0.97f;
            if (velocityX >= -1 && velocityX < 0)
                velocityX = 0;
        } else {
            if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                if (velocityX > 3)
                    velocityX -= 5;
                if (velocityX <= 3 && velocityX > 0)
                    velocityX = 0;
            }
            if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                if (velocityX < -3)
                    velocityX -= -5;
                if (velocityX >= -3 && velocityX < 0)
                    velocityX = 0;
            }
        }
    }

    if(canMoveRight && velocityX > 0){
        movement.x = velocityX * delta;
    }
    if(canMoveLeft && velocityX < 0){
        movement.x = velocityX * delta;
    }
    if(canMoveDown && velocityY > 0){
        movement.y = velocityY * delta;
    }
    if(canMoveUp && velocityY < 0){
        movement.y = velocityY * delta;
    }

    return movement;
}

void Player::draw(sf::RenderWindow& window)
{

//    hurtBoxes[EntityState::ATTACKING].pos = sf::Vector2f(this->getPosition().x/spriteScale, this->rectangle.getGlobalBounds().top/spriteScale);
//    sf::Vector2f test = this->getPosition();
//    hurtBoxes[EntityState::ATTACKING].updatePoly();
    window.draw(sprite);



}

void Player::process(float time) {
    Entity::process(time);


    if (velocityY > 0 && !grounded)
        setCurrentState(EntityState::FALLING);

    if(xp >= 100*lvl){
        lvl++;
    }
}



void Player::collision(std::list<Entity *> &entities)
{
    sf::Vector2f currentPos = this->rectangle.getPosition();
    TileCollisionData tcd = Entity::checkCollision(*this);
    Collider collider = this->collisionEnt(currentPos, entities);
    if(collider.ptr != nullptr)
    {
        if (collider.ptr->id == 2 && !invincible)
        {
            if(collider.direction == "top")
            {
                this->velocityY = -200.f;
            }
            else
            {
                if(collider.ptr->hp>0)
                    this->hp -= 1;
                bool diedToo = false;
                for(int lessXpId : enemiesDiedTooId){
                    if(collider.ptr->id == lessXpId)
                        diedToo = true;
                }
                if(!diedToo){
                    this->xp += collider.ptr->xpGive;
                    enemiesDiedTooId.push_back(collider.ptr->id);
                } else {
                    this->xp += collider.ptr->xpGive*0.5;
                }
                inTimer.restart();
            }
        } else if (collider.ptr->id == 3 && !invincible){
            if(collider.direction == "top")
            {
                this->velocityY = -50.f;
            }
            if(collider.ptr->hp>0)
                this->hp -= 1;
            bool diedToo = false;
            for(int lessXpId : enemiesDiedTooId){
                if(collider.ptr->id == lessXpId)
                    diedToo = true;
            }
            if(!diedToo){
                this->xp += collider.ptr->xpGive;
                enemiesDiedTooId.push_back(collider.ptr->id);
            } else {
                this->xp += collider.ptr->xpGive*0.5;
            }
            inTimer.restart();
        } else if (collider.ptr->id == 4 && !invincible){
            if(collider.direction == "top")
            {
                if(this->velocityY < 0)
                    this->velocityY *= -1;
            }
            else
            {
                if(collider.ptr->hp>0)
                    this->hp -= 1;
                bool diedToo = false;
                for(int lessXpId : enemiesDiedTooId){
                    if(collider.ptr->id == lessXpId)
                        diedToo = true;
                }
                if(!diedToo){
                    this->xp += collider.ptr->xpGive;
                    enemiesDiedTooId.push_back(collider.ptr->id);
                } else {
                    this->xp += collider.ptr->xpGive*0.5;
                }
                inTimer.restart();
            }
        } else if (collider.ptr->id == 5 && !invincible){
            if(collider.direction == "top")
            {
                this->currentJumpAmount = totalJumpAmount;
                this->velocityY = 0;
            }
            else
            {
                if(collider.ptr->hp>0)
                    this->hp -= 1;
                bool diedToo = false;
                for(int lessXpId : enemiesDiedTooId){
                    if(collider.ptr->id == lessXpId)
                        diedToo = true;
                }
                if(!diedToo){
                    this->xp += collider.ptr->xpGive;
                    enemiesDiedTooId.push_back(collider.ptr->id);
                } else {
                    this->xp += collider.ptr->xpGive*0.5;
                }
                inTimer.restart();
            }
        } else if (collider.ptr->id == 6 && !invincible){
            if(collider.direction == "top")
            {
                this->rectangle.setPosition(30, 20);
            }
            else
            {
                if(collider.ptr->hp>0)
                    this->hp -= 1;
                bool diedToo = false;
                for(int lessXpId : enemiesDiedTooId){
                    if(collider.ptr->id == lessXpId)
                        diedToo = true;
                }
                if(!diedToo){
                    this->xp += collider.ptr->xpGive;
                    enemiesDiedTooId.push_back(collider.ptr->id);
                } else {
                    this->xp += collider.ptr->xpGive*0.5;
                }
                inTimer.restart();
            }
        }
    }
    if(tcd.hurtful){
        this->xp += 5*this->hp;
        this->hp = 0;
    }
    invincibleHandler();
}

void Player::invincibleHandler()
{
    float p = inTimer.getElapsedTime().asSeconds();
    invincible = p < 1;
    if(invincible)
    {
        setCurrentState(EntityState::HURT);
        if(blink)
            sprite.setColor(sf::Color(255, 125, 125));
        else
            sprite.setColor(sf::Color(125, 125, 125));

        blink = !blink;
    }
    else
    {
        sprite.setColor(sf::Color(0xff, 0xff, 0xff));
    }
}

void Player::loadSounds()
{
    /*
    sf::Sound footstep1;

    footstep1.setBuffer(this->soundmgr.getRef("footstep1"));

    footstep2.setBuffer(this->soundmgr.getRef("footstep2"));
    footstep3.setBuffer(this->soundmgr.getRef("footstep3"));
    footstep4.setBuffer(this->soundmgr.getRef("footstep4"));
    footstep5.setBuffer(this->soundmgr.getRef("footstep5"));
    footstep6.setBuffer(this->soundmgr.getRef("footstep6"));
    footstep7.setBuffer(this->soundmgr.getRef("footstep7"));

    soundMap["footstep1"] = footstep1;
     */
}



animation Player::idle() {

    sf::Sprite* ptr = &sprite;

    animation idleAnimation(ptr);
    for(int i = 0; i < 4; i++)
    {
        idleAnimation.addFrame({sf::IntRect(spriteWidth * i, 0, spriteWidth, spriteHeight)});
    }
    return idleAnimation;
}
animation Player::crouched() {

    animation crouchedAnimation(&sprite);
    crouchedAnimation.addFrame({sf::IntRect(spriteWidth * 4, 0, spriteWidth, spriteHeight)});
    crouchedAnimation.addFrame({sf::IntRect(spriteWidth * 5, 0, spriteWidth, spriteHeight)});
    crouchedAnimation.addFrame({sf::IntRect(spriteWidth * 6, 0, spriteWidth, spriteHeight)});
    crouchedAnimation.addFrame({sf::IntRect(0, spriteHeight, spriteWidth, spriteHeight)});

    return crouchedAnimation;
}
animation Player::running() {
    animation runningAnimation(&sprite);
    for (int i = 1; i < 7; ++i) {
        runningAnimation.addFrame({sf::IntRect(spriteWidth*i, spriteHeight, spriteWidth, spriteHeight)});
    }
    return runningAnimation;
}

animation Player::jump()
{

    animation jumpingStartAnim(&sprite);
    for(int i = 0; i < 2; i ++)
    {
        jumpingStartAnim.addFrame({sf::IntRect(spriteWidth * i, spriteHeight * 2, spriteWidth, spriteHeight),0.1f});
    }
    for(int i = 2; i < 4; i ++)
    {
        jumpingStartAnim.addFrame({sf::IntRect(spriteWidth * i, spriteHeight * 2, spriteWidth, spriteHeight),0.2f});
    }
    return jumpingStartAnim;
}


animation Player::doublejump()
{

    animation jumpingAnim(&sprite);
    for(int i = 4; i < 7; i ++)
    {
        jumpingAnim.addFrame({sf::IntRect(spriteWidth * i, spriteHeight * 2, spriteWidth, spriteHeight),0.1f});
    }

    jumpingAnim.addFrame({sf::IntRect(0, spriteHeight * 3, spriteWidth, spriteHeight)});

    return jumpingAnim;
}

animation Player::attacking()
{

    animation attackingAnim(&sprite);
    for(int i = 0; i < 7; i ++)
    {
        attackingAnim.addFrame({sf::IntRect(spriteWidth * i, spriteHeight * 6, spriteWidth, spriteHeight), 0.1});
    }
    for(int i = 0; i < 4; i ++)
    {
        attackingAnim.addFrame({sf::IntRect(spriteWidth * i, spriteHeight * 7, spriteWidth, spriteHeight), 0.1});
    }
    return attackingAnim;
}

animation Player::airAttack()
{

    animation airAttack(&sprite);
    for(int i = 5; i < 7; i ++)
    {
        airAttack.addFrame({sf::IntRect(spriteWidth * i, spriteHeight * 13, spriteWidth, spriteHeight), 0.4});
    }
    for(int i = 0; i < 5; i ++)
    {
        airAttack.addFrame({sf::IntRect(spriteWidth * i, spriteHeight * 14, spriteWidth, spriteHeight), 0.4});
    }
    return airAttack;
}

void Player::loadHurtBoxes()
{
    std::vector<sf::Vector2f> attackPoints = {{0, 0}, {11, -2}, {19, 5}, {25, 11}, {22, 24}, {15, 32}, {0, 37}};
    Polygon attackPoly(attackPoints);
    attackPoly.setScale(sf::Vector2f(spriteScale, spriteScale));
    attackPoly.setFillColor(sf::Color::Green);
    hurtBoxes[EntityState::ATTACKING] = attackPoly;
}


animation Player::falling() {
    animation fallingAnimation(&sprite);
    fallingAnimation.addFrame({sf::IntRect(spriteWidth*1, spriteHeight*3, spriteWidth, spriteHeight)});
    fallingAnimation.addFrame({sf::IntRect(spriteWidth*2, spriteHeight*3, spriteWidth, spriteHeight)});

    return fallingAnimation;
}

animation Player ::sliding() {
    animation slideAnimation(&sprite);
    for (int i = 3; i < 6; ++i) {
        slideAnimation.addFrame({sf::IntRect(spriteWidth * i, spriteHeight * 3, spriteWidth, spriteHeight)});
    }
    slideAnimation.addFrame({sf::IntRect(0, spriteHeight * 4, spriteWidth, spriteHeight)});
    return slideAnimation;
}


animation Player::attack1() {
    animation attack1Animation(&sprite);
    for (int i = 0; i < 5; ++i) {
        attack1Animation.addFrame({sf::IntRect(spriteWidth * i, spriteHeight * 6, spriteWidth, spriteHeight),0.1f});
    }
    return attack1Animation;
}

animation Player::attack2() {
    animation attack2Animation(&sprite);
    for (int i = 5; i < 7; ++i) {
        attack2Animation.addFrame({sf::IntRect(spriteWidth * i, spriteHeight * 6, spriteWidth, spriteHeight),0.1f});
    }
    for (int i = 0; i < 4; ++i) {
        attack2Animation.addFrame({sf::IntRect(spriteWidth * i, spriteHeight * 7, spriteWidth, spriteHeight),0.1f});
    }
    return attack2Animation;
}

animation Player::attack3() {
    animation attack3Animation(&sprite);
    for (int i = 4; i < 7; ++i) {
        attack3Animation.addFrame({sf::IntRect(spriteWidth * i, spriteHeight * 7, spriteWidth, spriteHeight),0.1f});
    }
    for (int i = 0; i < 3; ++i) {
        attack3Animation.addFrame({sf::IntRect(spriteWidth * i, spriteHeight * 8, spriteWidth, spriteHeight),0.1f});
    }
    return attack3Animation;
}

animation Player::hurt() {
    animation hurtAnimation(&sprite);
    for (int i = 3; i < 6; ++i) {
        hurtAnimation.addFrame({sf::IntRect(spriteWidth * i, spriteHeight*8, spriteWidth, spriteHeight)});
    }
    return hurtAnimation;
}

animation Player::dying() {
    animation dyingAnimation(&sprite);
    dyingAnimation.addFrame({sf::IntRect(spriteWidth * 6, spriteHeight*8, spriteWidth, spriteHeight)});
//    for (int i = 3; i < 7; ++i) {
//        dyingAnimation.addFrame({sf::IntRect(spriteWidth * i, spriteHeight*8, spriteWidth, spriteHeight)});
//    }
    for (int i = 0; i < 6; ++i) {
        dyingAnimation.addFrame({sf::IntRect(spriteWidth * i, spriteHeight*9, spriteWidth, spriteHeight),0.2f});
    }
    return dyingAnimation;
}

animation Player::item() {
    animation itemAnimation(&sprite);
    for (int i = 2; i < 5; ++i) {
        itemAnimation.addFrame({sf::IntRect(spriteWidth * i, spriteHeight * 13, spriteWidth, spriteHeight), 0.1f});
    }
    return itemAnimation;
}

animation Player::air_attack1() {
    animation attack1Animation(&sprite);
    for (int i = 5; i < 7; ++i) {
        attack1Animation.addFrame({sf::IntRect(spriteWidth * i, spriteHeight * 13, spriteWidth, spriteHeight),0.1f});
    }
    for (int i = 0; i < 2; ++i) {
        attack1Animation.addFrame({sf::IntRect(spriteWidth * i, spriteHeight * 14, spriteWidth, spriteHeight),0.1f});
    }
    return attack1Animation;
}

animation Player::air_attack2() {
    animation attack2Animation(&sprite);
    for (int i = 2; i < 5; ++i) {
        attack2Animation.addFrame({sf::IntRect(spriteWidth * i, spriteHeight * 14, spriteWidth, spriteHeight),0.1f});
    }
    return attack2Animation;
}
animation Player::air_attack3() {
    animation attack3Animation(&sprite);
    for (int i = 5; i < 7; ++i) {
        attack3Animation.addFrame({sf::IntRect(spriteWidth * i, spriteHeight * 14, spriteWidth, spriteHeight),0.1f});
    }
    for (int i = 0; i < 4; ++i) {
        attack3Animation.addFrame({sf::IntRect(spriteWidth * i, spriteHeight * 15, spriteWidth, spriteHeight)});
    }
    return attack3Animation;
}
//animation Player::air_attack3start() {
//    animation attack3Animation(&sprite);
//    for (int i = 5; i < 6; ++i) {
//        attack3Animation.addFrame({sf::IntRect(spriteWidth * i, spriteHeight * 14, spriteWidth, spriteHeight)});
//    }
//    return attack3Animation;
//}
//animation Player::air_attack3fall() {
//    animation attack3Animation(&sprite);
//    for (int i = 6; i < 7; ++i) {
//        attack3Animation.addFrame({sf::IntRect(spriteWidth * i, spriteHeight * 14, spriteWidth, spriteHeight)});
//    }
//    for (int i = 0; i < 1; ++i) {
//        attack3Animation.addFrame({sf::IntRect(spriteWidth * i, spriteHeight * 15, spriteWidth, spriteHeight)});
//    }
//    return attack3Animation;
//}
//animation Player::air_attack3impact() {
//    animation attack3Animation(&sprite);
//    for (int i = 1; i < 4; ++i) {
//        attack3Animation.addFrame({sf::IntRect(spriteWidth * i, spriteHeight * 15, spriteWidth, spriteHeight)});
//    }
//    return attack3Animation;
//}

animation Player::bow() {
    animation bowAnimation(&sprite);
    for (int i = 0; i < 4; ++i) {
        bowAnimation.addFrame({sf::IntRect(spriteWidth * i, spriteHeight * 16, spriteWidth, spriteHeight), 0.1f});
    }
    for (int i = 0; i < 4; ++i) {
        bowAnimation.addFrame({sf::IntRect(spriteWidth * i, spriteHeight * 17, spriteWidth, spriteHeight), 0.1f});
    }
    return bowAnimation;
}
animation Player::air_bow() {
    animation air_bowAnimation(&sprite);
    for (int i = 0; i < 4; ++i) {
        air_bowAnimation.addFrame({sf::IntRect(spriteWidth * i, spriteHeight * 18, spriteWidth, spriteHeight), 0.1f});
    }
    for (int i = 0; i < 4; ++i) {
        air_bowAnimation.addFrame({sf::IntRect(spriteWidth * i, spriteHeight * 19, spriteWidth, spriteHeight), 0.1f});
    }
    return air_bowAnimation;
}