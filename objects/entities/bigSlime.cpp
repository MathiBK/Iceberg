
#include "bigSlime.h"
#include "../layer.h"

bigSlime::bigSlime(maptest &map, float x, float y, SoundManager& sm)
        : Slime (map, x, y, sm) {
    spriteScale = 3;
    velocityX = ms;
    id = 6;
    xpGive = 500;
    money = 10000;
    hp = 23;
    rectangle.setSize(sf::Vector2f(hitboxWidth * spriteScale + 20, hitboxHeight * spriteScale));
    rectangle.setPosition(sf::Vector2f(x, y));
    std::vector<sf::Vector2f> entPoints;
    for (size_t i = 0; i < rectangle.getPointCount(); i++) {
        entPoints.emplace_back(rectangle.getPoint(i));
    }
    entPoly = Polygon(entPoints);
    entPoly.name = "entpoly";
    rectangle.setOrigin(rectangle.getSize() / 2.0f);
}



sf::Vector2f bigSlime::move(float delta)
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

