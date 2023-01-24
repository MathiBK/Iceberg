//
// Created by axel on 23.11.2019.
//

#include <iostream>
#include "texture_manager.h"

void TextureManager::loadTexture(const std::string &name, const std::string &filename)
{

    sf::Texture texture;

    if(!texture.loadFromFile(filename))
    {
        return;
    }

    this->textures[name] = texture;

}

sf::Texture &TextureManager::getRef(const std::string &txtenum)
{
    try
    {
        return this->textures.at(txtenum);
    }
    catch(std::out_of_range &e)
    {
        return textures.at("none");
    }
}

TextureManager::TextureManager()
{
    sf::Texture none;
    none.create(300,300);
    this->textures["none"] = none;
}


