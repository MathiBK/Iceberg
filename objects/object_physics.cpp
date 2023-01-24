#include "object_physics.h"
#include "../utility/polygon.h"
#include "particles/particle.h"
#include <iostream>


void objectPhysics::applyPhysics(Entity* entity, const float deltaTime) {

    entity->velocityY += gravity*deltaTime;

}

 void objectPhysics::applyPhysics(Particle* part, const float deltaTime) {
        part->velocity.y += 150.f*deltaTime;
}



 bool objectPhysics::collisionSAT(Polygon &r1, Polygon &r2)
{
    Polygon *poly1 = &r1;
    Polygon *poly2 = &r2;


    float overlap = INFINITY;

    for (int shape = 0; shape < 2; shape++)
    {
        if (shape == 1)
        {
            poly1 = &r2;
            poly2 = &r1;
        }

        for (int a = 0; a < poly1->p.size() + 0; ++a)
        {
            //regn ut normalvektor til kant
            //denne aksen sjekkes i loopen
            auto current = poly1->p[a];
            auto next = poly1->p[(a + 1) % poly1->p.size()];
            auto edge = next - current;

            //dette er normalvektoren
            sf::Vector2f axisProj(-edge.y, edge.x);
            /*
            float d = sqrtf(axisProj.x * axisProj.x + axisProj.y * axisProj.y);
            axisProj = { axisProj.x / d, axisProj.y / d };
            */

            // regner ut min og max for p1
            float min_p1 = INFINITY, max_p1 = -INFINITY;
            for (auto & p : poly1->p)
            {
                //prikkprodukt (projiseres ned på axisproj)
                float q =(p.x * axisProj.x + p.y * axisProj.y);
                min_p1 = std::min(min_p1, q);
                max_p1 = std::max(max_p1, q);
            }

            // regner ut min og max for p2
            float min_p2 = INFINITY, max_p2 = -INFINITY;
            for (auto & p : poly2->p)
            {
                float q = (p.x * axisProj.x + p.y * axisProj.y);
                min_p2 = std::min(min_p2, q);
                max_p2 = std::max(max_p2, q);
            }

            overlap = std::min(std::min(max_p1, max_p2) - std::max(min_p1, min_p2), overlap);

            if (!(max_p2 >= min_p1 && max_p1 >= min_p2))
            {
                return false;
            }
        }
    }

    return true;
}


 Collision objectPhysics::collisionSAT_Helper(Polygon &r1, Polygon &r2)
{
    Collision c;
    c.overlap = INFINITY;
    c.minAx = {0,0};

    for (int a = 0; a < r1.p.size() + 0; ++a)
    {
        //regn ut normalvektor til kant
        //denne aksen sjekkes i loopen
        auto current = r1.p[a];
        auto next = r1.p[(a + 1) %r1.p.size()];
        auto edge = next - current;

        //dette er normalvektoren
        sf::Vector2f axisProj(-edge.y, edge.x);

        float length = sqrtf(axisProj.x * axisProj.x + axisProj.y * axisProj.y);
        axisProj = { axisProj.x / length, axisProj.y / length };


        // regner ut min og max for p1
        float min_p1 = INFINITY, max_p1 = -INFINITY;
        for (auto & p : r1.p)
        {
            //prikkprodukt (projiseres ned på axisproj)
            float q =(p.x * axisProj.x + p.y * axisProj.y);
            min_p1 = std::min(min_p1, q);
            max_p1 = std::max(max_p1, q);
        }

         // regner ut min og max for p2
        float min_p2 = INFINITY, max_p2 = -INFINITY;
        for (auto & p : r2.p)
        {
            float q = (p.x * axisProj.x + p.y * axisProj.y);
            min_p2 = std::min(min_p2, q);
            max_p2 = std::max(max_p2, q);
        }

        float old = c.overlap;
        c.overlap = std::min(std::min(max_p1, max_p2) - std::max(min_p1, min_p2), c.overlap);

        //finner akse med minst separering
        if (old >= c.overlap)
            c.minAx = axisProj;

        if (!(max_p2 >= min_p1 && max_p1 >= min_p2))
        {
            c.overlap = 0;
            return c;
        }
    }
    return c;
}

sf::Vector2f objectPhysics::collisionSAT_Intersect(Polygon &r1, Polygon &r2)
{
    Polygon *poly1 = &r1;
    Polygon *poly2 = &r2;


    sf::Vector2f MTV;


    float overlap = INFINITY;
    Collision overlap1 = collisionSAT_Helper(r1, r2);
    Collision overlap2 = collisionSAT_Helper(r2, r1);
    sf::Vector2f minAx;
    for (int shape = 0; shape < 2; shape++)
    {
        if (shape == 1)
        {
            poly1 = &r2;
            poly2 = &r1;
        }

        for (int a = 0; a < poly1->p.size() + 0; ++a)
        {
            //regn ut normalvektor til kant
            //denne aksen sjekkes i loopen
            auto current = poly1->p[a];
            auto next = poly1->p[(a + 1) % poly1->p.size()];
            auto edge = next - current;

            //dette er normalvektoren
            sf::Vector2f axisProj(-edge.y, edge.x);

            float length = sqrtf(axisProj.x * axisProj.x + axisProj.y * axisProj.y);
            axisProj = { axisProj.x / length, axisProj.y / length };


            // regner ut min og max for p1
            float min_p1 = INFINITY, max_p1 = -INFINITY;
            for (auto & p : poly1->p)
            {
                //prikkprodukt (projiseres ned på axisproj)
                float q =(p.x * axisProj.x + p.y * axisProj.y);
                min_p1 = std::min(min_p1, q);
                max_p1 = std::max(max_p1, q);
            }

            // regner ut min og max for p2
            float min_p2 = INFINITY, max_p2 = -INFINITY;
            for (auto & p : poly2->p)
            {
                float q = (p.x * axisProj.x + p.y * axisProj.y);
                min_p2 = std::min(min_p2, q);
                max_p2 = std::max(max_p2, q);
            }

            float old = overlap;
            overlap = std::min(std::min(max_p1, max_p2) - std::max(min_p1, min_p2), overlap);

            //finner akse med minst separering
            if (overlap < old)
                MTV = axisProj;


            if (!(max_p2 >= min_p1 && max_p1 >= min_p2))
            {
                return {0,0};
            }
        }
    }

    float length = sqrt(MTV.x * MTV.x + MTV.y * MTV.y);
    Collision c = Collision();



    r1.pos.x -= overlap * MTV.x / length;
    r1.pos.y -= overlap * MTV.y / length;
    r1.updatePoly();

    sf::FloatRect r1Bounds = r1.getGlobalBounds();
    sf::FloatRect r2Bounds = r2.getGlobalBounds();


    sf::Vector2f r1Center = sf::Vector2f(r1Bounds.left + (r1Bounds.width/2), r1Bounds.top + (r1Bounds.height/2));
    sf::Vector2f r2Center = sf::Vector2f(r2Bounds.left + (r2Bounds.width/2), r2Bounds.top + (r2Bounds.height/2));

    //vektor mellom sentrum til bounding boxes
    sf::Vector2f direction = r2Center - r1Center;
    //Denne sjekker om vektorene peker i samme retning, og flipper den om det er sant
    if((direction.x * MTV.x + direction.y * MTV.y) > 0 )
        MTV *= -1.f;

    return overlap * MTV/length;
}

