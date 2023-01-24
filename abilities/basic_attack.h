#ifndef ICEBERG_BASIC_ATTACK_H
#define ICEBERG_BASIC_ATTACK_H


#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <list>
#include "ability.h"

class BasicAttack : public ability
{
public:
    explicit BasicAttack(Entity* ent, std::list<Entity*>& entList);
    void fire() override;
    sf::Vector2f getDirection();


protected:
    std::list<Entity*>& entityList;
    sf::Vector2f attackDirection;
    sf::Sound sound;
    float reset;
    void checkCollision();
};


#endif //ICEBERG_BASIC_ATTACK_H
