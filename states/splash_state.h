#include "../BaseState.h"
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics.hpp>
#include "../map/maptest.h"

class splashState : public BaseState
{
public:


    void inputHandler();
    void update(float dt);
    void draw(float dt);
    maptest map;
    explicit splashState(StateMachine* stateMachine);


private:

    sf::Clock _clock;
    sf::Texture splash;
    sf::Sprite splashscreen;
};



