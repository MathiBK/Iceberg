#include <SFML/Graphics.hpp>
#include <iostream>

#include "layer.h"
#include "../map/maptest.h"

void Layer::draw(sf::RenderWindow& window, sf::Vector2f playerPos)
{
    // Render each tile
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            unsigned int gid = tilemap[x + y * width];

            // Skip empty tiles
            if (gid == 0)
                continue;

            sf::Sprite sprite;
            sprite.setPosition((float)(x * map.getTileWidth()), (float)(y * map.getTileHeight()));

            if(abs(sprite.getPosition().x - playerPos.x) > 350 || abs(sprite.getPosition().y - playerPos.y) > 200)
                continue;
            // Set source texture and position

            sprite.setPosition((float)(x * map.getTileWidth()), (float)(y * map.getTileHeight()));

            map.setSpriteTextureFromGid(sprite, gid);

            window.draw(sprite);
        }
    }

}


