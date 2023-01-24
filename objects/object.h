#ifndef ICEBERG_OBJECT_H
#define ICEBERG_OBJECT_H

#include <memory>

namespace sf
{
    class RenderWindow;
    class Texture;
}

class maptest;

// Base class representing any game object, including tile layers and sprites
class Object
{
public:
    explicit Object(maptest& map) : map(map) {}
    virtual ~Object() = default;
    int xpGive;

    virtual void process(float deltaTime) {}
    virtual void draw(sf::RenderWindow& window) {}

protected:
    // Reference to map class so objects can use the map to set textures for drawing
    maptest& map;
};

#endif //ICEBERG_OBJECT_H
