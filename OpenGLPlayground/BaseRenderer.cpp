#include <math.h>
#include <stdio.h>
#include <string.h>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "BaseRenderer.h"

mkp::BaseRenderer *mkp::BaseRenderer::r = 0;
void mkp::BaseRenderer::run (mkp::BaseRenderer *the_r) {
    bool running = true;
    r = the_r;

    // Initialize GLFW, return on error
    if (!glfwInit ()) {
        fprintf (stderr, "Failed to initialize GLFW\n");
        getchar (); // Pause for effect.
        return;
    }
    init ();

    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, info.majorVersion);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, info.minorVersion);

// Set up window hints
#ifndef _DEBUG
    if (info.flags.debug)
#endif
    {
        glfwWindowHint (GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    }
    if (info.flags.robust) {
        glfwWindowHint (GLFW_CONTEXT_ROBUSTNESS, GLFW_LOSE_CONTEXT_ON_RESET);
    }
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_SAMPLES, info.samples);
    glfwWindowHint (GLFW_STEREO, info.flags.stereo ? GL_TRUE : GL_FALSE);

    // create window
    window = glfwCreateWindow (info.windowWidth, info.windowHeight, info.title,
                               info.flags.fullscreen ? glfwGetPrimaryMonitor () : nullptr,
                               nullptr);
    // Check window
    if (!window) {
        fprintf (stderr, "Failed to open window\n");

        glfwTerminate ();
        return;
    }

    // Do something for glew
    glfwMakeContextCurrent (window);
    glewExperimental = true;

    // set window callbacks
    glfwSetWindowSizeCallback (window, glfw_onResize);
    glfwSetKeyCallback (window, glfw_onKey);
    glfwSetMouseButtonCallback (window, glfw_onMouseButton);
    glfwSetCursorPosCallback (window, glfw_onMouseMove);
    glfwSetScrollCallback (window, glfw_onMouseWheel);
    // if cursor is not set, hide the cursor.
    if (!info.flags.cursor) {
        glfwSetInputMode (window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }

    // Attempt to initialize glew, error out on fail
    if (glewInit () != GLEW_OK) {
        fprintf (stderr, "Failed to initialize GLEW.\n");

        glfwTerminate ();
        return;
    }

    startup ();

    // render at least once
    do {
        render (glfwGetTime ());

        // Swap in the display/render buffers
        glfwSwapBuffers (window);
        // Process any events in queue, perform callbacks
        glfwPollEvents ();

        // set running flag to false if escape key is pressed or window should
        // close.
        running &= (glfwGetKey (window, GLFW_KEY_ESCAPE) == GLFW_RELEASE);
        running &= (!glfwWindowShouldClose (window));
    } while (running);

    glfwTerminate ();
    return;
}

void mkp::BaseRenderer::init () {
#pragma warning(suppress : 4996)
    strcpy (info.title, "OpenGL Playground");
    info.windowWidth = 1024;
    info.windowHeight = 768;
    info.majorVersion = 3;
    info.minorVersion = 3;
    info.samples = 4;
    info.flags.all = 0;
    info.flags.cursor = 1;
#ifndef _DEBUG
    info.flags.debug = 1;
#endif
}

void mkp::BaseRenderer::glfw_onResize (GLFWwindow *window, int w, int h) {
    r->onResize (w, h);
}

void mkp::BaseRenderer::glfw_onKey (GLFWwindow *window, int key, int scancode, int action, int mods) {
    r->onKey (key, action);
}

void mkp::BaseRenderer::glfw_onMouseButton (GLFWwindow *window, int button, int action, int mods) {
    r->onMouseButton (button, action);
}

void mkp::BaseRenderer::glfw_onMouseMove (GLFWwindow *window, double x, double y) {
    r->onMouseMove (static_cast<int> (x), static_cast<int> (y));
}

void mkp::BaseRenderer::glfw_onMouseWheel (GLFWwindow *window, double xoffset, double yoffset) {
    r->onMouseWheel (static_cast<int> (yoffset));
}

void mkp::BaseRenderer::setVsync (bool enable) {
    info.flags.vsync = enable ? 1 : 0;
    glfwSwapInterval ((int)info.flags.vsync);
}