//
// Created by Andreas Eivindson on 09/12/2019.
//
#include "splash_state.h"
#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "play_state.h"
#include "main_menu_state.h"

void splashState::draw(const float deltaTime)
{
    this->stateMachine->renderWindow.clear(sf::Color::Red);

    this->stateMachine->renderWindow.draw(splashscreen);

}

splashState::splashState (StateMachine *stateMachine) : BaseState(stateMachine)
{this->stateMachine = stateMachine;



    splash.loadFromFile("media/splashPhoto2.jpg");
    splashscreen.setTexture(splash);

}

void splashState::update(const float deltaTime) {
    {
        if (this->_clock.getElapsedTime().asSeconds() > 3)
        {
           sf::Vector2u windowSize = this->stateMachine->renderWindow.getSize();
            sf::Texture texture;
            texture.create(windowSize.x, windowSize.y);
            texture.update(this->stateMachine->renderWindow);
            this->stateMachine->changeState(new mainMenuState(stateMachine));

            return;
        }
    }

}

void splashState::inputHandler()
{{
        sf::Event event;

        while (this->stateMachine->renderWindow.pollEvent(event))
        {
            if (sf::Event::Closed == event.type)
            {
                this->stateMachine->renderWindow.close();
            }
        }
    }}
