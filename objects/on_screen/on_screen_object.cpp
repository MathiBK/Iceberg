//
// Created by Sebas on 13.11.2019.
//

#include "on_screen_object.h"

#include <utility>

OnScreenObject::OnScreenObject(const std::string &setning, sf::Color color, float x, float y, int size) : setning(setning), x(x), y(y), size(size)

{
    font.loadFromFile("media/fonts/OptimusPrincepsSemiBold.ttf");

    text = sf::Text(setning, font, size);
    text.setStyle(sf::Text::Bold);
    text.setFillColor(color);
    sf::FloatRect bounds = text.getGlobalBounds();
    text.setOrigin(bounds.width * 0.5f, bounds.height * 0.5f);


    text.setPosition(x, y);
}

void OnScreenObject::draw(sf::RenderWindow &window)
{
    text.setFont(font);
    sf::FloatRect bounds = text.getGlobalBounds();
    //text.setOrigin(bounds.width * 0.5f, bounds.height * 0.5f);
    window.draw(text);
}

void OnScreenObject::setText(std::string& input)
{
    this->text.setString(input);

}

sf::FloatRect OnScreenObject::getBounds()
{
    text.setFont(font);
    return this->text.getGlobalBounds();

}

void OnScreenObject::setOrigin(sf::Vector2f origin)
{
    this->text.setOrigin(origin.x, origin.y);
}

void OnScreenObject::loadFont(const std::string& filename)
{
    this->font.loadFromFile(filename);
}

void OnScreenObject::setScale(float scale) {
    this->text.setScale(scale, scale);
}

void OnScreenObject::changeText(std::string &s) {
    this->text.setString(s);
}

void OnScreenObject::setPos(float x, float y) {
    this->x = x;
    this->y = y;
    this->text.setPosition(x, y);
}

