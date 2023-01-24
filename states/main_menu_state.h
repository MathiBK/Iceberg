//
// Created by isvol on 25.10.2019.
//

#ifndef ICEBERG_MAIN_MENU_STATE_H
#define ICEBERG_MAIN_MENU_STATE_H

#include "../BaseState.h"
#include "config_state.h"
#include "play_state.h"
#include <iostream>

class mainMenuState : public BaseState {
private:
    sf::Text text;
    sf::Text startText;
    sf::Text loadText;
    sf::Text settingsText;
    sf::Text quitText;
    sf::View view;
    sf::Font font;
    bool start, settings, load, quit;
    SaveManager* saveManager;
    sf::Texture menuPic;
    sf::Sprite sprite;

public:
    void draw(const float deltaTime) override;
    void update(const float deltaTime) override;
    void inputHandler();
    void loadNewGameState();
    void loadOldGameState();
    void loadSettingsState();

    mainMenuState(StateMachine *stateMachine);
};


#endif //ICEBERG_MAIN_MENU_STATE_H
