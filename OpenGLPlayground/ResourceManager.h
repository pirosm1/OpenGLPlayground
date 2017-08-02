#pragma once
#include <string>

#include <GL\glew.h>

namespace mkp {
class ResourceManager {
public:
    ResourceManager();

    GLuint createShaderProgram(const char *, const char *);
    GLuint loadShader(const char *, GLint &, int &, const GLenum);

private:
    int readShaderFile(const char *, std::string *);
    int compileShader(GLuint, const std::string, GLint &, int &);
};
} // namespace mkp