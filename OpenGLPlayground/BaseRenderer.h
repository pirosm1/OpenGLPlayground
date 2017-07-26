#pragma once
struct GLFWwindow;

namespace mkp {

class BaseRenderer {
public:
    BaseRenderer() {}
    virtual ~BaseRenderer() {}

    virtual void run(mkp::BaseRenderer *);

    virtual void init();

    virtual void startup() {}

    virtual void render(double) {}

    virtual void shutdown() {}

    struct APPINFO {
        char title[128];
        int windowWidth;
        int windowHeight;
        int majorVersion;
        int minorVersion;
        int samples;
        union {
            struct {
                unsigned int fullscreen : 1;
                unsigned int vsync : 1;
                unsigned int cursor : 1;
                unsigned int stereo : 1;
                unsigned int debug : 1;
                unsigned int robust : 1;
            };
            unsigned int all;
        } flags;
    };

    virtual void onResize(int w, int h) {
        info.windowWidth = w;
        info.windowHeight = h;
    }

    virtual void onKey(int key, int action) {}

    virtual void onMouseButton(int button, int action) {}

    virtual void onMouseMove(int x, int y) {}

    virtual void onMouseWheel(int pos) {}

protected:
    APPINFO info;
    static mkp::BaseRenderer *r;
    GLFWwindow *window;

    static void glfw_onResize(GLFWwindow *, int, int);
    static void glfw_onKey(GLFWwindow *, int, int, int, int);
    static void glfw_onMouseButton(GLFWwindow *, int, int, int);
    static void glfw_onMouseMove(GLFWwindow *, double, double);
    static void glfw_onMouseWheel(GLFWwindow *, double, double);
    void setVsync(bool);
};
} // namespace mkp