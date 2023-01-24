#include "entity.h"
#include "../../map/maptest.h"
#include "../layer.h"
#include "../../utility/polygon.h"
#include "../object_physics.h"
#include <math.h>



Entity::Entity(maptest &map, float x, float y, float hitboxWidth, float hitboxHeight,  int hp, float ms, float jumpPower, float spriteScale,
               sf::Color color, bool playerBool, int id, int spriteHeight, int spriteWidth, SoundManager& sm)
        : Object(map), x(x), y(y), hitboxWidth(hitboxWidth), hitboxHeight(hitboxHeight), velocityX(0), velocityY(0), hp(hp),
          ms(ms), jumpPower(jumpPower), spriteScale(spriteScale), player(playerBool), currentAnimation(&sprite),
          spriteHeight(spriteHeight), spriteWidth(spriteWidth), soundmgr(sm) {
    this->id = id;
    this->angle = 0;
    this->angleVelocity = 0;
    grounded = false;
    rectangle.setSize(sf::Vector2f(hitboxWidth*spriteScale, hitboxHeight*spriteScale));
    rectangle.setPosition(sf::Vector2f(x, y));
    rectangle.setFillColor(color);
    d_Left = true;
    invertedSprite = false;
    isHit = false;
    grappled = 0;
    isReady = true;
    deathTimer = 0;
    attackDmg = 1;
    currentState = EntityState::IDLE;

    ///Debugging
    bot = sf::RectangleShape(sf::Vector2f(0,0));
    top = sf::RectangleShape(sf::Vector2f(0,0));
    left = sf::RectangleShape(sf::Vector2f(0,0));
    right = sf::RectangleShape(sf::Vector2f(0,0));

    top.setOutlineThickness(1);
    bot.setOutlineThickness(1);
    top.setOutlineColor(sf::Color(0xff,0xff,0xff));
    bot.setOutlineColor(sf::Color(0xff,0,0xff));

    top.setFillColor(sf::Color::Transparent);
    bot.setFillColor(sf::Color::Transparent);


    left.setOutlineThickness(1);
    right.setOutlineThickness(1);
    left.setOutlineColor(sf::Color(0xff,0xff,0xff));
    right.setOutlineColor(sf::Color(0,0,0xff));

    left.setFillColor(sf::Color::Transparent);
    right.setFillColor(sf::Color::Transparent);
}

void Entity::draw(sf::RenderWindow& window)
{
    rectangle.setPosition(x, y);
    window.draw(rectangle);
}


Collider Entity::collisionEnt(sf::Vector2f &prev, std::list<Entity *> &entities) {
    Collider collider;
    sf::Vector2f currentPos = this->rectangle.getPosition();
    sf::FloatRect intersect;
    //Sjekker kollisjon med andre ents
    float f = this->rectangle.getPointCount();
    for (auto &e : entities)
    {
        sf::Vector2f otherPos = e->rectangle.getPosition();
        if (e == this)
            continue;
        if (this->rectangle.getGlobalBounds().intersects(e->rectangle.getGlobalBounds(), intersect))
        {

            float yCorrected = abs(currentPos.y - otherPos.y);
            float xCorrected = abs(currentPos.x - otherPos.x);
            bool yLarger = yCorrected > xCorrected;
        if(yLarger)
        {
            if (currentPos.y < otherPos.y)
            {
                //treffer toppen
                this->rectangle.move(0.0, -intersect.height / 2);
                e->rectangle.move(0.0, intersect.height / 2);
                collider.direction = "top";
            } else if (currentPos.y > otherPos.y)
            {
                //treffer bunnen
                this->rectangle.move(0.0, intersect.height / 2);
                e->rectangle.move(0.0, -intersect.height / 2);
                collider.direction = "bot";

            }
        }
        else
            {
            currentPos = rectangle.getPosition();
            // otherPos = rectangle.getPosition();

            if (currentPos.x < otherPos.x)
            {
                //treffer venstre
                this->rectangle.move(-intersect.width, 0.0f);
                e->rectangle.move(intersect.width, 0.0f);
                collider.direction = "left";
            } else if (currentPos.x > otherPos.x)
            {
                //treffer høyre
                this->rectangle.move(intersect.width, 0.0f);
                e->rectangle.move(-intersect.width, 0.0f);
                collider.direction = "right";
            }
         }
            collider.ptr = e;
            return collider;
        }
    }

    collider.ptr = nullptr;
    return collider;
}

