#include "checkpoint.h"

checkpoint::checkpoint(maptest &map, float x, float y)
        : Object(map), x(x), y(y)
{
    pos = {x, y};
    cost = 100;
    bought = false;
    rectangle.setSize(sf::Vector2f(hitboxWidth*spriteScale, hitboxHeight*spriteScale));
    texture.loadFromFile("media/noCapCP.png");
    sprite.setTexture(texture);
    sprite.setScale(sf::Vector2f(16.f/texture.getSize().x, 16.f/texture.getSize().y));
    sprite.setOrigin(sf::Vector2f(spriteWidth/2.f,spriteHeight-rectangle.getSize().y-1));
    sprite.setPosition(x, y);
    rectangle.setPosition(x, y);
    rectangle.setFillColor(sf::Color::Magenta);
}

void checkpoint::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

int checkpoint::buyCP(int& money, sf::Vector2f& saveCpPos) {
    if(saveCpPos == pos){
        return 2;
    } else {
        if (money >= this->cost) {
            money -= this->cost;
            saveCpPos = this->pos;
            bought = true;
            texture.loadFromFile("media/capCP.png");
            return 1;
        } else {
            return 3;
        }
    }
}

