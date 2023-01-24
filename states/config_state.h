#ifndef ICEBERG_CONFIG_STATE_H
#define ICEBERG_CONFIG_STATE_H

#include "../BaseState.h"
#include "../save_manager.h"
#include "../objects/on_screen/on_screen_object.h"
#include "main_menu_state.h"

class Config : public BaseState
{

private:
    std::vector<sf::Vector2i> resolutionsNumber;
    std::list<int> framerateNumber;
    std::list<sf::Text*> resolutions;
    std::list<sf::Text*> framerates;
    std::list<sf::Text*> options;

    sf::Sprite sprite;

    std::string chosenRes;
    int fullscreenInt;
    std::string chosenFrame;

    sf::Texture menuPic;
    sf::View view;
    sf::Text settings;
    sf::Text applyButton;
    sf::Text fullscreen;
    sf::Text res;
    sf::Text framerate;
    sf::Text backButton;
    sf::Font font;
    bool back;

public:
    void resizeView(const sf::RenderWindow& window, sf::View& view);
    void draw(const float deltaTime) override;
    void update(const float deltaTime) override;
    void inputHandler() override;
    void loadMainMenuState();
    void apply();


    explicit Config(StateMachine* stateMachine);
    ~Config() override;

};


#endif //ICEBERG_CONFIG_STATE_H
