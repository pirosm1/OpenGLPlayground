#pragma once
#include <string>

#include <GL\glew.h>

namespace mkp {
class ResourceManager {
public:
    ResourceManager();

    GLuint createShaderProgram(const char *, const char *);
    GLuint loadVertexShader(const char *, GLint &, int &);
    GLuint loadFragmentShader(const char *, GLint &, int &);

private:
    int readShaderFile(const char *, std::string *);
    int compileShader(GLuint, const std::string, GLint &, int &);
};
} // namespace mkp