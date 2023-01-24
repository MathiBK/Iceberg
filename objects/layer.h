//
// Created by Sebas on 24.10.2019.
//

#ifndef ICEBERG_LAYER_H
#define ICEBERG_LAYER_H
#include <string>
#include <list>
#include "../objects/object.h"
#include "sprite.h"
#include "entities/player.h"


class Layer : public Object
{
    // Map needs to access protected/private data
    friend class maptest;

public:
    explicit Layer(maptest& map) : Object(map) { }
    ~Layer() override { delete[] tilemap; }

    void draw(sf::RenderWindow& window, sf::Vector2f playerPos);

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    unsigned int* getTilemap() { return tilemap; }
    std::string& getName() {return name;}

    int id = 0;
    std::string name;
    int width = 0;
    int height = 0;
    bool visible = false;
    unsigned int* tilemap = nullptr;
};


#endif //ICEBERG_LAYER_H
