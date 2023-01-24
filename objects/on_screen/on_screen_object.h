#ifndef ICEBERG_ON_SCREEN_OBJECTS_H
#define ICEBERG_ON_SCREEN_OBJECTS_H

#include "../object.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
#include <string>


class OnScreenObject {

private:



public:
    OnScreenObject() = default;
    OnScreenObject(const std::string& setning, sf::Color color, float x, float y, int size);


    void draw(sf::RenderWindow& window);
    void setScale(float scale);
    void changeText(std::string &s);
    sf::Sprite getSprite() {return sprite;}
    sf::Font font;
    std::string setning;
    sf::Text text;
    float x, y;
    int size;
    sf::Sprite sprite;

    void setText(std::string& input);
    void setPos(float x, float y);

    sf::FloatRect getBounds();
    void setOrigin(sf::Vector2f origin);

    void loadFont(const std::string& filename);
};



#endif //ICEBERG_ON_SCREEN_OBJECTS_H