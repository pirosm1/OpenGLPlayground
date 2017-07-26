#include "RainbowBuffer.h"
#include "RainbowPoint.h"
#include "RedBuffer.h"
#include "Triangle.h"

int main(void) {
    // mkp::RedBuffer* r = new mkp::RedBuffer();
    // mkp::RainbowBuffer *r = new mkp::RainbowBuffer();
    // mkp::RainbowPoint *r = new mkp::RainbowPoint();
    mkp::Triangle *r = new mkp::Triangle();
    r->run(r);
    delete r;
    return 0;
}