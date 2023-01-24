//
// Created by Axel on 14/11/2019.
//

#ifndef ICEBERG_GRAPPLER_H
#define ICEBERG_GRAPPLER_H


#include <SFML/Graphics/RectangleShape.hpp>
#include "../object.h"
#include "../entities/entity.h"
#include "particle.h"

class grapplingHook;

enum class grapplerState
{
    MOVING,
    COLLIDED,
    MAXED
};

class Grappler : public Particle
{
friend class grapplingHook;
public:

    Grappler(maptest &map1, float x, float y, sf::Vector2f direction, grapplingHook& ent);
    ~Grappler() override ;

    void collision(float delta) override;

    sf::Vector2f move(float delta);

    void draw(sf::RenderWindow &window) override;
    void process(float deltaTime) override;

protected:
    grapplingHook& hook;
    float length;
    sf::Vector2f prevPos;
    sf::Vector2f initialGrapplingHookPos;
    bool moving, swinging;
    sf::VertexArray line;
    grapplerState state;
    double angleVelocity;
    double m_angle;
    float getDistance(sf::Vector2f thisPos, sf::Vector2f grapplingHookPos);

    void entityMove(float delta);

    void setAngleAcc(float delta);
};


#endif //ICEBERG_GRAPPLER_H
