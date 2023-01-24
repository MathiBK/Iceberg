//
// Created by axel on 13.10.2019.
//

#ifndef ICEBERG_BASESTATE_H
#define ICEBERG_BASESTATE_H


#include <memory>
#include "state_machine.h"

class BaseState

{
public:
    StateMachine* stateMachine;
    BaseState(StateMachine* sm) : stateMachine(sm) {};
    virtual void draw(const float deltaTime) = 0;
    virtual void update(const float deltaTime) = 0;
    virtual void inputHandler() = 0;
    virtual ~BaseState() = default;
};


#endif //ICEBERG_BASESTATE_H
