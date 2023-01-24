#include "pause_state.h"

PauseState::PauseState(StateMachine *stateMachine) : BaseState(stateMachine)
{

    this->saveManager = &this->stateMachine->saveManager;


    sf::View view = this->stateMachine->renderWindow.getDefaultView();
    this->stateMachine->renderWindow.setView(this->stateMachine->renderWindow.getDefaultView());
    //this->pauseText = OnScreenObject("Paused.", sf::Color(sf::Color::Green), (float)this->stateMachine->renderWindow.getSize().x/2, (float)this->stateMachine->renderWindow.getSize().y/2, 200);
    this->pauseText = OnScreenObject("Paused", sf::Color(0xab, 0xd1, 0xf3), (float)view.getSize().x/2, ((float)view.getSize().y/2)-100, 200);
    this->escText = OnScreenObject("Press esc to go to main menu", sf::Color(0xab, 0xd1, 0xf3), (float)view.getSize().x/2, ((float)view.getSize().y/2)+100, 50);


    sf::Vector2u windowSize = this->stateMachine->renderWindow.getSize();

    texture.create(windowSize.x, windowSize.y);

    this->stateMachine->renderWindow.setView(this->stateMachine->renderWindow.getDefaultView());
    texture.update(this->stateMachine->renderWindow, 0, 0);

    this->background.setTexture(texture);
    //må skaleres
    float scaleX = 1600.f/this->stateMachine->renderWindow.getSize().x;
    float scaleY = 900.f/this->stateMachine->renderWindow.getSize().y;
    this->background.setScale(scaleX, scaleY);
    this->background.setColor(sf::Color(0x7d, 0x7d, 0x7d));



}

void PauseState::draw(const float deltaTime)
{
    this->stateMachine->renderWindow.clear(sf::Color::Black);
    this->stateMachine->renderWindow.setView(this->stateMachine->renderWindow.getDefaultView());


    this->stateMachine->renderWindow.draw(background);

    pauseText.draw(this->stateMachine->renderWindow);
    escText.draw(this->stateMachine->renderWindow);
}


void PauseState::update(const float deltaTime)
{

}

void PauseState::inputHandler()
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

                background.setScale(
                        1600.f / background.getLocalBounds().width,
                        900.f / background.getLocalBounds().height);
                break;
            }
            case sf::Event::KeyPressed:
            {
                if(event.key.code == sf::Keyboard::Escape)
                {
                    saveManager->save();
                    this->stateMachine->popState();
                    this->stateMachine->popState();
                    return;
                }
                if(event.key.code == sf::Keyboard::Space)
                {
                    this->stateMachine->popState();
                    return;
                }

                break;
            }
        }

    }
}


