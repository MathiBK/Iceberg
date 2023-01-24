#ifndef ICEBERG_PLAYER_H
#define ICEBERG_PLAYER_H

#include "entity.h"
#include "../../map/maptest.h"
#include "../../animation/animation.h"
//class animation;


class maptest;
class Player : public Entity
{
public:
    Player(maptest &map, float x, float y, SoundManager& sm, std::list<int> &enemiesDiedTooId, sf::Texture& tex);
    void draw(sf::RenderWindow& window) override;
    void process(float time) override;

    sf::Vector2f move(float delta);
    float maxMs = 75.0f;
    bool canJump = true;
    int lvl;
    int totalJumpAmount = 1;
    int currentJumpAmount = 0;
    sf::Vector2f getPosition() { return rectangle.getPosition();}
    sf::RectangleShape& getBody() {return rectangle;}
    void setPosition(float x, float y)  {this->x = x; this->y = y;}
    bool checkCollision(sf::FloatRect& pos);
    std::list<int> enemiesDiedTooId;

    int getXp() {return this->xp;}
    void setXp(int xp) {this->xp=xp;}
    int getLvl() {return this->lvl;}
    void setLvl(int lvl) {this->lvl=lvl;}

    animation idle(),crouched(),running(), jump(), doublejump(), attacking(), airAttack(),falling(), sliding(),
    attack1(), attack2(), attack3(),air_attack1(), air_attack2(), air_attack3(), hurt(), dying(), bow(), air_bow(),
    item();



    void collision(std::list<Entity*>& entities);
private:
    float x, y;
    int xp;
    bool invincible;
    sf::Clock inTimer;
    bool blink = true;


    void invincibleHandler();

    void loadFrames();



    void loadSounds();



    void loadHurtBoxes();
};


#endif //ICEBERG_PLAYER_H
