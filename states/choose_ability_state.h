//
// Created by Axel on 07/12/2019.
//

#ifndef ICEBERG_CHOOSE_ABILITY_STATE_H
#define ICEBERG_CHOOSE_ABILITY_STATE_H


#include "../BaseState.h"
#include "../objects/on_screen/on_screen_object.h"
#include "../utility/texture_manager.h"


struct abilityData
{
    std::string name;
    sf::Texture* icon;
};

class ChooseAbilityState : public BaseState
{
public:
    explicit ChooseAbilityState(StateMachine* sm);

    void draw(float deltaTime) override;
    void update(const float deltaTime) override;
    void inputHandler() override;


protected:

    SaveManager* saveManager;
    TextureManager* texManager;
    sf::Texture texture;
    sf::Sprite background;

    sf::RectangleShape selector;

    std::list<abilityData> abilitiesUnlocked;
    std::list<abilityData> qList;
    std::list<abilityData> wList;
    std::list<abilityData> eList;


    sf::Sprite centerIcon;
    sf::Sprite leftIcon;
    sf::Sprite rightIcon;

    OnScreenObject Q;
    OnScreenObject E;
    OnScreenObject W;

    OnScreenObject instructionText;

    bool selectionAllowed;

    void boxMaker(sf::RectangleShape &box, sf::Vector2i pos, sf::Color color, float outlineThick);

    void saveConfig();
};


#endif //ICEBERG_CHOOSE_ABILITY_STATE_H
