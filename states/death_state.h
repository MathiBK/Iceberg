//
// Created by axel on 23.11.2019.
//

#ifndef ICEBERG_DEATH_STATE_H
#define ICEBERG_DEATH_STATE_H


#include "../BaseState.h"
#include "../save_manager.h"
#include "../objects/on_screen/on_screen_object.h"

class DeathState : public BaseState
{
public:
    DeathState(StateMachine* stateMachine);

    SaveManager* saveManager;

  void draw(const float deltaTime) override;
  void update(const float deltaTime) override;
  void inputHandler() override;
protected:
    OnScreenObject deathText;
    OnScreenObject restartText;
    sf::Sprite background;
    sf::Texture backgroundTex;

};


#endif //ICEBERG_DEATH_STATE_H
