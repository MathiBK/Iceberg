#include <memory>
#include <fstream>

#include <jsoncpp/json/json.h>

#include <SFML/Graphics/Sprite.hpp>
#include <iostream>
#include <regex>

#include "maptest.h"
#include "../objects/object.h"
#include "../objects/layer.h"
#include "../objects/sprite.h"
#include "../objects/entities/player.h"




bool maptest::loadFromFile(const std::string& filename)
{
    tilesets.clear();
    animations.clear();
    layers.clear();
    sprites.clear();

    // Will contain the data we read in
    Json::Value root;

    // Stream used for reading the data file. The data file has been saved as JSON in Tiled
    std::ifstream file(filename);

    // Factory to create char readers
    Json::CharReaderBuilder builder;

    // Read data from file into root object
    std::string errors;
    bool parsingSuccessful = Json::parseFromStream(builder, file, &root, &errors);

    if (!parsingSuccessful)
        return false;

    // Map properties
    width = root["width"].asInt();
    height = root["height"].asInt();
    tileWidth = root["tilewidth"].asInt();
    tileHeight = root["tileheight"].asInt();

    // Load all tilesets
    for (Json::Value& tileset: root["tilesets"])
        loadTileset(tileset);

    // Read in each layer
    for (Json::Value& layer: root["layers"])
    {
        if (layer["type"].asString() == "tilelayer")
            loadTileLayer(layer);
        else
            loadObjectLayer(layer);

    }

    return true;
}

std::shared_ptr<Layer> maptest::getLayer(const std::string& name)
{
    for (auto& layer: layers)
    {
        if (layer->name == name)
            return layer;
    }

    return std::shared_ptr<Layer>();
}

const std::shared_ptr<std::vector<AnimationFrame>> maptest::getAnimation(unsigned int gid)
{
    auto animationIt = animations.find(gid);

    if (animationIt != animations.end())
        return animationIt->second;

    return std::shared_ptr<std::vector<AnimationFrame>>();
}



void maptest::setSpriteTextureFromGid(sf::Sprite &sprite, unsigned int gid, int frame)
{
    // Extract flip flags
    unsigned int flipFlags = gid >> 29;

    gid &= ~(0b111 << 29);

    // Look for an animation for this gid
    auto animationIt = animations.find(gid);

    if (animationIt != animations.end())
    {
        auto& animation = *animationIt->second;

        if (frame >= 0 && frame < (int)animation.size())
            gid = animation[frame].gid;
    }

    // Find the correct tileset for this gid
    //auto tileset = *std::find_if(tilesets.rbegin(), tilesets.rend(), [gid](auto ts) { return gid >= ts->firstGid; } );
    Tileset* tileset;
    for(auto& tilesetTing : tilesets)
    {
        if(gid >= tilesetTing->firstGid)
            tileset = tilesetTing.get();

    }
    // Calculate x and y positions in the tileset
    int tileid = gid - tileset->firstGid;
    int x = (tileid % tileset->columns) * (tileset->tileWidth + tileset->spacing);
    int y = (tileid / tileset->columns) * (tileset->tileWidth + tileset->spacing);

    int textureRectWidth = tileset->tileWidth;
    int textureRectHeight = tileset->tileHeight;

    if (flipFlags & 2)
    {
        textureRectHeight *= -1;
        y += tileset->tileHeight;
    }

    // Horizontal flip
    if (flipFlags & 4)
    {
        textureRectWidth *= -1;
        x += tileset->tileWidth;
    }

    // Set the texture and texture source rectangle
    sprite.setTexture(tileset->texture);
    sprite.setTextureRect(sf::IntRect(x, y, textureRectWidth, textureRectHeight));


}

// Protected functions

