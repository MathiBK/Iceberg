#ifndef ICEBERG_DASH_H
#define ICEBERG_DASH_H

#include "ability.h"

class dash : public ability {
public:
    dash(Entity* ent);
    sf::Vector2f direction;
    float powerX = 200.0f;
    float powerY = 300.0f;

    void fire() override;
};


#endif //ICEBERG_DASH_H
