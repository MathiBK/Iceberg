
#include "grapplingHook.h"
#include "../objects/particles/grappler.h"


grapplingHook::grapplingHook(playState& pl, Entity* ent, const sf::SoundBuffer& s) : ability(ent), pl(pl)
{
    this->id = "Grappling Hook";
    this->uses = 1;
    this->maxLength = 150;
    this->body.setSize(sf::Vector2f(10, 10));
    this->sound.setBuffer(s);
    this->grappler = nullptr;
    this->totalUses = 1;
    this->cooldown = 0.5f;
    clock.restart();
   // this->sound.setBuffer(this->pl.stateMachine->soundmgr.getRef("cannon"));

}

void grapplingHook::fire()
{
    if(this->grappler != nullptr) {
        pl.objects.remove(grappler);
        this->grappler = nullptr;
        clock.restart();
        return;
    }

    elapsed = clock.getElapsedTime().asSeconds();
    if(elapsed >= cooldown) {
        if(uses < totalUses){
            uses = totalUses;
        }
    }

    if(uses > 0){
        sf::Vector2f dir = getDirection();
        sound.setVolume(5.f);
        //sound.play();
        ent->setCurrentState(EntityState::ITEM);
        this->grappler = std::make_shared<Grappler>(pl.map, ent->rectangle.getPosition().x + 2 * dir.x, ent->rectangle.getPosition().y + 2 * dir.y, dir, *this);
        this->ent->grappled += 1;
        pl.objects.push_back(this->grappler);
        uses--;
    }
}

sf::Vector2f grapplingHook::getDirection()
{

    sf::Vector2f dir(0,0);
    //Opp-ned, og høyre-venstre kan ikke kombineres

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        dir.y = -1;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        dir.y = 1;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        dir.x = -1;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        dir.x = 1;
    }

    return dir;
}

sf::Vector2f grapplingHook::getPosition()
{
    return this->ent->rectangle.getPosition();
}

grapplingHook::~grapplingHook()
{
    sound.stop();
    sound.resetBuffer();
}