void maptest::loadTileset(Json::Value& tileset)
{
    auto ts = std::make_shared<Tileset>();
    ts->tileCount = tileset["tilecount"].asUInt();
    ts->firstGid  = tileset["firstgid"].asUInt();
    ts->columns  = tileset["columns"].asInt();
    ts->imageWidth  = tileset["imagewidth"].asInt();
    ts->imageHeight  = tileset["imageheight"].asInt();
    ts->tileWidth  = tileset["tilewidth"].asInt();
    ts->tileHeight  = tileset["tileheight"].asInt();
    ts->spacing  = tileset["spacing"].asInt();

    ts->texture.loadFromFile("data/" + tileset["image"].asString());
    ts->texture.setSmooth(false);


    loadTileAtlas(ts);
    // Load all animations
    for (Json::Value& tile: tileset["tiles"])
    {
        int animationId = ts->firstGid + tile["id"].asInt();

        for (Json::Value& animation: tile["animation"]) {
            unsigned int gid = ts->firstGid + animation["tileid"].asUInt();
            int duration = animation["duration"].asInt();

            if (animations.find(animationId) == animations.end())
                animations[animationId]= std::make_shared<std::vector<AnimationFrame>>();

            animations[animationId]->push_back(AnimationFrame(gid, duration));
        }

        //henter ut basert på custom tile properties satt i Tiled
        for (Json::Value& properties: tile["properties"])
        {
            std::vector<int> pointPos;
            std::string p = properties["value"].asString();
            std::regex a(R"([0-9]?[0-9])");
            auto p_begin = std::sregex_iterator(p.begin(), p.end(), a);
            auto p_end = std::sregex_iterator();

            for (std::sregex_iterator i = p_begin; i != p_end; ++i) {
                std::smatch match = *i;
                std::string match_str = match.str();
                pointPos.push_back(std::stoi(match_str));
            }

            if(properties["name"].asString() == "damage")
            {
                if (properties["value"].asBool())
                    tileAtlas[tile["id"].asUInt() + ts->firstGid].hurtful = true;
                else
                    tileAtlas[tile["id"].asUInt() + ts->firstGid].hurtful = false;
            }
            if(std::regex_search(p, a)) {
                tileAtlas[tile["id"].asUInt() + ts->firstGid].polyPoints.clear();
                for (size_t i = 0; i < pointPos.size(); i += 2)
                    tileAtlas[tile["id"].asUInt() + ts->firstGid].polyPoints.emplace_back(pointPos[i], pointPos[i + 1]);
            }
        }

    }
    tilesets.push_back(ts);
}

void maptest::loadTileLayer(Json::Value& layer)
{
    auto tmp = std::make_shared<Layer>(*this);

    // Store info on layer
    tmp->id = layer["id"].asInt();
    tmp->name = layer["name"].asString();
    tmp->visible = layer["visible"].asBool();
    tmp->width = layer["width"].asInt();
    tmp->height = layer["height"].asInt();
    tmp->tilemap = new unsigned int[tmp->width * tmp->height];

    // Read in tilemap
    for (size_t i = 0; i < layer["data"].size(); i++)
        tmp->tilemap[i] = layer["data"][(int) i].asUInt();

    layers.push_back(tmp);
}

void maptest::loadObjectLayer(Json::Value& layer)
{
    // Get all objects from layer
    for (Json::Value& object: layer["objects"])
    {
        auto sprite = std::make_shared<Sprite>(*this);

        // Load basic object info
        sprite->id = object["id"].asInt();
        sprite->name = object["name"].asString();
        sprite->x = object["x"].asInt();
        sprite->y = object["y"].asInt();
        sprite->width = object["width"].asInt();
        sprite->height = object["height"].asInt();
        sprite->y -= sprite->height; // Not sure why Tiled anchor in the bottom left...


        sprites.push_back(sprite);
    }
}

TileData *maptest::getTileData(unsigned int gidIn)
{

    unsigned int flipFlags = gidIn >> 29;
    unsigned int gid = gidIn &= ~(0b111 << 29);


    try{
        //auto ting = &tileset.get()->tileAtlas.at(gid);
        auto ting = &tileAtlas.at(gid);
        return ting;
    }
    catch (std::out_of_range) {
        return nullptr;
    }

}

void maptest::loadTileAtlas(const std::shared_ptr<Tileset>& ts)
{

    unsigned int tileCAdj = ts->tileCount + ts->firstGid;
    for(unsigned int i = ts->firstGid; i < tileCAdj; i++)
    {
        tileAtlas[i].polyPoints = {{0,  0}, {16, 0}, {16, 16}, {0,  16}};
        tileAtlas[i].hurtful = false;
        tileAtlas[i].gid = i;
        tileAtlas[i].bounciness = 0;
    }
}





