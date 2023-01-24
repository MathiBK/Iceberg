//
// Created by Axel on 07/12/2019.
//

#include "upgrade_state.h"
#include "choose_ability_state.h"
#include "play_state.h"

void UpgradeState::boxMaker(sf::RectangleShape& box, sf::Vector2i pos, sf::Color color, float outlineThick)
{
    box.setPosition(pos.x, pos.y);
    box.setFillColor(sf::Color::Transparent);
    box.setOutlineColor(color);
    box.setOutlineThickness(outlineThick);
}

UpgradeState::UpgradeState(StateMachine *sm) : BaseState(sm)
{
    this->saveManager = &this->stateMachine->saveManager;
    this->textureManager = &this->stateMachine->texManager;

    sf::RenderTexture rTex;
    rTex.create(1600, 900);


    ///tester litt greier her

    sf::RectangleShape mainBox(sf::Vector2f(300, 300));
    sf::RectangleShape leftBox(sf::Vector2f(200, 200));
    sf::RectangleShape rightBox(sf::Vector2f(200, 200));

    sf::RectangleShape descriptionBox(sf::Vector2f(700, 500));
    sf::RectangleShape playerStatusBox(sf::Vector2f(700, 200));


    boxMaker(mainBox, sf::Vector2i(300,150), sf::Color::White, 3);
    boxMaker(leftBox, sf::Vector2i(150,200), sf::Color::White, 3);
    boxMaker(rightBox, sf::Vector2i(550,200), sf::Color::White, 3);

    mainBox.setFillColor(sf::Color::Black);

    boxMaker(descriptionBox, sf::Vector2i(850,50), sf::Color::White, 3);
    boxMaker(playerStatusBox, sf::Vector2i(850,600), sf::Color::White, 3);

    titleText = OnScreenObject("Upgrades!", sf::Color::White, mainBox.getPosition().x + mainBox.getSize().x/2.f, mainBox.getPosition().y - 80, 80);
    descriptionText = OnScreenObject("hallo!", sf::Color::White, descriptionBox.getPosition().x + descriptionBox.getSize().x/2.f, descriptionBox.getPosition().y + descriptionBox.getSize().y/2.f, 20);
    nameText = OnScreenObject("Heisann!", sf::Color::White, descriptionBox.getPosition().x + descriptionBox.getSize().x/2.f, descriptionBox.getPosition().y + 20, 40);
    costText = OnScreenObject("Cost: ", sf::Color::White, mainBox.getPosition().x + mainBox.getSize().x/2.f, mainBox.getPosition().y + mainBox.getSize().y + 50, 50);
    playerInfo = OnScreenObject("Current unused levels: " + std::to_string(saveManager->m_lvl-saveManager->m_usedLvls), sf::Color::White, playerStatusBox.getPosition().x + 350, playerStatusBox.getPosition().y + 50, 50);
    instructionText = OnScreenObject("Press enter to purchase.\nPress C to configure unlocked abilities\nPress U to restart.", sf::Color::White, mainBox.getPosition().x + mainBox.getSize().x/2.f, mainBox.getPosition().y + mainBox.getSize().y + 300, 20);

    rTex.draw(leftBox);
    rTex.draw(rightBox);
    rTex.draw(mainBox);
    rTex.draw(descriptionBox);
    rTex.draw(playerStatusBox);

    rTex.display();
    texture = rTex.getTexture();

    background.setTexture(texture);
    mainIcon.setPosition(mainBox.getPosition());
    leftIcon.setPosition(leftBox.getPosition());
    rightIcon.setPosition(rightBox.getPosition());

    sf::Vector2u windowSize = this->stateMachine->renderWindow.getSize();
    background.setScale(
            1600.f / background.getLocalBounds().width,
            900.f / background.getLocalBounds().height);

    ///


    ///laster inn abilities
    abilityInfo dash;
    dash.description = "A dash";

    dash.icon = &stateMachine->texManager.getRef("Dash");
    dash.name = "Dash";
    dash.cost = 2;

    abilityInfo grapplingHook;
    grapplingHook.description = "A hook";

    grapplingHook.icon = &stateMachine->texManager.getRef("Grappling Hook");
    grapplingHook.name = "Grappling Hook";
    grapplingHook.cost = 5;

    abilityInfo bow;
    bow.description = "A bow";

    bow.icon = &stateMachine->texManager.getRef("bow");
    bow.name = "Bow";
    bow.cost = 10;

    abilityInfo sword;
    sword.description = "A sword";

    sword.icon = &stateMachine->texManager.getRef("Sword");
    sword.name = "Sword";
    sword.cost = 1;

    std::list<abilityInfo> tempAb;
    tempAb.push_back(dash);
    tempAb.push_back(grapplingHook);
    tempAb.push_back(bow);
    tempAb.push_back(sword);

    for(const auto& i : tempAb)
    {
        bool in = false;
        for(auto& o : stateMachine->saveManager.m_abilitiesUnlocked)
        {
            if(o == i.name)
                in = true;

        }
        if(!in)
            abilities.push_back(i);
    }
}

