#pragma once
#define _USE_MATH_DEFINES

#include <vector>

#include "BaseRenderer.h"
#include "ResourceManager.h"

struct ShaderInfo;

namespace mkp {
class AnimatedTriangle : public BaseRenderer {
public:
    AnimatedTriangle(){};

    void startup() final {
        ResourceManager resMan;
        std::vector<ShaderInfo> shaders = { { "animatedTriVector.glsl", GL_VERTEX_SHADER },
                                            { "animatedTriFragment.glsl", GL_FRAGMENT_SHADER } };
        programID = resMan.createShaderProgram(shaders);

        glCreateVertexArrays(1, &vertexArrayObject);
        glBindVertexArray(vertexArrayObject);
    }

    void shutdown() final {
        glDeleteVertexArrays(1, &vertexArrayObject);
        glDeleteProgram(programID);
    }

    void render(double currentTime) final {
        const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        glClearBufferfv(GL_COLOR, 0, black);

        glUseProgram(programID);

        GLfloat attrib[] = { static_cast<float>(sin(4 * currentTime / M_PI) * 0.5),
                             static_cast<float>(cos(4 * currentTime / M_PI) * 0.5),
                             0.0f, 0.0f };
        const GLfloat colorAttrib[] =
        { static_cast<float>(sin(currentTime) * 0.5 + 0.5),
          static_cast<float>(sin(currentTime + M_PI / 2) * 0.5 + 0.5),
          static_cast<float>(sin(currentTime + M_PI) * 0.5 + 0.5), 1.0f };

        // Update the value of input attribute 0
        glVertexAttrib4fv(0, attrib);
        glVertexAttrib4fv(1, colorAttrib);

        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

private:
    GLuint programID;
    GLuint vertexArrayObject;
};
} // namespace mkp