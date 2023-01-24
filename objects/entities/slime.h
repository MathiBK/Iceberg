#ifndef ICEBERG_Slime_H
#define ICEBERG_Slime_H

#include "entity.h"
#include <random>
#include "../../map/maptest.h"

class Slime : public Entity{
private:
    sf::Clock clock2;
    float lastX;
public:
    Slime(maptest &map, float x, float y, SoundManager& sm);
    sf::Vector2f move(float delta) override;

    void draw(sf::RenderWindow &window) override;
    animation attack1(), hurt(), dying(), idle(), running();
protected:

    sf::Clock clock1;
    float jumpTimer;
    float turnTimer;
    int direction;
    void process(float time) override;
    void collision(std::list<Entity *> &entities) override;

};


#endif //ICEBERG_Slime_H