bool Entity::goingToCollide(Polygon* p)
{
    Polygon tile = *p;
    tile.name = "collisionChecker";

    std::vector<sf::Vector2f> gp;
    int pointCount = this->rectangle.getPointCount();
    for(int i = 0; i < pointCount; ++i)
    {
        gp.push_back(this->rectangle.getPoint(i));
    }
    //Polygon collisionHelper(gp);
    Polygon collisionHelper = entPoly;
    collisionHelper.setOrigin(rectangle.getSize().x/2.f, rectangle.getSize().y/2.f);
    collisionHelper.pos = sf::Vector2f(entPoly.pos.x + velocityX, entPoly.pos.y + velocityY);
    int xPos = (int)floor(collisionHelper.pos.x)  / map.getTileWidth();
    int yPos = (int)floor(collisionHelper.pos.y)  / map.getTileWidth();
    collisionHelper.name = "collision helper";
    collisionHelper.updatePoly();
    sf::Vector2f gMTV = objectPhysics::collisionSAT_Intersect(collisionHelper, tile);
    return gMTV.x != 0 || gMTV.y != 0;
}


TileCollisionData Entity::checkCollision(Entity &ent)
{
    TileCollisionData tcd = TileCollisionData();
    bool collided = false;

    int tilesCollided[9] = {0};

    sf::Vector2f currentPos = this->rectangle.getPosition();

    auto l = map.getLayer("Solid").get();
    int height = l->getHeight();
    int width = l->getWidth();


    //x-posisjon er lik uansett i y-aksen, såbruker samme for begge
    int xPos = (int)floor(currentPos.x)  / map.getTileWidth();
    int yPos = (int)floor(currentPos.y)  / map.getTileWidth();

    //Brukes for å finne id til tile under og over spiller
    int yPosBot = (int)floor(currentPos.y + (float)map.getTileHeight() ) / map.getTileHeight();
    int yPosTop = (int)floor(currentPos.y - (float)map.getTileHeight()) / map.getTileHeight();


    //Brukes for å finne id til tile til høyre og venstre for spiller
    int xPosleft = (int)floor(currentPos.x - (float)map.getTileWidth()) / map.getTileHeight();
    int xPosRight = (int)floor(currentPos.x + (float)map.getTileWidth()) / map.getTileHeight();

    unsigned int gidRight = l->getTilemap()[xPosRight + yPos * width];
    unsigned int gidLeft = l->getTilemap()[xPosleft + yPos * width];

    unsigned int gidBot = l->getTilemap()[xPos + yPosBot * width];
    unsigned int gidTop = l->getTilemap()[xPos + yPosTop * width];

    unsigned int gidBotLeft = l->getTilemap()[xPosleft + yPosBot * width];
    unsigned int gidBotRight = l->getTilemap()[xPosRight + yPosBot * width];

    unsigned int gidTopLeft = l->getTilemap()[xPosleft + yPosTop * width];
    unsigned int gidTopRight = l->getTilemap()[xPosRight + yPosTop * width];

    unsigned int gidThis = l->getTilemap()[xPos + yPos * width];

    ///Debugging
    this->bot.setPosition({(float) xPos * map.getTileWidth(), (float) (yPosBot * map.getTileHeight())});
    this->bot.setSize({(float)map.getTileHeight(), (float)map.getTileWidth()});

    this->top.setPosition({(float) xPos * map.getTileWidth(), (float) (yPosTop * map.getTileHeight())});
    this->top.setSize({(float)map.getTileHeight(), (float)map.getTileWidth()});


    this->left.setPosition({(float) xPosleft * map.getTileWidth(), (float) (yPos * map.getTileHeight())});
    this->left.setSize({(float)map.getTileHeight(), (float)map.getTileWidth()});

    this->right.setPosition({(float) xPosRight * map.getTileWidth(), (float) (yPos * map.getTileHeight())});
    this->right.setSize({(float)map.getTileHeight(), (float)map.getTileWidth()});
    ///


    entPoly.pos = sf::Vector2f(this->rectangle.getPosition().x - this->rectangle.getSize().x/2.f, this->rectangle.getPosition().y - this->rectangle.getSize().y/2.f);
    entPoly.updatePoly();
    sf::Vector2f MTVtest {0,0};

    if(gidTop != 0)

    {

        std::vector<sf::Vector2f> topP;

        //Lager polygoner for tile sin hitbox

        TileData* t = map.getTileData(gidTop);
        if(t == nullptr)
            topP = {{0,  0}, {16, 0}, {16, 16}, {0,  16}};
        else
            topP = t->polyPoints;

        Polygon topPoly(topP);
        topPoly.pos = sf::Vector2f((float) (xPos * map.getTileWidth()), (float) (yPosTop * map.getTileHeight())),sf::Vector2f(map.getTileHeight(), map.getTileWidth());
        topPoly.updatePoly();
        topPoly.name = "topPoly";
        polygonFlipHandler(&topPoly, gidTop);



        sf::Vector2f MTV = objectPhysics::collisionSAT_Intersect(entPoly, topPoly);
        MTVtest += MTV;

        if(MTV.y != 0 || MTV.x != 0)
        {
            rectangle.move(MTV.x, MTV.y);

            tcd.hurtful |= t->hurtful;
            tilesCollided[1] = 1;
            collided = true;
            if(ent.velocityY < 0){
                velocityY *= -0.3f;
            }
        }
    }

    if(gidTopLeft != 0)
    {
        std::vector<sf::Vector2f> topLeftP;

        //Lager polygoner for tile sin hitbox

        TileData* t = map.getTileData(gidTopLeft);
        if(t == nullptr)
            topLeftP = {{0,  0}, {16, 0}, {16, 16}, {0,  16}};
        else
            topLeftP = t->polyPoints;

        Polygon topLeftPoly(topLeftP);
        topLeftPoly.pos = sf::Vector2f((float) (xPosleft * map.getTileWidth()), (float) (yPosTop * map.getTileHeight())),sf::Vector2f(map.getTileHeight(), map.getTileWidth());
        topLeftPoly.updatePoly();
        topLeftPoly.name = "topleftpoly";
        polygonFlipHandler(&topLeftPoly, gidTopLeft);



        sf::Vector2f MTV = objectPhysics::collisionSAT_Intersect(entPoly, topLeftPoly);
        MTVtest += MTV;

        if(MTV.y != 0 || MTV.x != 0)
        {

            rectangle.move(0, MTV.y);
            tcd.hurtful |= t->hurtful;
            tilesCollided[0] = 1;
            collided = true;
        }
    }

    if(gidTopRight != 0)
    {
        std::vector<sf::Vector2f> topRightP;

        TileData* t = map.getTileData(gidTopRight);
        if(t == nullptr)
            topRightP = {{0,  0}, {16, 0}, {16, 16}, {0,  16}};
        else
            topRightP = t->polyPoints;

        Polygon topRightPoly(topRightP);
        topRightPoly.pos = sf::Vector2f((float) (xPosRight * map.getTileWidth()), (float) (yPosTop * map.getTileHeight())),sf::Vector2f(map.getTileHeight(), map.getTileWidth());
        topRightPoly.updatePoly();
        topRightPoly.name = "botRightPoly";
        polygonFlipHandler(&topRightPoly, gidTopRight);



        sf::Vector2f MTV = objectPhysics::collisionSAT_Intersect(entPoly, topRightPoly);
        MTVtest += MTV;

        if(MTV.y != 0 || MTV.x != 0)
        {

            rectangle.move(MTV.x, MTV.y);

            tcd.hurtful |= t->hurtful;
            tilesCollided[2] = 1;

            collided = true;
        }


    }

    if(gidLeft != 0)
    {
        std::vector<sf::Vector2f> leftP;

        TileData* t = map.getTileData(gidLeft);
        if(t == nullptr)
            leftP = {{0,  0}, {16, 0}, {16, 16}, {0,  16}};
        else
            leftP = t->polyPoints;

        Polygon leftPoly = Polygon(leftP);
        leftPoly.pos = sf::Vector2f((float) (xPosleft * map.getTileWidth()), (float) (yPos * map.getTileHeight())),sf::Vector2f(map.getTileHeight(), map.getTileWidth());
        leftPoly.updatePoly();
        leftPoly.name = "leftpoly";
        polygonFlipHandler(&leftPoly, gidLeft);




        sf::Vector2f MTV = objectPhysics::collisionSAT_Intersect(entPoly, leftPoly);
        MTVtest += MTV;
        if(MTV.x != 0 || MTV.y != 0) {
            tilesCollided[3] = 1;
            collided = true;
            rectangle.move(MTV.x, MTV.y);

            tcd.hurtful |= t->hurtful;
            velocityX = 0;
        }
    }


    if(gidThis != 0)
    {

        std::vector<sf::Vector2f> leftP;
        TileData* t = map.getTileData(gidThis);
        if(t == nullptr)
            leftP = {{0,  0}, {16, 0}, {16, 16}, {0,  16}};
        else
            leftP = t->polyPoints;

        Polygon thisPoly(leftP);
        thisPoly.pos = sf::Vector2f((float) (xPos * map.getTileWidth()), (float) (yPos * map.getTileHeight())),sf::Vector2f(map.getTileHeight(), map.getTileWidth());
        thisPoly.updatePoly();
        thisPoly.name = "thispoly";
        polygonFlipHandler(&thisPoly, gidThis);

        sf::Vector2f MTV = objectPhysics::collisionSAT_Intersect(entPoly, thisPoly);
        MTVtest += MTV;

        if(MTV.x != 0 || MTV.y != 0)
        {
            tilesCollided[4] = 1;
            rectangle.move(MTV.x, MTV.y);

            tcd.hurtful |= t->hurtful;
            if(gidThis == 256) {
                velocityX *= 0.75;
                velocityY *= 0.5;
            }
            collided = true;
            //grounded = false;
        }


    }

    if(gidRight != 0)
    {
        std::vector<sf::Vector2f> rightP;

        TileData* t = map.getTileData(gidRight);
        if(t == nullptr)
            rightP = {{0,  0}, {16, 0}, {16, 16}, {0,  16}};
        else
            rightP = t->polyPoints;
        Polygon rightPoly(rightP);
        rightPoly.pos = sf::Vector2f((float) (xPosRight * map.getTileWidth()), (float) (yPos * map.getTileHeight())),sf::Vector2f(map.getTileHeight(), map.getTileWidth());
        rightPoly.updatePoly();
        rightPoly.name = "rightPoly";
        polygonFlipHandler(&rightPoly, gidRight);


        sf::Vector2f MTV = objectPhysics::collisionSAT_Intersect(entPoly, rightPoly);
        MTVtest += MTV;

        if(MTV.x != 0 || MTV.y != 0) {
            tilesCollided[5] = 1;
            collided = true;
            rectangle.move(MTV.x, MTV.y);

            tcd.hurtful |= t->hurtful;
            if(gidRight == 256) {
                velocityX *= 0.75;
                velocityY *= 0.5;
            } else {
                velocityX = 0;
            }
        }


    }

    if(gidBot != 0)
    {
        std::vector<sf::Vector2f> botP;
        TileData* t = map.getTileData(gidBot);
        if(t == nullptr)
            botP = {{0,  0}, {16, 0}, {16, 16}, {0,  16}};
        else
            botP = t->polyPoints;


        Polygon botPoly(botP);
        botPoly.pos = sf::Vector2f((float) (xPos * map.getTileWidth()), (float) (yPosBot * map.getTileHeight())),sf::Vector2f(map.getTileHeight(), map.getTileWidth());
        botPoly.updatePoly();
        botPoly.name = "botPoly";
        polygonFlipHandler(&botPoly, gidBot);


        sf::Vector2f MTV = objectPhysics::collisionSAT_Intersect(entPoly, botPoly);
        MTVtest += MTV;


        if(MTV.y != 0 || MTV.x != 0)
        {
            tilesCollided[7] = 1;

            tcd.hurtful |= t->hurtful;
            rectangle.move(MTV.x, MTV.y);
            if(gidBot == 256) {
                velocityX *= 0.75;
                velocityY *= 0.5;
            }

            collided = true;
            grounded = true;
        }
    }

    if(gidBotLeft != 0)
    {
        std::vector<sf::Vector2f> botP;

        //Lager polygoner for tile sin hitbox
        TileData* t = map.getTileData(gidBotLeft);
        if(t == nullptr)
            botP = {{0,  0}, {16, 0}, {16, 16}, {0,  16}};
        else
            botP = t->polyPoints;

        Polygon botLeftPoly(botP);
        botLeftPoly.pos = sf::Vector2f((float) (xPosleft * map.getTileWidth()), (float) (yPosBot * map.getTileHeight())),sf::Vector2f(map.getTileHeight(), map.getTileWidth());
        botLeftPoly.updatePoly();
        botLeftPoly.name = "botPoly";
        polygonFlipHandler(&botLeftPoly, gidBotLeft);

        sf::Vector2f MTV = objectPhysics::collisionSAT_Intersect(entPoly, botLeftPoly);
        MTVtest += MTV;

        if(MTV.y != 0 || MTV.x != 0)
        {
            tilesCollided[6] = 1;
            rectangle.move(MTV.x, MTV.y);

            tcd.hurtful |= t->hurtful;
            collided = true;
        }
    }

    if(gidBotRight != 0)
    {
        std::vector<sf::Vector2f> botP;

        //Lager polygoner for tile sin hitbox
        TileData* t = map.getTileData(gidBotRight);
        if(t == nullptr)
            botP = {{0,  0}, {16, 0}, {16, 16}, {0,  16}};
        else
            botP = t->polyPoints;
        Polygon botRightPoly(botP);
        botRightPoly.pos = sf::Vector2f((float) (xPosRight * map.getTileWidth()), (float) (yPosBot * map.getTileHeight())),sf::Vector2f(map.getTileHeight(), map.getTileWidth());
        botRightPoly.updatePoly();
        botRightPoly.name = "botRightPoly";
        polygonFlipHandler(&botRightPoly, gidBotRight);


        sf::Vector2f MTV = objectPhysics::collisionSAT_Intersect(entPoly, botRightPoly);
        MTVtest += MTV;

        if(MTV.y != 0 || MTV.x != 0)
        {
            tilesCollided[8] = 1;
            rectangle.move(MTV.x, MTV.y);

            tcd.hurtful |= t->hurtful;
            if(gidBotRight == 256) {
                velocityX *= 0.75;
                velocityY *= 0.5;
            }

            collided = true;
        }


    }



    if(tilesCollided[6] == 0 && tilesCollided[7] == 0 && tilesCollided[8] == 0)
        grounded = false;
    if(grounded)
        velocityY = 0;

    std::copy(std::begin(tilesCollided), std::end(tilesCollided), std::begin(tcd.tilesCollided));
    return tcd;
}

