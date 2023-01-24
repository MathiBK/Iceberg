#include <SFML/Graphics.hpp>
#include "sprite.h"
#include "../map/maptest.h"

void Sprite::process(float deltaTime)
{
    auto& animation = map.getAnimation(gid);

    if (!animation)
        return;

    // Go to next animation frame if required
    if (clock.getElapsedTime().asMilliseconds() < (*animation)[frame].duration)
        return;

    if (++frame >= (int)animation->size())
        frame = 0;

    clock.restart();
}

void Sprite::draw(sf::RenderWindow& window)
{
    sf::Sprite sprite;
    sprite.setPosition((float)x, (float)y);

    window.draw(sprite);
}