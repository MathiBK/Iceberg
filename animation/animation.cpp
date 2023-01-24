

#include <iostream>
#include <utility>
#include "animation.h"

animation::animation(sf::Sprite* target) : target(target) {
    progress = totalLength = 0.0;
    lock = false;
}
void animation::addFrame(Frame&& frame) {
    totalLength += frame.duration;
    frames.push_back(std::move(frame));
}
void animation::update(double elapsed, bool loop) {
    progress += elapsed;
    if(progress>totalLength && !frames.empty())
    {
        progress = 0.0f;
        lock = false;

    }
    double p = progress;
    for (size_t i = 0; i < frames.size(); i++) {
        p -= frames[i].duration;

        // Hvis vi har kommet til neste eller siste frame
        if (p <= 0.0 || &(frames[i]) == &frames.back()) {
            target->setTextureRect(frames[i].rect);

            // Sjekker om animasjonen skal loope og om det gjenstår mer tid av animasjonen
            if (loop && p > 0.0 && &(frames[i]) == &(frames.back())) {
                i = 0;    // gå tilbake til første frame
                continue; // fortsett hvor i er
            }
            break; // frame er funnet
        }
    }

}

//Overskriver frames i en animasjon fra den en til den andre
void animation::overrideFrames(const animation& a) {
//    frames.erase(frames.begin()+a.frames.size(),frames.begin()+frames.size());
//    for (int i = 0; i <a.frames.size() ; ++i) {
//        if(frames.size()>=a.frames.size())
//        frames[i].rect = a.frames[i].rect;
//        else{
//            frames.push_back(a.frames[i]);
//        }
//    }
//    if (lock)
//        return;
    frames.clear();
    totalLength = 0;
    progress = 0;
    for (const auto & frame : a.frames) {
        frames.push_back(frame);
        totalLength += frame.duration;
    }

}

void animation::setAnimationLock(bool l) {
    lock = l;

}



