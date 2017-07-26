#pragma once
#define _USE_MATH_DEFINES
#include <cmath>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "BaseRenderer.h"

namespace mkp {
class RainbowBuffer : public mkp::BaseRenderer {
public:
    RainbowBuffer(){};
    ~RainbowBuffer(){};

    void render(double currentTime) final {
        const GLfloat color[] = { static_cast<float>(sin(currentTime) * 0.5 + 0.5),
                                  static_cast<float>(sin(currentTime + M_PI / 2) * 0.5 + 0.5),
                                  static_cast<float>(sin(currentTime + M_PI) * 0.5 + 0.5),
                                  1.0f };
        glClearBufferfv(GL_COLOR, 0, color);
    }
};
} // namespace mkp
