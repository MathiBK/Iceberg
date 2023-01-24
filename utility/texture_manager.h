//
// Created by axel on 23.11.2019.
//

#ifndef ICEBERG_TEXTURE_MANAGER_H
#define ICEBERG_TEXTURE_MANAGER_H


#include <map>
#include <SFML/Graphics/Texture.hpp>

enum class TexturesEnum
{

};

class TextureManager
{
private:
    std::map<std::string, sf::Texture> textures;

public:
    void loadTexture(const std::string& name, const std::string& filename);

    sf::Texture& getRef(const std::string& txtenum);

    TextureManager();
};


#endif //ICEBERG_TEXTURE_MANAGER_H
