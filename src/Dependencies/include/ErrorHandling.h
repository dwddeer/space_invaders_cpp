#ifndef SPACE_INVADERS_ERRORHANDLING_H
#define SPACE_INVADERS_ERRORHANDLING_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>


void error_callback(int error, const char *description);

void validateShader(GLuint shader, const char *file);

bool validateProgram(GLuint program);

#endif //SPACE_INVADERS_ERRORHANDLING_H
