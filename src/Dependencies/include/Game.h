#ifndef SPACE_INVADERS_GAME_H
#define SPACE_INVADERS_GAME_H

#include "Rendering.h"

struct Alien {
    size_t x, y;
    uint8_t type;
};

struct PLayer {
    size_t x, y;
    size_t lives;
};

struct Game {
    size_t width, height;
    size_t aliens_num;
    Alien *aliens;
    PLayer player;
};

struct SpriteAnimation {
    bool loop;
    size_t frames_num;
    size_t frame_duration;
    size_t time;
    Sprite **frames;
};

#endif //SPACE_INVADERS_GAME_H
