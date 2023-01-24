#ifndef ICEBERG_SKELETON_H
#define ICEBERG_SKELETON_H


#include "entity.h"
#include <random>
#include "../../map/maptest.h"

class Skeleton : public Entity{
private:
    sf::Clock clock1;
    sf::Clock clock2;
    float jumpTimer;
    float turnTimer;
    int direction;
public:
    Skeleton(maptest &map, float x, float y, SoundManager& sm, Player &player);

    sf::Vector2f move(float delta) override ;

    Player* spiller;

    void draw(sf::RenderWindow &window) override;
    void chase();
    animation attack1(), hurt(), dying(), idle(), running();

protected:

    void process(float time) override;
    void collision(std::list<Entity *> &entities) override;
};

#endif //ICEBERG_SKELETON_H
