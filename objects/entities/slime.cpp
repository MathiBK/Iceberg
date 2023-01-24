#include "slime.h"
#include "../layer.h"
#include "../../abilities/basic_attack.h"

Slime::Slime(maptest &map, float x, float y, SoundManager& sm)
        : Entity(map, x, y, 16, 10, 3, 18.0f, 100.0f, 0.5f, sf::Color::Yellow, false, 2, 25, 32, sm)
{
    ms = (float)(rand() % 65 + 20);
    xpGive = 35;
    money = 25;
    invertedSprite = true;
    direction = 1;

    std::vector<sf::Vector2f> entPoints;
    for(size_t i = 0; i < rectangle.getPointCount(); i++)
    {
        entPoints.emplace_back(rectangle.getPoint(i));
    }
    entPoly = Polygon(entPoints);
    entPoly.name = "entpoly";

    //NB!!!: om du fjerner denn, blir kollisjonen fakked
    rectangle.setOrigin(rectangle.getSize() / 2.0f);

//    sprite.setScale(sf::Vector2f(spriteScale, spriteScale));
//    sprite.setOrigin(sf::Vector2f(spriteWidth/2.f,spriteHeight-rectangle.getSize().y-1));// setter midten av spriten slik at det blir riktig når spriten skal snus fra side til side og at den ligger i riktighøyde

    setSprite("media/SpriteSheets/slime-Sheet.png");

    for(int i = 0; i < 4; i++)
    {
        currentAnimation.addFrame({sf::IntRect(spriteWidth * i, 0, spriteWidth, spriteHeight)});
    }
    animationMap[EntityState::RUNNING] = running();
    animationMap[EntityState::IDLE] = idle();
    animationMap[EntityState::HURT] =  hurt();
    animationMap[EntityState::DYING] =  dying();
    animationMap[EntityState::ATTACKING] = attack1();
    animationMap[EntityState::ATTACK1] = attack1();

    initHitArea();
    clock1.restart();
    clock2.restart();
}

sf::Vector2f Slime::move(float delta)
{
    sf::Vector2f pos = rectangle.getPosition();
    sf::Vector2f movement(0.f, 0.f);
    velocityX = ms;
    jumpTimer = clock1.getElapsedTime().asSeconds();
    turnTimer = clock2.getElapsedTime().asSeconds();

    if(grounded){
        if (turnTimer > 4.0f) {
            direction *= -1;
            clock2.restart();
        } else if (lastX == pos.x){
            direction *= -1;
            clock1.restart();
        }
        if(jumpTimer > 10.0f) {
            velocityY -= jumpPower;
            clock1.restart();
        }
        lastX = pos.x;
    }

    velocityX *= (float)direction;
    movement.x += velocityX * delta;
    movement.y += velocityY * delta;

    return movement;
}

void Slime::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}
void Slime::collision(std::list<Entity *> &entities)
{
    Entity::checkCollision(*this);
}

void Slime::process(float time)
{
    Entity::process(time);
}

animation Slime::idle() {
    animation idleAnimation(&sprite);
    for (int i = 0; i < 4; ++i) {
        idleAnimation.addFrame({sf::IntRect(spriteWidth * i, 0, spriteWidth, spriteHeight)});
    }
    return idleAnimation;
}
animation Slime::running() {
    animation runningAnimation(&sprite);
    for (int i = 4; i < 8; ++i) {
        runningAnimation.addFrame({sf::IntRect(spriteWidth*i, 0, spriteWidth, spriteHeight)});
    }
    return runningAnimation;
}
animation Slime::attack1()  {
    animation attackAnimation(&sprite);
    for (int i = 0; i < 5; ++i) {
        attackAnimation.addFrame({sf::IntRect(spriteWidth*i, spriteHeight, spriteWidth, spriteHeight)});
    }
    return attackAnimation;
}
animation Slime::hurt() {
    animation hurtAnimation(&sprite);
    for (int i = 5; i < 8; ++i) {
        hurtAnimation.addFrame({sf::IntRect(spriteWidth * i, spriteHeight, spriteWidth, spriteHeight)});
    }
    return hurtAnimation;
}
animation Slime::dying() {
    animation dyingAnimation(&sprite);
    for (int i = 0; i < 5; ++i) {
        dyingAnimation.addFrame({sf::IntRect(spriteWidth * i, spriteHeight*2, spriteWidth, spriteHeight)});
    }
    return dyingAnimation;
}


