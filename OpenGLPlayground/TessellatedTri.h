#pragma once
#include "BaseRenderer.h"

namespace mkp {

class TessellatedTri : public BaseRenderer {
public:
    TessellatedTri(){};

    void startup() final {}
};
} // namespace mkp