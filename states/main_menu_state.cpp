//
// Created by isvol on 25.10.2019.
//

#include "main_menu_state.h"

static const float VIEW_HEIGHT = 900.f;
static const float VIEW_WIDTH = 1600.f;

void mainMenuState::draw(const float deltaTime)
{
    this->stateMachine->renderWindow.clear(sf::Color(129, 161, 188));
    this->stateMachine->renderWindow.draw(sprite);
    this->stateMachine->renderWindow.draw(startText);
    this->stateMachine->renderWindow.draw(loadText);
    this->stateMachine->renderWindow.draw(settingsText);
    this->stateMachine->renderWindow.draw(quitText);
}

mainMenuState::mainMenuState(StateMachine *stateMachine) : BaseState(stateMachine) {
    this->stateMachine = stateMachine;
    this->saveManager = &this->stateMachine->saveManager;
    this->text.setFillColor(sf::Color::Blue);
    this->font.loadFromFile("media/fonts/OptimusPrincepsSemiBold.ttf");

    menuPic = stateMachine->texManager.getRef("iceberg");
    sprite.setTexture(menuPic);
    sprite.setPosition(600.0f, 0);
    menuPic.setSmooth(true);
    startText.setFont( font );
    startText.setStyle( sf::Text::Bold );
    startText.setString( "Create New Game" );
    startText.setCharacterSize( 48 );
    startText.setPosition( 50.0f, 320.0f );
    loadText.setFont( font );
    loadText.setStyle( sf::Text::Bold );
    loadText.setString( "Load Game" );
    loadText.setCharacterSize( 48 );
    loadText.setPosition( 50.0f, 380.0f );
    settingsText.setFont( font );
    settingsText.setStyle( sf::Text::Bold );
    settingsText.setString( "Settings" );
    settingsText.setCharacterSize( 48 );
    settingsText.setPosition( 50.0f, 440.0f );
    quitText.setCharacterSize(20);
    quitText.setPosition(50.0f, 500.0f);
    quitText.setFont(font);
    quitText.setStyle(sf::Text::Bold);
    quitText.setString("Quit Game");

}

void mainMenuState::inputHandler()
{
    sf::Event event {};

    while(this->stateMachine->renderWindow.pollEvent(event))
    {
        switch(event.type)
        {
            case sf::Event::Closed:
            {
                this->saveManager->save();
                this->stateMachine->renderWindow.close();
                break;
            }
            case sf::Event::Resized:
            {
                //TODO: håndtere resizing
                this->stateMachine->renderWindow.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
                break;
            }
            case sf::Event::KeyPressed:
            {
                if(event.key.code == sf::Keyboard::Escape)
                {
                    stateMachine->saveManager.save();
                    this->stateMachine->renderWindow.close();
                    return;
                }
                break;
            }

            case sf::Event::MouseButtonPressed:
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition( this->stateMachine->renderWindow );
                sf::Vector2f mouseCorrected = stateMachine->renderWindow.mapPixelToCoords(mousePos);
                if ( startText.getGlobalBounds().contains( mouseCorrected ) )
                {
                    this->loadNewGameState();
                }

                if ( loadText.getGlobalBounds().contains( mouseCorrected ))
                {
                    this->loadOldGameState();
                }

                if(settingsText.getGlobalBounds().contains( mouseCorrected) )
                {
                    this->loadSettingsState();
                }
                if(quitText.getGlobalBounds().contains(mouseCorrected))
                {
                    exit(0);
                }

                break;

            }

            case sf::Event::MouseMoved:
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition( this->stateMachine->renderWindow );
                sf::Vector2f mouseCorrected = stateMachine->renderWindow.mapPixelToCoords(mousePos);
                start = startText.getGlobalBounds().contains(mouseCorrected);
                load = loadText.getGlobalBounds().contains(mouseCorrected);
                settings = settingsText.getGlobalBounds().contains(mouseCorrected);
                quit = quitText.getGlobalBounds().contains(mouseCorrected);
                break;
            }
        }
    }
}

void mainMenuState::loadNewGameState()
{
    saveManager->save();
    this->stateMachine->pushState(new playState(this->stateMachine));
}

void mainMenuState::loadOldGameState()
{
    std::string fileName = "saves/save.json";
    saveManager->load(fileName);
    this->stateMachine->pushState(new playState(this->stateMachine));
}

void mainMenuState::loadSettingsState()
{
    this->stateMachine->pushState(new Config(this->stateMachine));
}

void mainMenuState::update(const float deltaTime)
{

    if(start)
    {
        startText.setFillColor(sf::Color::Yellow);
    }
    else
    {
        startText.setFillColor(sf::Color::Black);
    }
    if(load)
    {
        loadText.setFillColor( sf::Color::Yellow );
    }
    else {
        loadText.setFillColor(sf::Color::Black);
    }

    if(settings)
    {
        settingsText.setFillColor( sf::Color:: Yellow);
    }
    else {
        settingsText.setFillColor(sf::Color::Black);
    }

    if(quit)
    {
        quitText.setFillColor(sf::Color::Yellow);
    }
    else{
        quitText.setFillColor(sf::Color::Black);
    }

}
