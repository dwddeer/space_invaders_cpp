#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Dependencies/include/Rendering.h"
#include "Dependencies/include/ErrorHandling.h"
#include "Dependencies/include/Game.h"

using namespace std;

int main() {
    const size_t buffer_width = 224;
    const size_t buffer_height = 256;

    glfwSetErrorCallback(error_callback);

    if(!glfwInit())
        return -1;

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window;
    window = glfwCreateWindow(2.5 * buffer_width, 2.5 * buffer_height, "Space Invaders", nullptr, nullptr);
    if(!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if(err != GLEW_OK) {
        cerr << "Error initializing GLEW" << endl;
        glfwTerminate();
        return -1;
    }

    int glVersion[2] = {-1, 1};
    glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]);
    glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]);

    cout << "Using OpenGL: " << glVersion[0] << "." << glVersion[1] << endl;
    cout << "Renderer used: " << glGetString(GL_RENDERER) << endl;
    cout << "Shading language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

    glClearColor(1.0, 0.0, 0.0, 1.0);

    glfwSwapInterval(1); //V-sync

    Buffer buffer;
    buffer.width = buffer_width;
    buffer.height = buffer_height;
    buffer.data = new uint32_t[buffer_width * buffer_height];
    bufferClear(&buffer, 0);

    GLuint buffer_texture;
    glGenTextures(1, &buffer_texture);
    glBindTexture(GL_TEXTURE_2D, buffer_texture);
    glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGB8,
            buffer.width, buffer.height, 0,
            GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, buffer.data
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    GLuint fullscreen_triangle_vao;
    glGenVertexArrays(1, &fullscreen_triangle_vao);



    GLuint shader_id = glCreateProgram();
    {
        GLuint shader_vp = glCreateShader(GL_VERTEX_SHADER);

        glShaderSource(shader_vp, 1, &vertex_shader, 0);
        glCompileShader(shader_vp);
        validateShader(shader_vp, vertex_shader);
        glAttachShader(shader_id, shader_vp);

        glDeleteShader(shader_vp);
    }

    {
        GLuint shader_fp = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(shader_fp, 1, &fragment_shader, 0);
        glCompileShader(shader_fp);
        validateShader(shader_fp, fragment_shader);
        glAttachShader(shader_id, shader_fp);

        glDeleteShader(shader_fp);
    }

    glLinkProgram(shader_id);
    if(!validateProgram(shader_id)) {
        cerr << "Error while validating shader" << endl;
        glfwTerminate();
        glDeleteVertexArrays(1, &fullscreen_triangle_vao);
        delete[] buffer.data;
        return -1;
    }

    glUseProgram(shader_id);

    GLint location = glGetUniformLocation(shader_id, "buffer");
    glUniform1i(location, 0);

    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(fullscreen_triangle_vao);

    Sprite alien_sprite_pos1;
    alien_sprite_pos1.width = 11;
    alien_sprite_pos1.height = 8;
    alien_sprite_pos1.data = new uint8_t[alien_sprite_pos1.width * alien_sprite_pos1.height]
    {
            0,0,1,0,0,0,0,0,1,0,0, // . . @ . . . . . @ . .
            0,0,0,1,0,0,0,1,0,0,0, // . . . @ . . . @ . . .
            0,0,1,1,1,1,1,1,1,0,0, // . . @ @ @ @ @ @ @ . .
            0,1,1,0,1,1,1,0,1,1,0, // . @ @ . @ @ @ . @ @ .
            1,1,1,1,1,1,1,1,1,1,1, // @ @ @ @ @ @ @ @ @ @ @
            1,0,1,1,1,1,1,1,1,0,1, // @ . @ @ @ @ @ @ @ . @
            1,0,1,0,0,0,0,0,1,0,1, // @ . @ . . . . . @ . @
            0,0,0,1,1,0,1,1,0,0,0  // . . . @ @ . @ @ . . .
    };

    Sprite alien_sprite_pos2;
    alien_sprite_pos2.width = 11;
    alien_sprite_pos2.height = 8;
    alien_sprite_pos2.data = new uint8_t[alien_sprite_pos2.width * alien_sprite_pos2.height]
    {
            0,0,1,0,0,0,0,0,1,0,0, // ..@.....@..
            1,0,0,1,0,0,0,1,0,0,1, // @..@...@..@
            1,0,1,1,1,1,1,1,1,0,1, // @.@@@@@@@.@
            1,1,1,0,1,1,1,0,1,1,1, // @@@.@@@.@@@
            1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@
            0,1,1,1,1,1,1,1,1,1,0, // .@@@@@@@@@.
            0,0,1,0,0,0,0,0,1,0,0, // ..@.....@..
            0,1,0,0,0,0,0,0,0,1,0  // .@.......@.
    };

    Sprite player_sprite;
    player_sprite.width = 11;
    player_sprite.height = 7;
    player_sprite.data = new uint8_t[player_sprite.width * player_sprite.height]
    {
            0,0,0,0,0,1,0,0,0,0,0, // .....@.....
            0,0,0,0,1,1,1,0,0,0,0, // ....@@@....
            0,0,0,0,1,1,1,0,0,0,0, // ....@@@....
            0,1,1,1,1,1,1,1,1,1,0, // .@@@@@@@@@.
            1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@
            1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@
            1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@
    };

    SpriteAnimation* alien_animation = new SpriteAnimation;
    alien_animation -> loop = true;
    alien_animation -> frames_num = 2;
    alien_animation -> frame_duration = 10;
    alien_animation -> time = 0;

    alien_animation -> frames = new Sprite *[2];
    alien_animation -> frames[0] = &alien_sprite_pos1;
    alien_animation -> frames[1] = &alien_sprite_pos2;

    Game my_game;
    my_game.width = buffer_width;
    my_game.height = buffer_height;
    my_game.aliens_num = 55;
    my_game.aliens = new Alien[my_game.aliens_num];

    my_game.player.x = 112 - 5;
    my_game.player.y = 32;
    my_game.player.lives = 3;

    uint32_t clear_color = rgbToUint32(252, 245, 237);

    for(size_t yi = 0; yi < 5; yi++) {
        for(size_t xi = 0; xi < 11; xi++) {
            my_game.aliens[yi * 11 + xi].x = 16 * xi + 20;
            my_game.aliens[yi * 11 + xi].y = 17 * yi + 128;
        }
    }

    int player_move_dir = 1;

    //MAIN GAME LOOP
    while(!glfwWindowShouldClose(window)) {
        bufferClear(&buffer, clear_color);

        for(size_t i = 0; i < my_game.aliens_num; i++) {
            const Alien &alien = my_game.aliens[i];
            size_t current_frame = alien_animation -> time / alien_animation -> frame_duration;
            const Sprite &sprite = *alien_animation -> frames[current_frame];

            //ALIEN SPRITES AND COLOR
            bufferDrawSprite(&buffer, sprite, alien.x, alien.y, rgbToUint32(206, 90, 103));
        }

        //PLAYER SPRITE AND COLOR
        bufferDrawSprite(&buffer, player_sprite, my_game.player.x, my_game.player.y, rgbToUint32(206, 90, 103));

        alien_animation -> time++;
        if(alien_animation -> time == alien_animation -> frames_num * alien_animation -> frame_duration) {
            if(alien_animation -> loop)
                alien_animation -> time = 0;
            else {
                delete alien_animation;
                alien_animation = nullptr;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);

        glTexSubImage2D(
                GL_TEXTURE_2D, 0, 0, 0,
                buffer.width, buffer.height,
                GL_RGBA, GL_UNSIGNED_INT_8_8_8_8,
                buffer.data
        );

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glfwSwapBuffers(window);


        //WINDOW BOUNDARIES FOR PLAYER OBJECT
        if(my_game.player.x + player_sprite.width + player_move_dir >= my_game.width - 1) {
            my_game.player.x = my_game.width - player_sprite.width - player_move_dir - 1;
            player_move_dir *= -1;
        }
        else if((int)my_game.player.x + player_move_dir <= 0) {
            my_game.player.x = 0;
            player_move_dir *= -1;
        }
        else my_game.player.x += player_move_dir;

        glfwPollEvents();
    }


    glfwDestroyWindow(window);
    glfwTerminate();

    delete[] buffer.data;
    delete[] player_sprite.data;
    delete[] alien_sprite_pos2.data;
    delete[] alien_sprite_pos2.data;
    delete[] alien_animation -> frames;
    delete alien_animation;
    delete my_game.aliens;

    return 0;
}