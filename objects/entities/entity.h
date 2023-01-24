#ifndef ICEBERG_entity_H
#define ICEBERG_entity_H

#include "../object.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
#include "../../abilities/ability.h"
#include "../../animation/animation.h"
#include "../../utility/sound_manager.h"
#include "../../utility/polygon.h"

struct Collider
{
    Entity* ptr;
    std::string direction;
    sf::FloatRect intersect;
};

struct TileCollisionData
{
    bool hurtful;
    int tilesCollided[9] = {0};
    std::string direction;
    sf::FloatRect intersect;
};

enum class EntityState
{
    RUNNING,
    CROUCHING,
    IDLE,
    JUMPING,
    DOUBLEJUMPING,
    FALLING,
    SLIDING,
    ATTACKING,
    AIR_ATTACK,
    HURT,
    DYING,
    ATTACK1,
    ATTACK2,
    ATTACK3,
    AIR_ATTACK1,
    AIR_ATTACK2,
    AIR_ATTACK3,
    BOW,
    AIR_BOW,
    ITEM
};

enum class AttackArea
{
    FULL,
    SIDES,
    TOP,
    BOTTOM,
};

class Entity : public Object {
public:

    Entity(maptest &map, float x, float y, float hitboxWidth, float hitboxHeight, int hp, float ms, float jumpPower, float spriteScale,
            sf::Color color, bool player, int id, int spriteHeight, int spriteWidth, SoundManager& sm);
    virtual ~Entity() = default;

    virtual void collision(std::list<Entity*> &entities);
    TileCollisionData checkCollision(Entity &ent);

    Entity * collisionEntX(sf::Vector2f& prev, std::list<Entity*> &entities);
    Entity * collisionEntY(sf::Vector2f& prev, std::list<Entity*> &entities);
    Collider collisionEnt(sf::Vector2f &prev, std::list<Entity *> &entities);

    std::map<std::string, ability*> activeAbilities;
    std::map<std::string, animation*> animations;

    virtual sf::Vector2f move(float delta) = 0;

    void process(float time) override;
    void draw(sf::RenderWindow& window) override;
    sf::Sprite getSprite() {return sprite;}


    float x, y, velocityX, velocityY, ms, jumpPower, spriteScale;

    bool grounded, d_Left,  invertedSprite, isHit, isIdle{}, isAttacking{}, isMoving{}, isJumping{}, isFalling{}, isCrouching{}, isReady{}, isSliding{}, lockAnimation{};


    int id, money, hp, grappled, attackDmg;
    double angleVelocity, angle;
    const bool player;
    const float hitboxWidth, hitboxHeight;
    float spriteWidth, spriteHeight;
    bool canMoveLeft = true;
    bool canMoveRight = true;
    bool canMoveUp = true;
    bool canMoveDown = true;
    sf::Sprite sprite;
    SoundManager& soundmgr;
    sf::Texture texture;
    sf::RectangleShape rectangle;
    sf::RectangleShape hitArea;
    animation currentAnimation;
    std::map<EntityState , animation>animationMap;
    std::map<AttackArea, sf::RectangleShape> hitAreas;
    std::map<EntityState, Polygon> hurtBoxes;
    Polygon currentHurtBox;
    sf::FloatRect boundingBox;
    AttackArea currentAttackArea;
    EntityState currentState;
    std::map<std::string, sf::Sound> soundMap;
    float deathTimer{};

    ///Brukes til debugging, skal fjernes -A
    Polygon entPoly;

    sf::RectangleShape top;
    sf::RectangleShape bot;
    sf::RectangleShape left;
    sf::RectangleShape right;

    void initHitArea();
    void reduceHitBox();
    void reduceHitBoxAir();
    void resetHitBox();
    void setCurrentState(EntityState s);
    void currentStateChanged();
    void setSprite(const std::string& filepath);


    bool goingToCollide(Polygon* p);

    void polygonFlipHandler(Polygon *p, unsigned int gid);
};


#endif //ICEBERG_entity_H
