//
// Created by Axel on 06/12/2019.
//

#ifndef ICEBERG_ARROW_H
#define ICEBERG_ARROW_H


#include "particle.h"

class Arrow : public Particle
{
public:
    Arrow(maptest& map);

    void collision(float delta) override;
    void draw(sf::RenderWindow &window) override;
    void process(float deltaTime) override;
};


#endif //ICEBERG_ARROW_H
