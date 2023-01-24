//
// Created by Axel on 20/11/2019.
//

#ifndef ICEBERG_SOUND_MANAGER_H
#define ICEBERG_SOUND_MANAGER_H


#include <map>
#include <SFML/Audio.hpp>
#include <memory>

class SoundManager
{
private:
    std::map<std::string, sf::SoundBuffer> soundBuffers;

public:
    void loadSoundBfr(const std::string& name, const std::string& filename);

    sf::SoundBuffer& getRef(const std::string &sound);

    SoundManager() = default;
    ~SoundManager();
};


#endif //ICEBERG_SOUND_MANAGER_H
