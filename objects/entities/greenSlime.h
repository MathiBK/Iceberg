#ifndef ICEBERG_greenSlime_H
#define ICEBERG_greenSlime_H
#include "slime.h"

class greenSlime : public Slime {
private:
    float lastX;
public:
    greenSlime(maptest &map, float x, float y, SoundManager& sm);
    sf::Vector2f move(float delta) override ;
};


#endif //ICEBERG_GREENSLIME_H
