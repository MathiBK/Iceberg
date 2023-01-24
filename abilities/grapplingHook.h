#ifndef ICEBERG_GRAPPLINGHOOK_H
#define ICEBERG_GRAPPLINGHOOK_H

#include <SFML/Graphics/RectangleShape.hpp>
#include "ability.h"
#include "../objects/entities/entity.h"
#include "../states/play_state.h"
#include "../objects/particles/grappler.h"
#include <set>


class grapplingHook : public ability {
    friend class Grappler;
public:
    explicit grapplingHook(playState& pl, Entity* ent, const sf::SoundBuffer& s);
    ~grapplingHook();
    sf::Vector2f getPosition();

    float maxLength;
    sf::RectangleShape body;

protected:
    playState& pl;
    sf::Sound sound;

    void fire() override;

    sf::Vector2f getDirection();
    std::shared_ptr<Particle> grappler;
};


#endif //ICEBERG_GRAPPLINGHOOK_H
