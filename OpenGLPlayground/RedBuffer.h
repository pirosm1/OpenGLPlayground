#pragma once

#include "BaseRenderer.h"

namespace mkp {
class RedBuffer : public mkp::BaseRenderer {
public:
    RedBuffer(){};
    ~RedBuffer(){};

    void render(double) final {
        static const GLfloat red[] = { 1.0f, 0.0f, 0.0f, 1.0f };

        glClearBufferfv(GL_COLOR, 0, red);
    }
};
} // namespace mkp
