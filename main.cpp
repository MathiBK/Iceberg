#include <iostream>
#include <states/choose_ability_state.h>
#include "state_machine.h"
#include "states/splash_state.h"



int main()
{
    StateMachine sm;

    sm.pushState(new splashState(&sm));

    sm.gameLoop();

    return 0;
}