void Entity::polygonFlipHandler(Polygon* p, unsigned int gid)
{
    unsigned int flipFlags = gid >> 29;

    if (flipFlags & 2)
    {
        p->verticalFlip();
    }

    // Horizontal flip
    if (flipFlags & 4)
    {
        p->horizontalFlip();
    }
}




void Entity::process(float time)
{
}

void Entity::collision(std::list<Entity *> &entities)
{

}

void Entity::initHitArea() {
    boundingBox = sprite.getGlobalBounds();
    boundingBox.width = spriteWidth*spriteScale;
    boundingBox.height = spriteHeight*spriteScale;
    hitArea.setOutlineThickness(1.f);
    hitArea.setOutlineColor(sf::Color::Red);
    hitArea.setSize(sf::Vector2f(boundingBox.width,boundingBox.height));
    hitArea.setFillColor(sf::Color::Transparent);
    hitArea.setOrigin(sf::Vector2f(spriteWidth*spriteScale*spriteScale,(spriteHeight*spriteScale*spriteScale)));

    hitAreas[AttackArea::FULL] = hitArea;
    hitArea.setSize(sf::Vector2f(boundingBox.width/2,boundingBox.height));
    hitAreas[AttackArea::SIDES] = hitArea;
    hitArea.setSize(sf::Vector2f(boundingBox.width,boundingBox.height/2));
    hitAreas[AttackArea::TOP] = hitArea;
    hitArea.setScale(1.0f,-1.0f);
    hitAreas[AttackArea::BOTTOM] = hitArea;
    hitArea = hitAreas[AttackArea::SIDES];
}

