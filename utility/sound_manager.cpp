//
// Created by Axel on 20/11/2019.
//

#include <iostream>
#include "sound_manager.h"

void SoundManager::loadSoundBfr(const std::string &name, const std::string &filename)
{


    //auto soundBfr = std::make_shared<sf::SoundBuffer>();
    sf::SoundBuffer soundBfr;

    if(!soundBfr.loadFromFile(filename))
    {
        return;
    }

    this->soundBuffers[name] = soundBfr;

}

sf::SoundBuffer& SoundManager::getRef(const std::string &sound)
{
    return this->soundBuffers.at(sound);
}

SoundManager::~SoundManager()
{


}
