#include "../include/rendering.h"

uint32_t rgbToUint32(uint8_t r, uint8_t g, uint8_t b) {
    return (r << 24) | (g << 16) | (b << 8) | 255;
}

void bufferClear(Buffer *buffer, uint32_t color) {
    size_t dim = buffer -> width * buffer -> height;
    for(size_t i = 0; i < dim; i++)
        buffer -> data[i] = color;
}

void bufferDrawSprite(Buffer *buffer, const Sprite &sprite, size_t x, size_t y, uint32_t color) {
    for(size_t xi = 0; xi < sprite.width; xi++) {
        for(size_t yi = 0; yi < sprite.height; yi++) {
            size_t sy = sprite.height - 1 + y - yi;
            size_t sx = x + xi;
            if(sprite.data[yi * sprite.width + xi] && sy < buffer->height && sx < buffer->width)
                buffer->data[sy * buffer->width + sx] = color;
        }
    }
}