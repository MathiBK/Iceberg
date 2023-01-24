//
// Created by Axel on 06/12/2019.
//

#ifndef ICEBERG_PAUSE_STATE_H
#define ICEBERG_PAUSE_STATE_H


#include "../BaseState.h"
#include "../objects/on_screen/on_screen_object.h"

class PauseState : public BaseState
{
public:
    PauseState(StateMachine* stateMachine);

    SaveManager* saveManager;

    void draw(const float deltaTime) override;
    void update(const float deltaTime) override;
    void inputHandler() override;
protected:
    sf::Texture texture;
    OnScreenObject pauseText;
    OnScreenObject escText;
    sf::Sprite background;


};


#endif //ICEBERG_PAUSE_STATE_H