void Entity::resetHitBox() {
    rectangle.setScale(1.0f,1.0f);
    rectangle.setOrigin(rectangle.getSize().x/2,rectangle.getSize().y/2);

//    std::vector<sf::Vector2f> entPoints;
//    for(size_t i = 0; i < rectangle.getPointCount(); i++)
//    {
//        entPoints.emplace_back(rectangle.getPoint(i));
//    }
//    entPoly = Polygon(entPoints);
    //entPoly.updatePoly();

}


void Entity::reduceHitBox() {
    rectangle.setScale(1.0f,0.7f);
    rectangle.setOrigin(rectangle.getSize().x/2,rectangle.getSize().y/4);
    std::vector<sf::Vector2f> entPoints;
//    for(size_t i = 0; i < rectangle.getPointCount(); i++)
//    {
////        entPoints.emplace_back(rectangle.getTransform().transformPoint(rectangle.getPoint(i)));
//        entPoly.p.at(i) = rectangle.getTransform().transformPoint(rectangle.getPoint(i));
//        entPoly.o.at(i) = rectangle.getTransform().transformPoint(rectangle.getPoint(i));
//    }
//    entPoly.updatePoly();
}
void Entity::reduceHitBoxAir() {
    rectangle.setScale(1.0f,0.7f);
    rectangle.setOrigin(rectangle.getSize().x/2,rectangle.getSize().y/2);
}

