#include <algorithm>

#include "grappler.h"
#include "../../map/maptest.h"
#include "../layer.h"
#include "../../abilities/grapplingHook.h"

Grappler::Grappler(maptest &map1, float x, float y, sf::Vector2f direction, grapplingHook& ent) : Particle(map), hook(ent)
{
    this->body.setPosition(x, y);
    this->body.setFillColor(sf::Color::White);
    this->body.setSize(sf::Vector2f(5, 5));
    this->body.setOrigin(body.getSize() / 2.0f);
    this->size = 5;

    this->direction.x = direction.x;
    this->direction.y = direction.y;
    this->velocity.x = 250;
    this->velocity.y = 250;
    this->velocity.x *= direction.x;
    this->velocity.y *= direction.y;
    this->prevPos = {0, 0};
    this->moving = true;
    this->m_angle = 0;
    this->length = 0;
    this->angleVelocity = 0;

    this->initialGrapplingHookPos = this->hook.getPosition();
    this->line = sf::VertexArray(sf::LineStrip, 2);
    this->state = grapplerState::MOVING;
}

void Grappler::draw(sf::RenderWindow& window)
{


    window.draw(body);
    window.draw(line);

}
void Grappler::setAngleAcc(float delta)
{
    sf::Vector2f  grapplePos = body.getPosition();
    sf::Vector2f  playerPos = hook.getPosition();

    sf::Vector2f playerToHook(grapplePos.x - playerPos.x, grapplePos.y - playerPos.y);
    sf::Vector2f yAx(0, 1);

    float playerToHookMagnitude = this->length;

    //dette er cos til vinkelen mellom y-aksen og grapplinghook
    float f = ((playerToHook.x * yAx.x) + (playerToHook.y * yAx.y)) / (playerToHookMagnitude);

    //Dette "clamper" verdien mellom -1 og 1 (området acos er definert)

    float fClamped = std::max(-1.f, std::min(f, 1.f));
    m_angle = acos(fClamped);
    if(body.getPosition().x < hook.getPosition().x)
        m_angle *= -1;

    double angleAcc = 0;
    if(this->length != 0) {
        angleAcc = (-200.f / this->length) * sin(m_angle); //dette er vinkelakslerasjonen

    }

    if(angleAcc < 0.00001 && angleAcc > -0.00001)
        angleAcc = 0;

    angleVelocity += angleAcc * delta;
    m_angle += angleVelocity * delta;

    sf::Vector2f currentPos = this->hook.getPosition();

    if (this->length > hook.maxLength)
        this->length = hook.maxLength;

    this->body.setPosition(currentPos.x + (float)(sin(m_angle) * this->length), currentPos.y + (float)(cos(m_angle) * this->length));
}

sf::Vector2f Grappler::move(float delta)
{
    sf::Vector2f movement(0.f, 0.f);

    objectPhysics::applyPhysics(this, delta);
    if(state == grapplerState::MOVING)
    {
        movement.x += velocity.x * delta;
        movement.y += velocity.y * delta;
        body.move(movement);
    } else {
        angleVelocity *= 0.995f;
        setAngleAcc(delta);
        sf::Vector2f temp = {this->body.getPosition().x, this->body.getPosition().y+(velocity.y*delta)};
        if(getDistance(temp, this->hook.getPosition()) < hook.maxLength) {
            this->body.setPosition(this->body.getPosition().x, this->body.getPosition().y + (velocity.y * delta));
        } else {
            velocity.y = 0;
        }
    }

    if(this->length >= hook.maxLength && state != grapplerState::MAXED)

    {
        this->length = hook.maxLength;
        state = grapplerState::MAXED;
        initialGrapplingHookPos = this->hook.getPosition();
        sf::Vector2f thisPos = this->body.getPosition();
        sf::Vector2f vectorBetween(initialGrapplingHookPos.x - thisPos.x, initialGrapplingHookPos.y - thisPos.y);
        //fartsvektoren settes til en enhetsvektor som står normalt på vektor mellom spiller/grapple
        velocity = {vectorBetween.x/this->length + velocity.x, -vectorBetween.y/this->length + velocity.y};

    }
    return movement;
}

float Grappler::getDistance(sf::Vector2f thisPos, sf::Vector2f grapplingHookPos)
{
    sf::Vector2f vectorBetween(grapplingHookPos.x - thisPos.x, grapplingHookPos.y - thisPos.y);
    float dotProd = (vectorBetween.x * vectorBetween.x) + (vectorBetween.y * vectorBetween.y);
    if (dotProd <= 0)
        dotProd = 0;
    return sqrt(dotProd);
}

void Grappler::process(float deltaTime)
{
    this->length = getDistance(this->body.getPosition(), this->hook.getPosition());
    if(this->moving)
    {
        this->move(deltaTime);
        this->collision(deltaTime);
    } else {
        entityMove(deltaTime);
    }

    line[0].position = sf::Vector2f(body.getPosition());
    line[1].position = sf::Vector2f(hook.getPosition());
}


void Grappler::collision(float delta) {

    sf::Vector2f currentPos = this->body.getPosition();

    auto l = this->hook.pl.map.getLayer("Solid").get();
    unsigned int * tilemap  = this->hook.pl.map.getLayer("Solid")->getTilemap();

    int height = this->hook.pl.map.getTileHeight();
    int width = this->hook.pl.map.getTileWidth();

    //x-posisjon er lik uansett i y-aksen, såbruker samme for begge
    int xPos = (int)floor(currentPos.x)  /width;
    int yPos = (int)floor(currentPos.y)  / width;

    unsigned int thisGid = tilemap[xPos + yPos * l->getWidth()];

    sf::FloatRect thisTile(sf::Vector2f((float) (xPos * width), (float) (yPos * height)), sf::Vector2f(height, width));

    if(thisGid != 0)
    {
        if (body.getGlobalBounds().intersects(thisTile)) {
            this->moving = false;
            this->state = grapplerState::COLLIDED;
            this->prevPos = this->hook.ent->rectangle.getPosition();
        }
    }
}

