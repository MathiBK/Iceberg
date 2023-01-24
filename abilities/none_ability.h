//
// Created by Axel on 12/12/2019.
//

#ifndef ICEBERG_NONE_ABILITY_H
#define ICEBERG_NONE_ABILITY_H


#include "ability.h"

class NoneAbility : public ability
{
public:
    explicit NoneAbility(Entity* ent) : ability(ent) {};
    void fire() override {};
};


#endif //ICEBERG_NONE_ABILITY_H
