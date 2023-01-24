#include <iostream>
#include "basic_attack.h"
#include "../objects/entities/entity.h"
#include "../objects/object_physics.h"

BasicAttack::BasicAttack(Entity *ent, std::list<Entity*>& entList) : ability(ent), entityList(entList)
{
    this->id = "Sword";
    this->attackDirection = sf::Vector2f(0,0);
    this->cooldown = ent->currentAnimation.getLength(); // hvorlang tid du må vente før du kan angripe igjen
    this->reset = 1.5f; // hvor lang tid det tar før angreps kombo resettes
    this->uses = 0;  //Hvor du er i komboen
    this->totalUses = 3; // hvor lang komboen er
}



sf::Vector2f BasicAttack::getDirection()
{
    sf::Vector2f dir(0,0);
    //Opp-ned, og høyre-venstre kan ikke kombineres


    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        dir.y = -1;
        //ent.hitArea.setScale(x,1.0f);
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        dir.y = 1;
        //ent.hitArea.setScale(x,-1.0f);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        dir.x = -1;
        //ent.hitArea.setScale(1.0f,1.0f);
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        dir.x = 1;
        //ent.hitArea.setScale(-1.0f,1.0f);
    }
    return dir;
}



void BasicAttack::fire()
{

    elapsed = clock.getElapsedTime().asSeconds();
    float y = 1.0f;
    cooldown = ent->currentAnimation.getLength();
    cooldown -= 0.1;

    if(elapsed >= cooldown) {
        ///Får sjekke om en ent allerde er blitt truffet av et angrep, men gjør ingenting nå ettersom den sjekker kun etter kollisjon etter i begynnelsen av angrepet
//        for (auto& it : entityList) {
//            if (it->isHit)
//                it->isHit = !it->isHit;
//        }
        uses++;
        if (elapsed >= reset+cooldown)
            uses = 1;
        if(uses > totalUses){
            uses = 1;
        }
        clock.restart();

    }
    if(elapsed >= cooldown) {
        if (ent->currentState == EntityState::ATTACKING || ent->currentState == EntityState::AIR_ATTACK)
        {
            ent->hitArea = ent->hitAreas[AttackArea::SIDES];
            totalUses = 3;
        }

        //sjekker om ent er player
        if (ent->player) {
            attackDirection = getDirection();
            if (ent->currentState == EntityState::ATTACKING) {
                if (uses == 1) {
                    {
                        ent->currentAnimation.overrideFrames(ent->animationMap.at(EntityState::ATTACK1));
                        ent->attackDmg = 1;
                    }
                } else if (uses == 2) {
                    {
                        ent->currentAnimation.overrideFrames(ent->animationMap.at(EntityState::ATTACK2));
                        ent->attackDmg = 1;
                    }
                } else if (uses == 3) {
                    ent->hitArea = ent->hitAreas[AttackArea::BOTTOM];
                    y = -1.0f;
                    ent->currentAnimation.overrideFrames(ent->animationMap.at(EntityState::ATTACK3));
                    ent->attackDmg = 3;
                }
            }
            else if (ent->currentState == EntityState::AIR_ATTACK)
            {
                if (uses == 1) {
                    {
                        ent->currentAnimation.overrideFrames(ent->animationMap.at(EntityState::AIR_ATTACK1));
                        ent->attackDmg = 1;
                    }
                } else if (uses == 2) {
                    {
                        ent->currentAnimation.overrideFrames(ent->animationMap.at(EntityState::AIR_ATTACK2));
                        ent->attackDmg = 1;
                    }
                } else if (uses == 3) {
                    ent->hitArea = ent->hitAreas[AttackArea::FULL];
                    y = -1.0f;
                    ent->currentAnimation.overrideFrames(ent->animationMap.at(EntityState::AIR_ATTACK3));
                    ent->attackDmg = 2;
                }
            }
        } else
            {
                ent->currentAnimation.overrideFrames(ent->animationMap.at(EntityState::ATTACK1));
        }

///Bruker ikke attackDirection til noe akkurat nå
       // if (attackDirection.x == 0 && attackDirection.y == 0) {
            //om ingen keys er trykket, angrip retning karakter står
            if (ent->d_Left) {
              //  attackDirection = sf::Vector2f(-1, 0);
                ent->hitArea.setScale(1.0f, y);
            } else {
            //    attackDirection = sf::Vector2f(1, 0);
                ent->hitArea.setScale(-1.0f, y);
            }
      //  }
        /// oppdater hurtboxes her basert på direction

        ent->hitArea.setPosition(ent->sprite.getPosition());
        ent->boundingBox = ent->hitArea.getGlobalBounds();

            checkCollision();


    }

}

void BasicAttack::checkCollision()
//{
//    Polygon hurtBox = this->ent.currentHurtBox;
//
//    for (auto& it : entityList)
//    {
//        if(it == &this->ent)
//            continue;
//        std::vector<sf::Vector2f> otherP;
//
//        int pointCount = it->rectangle.getPointCount();
//        otherP.reserve(pointCount);
//        for(int i = 0; i < pointCount; i++ )
//        {
//            otherP.push_back(it->rectangle.getPoint(i));
//        }
//
//        Polygon otherPoly(otherP);
//        otherPoly.pos = it->rectangle.getPosition()*it->spriteScale;
//
//        Polygon otherTest = it->entPoly;
//        //aner ikke hva jeg gjorde her, men det funker
//        otherTest.pos = sf::Vector2f(it->rectangle.getPosition().x - it->rectangle.getSize().x/2.f, it->rectangle.getPosition().y - it->rectangle.getSize().y/2.f) / it->spriteScale;
//        otherTest.updatePoly();
//        sf::Vector2f collision = objectPhysics::collisionSAT_Intersect(hurtBox, otherTest);
//
//        if(collision.x != 0 || collision.y != 0)
//        {
//            std::cout << "Hit!";
//            it->hp--;
//        }
//
//
//
//    }
//
//
//}
{

    for (auto& it : entityList)
    {
        if(it == this->ent || it->isHit) // skal legge til at den sjekker kolisjon gjennom hele angrepet, men ikke for de som allerede er truffet
            continue;
        it->boundingBox = it->rectangle.getGlobalBounds();
        if(this->ent->boundingBox.intersects(it->boundingBox))
        {
            it->hp -= ent->attackDmg;
            if(!it->player && it->hp > 0)
                it->setCurrentState(EntityState::HURT);
        }
    }
}