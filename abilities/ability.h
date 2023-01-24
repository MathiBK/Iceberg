#ifndef ICEBERG_ABILITY_H
#define ICEBERG_ABILITY_H

#include <string>
#include <SFML/Graphics.hpp>

class Entity;


class ability
{
public:
    sf::Clock clock;
    float elapsed;
    float cooldown;
    std::string id;
    int uses;
    int totalUses;
    Entity* ent;
    explicit ability(Entity* ent) : ent(ent) {};
    virtual ~ability() = default;
    virtual void fire(){};
    float getElapsed(){ return clock.getElapsedTime().asSeconds();}
};

#endif //ICEBERG_ABILITY_H
