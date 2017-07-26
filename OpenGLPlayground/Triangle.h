#pragma once
#define _USE_MATH_DEFINES

#include "BaseRenderer.h"
#include "ResourceManager.h"

namespace mkp {
class Triangle : public BaseRenderer {
public:
    Triangle(){};

    void startup() final {
        ResourceManager resMan;
        programID = resMan.createShaderProgram("triangleVector.glsl",
                                               "rainbowPointFragment.glsl");
        glCreateVertexArrays(1, &vertexArrayObject);
        glBindVertexArray(vertexArrayObject);
    }

    void shutdown() final {
        glDeleteVertexArrays(1, &vertexArrayObject);
        glDeleteProgram(programID);
    }

    void render(double currentTime) final {
        const GLfloat color[] = { static_cast<float>(sin(currentTime) * 0.5 + 0.5),
                                  static_cast<float>(sin(currentTime + M_PI / 2) * 0.5 + 0.5),
                                  static_cast<float>(sin(currentTime + M_PI) * 0.5 + 0.5),
                                  1.0f };
        glClearBufferfv(GL_COLOR, 0, color);

        glUseProgram(programID);

        glDrawArrays(GL_TRIANGLES, 0, 3);
    };

private:
    GLuint programID;
    GLuint vertexArrayObject;
};
} // namespace mkp