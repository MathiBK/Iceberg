
#include "dash.h"
#include "../objects/entities/entity.h"

dash::dash(Entity* ent) : ability(ent)
{
    this->uses = 1;
    this->id = "Dash";
    this->totalUses = 1;
    this->cooldown = 2.0f;
    clock.restart();
}

void dash::fire()
{
    elapsed = clock.getElapsedTime().asSeconds();

    if(elapsed >= cooldown) {
        if(uses < totalUses){
            uses = totalUses;
            clock.restart();
        }
    }

    if(uses > 0) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            direction.x += powerX;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
            direction.x -= powerX;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
            direction.y -= powerY;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
            direction.y += powerY;
        }

        if(direction.x == 0 && direction.y == 0){
            return;
        } else {
            if(direction.x != 0) {
                ent->velocityX = direction.x;
            }
            ent->velocityY = direction.y;
            direction.x = 0;
            direction.y = 0;
            uses -= 1;
        }
    }
}

