//
// Created by Axel on 06/12/2019.
//

#ifndef ICEBERG_BOW_H
#define ICEBERG_BOW_H


#include "ability.h"
#include "../objects/particles/arrow.h"

class Bow : public ability
{
public:
    explicit Bow(Entity* ent);
    void fire() override;

protected:
    //Arrow arrow;
};


#endif //ICEBERG_BOW_H