void Entity::setCurrentState(EntityState s) {
    if (s == EntityState::DYING && deathTimer == 0)
    {
        currentState = s;
        currentStateChanged();
    }
    else if (currentState != s && !currentAnimation.getAnimationLock()) {
        currentState = s;
        currentStateChanged();
    }

}

void Entity::currentStateChanged() {
    switch(currentState)
    {
        case EntityState::RUNNING:

            currentAnimation.overrideFrames(animationMap.at(EntityState::RUNNING));
            break;
        case EntityState::CROUCHING:
            currentAnimation.overrideFrames(animationMap.at(EntityState::CROUCHING));
            break;
        case EntityState::IDLE:

            currentAnimation.overrideFrames(animationMap.at(EntityState::IDLE));
            break;
        case EntityState::FALLING:
            currentAnimation.overrideFrames(animationMap.at(EntityState::FALLING));
            break;
        case EntityState::SLIDING:
            currentAnimation.overrideFrames(animationMap.at(EntityState::SLIDING));
            break;
        case EntityState::JUMPING:
            currentAnimation.overrideFrames(animationMap.at(EntityState::JUMPING));
            break;
        case EntityState::DOUBLEJUMPING:
            currentAnimation.overrideFrames(animationMap.at(EntityState::DOUBLEJUMPING));
            break;
        case EntityState::ATTACKING:
            currentAnimation.setAnimationLock(true);
           // currentAnimation.overrideFrames(animationMap.at(EntityState::ATTACKING));
            break;
        case EntityState::AIR_ATTACK:
            currentAnimation.setAnimationLock(true);
            //currentAnimation.overrideFrames(animationMap.at(EntityState::AIR_ATTACK));
            break;
        case EntityState::DYING:
            currentAnimation.setAnimationLock(true);
            currentAnimation.overrideFrames(animationMap.at(EntityState::DYING));
            break;
        case EntityState::HURT:
            currentAnimation.overrideFrames(animationMap.at(EntityState::HURT));
            if(!player)
                currentAnimation.setAnimationLock(true);
            break;
        case EntityState::ITEM:
            currentAnimation.setAnimationLock(true);
            currentAnimation.overrideFrames(animationMap.at(EntityState::ITEM));
            break;
    }
    if (currentState == EntityState::CROUCHING || currentState == EntityState::ATTACKING || currentState == EntityState::SLIDING)
    {
        reduceHitBox();
    } else if (currentState == EntityState::DOUBLEJUMPING || currentState == EntityState::AIR_ATTACK || currentState == EntityState::FALLING || currentState == EntityState::JUMPING)
    {
        reduceHitBoxAir();
    }else
        resetHitBox();

}

void Entity::setSprite(const std::string& filepath) {
    texture.loadFromFile(filepath);
    sprite.setTexture(texture);
    sprite.setScale(sf::Vector2f(spriteScale, spriteScale));
    sprite.setOrigin(sf::Vector2f(spriteWidth/2.f,spriteHeight-rectangle.getSize().y-1));// setter midten av spriten slik at det blir riktig når spriten skal snus fra side til side og at den ligger i riktighøyde
}



