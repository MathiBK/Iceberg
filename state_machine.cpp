#include "state_machine.h"
#include "BaseState.h"
#include "save_manager.h"

void StateMachine::pushState(BaseState* state)
{
    this->states.push(state);
}

void StateMachine::changeState(BaseState* state)
{
    if(!this->states.empty())
        this->states.pop();
    this->states.push(state);
}

void StateMachine::popState()
{
    delete this->states.top();
    this->states.top() = nullptr;
    this->states.pop();

}

BaseState* StateMachine::peekState()
{
    if(this->states.empty())
        return nullptr;
    return this->states.top();

}

void StateMachine::gameLoop()
{
    sf::Clock clock;

    while(this->renderWindow.isOpen())
    {
        sf::Time elapsed = clock.restart();
        float deltaTime = elapsed.asSeconds();

        if(deltaTime > 0.15f)
            deltaTime = 0.15f;

        if(peekState() == nullptr)
            continue;
        peekState()->inputHandler();
        peekState()->update(deltaTime);

        this->renderWindow.clear(sf::Color::Black);

        peekState()->draw(deltaTime);

        this->renderWindow.display();

    }
}

void StateMachine::loadSounds()
{

    this->soundmgr.loadSoundBfr("cannon", "media/audio/cannon.wav");
    this->soundmgr.loadSoundBfr("footstep1", "media/audio/footstep1.wav");

}

void StateMachine::loadTextures()
{

    this->texManager.loadTexture("player", "media/SpriteSheets/adventurer.png");
    this->texManager.loadTexture("slime", "media/SpriteSheets/slime-Sheet.png");
    this->texManager.loadTexture("skeleton", "media/SpriteSheets/skeleton.png");
    this->texManager.loadTexture("heart", "media/heart_sprite.png");
    this->texManager.loadTexture("coins", "media/gold_coins.png");
    this->texManager.loadTexture("bow", "media/bow.png");
    this->texManager.loadTexture("Bow", "media/bow.png");
    this->texManager.loadTexture("Sword", "media/sword.png");
    this->texManager.loadTexture("Dash", "media/dash.png");
    this->texManager.loadTexture("Grappling Hook", "media/Grappling Hook.png");

    this->texManager.loadTexture("iceberg", "media/Iceberg.png");


}

StateMachine::~StateMachine()
{
    while(!this->states.empty())
        popState();
}

StateMachine::StateMachine()
{
    this->texManager = TextureManager();
    this->loadSounds();
    this->loadTextures();
    saveManager = SaveManager();
    std::string fileName = "saves/save.json";
    saveManager.loadSettings(fileName);
    if(saveManager.m_fullscreen == 1){
        this->renderWindow.create(sf::VideoMode(saveManager.m_screenWidth, saveManager.m_screenHeight), "Iceberg", sf::Style::Fullscreen);
    } else {
        this->renderWindow.create(sf::VideoMode(saveManager.m_screenWidth, saveManager.m_screenHeight), "Iceberg");
    }
    this->renderWindow.setKeyRepeatEnabled(false);
    this->renderWindow.setVerticalSyncEnabled(true);
    this->renderWindow.setFramerateLimit(saveManager.m_framerate);

}