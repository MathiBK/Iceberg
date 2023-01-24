#ifndef ICEBERG_BIGSLIME_H
#define ICEBERG_BIGSLIME_H

#include <random>
#include "../../map/maptest.h"
#include "slime.h"
#include "redSlime.h"
#include "greenSlime.h"

class bigSlime : public Slime{
private:
    float lastX;
public:
    bigSlime(maptest &map, float x, float y, SoundManager& sm);

    sf::Vector2f move(float delta) override ;

};


#endif //ICEBERG_BIGSLIME_H
