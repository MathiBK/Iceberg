//
// Created by Axel on 14/11/2019.
//

#ifndef ICEBERG_PARTICLE_H
#define ICEBERG_PARTICLE_H

#include "../object.h"
#include <SFML/Graphics.hpp>

class Particle : public Object
{
public:
    explicit Particle(maptest& map) : Object(map) {};
    virtual ~Particle() = default;
    virtual void collision(float delta) = 0;
    sf::RectangleShape body;

    sf::Vector2f velocity;
    sf::Vector2f direction;
protected:
    int size;

};
#endif //ICEBERG_PARTICLE_H
