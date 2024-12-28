#include <cstdlib>
#include <glm/ext/vector_double2.hpp>
#include <sys/cdefs.h>

#include "window.hpp"
#include "log.h"

void defaultGLFWMouseButtonCallback(GLFWwindow* window, int button, int action, __attribute_maybe_unused__ int mods){
    WindowData* windata = (WindowData*) glfwGetWindowUserPointer(window);
    Window* win = windata->win;

    if (win->useDefaultMouseButtonCallback){
        switch (button) {
            case GLFW_MOUSE_BUTTON_1:
                windata->m1_pressed = action > 0; // RELEASE=0, PRESS=1, REPEAT=2
                break;
            case GLFW_MOUSE_BUTTON_2:
                windata->m2_pressed = action > 0; // RELEASE=0, PRESS=1, REPEAT=2
                break;
        }
    }

    // Call the user-defined callback
    if (win->mouseButtonCallback) win->mouseButtonCallback(win, button, action, mods);

}

void defaultGLFWCursorPositionCallback(GLFWwindow* window, double x, double y){
    WindowData* windata = (WindowData*) glfwGetWindowUserPointer(window);
    Window* win = windata->win;

    if (win->useDefaultCursorPositionCallback){
        windata->mouse_screen_xy[0] = x;
        windata->mouse_screen_xy[1] = y;
    }

    // Call the user-defined callback
    if (win->cursorPosCallback) win->cursorPosCallback(win, x, y);
}

void defaultGLFWFrameBufferSizeCallback(GLFWwindow* window, int width, int height) {
    WindowData* windata = (WindowData*) glfwGetWindowUserPointer(window);
    Window* win = windata->win;

    if (win->useDefaultFrameBufferSizeCallback){
        glViewport(0, 0, width, height); 
        windata->frame_h = height;
        windata->frame_w = width;
    }

    // Call the user-defined callback
    if (win->framebufferSizeCallback) win->framebufferSizeCallback(win, width, height);
}


Window::Window(int _width, int _height, char const* _name, Monitor* _monitor, Window* _share) : name(_name), width(_width), height(_height){
    DEBUG("Creating window '%s'", name);
    glfwInit();

    GLFWmonitor* m = _monitor == NULL ? NULL : _monitor->monitor;
    GLFWwindow* w = _share == NULL ? NULL : _share->win;
    share = _share;
    monitor = _monitor;

    win = glfwCreateWindow(width, height, name, m, w); 
    if (win == NULL){
        glfwTerminate();
        CRITICAL_ERROR("Failed to initialize window '%s'", name);
        exit(0);
    }
    glfwMakeContextCurrent(win);

    int const version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        glfwTerminate();
        CRITICAL_ERROR("Failed to initialize GLAD");
        exit(0);
    }
    DEBUG("Loaded OpenGL %d.%d with GLAD for windos '%s'", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version), name);

    windata = new WindowData;
    windata->frame_h = height;
    windata->frame_w = width;
    windata->win = this;
    glfwSetWindowUserPointer(win, windata);

    // Set the custom callback to null
    cursorPosCallback = NULL;
    framebufferSizeCallback = NULL;
    mouseButtonCallback = NULL;

    // Attach the default callbacks to the GLFW window
    glfwSetCursorPosCallback(win, defaultGLFWCursorPositionCallback);
    glfwSetFramebufferSizeCallback(win, defaultGLFWFrameBufferSizeCallback);
    glfwSetMouseButtonCallback(win, defaultGLFWMouseButtonCallback);
}

Window::~Window(){
    DEBUG("Destroying window '%s'", name);
    glfwDestroyWindow(win);
    delete windata;
}