void Grappler::entityMove(float delta) {
    if(this->state != grapplerState::COLLIDED) {
        this->hook.ent->canMoveLeft = true;
        this->hook.ent->canMoveRight = true;
        this->hook.ent->canMoveUp = true;
        this->hook.ent->canMoveDown = true;
        return;
    }

    if(this->state == grapplerState::COLLIDED){
        if(this->length < this->hook.maxLength){
            this->hook.ent->canMoveLeft = true;
            this->hook.ent->canMoveRight = true;
            this->hook.ent->canMoveUp = true;
            this->hook.ent->canMoveDown = true;
        }
        if(this->length >= this->hook.maxLength && this->body.getPosition().x < this->hook.ent->rectangle.getPosition().x){
            this->hook.ent->canMoveRight = false;
        } else if(this->length >= this->hook.maxLength && this->body.getPosition().x > this->hook.ent->rectangle.getPosition().x){
            this->hook.ent->canMoveLeft = false;
        }
        if(this->length >= this->hook.maxLength && this->body.getPosition().y > this->hook.ent->rectangle.getPosition().y){
            this->hook.ent->canMoveUp = false;
        } else if(this->length >= this->hook.maxLength && this->body.getPosition().y < this->hook.ent->rectangle.getPosition().y){
            this->hook.ent->canMoveDown = false;
        }

        if(!this->hook.ent->grounded){
            if(this->hook.ent->velocityY > 0 && this->hook.ent->rectangle.getPosition().y + (this->hook.ent->rectangle.getSize().y/2) > this->body.getPosition().y) {
                if(!this->swinging){
                    this->swinging = true;
                    this->hook.ent->angleVelocity = sqrt((this->hook.ent->velocityX*this->hook.ent->velocityX)+(this->hook.ent->velocityY*this->hook.ent->velocityY))/this->length;
                    if(this->hook.ent->rectangle.getPosition().x > this->body.getPosition().x)
                        this->hook.ent->angleVelocity *= -1;
                } else {
                    this->hook.ent->canMoveUp = false;
                    this->hook.ent->canMoveDown = false;
                    this->hook.ent->canMoveLeft = false;
                    this->hook.ent->canMoveRight = false;
                }

                sf::Vector2f grapplePos = body.getPosition();
                sf::Vector2f playerPos = hook.ent->rectangle.getPosition();

                sf::Vector2f hookToPlayer(playerPos.x - grapplePos.x, playerPos.y - grapplePos.y);
                sf::Vector2f yAx(0, 1);

                float hookToPlayerMagnitude = this->length;

                float f = ((hookToPlayer.x * yAx.x) + (hookToPlayer.y * yAx.y)) / (hookToPlayerMagnitude);
                float fClamped = std::max(-1.f, std::min(f, 1.f));
                this->hook.ent->angle = acos(fClamped);
                if (this->hook.ent->rectangle.getPosition().x < body.getPosition().x)
                    this->hook.ent->angle *= -1;

                double angleAcc = 0;
                if (this->length != 0)
                    angleAcc = (-200.f / this->length) * sin(this->hook.ent->angle); //dette er vinkelakslerasjonen
                if (angleAcc < 0.00001 && angleAcc > -0.00001)
                    angleAcc = 0;

                if(prevPos.x == this->hook.ent->rectangle.getPosition().x){
                    this->hook.ent->angleVelocity *= -0.5;
                }
                if(prevPos.y == this->hook.ent->rectangle.getPosition().y){
                    this->hook.ent->angleVelocity *= -0.5;
                }

                this->hook.ent->angleVelocity += angleAcc * delta;
                this->hook.ent->angle += this->hook.ent->angleVelocity * delta;

                sf::Vector2f currentPos = this->body.getPosition();

                prevPos = this->hook.ent->rectangle.getPosition();
                this->hook.ent->rectangle.setPosition(
                        currentPos.x + (float) (sin(this->hook.ent->angle) * this->length),
                        currentPos.y + (float) (cos(this->hook.ent->angle) * this->length));
            } else {
                if(swinging){
                    if(this->state == grapplerState::COLLIDED && !this->hook.ent->grounded){
                        if(this->hook.ent->velocityY > 0){
                            this->hook.ent->velocityY = ((float)this->hook.ent->angleVelocity * this->length);
                            if(this->hook.ent->rectangle.getPosition().x > this->body.getPosition().x){
                                this->hook.ent->velocityY *= -1;
                            }
                        }
                        swinging = false;
                    }
                }
            }
        }
    }
}

Grappler::~Grappler() {
    if(this->state == grapplerState::COLLIDED && !this->hook.ent->grounded && this->swinging){
        this->hook.ent->velocityY = ((float)this->hook.ent->angleVelocity * this->length);
        if(this->hook.ent->velocityY > 0)
            this->hook.ent->velocityY *= -1;
        this->hook.ent->velocityX = ((float)this->hook.ent->angleVelocity * this->length);
    }
    this->hook.ent->grappled -= 1;
    this->swinging = false;
    this->hook.ent->canMoveLeft = true;
    this->hook.ent->canMoveRight = true;
    this->hook.ent->canMoveUp = true;
    this->hook.ent->canMoveDown = true;


}
