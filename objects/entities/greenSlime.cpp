#include "greenSlime.h"
#include "../layer.h"
#include "math.h"

greenSlime::greenSlime(maptest &map, float x, float y, SoundManager& sm)
        : Slime (map, x, y, sm)
{
    lastX = 0;
    id = 4;
    ms = (float)(rand() % 100 + 50);
    xpGive = 50;
    money = 30;
    sprite.setColor(sf::Color::Green);
    clock1.restart();
}

sf::Vector2f greenSlime::move(float delta)
{
    velocityX = ms;
    sf::Vector2f pos = rectangle.getPosition();
    sf::Vector2f movement(0.f, 0.f);

    turnTimer = clock1.getElapsedTime().asSeconds();

    if(grounded) {
        if(turnTimer > 2.5f) {
            direction *= -1;
            clock1.restart();
        } else if (lastX == pos.x){
            direction *= -1;
            clock1.restart();
        }
        lastX = pos.x;
    }

    velocityX *= (float)direction;
    movement.x += velocityX * delta;
    movement.y += velocityY * delta;

    return movement;
}

