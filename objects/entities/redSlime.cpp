#include "redSlime.h"
#include "../layer.h"
#include "math.h"

redSlime::redSlime(maptest &map, float x, float y, SoundManager& sm)
        : Slime (map, x, y, sm)
{
    id = 5;
    xpGive = 40;
    money = 35;
    sprite.setColor(sf::Color::Red);
    clock1.restart();
}

sf::Vector2f redSlime::move(float delta)
{
    sf::Vector2f pos = rectangle.getPosition();
    sf::Vector2f movement(0.f, 0.f);

    jumpTimer = clock1.getElapsedTime().asSeconds();

    if(grounded){
        if(jumpTimer > 1.0f){
            int ran = rand() % 100 + 1;
            if(ran > 50){
                velocityY = -jumpPower - 10;
            }
            clock1.restart();
        }
    }

    velocityX = 0;
    movement.x += velocityX * delta;
    movement.y += velocityY * delta;

    return movement;
}
