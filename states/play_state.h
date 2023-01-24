#ifndef ICEBERG_PLAY_STATE_H
#define ICEBERG_PLAY_STATE_H

#include <list>
#include <array>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../map/maptest.h"
#include "../BaseState.h"
#include "../objects/object.h"
#include "../objects/object_physics.h"
#include "../objects/layer.h"
#include "../objects/sprite.h"
#include "../objects/entities/player.h"
#include "../objects/entities/entity.h"
#include "../objects/entities/slime.h"
#include "../objects/entities/redSlime.h"
#include "../objects/entities/greenSlime.h"
#include "../objects/entities/bigSlime.h"
#include "../objects/entities/skeleton.h"
#include "../abilities/ability.h"
#include "../abilities/dash.h"
#include "../animation/animation.h"
#include "../objects/particles/particle.h"
#include "../save_manager.h"
#include "../objects/checkpoint.h"
#include "../objects/on_screen/on_screen_object.h"

class playState : public BaseState
{
private:

public:
    void draw(const float deltaTime) override;
    void update(const float deltaTime) override;
    void inputHandler() override;
    maptest map;
    SaveManager* saveManager;
    SoundManager* soundManager;
    TextureManager* texManager;

    OnScreenObject o;

    objectPhysics physics;

    std::list<checkpoint*> checkPoints;
    std::list<std::shared_ptr<Object>> objects;
    std::list<std::shared_ptr<Layer>> layers;
    std::list<std::unique_ptr<Particle>> particles;
    std::list<Entity*> activeEntities;
    std::list<Polygon> shapes;
    std::list<Entity*> entities;

    Player* player;
    std::vector<ability*> abilites;

    int closeCP;

    explicit playState(StateMachine* stateMachine);
    ~playState() override;



protected:
    float zoomLevel;
    sf::Vector2f moveEntity(sf::Vector2f& movement, Entity& ent);
    sf::View gameView;
    sf::View uiView;
    OnScreenObject goldText;
    sf::RectangleShape abilityBox;
    sf::Sprite qAbility;
    sf::Sprite wAbility;
    sf::Sprite eAbility;


    void resizeView(const sf::RenderWindow &window, sf::View &view, bool zoom);


};


#endif //ICEBERG_PLAY_STATE_H
