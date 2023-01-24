#ifndef ICEBERG_OBJECT_PHYSICS_H
#define ICEBERG_OBJECT_PHYSICS_H

#include <list>
#include "object.h"
#include "entities/entity.h"
#include "../utility/polygon.h"
#include "particles/particle.h"
struct Collision
{
    std::string collisionFace;
    sf::Vector2f minAx;
    float overlap{};
};
class objectPhysics {
private:
    float gravity = 250.0f;
public:
    void applyPhysics(Entity* entity, float deltaTime);
    static bool collisionSAT(Polygon &r1, Polygon &r2);

    static void applyPhysics(Particle *part, const float deltaTime);

    static sf::Vector2f collisionSAT_Intersect(Polygon &r1, Polygon &r2);

    static Collision collisionSAT_Helper(Polygon &r1, Polygon &r2);
};


#endif //ICEBERG_OBJECT_PHYSICS_H
