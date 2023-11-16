#ifndef SPACE_INVADERS_RENDERING_H
#define SPACE_INVADERS_RENDERING_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

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

static const char* fragment_shader =
        "\n"
        "#version 330\n"
        "\n"
        "uniform sampler2D buffer;\n"
        "noperspective in vec2 TexCoord;\n"
        "\n"
        "out vec3 outColor;\n"
        "\n"
        "void main(void){\n"
        "    outColor = texture(buffer, TexCoord).rgb;\n"
        "}\n";

static const char* vertex_shader =
        "\n"
        "#version 330\n"
        "\n"
        "noperspective out vec2 TexCoord;\n"
        "\n"
        "void main(void){\n"
        "\n"
        "    TexCoord.x = (gl_VertexID == 2)? 2.0: 0.0;\n"
        "    TexCoord.y = (gl_VertexID == 1)? 2.0: 0.0;\n"
        "    \n"
        "    gl_Position = vec4(2.0 * TexCoord - 1.0, 0.0, 1.0);\n"
        "}\n";

uint32_t rgbToUint32(uint8_t r, uint8_t g, uint8_t b);

void bufferClear(Buffer *buffer, uint32_t color);

void bufferDrawSprite(Buffer *buffer, const Sprite &sprite, size_t x, size_t y, uint32_t color);

#endif //SPACE_INVADERS_RENDERING_H
