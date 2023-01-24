#include "death_state.h"
#include "play_state.h"
#include "upgrade_state.h"

DeathState::DeathState(StateMachine *stateMachine) : BaseState(stateMachine)
{

    this->saveManager = &this->stateMachine->saveManager;
    sf::View view = this->stateMachine->renderWindow.getDefaultView();
    this->stateMachine->renderWindow.setView(this->stateMachine->renderWindow.getDefaultView());
    this->deathText = OnScreenObject("You died.", sf::Color(255, 0, 0), (float)view.getSize().x/2, (float)view.getSize().y/2, 100);
    this->restartText = OnScreenObject("Press R to restart. Press B to buy abilities.", sf::Color(255, 255, 255), (float)view.getSize().x/2, (float)view.getSize().y/2 + 100, 20);


    sf::Vector2u windowSize = this->stateMachine->renderWindow.getSize();

    backgroundTex.create(windowSize.x, windowSize.y);

    this->stateMachine->renderWindow.setView(this->stateMachine->renderWindow.getDefaultView());
    backgroundTex.update(this->stateMachine->renderWindow, 0, 0);

    this->background.setTexture(backgroundTex);
    //må skaleres
    float scaleX = 1600.f/this->stateMachine->renderWindow.getSize().x;
    float scaleY = 900.f/this->stateMachine->renderWindow.getSize().y;
    this->background.setScale(scaleX, scaleY);
    this->background.setColor(sf::Color(0x7d, 0x7d, 0x7d));

}

void DeathState::draw(const float deltaTime)
{
    this->stateMachine->renderWindow.clear(sf::Color::Black);
    this->stateMachine->renderWindow.setView(this->stateMachine->renderWindow.getDefaultView());

    this->stateMachine->renderWindow.draw(background);

    deathText.draw(this->stateMachine->renderWindow);
    restartText.draw(this->stateMachine->renderWindow);
}

void DeathState::update(const float deltaTime) {

}

void DeathState::inputHandler()
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
                //this->gameView.setSize(event.size.width, event.size.height);

                //uiView.setCenter(stateMachine->renderWindow.getSize().x * 0.5f, stateMachine->renderWindow.getSize().y * 0.5f);
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
                if(event.key.code == sf::Keyboard::R)
                {
                    this->stateMachine->changeState(new playState(stateMachine));
                    return;
                }

                if(event.key.code == sf::Keyboard::B)
                {
                    this->stateMachine->changeState(new UpgradeState(this->stateMachine));
                    return;
                }
                break;
            }
        }

    }


}