void UpgradeState::draw(float deltaTime)
{
    this->stateMachine->renderWindow.clear(sf::Color::Black);


    titleText.draw(this->stateMachine->renderWindow);
    descriptionText.draw(this->stateMachine->renderWindow);
    nameText.draw(this->stateMachine->renderWindow);
    costText.draw(this->stateMachine->renderWindow);
    playerInfo.draw(this->stateMachine->renderWindow);
    instructionText.draw(this->stateMachine->renderWindow);


    this->stateMachine->renderWindow.draw(leftIcon);
    this->stateMachine->renderWindow.draw(rightIcon);
    this->stateMachine->renderWindow.draw(background);
    this->stateMachine->renderWindow.draw(mainIcon);

}

void UpgradeState::update(const float deltaTime)
{
    while(abilities.size() < 3)
    {
        abilityInfo a = abilityInfo();
        a.name = "";
        a.icon = &textureManager->getRef("none");
        a.description = "";
        a.cost = 0;
        abilities.push_back(a);
    }
    auto i = abilities.begin();
    auto icontst = *(*++abilities.begin()).icon;
    leftIcon.setTexture(*(*i).icon);
    mainIcon.setTexture(*(*++i).icon);
    rightIcon.setTexture(*(*++i).icon);

    mainIcon.setScale(300.f/mainIcon.getTexture()->getSize().x, 300.f/mainIcon.getTexture()->getSize().y);
    rightIcon.setScale(200.f/rightIcon.getTexture()->getSize().x, 200.f/rightIcon.getTexture()->getSize().y);
    leftIcon.setScale(200.f/leftIcon.getTexture()->getSize().x, 200.f/leftIcon.getTexture()->getSize().y);

    descriptionText.setText((*++abilities.begin()).description);
    descriptionText.setOrigin(sf::Vector2f(descriptionText.getBounds().width/2.f, descriptionText.getBounds().height/2.f));
    nameText.setText((*++abilities.begin()).name);
    nameText.setOrigin(sf::Vector2f(nameText.getBounds().width/2.f, nameText.getBounds().height/2.f));
    std::string costCalc = "Cost: " + std::to_string((*++abilities.begin()).cost);
    costText.setText(costCalc);
}

void UpgradeState::inputHandler()
{
    sf::Event event{};

    while(this->stateMachine->renderWindow.pollEvent(event))
    {
        switch(event.type)
        {
            case sf::Event::Closed:
            {
                this->stateMachine->renderWindow.close();
                break;
            }

            case sf::Event::Resized:
            {
                this->background.setPosition(this->stateMachine->renderWindow.mapPixelToCoords(sf::Vector2i(0,0)));
                this->background.setScale(float(event.size.width) / float(this->background.getTexture()->getSize().x),
                                          float(event.size.height) / float(this->background.getTexture()->getSize().y));
                break;
            }
            case sf::Event::KeyPressed:
            {
                if(event.key.code == sf::Keyboard::Escape)
                {
                    this->stateMachine->renderWindow.close();
                }
                if(event.key.code == sf::Keyboard::U)
                {
                    this->stateMachine->changeState(new playState(this->stateMachine));
                    return;
                }

                if(event.key.code == sf::Keyboard::C)
                {
                    this->stateMachine->pushState(new ChooseAbilityState(this->stateMachine));
                    return;
                }

                if(event.key.code == sf::Keyboard::Left)
                {
                    abilities.splice(abilities.end(), abilities, abilities.begin());
                }

                if(event.key.code == sf::Keyboard::Right)
                {
                    abilities.splice(abilities.begin(), abilities, (--abilities.end()));
                }

                if(event.key.code == sf::Keyboard::Return)
                {
                    if((*++abilities.begin()).cost <= this->saveManager->m_lvl - this->saveManager->m_usedLvls){
                        if(!(*++abilities.begin()).name.empty())
                            saveManager->m_abilitiesUnlocked.push_back((*++abilities.begin()).name);
                        this->saveManager->setUsedLvls(saveManager->m_usedLvls + (*++abilities.begin()).cost);
                        abilities.erase((++abilities.begin()));
                        std::string temp = "Current unused levels: " + std::to_string(saveManager->m_lvl - saveManager->m_usedLvls);
                        playerInfo.setText(temp);
                    } else {
                        //Not enough lvl
                    }

                }

                break;
            }
        }

    }
}
