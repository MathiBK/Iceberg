#ifndef ICEBERG_ANIMATION_H
#define ICEBERG_ANIMATION_H

#include <vector>
#include <SFML/Graphics/Sprite.hpp>
#include "../objects/object.h"


struct Frame {
    sf::IntRect rect;
    double duration = 0.15f; // sekunder
};

class animation {
    std::vector<Frame> frames;
    double totalLength;
    double progress;
    sf::Sprite* target;
    bool lock;


public:
    animation() = default;
    explicit animation(sf::Sprite* target);
    virtual ~animation() = default;
    void addFrame(Frame&& frame);
    void update(double elapsed, bool loop);
    //void update(double elapsed, bool loop, animation a);
    void overrideFrames(const animation& a);


    double getLength() const { return totalLength; }
    double getProgress() const { return progress; }
    bool getAnimationLock() { return lock; }
    void setAnimationLock(bool l);

};


#endif //ICEBERG_ANIMATION_H
