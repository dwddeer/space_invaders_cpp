#include "../include/ErrorHandling.h"

void error_callback(int error, const char *description) {
    std::cerr << "Error " << description << std::endl;
}

void validateShader(GLuint shader, const char *file) {
    static const unsigned int BUFFER_SIZE = 512;
    char buffer[BUFFER_SIZE];
    GLsizei length = 0;

    glGetShaderInfoLog(shader, BUFFER_SIZE, &length, buffer);
    if(length > 0)
        std::cout << "Shader " << shader << "(" << (file ? file : "") << ") compile error: " << buffer << std::endl;
}

bool validateProgram(GLuint program) {
    static const GLsizei BUFFER_SIZE = 512;
    GLchar buffer[BUFFER_SIZE];
    GLsizei length = 0;

    glGetProgramInfoLog(program, BUFFER_SIZE, &length, buffer);

    if(length > 0) {
        std::cout << "Program " << program << " link error: " << buffer << std::endl;
        return false;
    }
    return true;
}