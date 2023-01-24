//
// Created by Sebas on 24.10.2019.
//

#ifndef ICEBERG_SPRITE_H
#define ICEBERG_SPRITE_H

#include <SFML/System/Clock.hpp>

#include "object.h"

class Sprite : public Object
{
    // Map needs to access protected/private data
    friend class maptest;

public:
    explicit Sprite(maptest& map) : Object(map) {}

    void process(float time) override;
    void draw(sf::RenderWindow& window) override;
    sf::Vector2f getPosition() { return body.getPosition();}

// Object id and name
int id = 0;
std::string name = "";
// Id of first tile
unsigned int gid = 0;
// Location on screen
int x = 0;
int y = 0;
int height = 0;
bool solid;
// Dimensions
int width = 0;
protected:

    sf::RectangleShape body;
    // Current animation frame
    int frame = 0;

    // Times the animation
    sf::Clock clock;
};


#endif //ICEBERG_SPRITE_H
