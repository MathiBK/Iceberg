#include "skeleton.h"
#include "../layer.h"

Skeleton::Skeleton(maptest &map, float x, float y, SoundManager& sm, Player &player)

        : Entity(map, x, y, 20, 28, 3, 18.0f, 100.0f, 0.5f, sf::Color::Yellow, false, 3, 32, 24, sm)
{
    ms = (float)(rand() % 30 + 30);
    direction = 1;
    xpGive = 50;
    money = 40;
//    rectangle.setSize(sf::Vector2f(hitboxWidth*spriteScale, hitboxHeight*spriteScale));

    std::vector<sf::Vector2f> entPoints;
    for(size_t i = 0; i < rectangle.getPointCount(); i++)
    {
        entPoints.emplace_back(rectangle.getPoint(i));
    }
    entPoly = Polygon(entPoints);
    entPoly.name = "entpoly";

    spiller = &player;

    //NB!!!: om du fjerner denn, blir kollisjonen fakked
    rectangle.setOrigin(rectangle.getSize() / 2.0f);



//    sprite.setScale(sf::Vector2f(spriteScale, spriteScale));
//    sprite.setOrigin(sf::Vector2f(spriteWidth/2.f,spriteHeight-rectangle.getSize().y-1));
    setSprite("media/SpriteSheets/skeleton.png");

    animationMap[EntityState::RUNNING] = running();
    animationMap[EntityState ::IDLE] = idle();
    animationMap[EntityState ::HURT] =  hurt();
    animationMap[EntityState ::DYING] =  dying();
    animationMap[EntityState ::ATTACKING] = attack1();
    animationMap[EntityState ::ATTACK1] = attack1();

    spriteWidth = 24;
    for (int i = 0; i < 11; i++)
    {
        currentAnimation.addFrame({sf::IntRect(spriteWidth * i, 0, spriteWidth, spriteHeight)});
    }
    clock1.restart();
    clock2.restart();
    initHitArea();

}

sf::Vector2f Skeleton::move(float delta)
{
    sf::Vector2f pos = rectangle.getPosition();
    sf::Vector2f movement(0.f, 0.f);
    velocityX = ms;
    float distanceX = spiller->getPosition().x - pos.x;
    float distanceY = spiller->getPosition().y - pos.y;
    float distanceX2 = distanceX * distanceX;
    float distanceY2 = distanceY * distanceY;
    float dxy2 = distanceX2 + distanceY2;
    float distance = sqrt(dxy2);

    jumpTimer = clock1.getElapsedTime().asSeconds();
    turnTimer = clock2.getElapsedTime().asSeconds();

    if(grounded) {
        if (distance < 100) {
            chase();
        } else {
            if (turnTimer > 4.0f) {
                direction *= -1;
                clock2.restart();
            } else if (jumpTimer > 2.0f) {
                velocityY = -jumpPower;
                clock1.restart();
            }
        }
    }

    velocityX *= (float)direction;
    movement.x += velocityX * delta;
    movement.y += velocityY * delta;

    return movement;
}


void Skeleton::chase()
{
    sf::Vector2f pos = rectangle.getPosition();

    if (!spiller->grounded && jumpTimer > 2.0f) {
        velocityY = -jumpPower;
        clock1.restart();
    }

    if (turnTimer > 1.0f) {
        if (spiller->getPosition().x > pos.x) {
            direction = 1;
        } else if (spiller->getPosition().x < pos.x) {
            direction = -1;
        }
        clock2.restart();
    }
}


void Skeleton::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}
void Skeleton::collision(std::list<Entity *> &entities)
{
    Entity::checkCollision(*this);
}

void Skeleton::process(float time)
{

    Entity::process(time);
//    sprite.setPosition(rectangle.getPosition());
//    currentAnimation.update(0.015f,true);
//    sprite.setScale(0.5f, 0.5f);
}



animation Skeleton::idle() {
    spriteWidth = 24;
    animation idleAnimation(&sprite);
    for (int i = 0; i < 11; ++i) {
        idleAnimation.addFrame({sf::IntRect(spriteWidth * i, 0, spriteWidth, spriteHeight)});
    }
    return idleAnimation;
}
animation Skeleton::running() {
    spriteWidth = 22;
    animation runningAnimation(&sprite);
    for (int i = 0; i < 13; ++i) {
        runningAnimation.addFrame({sf::IntRect(spriteWidth*i, spriteHeight, spriteWidth, spriteHeight)});
    }
    return runningAnimation;
}

animation Skeleton::hurt() {
    spriteWidth = 30;
    animation hurtAnimation(&sprite);
    for (int i = 0; i < 8; ++i) {
        hurtAnimation.addFrame({sf::IntRect(spriteWidth * i, spriteHeight*2, spriteWidth, spriteHeight)});
    }
    return hurtAnimation;
}
animation Skeleton::dying() {
    spriteWidth = 33;
    animation dyingAnimation(&sprite);
    for (int i = 0; i < 16; ++i) {
        dyingAnimation.addFrame({sf::IntRect(spriteWidth * i, spriteHeight*3, spriteWidth, spriteHeight)});
    }
    return dyingAnimation;
}
animation Skeleton::attack1()  {
    spriteWidth = 42;
    animation attackAnimation(&sprite);
    for (int i = 0; i < 8; ++i) {
        attackAnimation.addFrame({sf::IntRect(spriteWidth*i, spriteHeight*4, spriteWidth, spriteHeight+5)});
    }
    attackAnimation.addFrame({sf::IntRect(304, spriteHeight*4, spriteWidth, spriteHeight+5)});
    for (int i = 0; i < 11; ++i) {
        attackAnimation.addFrame({sf::IntRect(348+spriteWidth*i, spriteHeight*4, spriteWidth, spriteHeight+5)});
    }
    return attackAnimation;
}