//
// Created by axel on 13.10.2019.
//

#ifndef ICEBERG_STATE_MACHINE_H
#define ICEBERG_STATE_MACHINE_H

#include <memory>
#include <stack>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "utility/sound_manager.h"
#include "save_manager.h"
#include "utility/texture_manager.h"

class BaseState;

class StateMachine
{

public:

    StateMachine();
    ~StateMachine();

    std::stack<BaseState*> states;

    sf::RenderWindow renderWindow;
    SaveManager saveManager;
    SoundManager soundmgr;
    TextureManager texManager;


    void pushState(BaseState* state);
    void popState();
    void changeState(BaseState* state);
    BaseState* peekState();

    void gameLoop();

protected:
    void loadSounds();

    void loadTextures();
};


#endif //ICEBERG_STATE_MACHINE_H
