//
// Created by Sebas on 13.12.2019.
//

#ifndef ICEBERG_redSlime_H
#define ICEBERG_redSlime_H
#include "slime.h"

class redSlime : public Slime{
public:
    redSlime(maptest &map, float x, float y, SoundManager& sm);
    sf::Vector2f move(float delta) override ;
};


#endif //ICEBERG_redSlime_H
