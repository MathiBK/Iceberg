//
// Created by Axel on 07/12/2019.
//

#include <jsoncpp/json/json.h>
#include "choose_ability_state.h"

void ChooseAbilityState::boxMaker(sf::RectangleShape& box, sf::Vector2i pos, sf::Color color, float outlineThick)
{
    box.setPosition(pos.x, pos.y);
    box.setFillColor(sf::Color::Transparent);
    box.setOutlineColor(color);
    box.setOutlineThickness(outlineThick);
}

ChooseAbilityState::ChooseAbilityState(StateMachine *sm) : BaseState(sm)
{
    this->saveManager = &stateMachine->saveManager;
    this->texManager = &stateMachine->texManager;

    sf::RenderTexture rTex;
    rTex.create(1600, 900);
    ///brukes når den er ferdig

    for(auto & i : saveManager->m_abilitiesUnlocked)
    {
        abilityData ability;
        ability.icon = &texManager->getRef(i);
        ability.name = i;
        abilitiesUnlocked.push_back(ability);
    }

    ///testing
    abilityData dash = abilityData();

    dash.icon = &stateMachine->texManager.getRef("Dash");
    dash.name = "Dash";

    abilityData bow = abilityData();

    bow.icon = &stateMachine->texManager.getRef("bow");
    bow.name = "Bow";

    abilityData grapple = abilityData();

    grapple.icon = &stateMachine->texManager.getRef("Grappling Hook");
    grapple.name = "Grappling Hook";

    abilityData none = abilityData();

    none.icon = &stateMachine->texManager.getRef("none");
    none.name = "none";


    abilitiesUnlocked.push_back(none);


    sf::RectangleShape centerBox(sf::Vector2f(400, 400));
    sf::RectangleShape leftBox(sf::Vector2f(400, 400));
    sf::RectangleShape rightBox(sf::Vector2f(400, 400));

    boxMaker(centerBox, sf::Vector2i(600,150), sf::Color::White, 3);
    boxMaker(leftBox, sf::Vector2i(100,150), sf::Color::White, 3);
    boxMaker(rightBox, sf::Vector2i(1100,150), sf::Color::White, 3);

    Q = OnScreenObject("Q", sf::Color::White, leftBox.getPosition().x + leftBox.getSize().x/2.f, leftBox.getPosition().y - 80, 80);
    E = OnScreenObject("E", sf::Color::White, rightBox.getPosition().x + rightBox.getSize().x/2.f, rightBox.getPosition().y - 80, 80);
    W = OnScreenObject("W", sf::Color::White, centerBox.getPosition().x + centerBox.getSize().x/2.f, centerBox.getPosition().y - 80, 80);
    instructionText = OnScreenObject("Press Q, W, or E to loop through unlocked abilities\nPress enter to save and return, or ESC to exit without saving", sf::Color::White, centerBox.getPosition().x + centerBox.getSize().x/2.f, centerBox.getPosition().y + centerBox.getSize().y + 100, 30);

    rTex.draw(leftBox);
    rTex.draw(rightBox);
    rTex.draw(centerBox);

    rTex.display();
    texture = rTex.getTexture();

    this->background.setTexture(texture);
    centerIcon.setPosition(centerBox.getPosition());
    leftIcon.setPosition(leftBox.getPosition());
    rightIcon.setPosition(rightBox.getPosition());

    sf::Vector2u windowSize = this->stateMachine->renderWindow.getSize();
    background.setScale(
            1600.f / background.getLocalBounds().width,
            900.f / background.getLocalBounds().height);



    qList = abilitiesUnlocked;
    eList = abilitiesUnlocked;
    wList = abilitiesUnlocked;
}

void ChooseAbilityState::draw(float deltaTime)
{
    this->stateMachine->renderWindow.clear(sf::Color::Black);
    this->stateMachine->renderWindow.setView(this->stateMachine->renderWindow.getDefaultView());


    Q.draw(this->stateMachine->renderWindow);
    W.draw(this->stateMachine->renderWindow);
    E.draw(this->stateMachine->renderWindow);
    instructionText.draw(this->stateMachine->renderWindow);
    this->stateMachine->renderWindow.draw(this->background);

    this->stateMachine->renderWindow.draw(leftIcon);
    this->stateMachine->renderWindow.draw(rightIcon);
    this->stateMachine->renderWindow.draw(centerIcon);




}

void ChooseAbilityState::update(const float deltaTime)
{

    bool icon1;
    bool icon2;
    leftIcon.setTexture(*qList.begin()->icon);
    centerIcon.setTexture(*wList.begin()->icon);
    rightIcon.setTexture(*eList.begin()->icon);


    if ((centerIcon.getTexture() == leftIcon.getTexture()) && wList.begin()->name != "none") {
        centerIcon.setColor(sf::Color(0x80, 0x80, 0x80));
        icon1 = false;

    }
    else
    {
        centerIcon.setColor(sf::Color::White);
        icon1 = true;
    }



    if ((rightIcon.getTexture() == centerIcon.getTexture() || rightIcon.getTexture() == leftIcon.getTexture()) && eList.begin()->name != "none") {
        rightIcon.setColor(sf::Color(0x80, 0x80, 0x80));
        icon2 = false;
    }
    else
    {
        rightIcon.setColor(sf::Color::White);
        icon2 = true;
    }


    centerIcon.setScale(400.f/centerIcon.getTexture()->getSize().x, 400.f/centerIcon.getTexture()->getSize().y);
    rightIcon.setScale(400.f/rightIcon.getTexture()->getSize().x, 400.f/rightIcon.getTexture()->getSize().y);
    leftIcon.setScale(400.f/leftIcon.getTexture()->getSize().x, 400.f/leftIcon.getTexture()->getSize().y);

    selectionAllowed = (icon1 & icon2);
}


void ChooseAbilityState::inputHandler()
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
                    this->stateMachine->popState();
                    return;
                }
                if(event.key.code == sf::Keyboard::Return)
                {
                    if(selectionAllowed)
                    {
                        saveConfig();
                        this->stateMachine->popState();
                    }
                    else
                    {

                    }

                    return;
                }
                if(event.key.code == sf::Keyboard::U)
                {
                    this->stateMachine->popState();
                    return;
                }

                if(event.key.code == sf::Keyboard::Q)
                {
                    qList.splice(qList.end(), qList, qList.begin());
                }
                if(event.key.code == sf::Keyboard::W)
                {
                    wList.splice(wList.end(), wList, wList.begin());
                }
                if(event.key.code == sf::Keyboard::E)
                {
                    eList.splice(eList.end(), eList, eList.begin());
                }



                break;
            }
        }

    }
}


void ChooseAbilityState::saveConfig()
{
    saveManager->m_abilitiesConfigured.at("q") = qList.begin()->name;
    saveManager->m_abilitiesConfigured.at("w") = wList.begin()->name;
    saveManager->m_abilitiesConfigured.at("e") = eList.begin()->name;
    saveManager->save();
}
