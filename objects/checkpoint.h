#ifndef ICEBERG_CHECKPOINT_H
#define ICEBERG_CHECKPOINT_H

#include "object.h"
#include "../animation/animation.h"
#include <SFML/Graphics.hpp>

class checkpoint : public Object {
public:
    checkpoint(maptest &map, float x, float y);
    void draw(sf::RenderWindow& window) override;

    int buyCP(int& money, sf::Vector2f& saveCpPos);

    sf::Vector2f cpGetPos(){return pos;};

    const float x, y;
    bool bought;
    int cost;
    const float hitboxWidth = 10, hitboxHeight = 10, spriteWidth = 10, spriteHeight = 10;
    const float spriteScale = 0.5;
    sf::Sprite sprite;
    sf::Texture texture;
    sf::RectangleShape rectangle;

private:
    sf::Vector2f pos;
};


#endif //ICEBERG_CHECKPOINT_H
