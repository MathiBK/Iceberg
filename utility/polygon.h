//
// Created by axel on 05.11.2019.
//

#ifndef ICEBERG_POLYGON_H
#define ICEBERG_POLYGON_H


#include <SFML/Graphics/Shape.hpp>
#include <math.h>
#include <string>

class Polygon : public sf::Shape
{
public:

    Polygon() = default;
    explicit Polygon(std::vector<sf::Vector2f>& vp)
    {
        o = vp;
        p = vp;
        angle = 0.0f;
        overlap = false;
        Shape::setFillColor(sf::Color(255.f, 0.f, 0.f));
        update();
    }

    std::size_t getPointCount() const override { return p.size();}
    sf::Vector2f getPoint(std::size_t index) const override{return p[index];}

    std::vector<sf::Vector2f> p; //transformed points
    float angle;
    std::vector<sf::Vector2f> o; //original points
    bool overlap;
    sf::Vector2f pos; //position
    std::string name;

    void updatePoly() {
        for (int i = 0; i < o.size(); i++)
        {
            p[i] = sf::Vector2f((o[i].x * cosf(angle)) - o[i].y * sinf(angle) + pos.x, o[i].x * sinf(angle) + o[i].y * cosf(angle) + pos.y);
        }
        sf::Shape::update();
    }

    void horizontalFlip() {
        float max = -INFINITY;
        float min = INFINITY;
        for(auto& xPoint : o)
        {
            if(xPoint.x > max)
                max = xPoint.x;

            if(xPoint.x < min)
                min = xPoint.x;
        }
        float axis = max - min;

        for (int i = 0; i < o.size(); i++)
        {
            o[i].x = -o[i].x + axis;
        }
        this->updatePoly();
    }

    void verticalFlip() {
        float max = -INFINITY;
        float min = INFINITY;
        for(auto& yPoint : o)
        {
            if(yPoint.y > max)
                max = yPoint.y;

            if(yPoint.y < min)
                min = yPoint.y;
        }
        float axis = max - min;

        for (int i = 0; i < o.size(); i++)
        {
            o[i].y = -o[i].y + axis;
        }
        this->updatePoly();
    }

};

#endif //ICEBERG_POLYGON_H
