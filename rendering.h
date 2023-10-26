#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>


#ifndef SPACE_INVADERS_RENDERING_H
#define SPACE_INVADERS_RENDERING_H

struct Buffer {
    size_t width;
    size_t height;
    uint32_t *data;
};

struct Sprite {
    size_t width;
    size_t height;
    uint8_t *data;
};

uint32_t rgbToUint32(uint8_t r, uint8_t g, uint8_t b);

void bufferClear(Buffer *buffer, uint32_t color);

void bufferDrawSprite(Buffer *buffer, const Sprite &sprite, size_t x, size_t y, uint32_t color);

#endif //SPACE_INVADERS_RENDERING_H
