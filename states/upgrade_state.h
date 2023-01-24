//
// Created by Axel on 07/12/2019.
//

#ifndef ICEBERG_UPGRADE_STATE_H
#define ICEBERG_UPGRADE_STATE_H


#include "../BaseState.h"
#include "../abilities/ability.h"
#include "../objects/on_screen/on_screen_object.h"

struct abilityInfo
{
    std::string name;
    std::string description;
    sf::Texture* icon;
    int cost;
};

class UpgradeState : public BaseState
{
public:
    UpgradeState(StateMachine* sm);

    void draw(float deltaTime) override;
    void update(const float deltaTime) override;
    void inputHandler() override;

protected:

    static void boxMaker(sf::RectangleShape& box, sf::Vector2i pos, sf::Color color, float outlineThick);

    SaveManager* saveManager;
    TextureManager* textureManager;
    sf::Texture texture;
    sf::Sprite background;

    std::list<abilityInfo> abilities;
    sf::Sprite mainIcon;
    sf::Sprite leftIcon;
    sf::Sprite rightIcon;

    OnScreenObject titleText;
    OnScreenObject costText;

    OnScreenObject descriptionText;
    OnScreenObject nameText;

    OnScreenObject playerInfo;
    OnScreenObject instructionText;


};


#endif //ICEBERG_UPGRADE_STATE_H
